#include <stdafx.h>

#include <Obj.h>
#include <lbo.h>
#include <Bvh.h>
#include <CogGeo.h>
#include <geometry.h>
#include <BufStreamSrv.h>

CogGeo::CogGeo() {
    m_srvStreamVertices = nullptr;
    m_srvStreamIndices = nullptr;
    m_srvStreamInstances = nullptr;
    m_srvStreamLights = nullptr;
    m_srvStreamNodes = nullptr;

    m_lightsCnt = 0;
    m_instancesCnt = 0;
}
CogGeo::~CogGeo() {
    assert(m_srvStreamVertices!=nullptr);
    assert(m_srvStreamIndices!=nullptr);
    assert(m_srvStreamInstances!=nullptr);
    assert(m_srvStreamLights!=nullptr);
    assert(m_srvStreamNodes!=nullptr);

    delete m_srvStreamVertices;
    delete m_srvStreamIndices;
    delete m_srvStreamInstances;
    delete m_srvStreamLights;
    delete m_srvStreamNodes;

    for(size_t i = 0; i<m_objects.size(); i++) {
        assert(m_objects.at(i)!=nullptr);
        delete m_objects.at(i);
    }

    // temp
    assert(m_nodes!=nullptr);
    delete[] m_nodes;
}

HRESULT CogGeo::init(ID3D11Device* p_device, ID3D11DeviceContext* p_devcon) {
    m_srvStreamVertices = new BufStreamSrv<Vertex>();
    m_srvStreamIndices = new BufStreamSrv<unsigned>();
    m_srvStreamInstances = new BufStreamSrv<ObjInstance>();
    m_srvStreamLights = new BufStreamSrv<LightPoint>();
    m_srvStreamNodes = new BufStreamSrv<Bvh_Node_Flat>();

    bool success = true;
    for(unsigned i = 0; i<1 && success==true; i++) {
        std::vector<float> vertices;
        std::vector<unsigned> indices;
        success = lbo::parse("../../../obj/dv2520/box2.obj", vertices, indices);
        assert(success==true);

        std::vector<Vertex> vertices_struct;
        for(size_t i = 0; i<vertices.size(); i+=8) {
            float* p = &vertices.at(i);
            Vertex v(Vec3F(p[0], p[1], p[2]),
                     Vec3F(p[5], p[6], p[7]),
                     Vec2F(p[3], p[4]));
            vertices_struct.push_back(v);
        }
        Obj* obj = new Obj(vertices_struct.size(), indices.size(),
                           &vertices_struct.at(0), &indices.at(0));

        Bvh bvh(obj, 2);
        bvh.init();
        numNodes = bvh.getNodesCnt();
        m_nodes = new Bvh_Node_Flat[numNodes];
        memcpy(m_nodes, bvh.getNodes(), sizeof(Bvh_Node_Flat) * numNodes);

        m_objects.push_back(obj);
    }

    for(unsigned i = 0; i<m_objects.size(); i++) {
        Obj* obj = m_objects.at(i);
        m_srvStreamVertices->pushElements(obj->getVertices(),
                                          obj->getVerticesCnt());
        m_srvStreamIndices->pushElements(obj->getIndices(),
                                         obj->getIndicesCnt());
        m_srvStreamNodes->pushElements(m_nodes, numNodes);
    }
    m_srvStreamVertices->updateBufStream(p_device, p_devcon);
    m_srvStreamIndices->updateBufStream(p_device, p_devcon);
    m_srvStreamInstances->updateBufStream(p_device, p_devcon);
    m_srvStreamNodes->updateBufStream(p_device, p_devcon);

    return success==true ? S_OK : S_FALSE;
}

void CogGeo::update(ID3D11Device* p_device, ID3D11DeviceContext* p_devcon) {
    m_srvStreamInstances->reset();
    m_srvStreamLights->reset();

    m_lightsCnt = 0;
    LightPoint light;
    light.pos = Vec3F(0.0f, 5.0f, 0.0f);
    light.ambient = Vec4F(0.2f, 0.2f, 0.2f, 1.0f);
    light.diffuse = Vec4F(0.0f, 0.6f, 0.0f, 1.0f);
    light.specular = Vec4F(0.0f, 0.8f, 0.0f, 1.0f);
    light.attenuation = Vec3F(0.5f, 1.0f, 0.0f);
    light.dist = 100.0f;
    m_srvStreamLights->pushElement(light);
    m_lightsCnt++;

    light.pos = Vec3F(-2.0, 3.0f, -2.0f);
    light.ambient = Vec4F(0.2f, 0.2f, 0.2f, 1.0f);
    light.diffuse = Vec4F(0.6f, 0.0f, 0.0f, 1.0f);
    light.specular = Vec4F(0.8f, 0.0f, 0.0f, 1.0f);
    light.attenuation = Vec3F(0.5f, 1.0f, 0.0f);
    light.dist = 100.0f;
    m_srvStreamLights->pushElement(light);
    m_lightsCnt++;

    light.pos = Vec3F(2.0f, 3.0f, 2.0f);
    light.ambient = Vec4F(0.2f, 0.2f, 0.2f, 1.0f);
    light.diffuse = Vec4F(0.0f, 0.0f, 0.6f, 1.0f);
    light.specular = Vec4F(0.0f, 0.0f, 0.8f, 1.0f);
    light.attenuation = Vec3F(0.5f, 1.0f, 0.0f);
    light.dist = 100.0f;
    m_srvStreamLights->pushElement(light);
    m_lightsCnt++;

    unsigned indexOffset = 0;
    m_instancesCnt = 0;
    for(unsigned i = 0; i<m_objects.size(); i++) {
        Obj* obj = m_objects.at(i);

        static float rotate = 0.0f;
        obj->getRotation().rotate(0.0f, rotate, 0.0f);
        rotate += 1.0f;

        ObjInstance instance;
        instance.indexStart = indexOffset;
        instance.indexCnt = obj->getIndicesCnt();
        instance.vertexCnt = obj->getVerticesCnt();
        instance.world = obj->getWorldTransform();
        instance.worldInv = obj->getWorldTransform();
        instance.worldInv.inverse();
        indexOffset += instance.indexCnt;

        m_srvStreamInstances->pushElement(instance);
        m_instancesCnt++;
    }
    m_srvStreamInstances->updateBufStream(p_device, p_devcon);
    m_srvStreamLights->updateBufStream(p_device, p_devcon);
}


size_t CogGeo::getLightsCnt() const {
    return m_lightsCnt;
}
size_t CogGeo::getInstancesCnt() const {
    return m_instancesCnt;
}
ID3D11ShaderResourceView* CogGeo::getSrvVertices() {
    return m_srvStreamVertices->getSrv();
}
ID3D11ShaderResourceView* CogGeo::getSrvIndices() {
    return m_srvStreamIndices->getSrv();
}
ID3D11ShaderResourceView* CogGeo::getSrvInstances() {
    return m_srvStreamInstances->getSrv();
}
ID3D11ShaderResourceView* CogGeo::getSrvLights() {
    return m_srvStreamLights->getSrv();
}
ID3D11ShaderResourceView* CogGeo::getSrvNodes() {
    return m_srvStreamNodes->getSrv();
}
