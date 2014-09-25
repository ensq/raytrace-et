#ifndef DV2520_COGFX_H
#define DV2520_COGFX_H

class Fx;
class TimerD3d;

enum Fxs {
    Fxs_CS_RAYSGENERATE,
    Fxs_CS_RAYSINTERSECT,
    Fxs_CS_LIGHTING,
    Fxs_CS_COMBINE
};

static const LPCWSTR gFxCsoPathCsRaysGenerate = L"CsRaysGenerate.cso";
static const LPCWSTR gFxCsoPathCsRaysIntersect = L"CsRaysIntersect.cso";
static const LPCWSTR gFxCsoPathCsLighting = L"CsLighting.cso";
static const LPCWSTR gFxCsoPathCsCombine = L"CsCombine.cso";

class CogFx {
  public:
    CogFx();
    ~CogFx();

    HRESULT init(ID3D11Device* p_device);

    double dispatch(ID3D11DeviceContext* p_devcon, Fxs p_fx,
                    unsigned p_x, unsigned p_y);
  protected:
  private:
    void fxSet(ID3D11DeviceContext* p_devcon, Fxs p_fx);
    void fxUnset(ID3D11DeviceContext* p_devcon, Fxs p_fx);
    
    Fx* m_csRaysGenerate;
    Fx* m_csRaysIntersect;
    Fx* m_csLighting;
    Fx* m_csCombine;

    TimerD3d* m_timer;
};

#endif // DV2520_COGFX_H
