#ifndef DV2520_DX_H
#define DV2520_DX_H

#define BLOCK_SIZE 16

enum Fxs;

struct ID3D11RenderTargetView;
struct ID3D11UnorderedAccessView;

class Win;
class Obj;
class CogFx;
class CogCb;
class CogSS;
class CogD3d;
class CogTex;
class BufUav;
class TimerD3d;

#include <Bvh.h>
#include <structs.h>
#include <geometry.h>
#include <BufStreamSrv.h>

namespace DxClearColor {
const FLOAT Black[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
const FLOAT White[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };

const FLOAT Red[ 4 ] = { 1.0f, 0.0f, 0.0f, 1.0f };
const FLOAT Green[ 4 ] = { 0.0f, 1.0f, 0.0f, 1.0f };
const FLOAT Blue[ 4 ] = { 0.0f, 0.0f, 1.0f, 1.0f };
}

class Dx {
  public:
    Dx(Win& p_win);
    ~Dx();

    HRESULT init();
    HRESULT render(double p_delta, Vec3F& p_pos, Mat4F& p_view, Mat4F& p_proj);
  protected:
  private:
    double dispatch(ID3D11DeviceContext* p_devcon, Fxs p_fx);
    bool initObjects(ID3D11Device* p_device);

    ID3D11UnorderedAccessView* m_uavBackbuffer; // These ought to be put into some sort of structure.
    ID3D11RenderTargetView* m_rtvBackbuffer;

    BufStreamSrv< Vertex >* m_srvStreamVertices;
    BufStreamSrv< unsigned >* m_srvStreamIndices;
    BufStreamSrv< ObjInstance >* m_srvStreamInstances;
    BufStreamSrv< LightPoint >* m_srvStreamLights;
    BufStreamSrv< Bvh_Node_Flat >* m_srvStreamNodes;
    BufUav* m_uavRays;
    BufUav* m_uavIntersections;
    BufUav* m_uavColor;

    CogD3d* m_cogD3d;
    CogFx* m_cogFx;
    CogCb* m_cogCb;
    CogSS* m_cogSS;
    CogTex* m_cogTex;

    std::vector< Obj* > m_objects;

    // temp:
    Bvh_Node_Flat* m_nodes;
    unsigned numNodes;

    TimerD3d* m_timer;
    Win* m_win;
};

#endif // DV2520_DX_H
