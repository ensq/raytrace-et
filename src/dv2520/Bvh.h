#ifndef DV2520_BVH_H
#define DV2520_BVH_H

class Obj;

#include <geometry.h>

struct Bvh_Primitive {
    int id;
    Vec3F center;
    BBox bbox;

    Bvh_Primitive();
    Bvh_Primitive(int p_id, const BBox& p_bbox);
    ~Bvh_Primitive();
};

struct Bvh_Node {
    BBox bbox;
    Bvh_Node* children[2];
    size_t primitivesIdx; // firstPrimOffset;
    size_t primitivesCnt; // nPrimitives
    size_t axisSplit;
    
    Bvh_Node();
    ~Bvh_Node();
    void asLeaf(size_t p_primitivesIdx,
                size_t p_primitivesCnt, const BBox& p_bbox);
    void asBranch(size_t p_axisSplit, Bvh_Node* p_l, Bvh_Node* p_r);
};

struct Bvh_Node_Flat {
    BBox bbox;
    union {
        int primitivesCnt; // Leaf.
        int axisSplit; // Branch.
    };
    union {
        size_t primitivesIdx; // Leaf.
        size_t secondChildOffset; // Branch.
    };

    Bvh_Node_Flat();
    ~Bvh_Node_Flat();
};

struct ComparePoints // omg, refactor this
{
    ComparePoints(int d) { dim = d; }
    int dim;
    bool operator()(Bvh_Primitive &a, Bvh_Primitive &b) const 
    {
        return a.center[dim] < b.center[dim];
    }
};

class Bvh {
 public:
    Bvh(Obj* p_obj, size_t p_nodeMaxPrimitives);
    ~Bvh();

    void init();

    Bvh_Node_Flat* getNodes() {
        return m_nodes_flat;
    }
    unsigned getNodesCnt() {
        return totalNodes;
    }
 protected:
 private:
     void build(Vertex* p_vertices,
         size_t p_verticesCnt, 
         unsigned* p_indices,
         size_t p_indicesCnt);
    Bvh_Node* buildRecursive(Bvh_Primitive* p_primitives,
                             size_t p_idxStart,
                             size_t p_idxEnd,
                             size_t* io_totalNodes,
                             Bvh_Primitive* io_primitivesHierarchy);
    size_t flattenRecursive(Bvh_Node* p_node, size_t* p_nodeIdx);
    void freeRecursive(Bvh_Node* p_node);
    
    const Obj* m_obj;
    Bvh_Primitive* m_primitives;
    Bvh_Node_Flat* m_nodes_flat;

    // Different names please:
    unsigned maxPrimsInNode;
    unsigned totalNodes;
};

#endif // DV2520_BVH_H
