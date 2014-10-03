#ifndef DV2520_COGGEO_H
#define DV2520_COGGEO_H

typedef long HRESULT;

class Obj;

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
 protected:
 private:
    size_t m_lightsCnt;
    size_t m_instancesCnt;
    
    BufStreamSrv<Vertex>* m_srvStreamVertices;
    BufStreamSrv<unsigned>* m_srvStreamIndices;
    BufStreamSrv<ObjInstance>* m_srvStreamInstances;
    BufStreamSrv<LightPoint>* m_srvStreamLights;

    std::vector<Obj*> m_objects;
};

#endif // DV2520_COGGEO_H

// void update
