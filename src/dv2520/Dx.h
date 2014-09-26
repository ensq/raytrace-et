#ifndef DV2520_DX_H
#define DV2520_DX_H

struct ID3D11RenderTargetView;
struct ID3D11UnorderedAccessView;

class Win;
class Fov;
class CogFx;
class CogCb;
class CogSS;
class CogD3d;
class CogTex;
class CogGeo;

namespace DxClearColor {    
    const FLOAT Red  [4] = {1.0f, 0.0f, 0.0f, 1.0f};
    const FLOAT Green[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    const FLOAT Blue [4] = {0.0f, 0.0f, 1.0f, 1.0f};
    const FLOAT Black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    const FLOAT White[4] = {1.0f, 1.0f, 1.0f, 1.0f};
}

class Dx {
  public:
    Dx(Win& p_win);
    ~Dx();

    HRESULT init();
    HRESULT render(double p_delta, Vec3F& p_pos,
                   Mat4F& p_view, Mat4F& p_proj);
  protected:
  private:
    ID3D11UnorderedAccessView* m_uavBackbuffer; // These ought to be put into some sort of structure.
    ID3D11RenderTargetView* m_rtvBackbuffer;

    CogD3d* m_cogD3d;
    CogFx* m_cogFx;
    CogCb* m_cogCb;
    CogSS* m_cogSS;
    CogTex* m_cogTex;
    CogGeo* m_cogGeo;

    Fov* m_fov;
    Fov* m_fov2;
    
    Win* m_win;
};

#endif // DV2520_DX_H
