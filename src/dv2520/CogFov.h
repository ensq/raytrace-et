#ifndef DV2520_COGFOV_H
#define DV2520_COGFOV_H

class Cam;
class CogFx;
class CogCb;

#include <Fov.h>

// Lo and Hi resolution render target dimensions in pixels
// (independant of total screen resolution):
static const unsigned s_widthLo = 200;
static const unsigned s_heightLo = 200;
static const unsigned s_widthHi = 200;
static const unsigned s_heightHi = 200;

class CogFov {
 public:
    CogFov(unsigned p_screenWidth, unsigned p_screenHeight, float p_fov);
    ~CogFov();

    HRESULT init(ID3D11Device* p_device, ID3D11DeviceContext* p_devcon);

    void render(CogFx* p_cogFx, CogCb* p_cogCb, Cam* p_cam,
                double p_eyePosX, double p_eyePosY,
                ID3D11Device* p_device, ID3D11DeviceContext* p_devcon);
    void combine(CogFx* p_cogFx, CogCb* p_cogCb,
                 ID3D11UnorderedAccessView* p_uavBackbuffer);
 protected:
 private:
    const unsigned m_screenWidth;
    const unsigned m_screenHeight;
    DescFov m_descLo;
    DescFov m_descHi;
    
    Fov* m_lo;
    Fov* m_hi;
};

#endif // DV2520_COGFOV_H
