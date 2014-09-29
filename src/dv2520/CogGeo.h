#ifndef DV2520_COGGEO_H
#define DV2520_COGGEO_H

typedef long HRESULT;

class Obj;

#include <Bvh.h>
#include <structs.h>
#include <geometry.h>
#include <BufStreamSrv.h>

class CogGeo {
 public:
    CogGeo();
    ~CogGeo();
    
    HRESULT init(ID3D11Device* p_device, ID3D11DeviceContext* p_devcon);
    void update(ID3D11Device* p_device, ID3D11DeviceContext* p_devcon);

    size_t getLightsCnt() const;
    size_t getInstancesCnt() const;
    ID3D11ShaderResourceView* getSrvVertices();
    ID3D11ShaderResourceView* getSrvIndices();
    ID3D11ShaderResourceView* getSrvInstances();
    ID3D11ShaderResourceView* getSrvLights();
    ID3D11ShaderResourceView* getSrvNodes();
 protected:
 private:
    size_t m_lightsCnt;
    size_t m_instancesCnt;
    
    BufStreamSrv<Vertex>* m_srvStreamVertices;
    BufStreamSrv<unsigned>* m_srvStreamIndices;
    BufStreamSrv<ObjInstance>* m_srvStreamInstances;
    BufStreamSrv<LightPoint>* m_srvStreamLights;
    BufStreamSrv<Bvh_Node_Flat>* m_srvStreamNodes;

    std::vector<Obj*> m_objects;

    // temp:
    Bvh_Node_Flat* m_nodes;
    unsigned numNodes;
};

#endif // DV2520_COGGEO_H

// void update
