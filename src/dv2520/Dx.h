#ifndef DV2520_DX_H
#define DV2520_DX_H

struct ID3D11RenderTargetView;
struct ID3D11UnorderedAccessView;

class Win;
class Fov;
class Cam;
class CogFx;
class CogCb;
class CogSS;
class CogD3d;
class CogTex;
class CogGeo;
class CogFov;

class Dx {
  public:
    Dx(Win& p_win);
     ~Dx();

     HRESULT init();
     HRESULT render(double p_delta, Cam* p_cam,
                    double p_eyePosX, double p_eyePosY);
   protected:
   private:
     ID3D11UnorderedAccessView* m_uavBackbuffer;
     ID3D11RenderTargetView* m_rtvBackbuffer;
     
     CogD3d* m_cogD3d;
     CogFx* m_cogFx;
     CogCb* m_cogCb;
     CogSS* m_cogSS;
     CogTex* m_cogTex;
     CogGeo* m_cogGeo;
     CogFov* m_cogFov;

     Win* m_win;
 };

 #endif // DV2520_DX_H
