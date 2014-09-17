#include <stdafx.h>

#include <Obj.h>
#include <Bvh.h>
#include <geometry.h>

Bvh_Primitive::Bvh_Primitive() {
    id = -1;
}
Bvh_Primitive::Bvh_Primitive(int p_id, const BBox& p_bbox) {
    id = p_id;
    bbox = p_bbox;
    center = bbox.min * 0.5f + p_bbox.max * 0.5f;
}
Bvh_Primitive::~Bvh_Primitive() {
}

Bvh_Node::Bvh_Node() {
    children[0] = children[1] = nullptr;
}
Bvh_Node::~Bvh_Node() {
}
void Bvh_Node::asLeaf(size_t p_primitivesIdx,
                      size_t p_primitivesCnt, const BBox& p_bbox) {
    primitivesIdx = p_primitivesIdx;
    primitivesCnt = p_primitivesCnt;
    bbox = p_bbox;
}
#define CHILD_LEFT 0
#define CHILD_RIGHT 1
void Bvh_Node::asBranch(size_t p_axisSplit, Bvh_Node* p_l, Bvh_Node* p_r) {
    children[CHILD_LEFT] = p_l;
    children[CHILD_RIGHT] = p_r;
    bbox = BBox(p_l->bbox, p_r->bbox);
    axisSplit = p_axisSplit;
    primitivesCnt = 0;
}

Bvh_Node_Flat::Bvh_Node_Flat() {
    primitivesCnt = 0;
    primitivesIdx = 0;
}
Bvh_Node_Flat::~Bvh_Node_Flat() {
}

// Should take an inout argument for the hierarchy of primitives:
Bvh::Bvh(Obj* p_obj, size_t p_nodeMaxPrimitives) : m_obj(p_obj) {
    m_primitives = nullptr;

    assert(p_nodeMaxPrimitives<256); // ?
    maxPrimsInNode = p_nodeMaxPrimitives; // Insert limit?

    totalNodes = 0;
}
Bvh::~Bvh() {
    assert(m_primitives!=nullptr);
    delete[] m_primitives;

    assert(m_nodes_flat!=nullptr);
    delete[] m_nodes_flat;
}

void Bvh::init() {
    build(m_obj->getVertices(),
          m_obj->getVerticesCnt(),
          m_obj->getIndices(),
          m_obj->getIndicesCnt());
}

void Bvh::build(Vertex* p_vertices,
                size_t p_verticesCnt, 
                unsigned* p_indices,
                size_t p_indicesCnt) {
    assert(p_indicesCnt % 3==0);

    // Split list of vertices into list of primitives:
    size_t numPrimitives = p_indicesCnt / 3;
    size_t j = 0;
    m_primitives = new Bvh_Primitive[numPrimitives];
    for(size_t i = 2; i<p_indicesCnt; i+=3) {
        Vec3F v1 = p_vertices[p_indices[i - 2]].pos;
        Vec3F v2 = p_vertices[p_indices[i - 1]].pos;
        Vec3F v3 = p_vertices[p_indices[i - 0]].pos;
        m_primitives[j] = Bvh_Primitive(j, BBox(BBox(v1, v2), v3));
        j++;
    }
    Bvh_Primitive* primitives = new Bvh_Primitive[numPrimitives]; // This
                                                                  // might
                                                                  // possibly
                                                                  // be
                                                                  // replaced
                                                                  // by
                                                                  // simply
                                                                  // using
                                                                  // m_primitives.
    memcpy(primitives, m_primitives,
           sizeof(Bvh_Primitive) * numPrimitives);
    Bvh_Primitive* primitivesHierarchy = new Bvh_Primitive[numPrimitives];
    Bvh_Node* root = buildRecursive(primitives, 0, numPrimitives,
                                    &totalNodes, primitivesHierarchy);
    memcpy(m_primitives, primitivesHierarchy,
           sizeof(Bvh_Primitive) * numPrimitives);
    assert(primitives!=nullptr);
    delete[] primitives;
    assert(primitivesHierarchy!=nullptr);
    delete[] primitivesHierarchy;

    m_nodes_flat = new Bvh_Node_Flat[totalNodes];
    size_t node_offset = 0;
    flattenRecursive(root, &node_offset);

    freeRecursive(root);
    delete root;
}

Bvh_Node* Bvh::buildRecursive(Bvh_Primitive* p_primitives,
                              size_t p_idxStart,
                              size_t p_idxEnd,
                              size_t* io_totalNodes,
                              Bvh_Primitive* io_primitivesHierarchy) {
    assert(p_idxStart<p_idxEnd);
    *io_totalNodes += 1;
    Bvh_Node* node = new Bvh_Node();

    BBox bbox;
    for(size_t i = p_idxStart; i<p_idxEnd; i++) {
        bbox = BBox(bbox, p_primitives[i].bbox);
    }
    
    size_t primitivesCnt = p_idxEnd - p_idxStart;
    if(primitivesCnt==maxPrimsInNode) {
        size_t primitivesIdx = p_idxStart;
        for(size_t i = primitivesIdx; i<p_idxEnd; i++) {
            size_t primitiveIdx = p_primitives[i].id;
            io_primitivesHierarchy[i] = m_primitives[primitiveIdx]; // obs: m_primitives
        }
        node->asLeaf(primitivesIdx, primitivesCnt, bbox);
    } else {
        BBox bbox_center;
        for(size_t i = p_idxStart; i<p_idxEnd; i++) {
            bbox_center = BBox(bbox_center, p_primitives[i].center);
        }
        int idxMaxSpan = bbox_center.getAxisIdxMax(); // The axis to split.

        // Not quite sure what this particular leaf node creation is for:
        if(maxPrimsInNode>1
           && bbox_center.max[idxMaxSpan]==bbox_center.min[idxMaxSpan]) {
            size_t primitivesIdx = p_idxStart;
            for(size_t i = primitivesIdx; i<p_idxEnd; i++) {
                size_t primitiveIdx = p_primitives[i].id;
                io_primitivesHierarchy[i] = m_primitives[primitiveIdx]; // obs: m_primitives
            }
            node->asLeaf(primitivesIdx, primitivesCnt, bbox);
            return node;
        }

        size_t idxMid = (p_idxStart + p_idxEnd) / 2;
        // Use split on equal counts (more methods here if you so desire):
        std::nth_element(&p_primitives[p_idxStart], &p_primitives[idxMid],
                         &p_primitives[p_idxEnd - 1] + 1, ComparePoints(idxMaxSpan));
        Bvh_Node* node_left = buildRecursive(p_primitives, p_idxStart, idxMid,
                                             io_totalNodes, io_primitivesHierarchy);
        Bvh_Node* node_right = buildRecursive(p_primitives, idxMid, p_idxEnd,
                                              io_totalNodes, io_primitivesHierarchy);
        node->asBranch(idxMaxSpan, node_left, node_right);
    }
    return node;
}

size_t Bvh::flattenRecursive(Bvh_Node* p_node, size_t* p_nodeIdx) {
    Bvh_Node_Flat* node_flat = &m_nodes_flat[*p_nodeIdx];
    node_flat->bbox = p_node->bbox;
    size_t nodeIdx = (*p_nodeIdx)++;
    if(p_node->primitivesCnt>0) { // Leaf node, simply copy.
        assert(p_node->children[0]==nullptr && p_node->children[1]==nullptr);
        node_flat->primitivesIdx = p_node->primitivesIdx;
        node_flat->primitivesCnt = p_node->primitivesCnt;
    } else { // Flat out branch node.
        node_flat->primitivesCnt = (-1) * p_node->axisSplit; // Negative to indicate it's an axis.
        flattenRecursive(p_node->children[0], p_nodeIdx);
        node_flat->secondChildOffset = flattenRecursive(p_node->children[1], p_nodeIdx);
    }
    return nodeIdx;
}

void Bvh::freeRecursive(Bvh_Node* p_node) {
    if(p_node->primitivesCnt==0) {
        for(size_t i = 0; i < 2; i++) {
            assert(p_node->children[i]!=nullptr);
            freeRecursive(p_node->children[i]);
            delete p_node->children[i];
        }
    }
}
