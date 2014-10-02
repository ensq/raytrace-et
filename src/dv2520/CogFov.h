#ifndef DV2520_COGFOV_H
#define DV2520_COGFOV_H

class Cam;
class CogFx;
class CogCb;

#include <Fov.h>

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
    DescFov m_descMi;
    DescFov m_descHi;
    
    Fov* m_lo; // Peripheral
    Fov* m_mi; // Parafovea
    Fov* m_hi; // Fovea
};

#endif // DV2520_COGFOV_H
