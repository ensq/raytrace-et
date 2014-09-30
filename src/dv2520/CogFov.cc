#include <stdafx.h>

#include <Cam.h>
#include <CogFx.h>
#include <CogCb.h>
#include <CogFov.h>

CogFov::CogFov(unsigned p_screenWidth, unsigned p_screenHeight,
               float p_fov) : m_screenWidth(p_screenWidth),
                              m_screenHeight(p_screenHeight) {
    assert(p_screenWidth>=s_widthHi);
    assert(p_screenHeight>=s_heightHi);

    m_descLo.fov = p_fov;
    m_descLo.width = s_widthLo;
    m_descLo.height = s_heightLo;
    m_descLo.widthUpscale = p_screenWidth;
    m_descLo.heightUpscale = p_screenHeight;
    m_descLo.ofsX = 0;
    m_descLo.ofsY = 0;
    m_descLo.aspect = (float)m_descLo.width / (float)m_descLo.height;

    m_descHi.fov = p_fov;
    m_descHi.width = s_widthHi;
    m_descHi.height = s_heightHi;
    m_descHi.widthUpscale = s_widthHi;
    m_descHi.heightUpscale = s_heightHi;
    // Only initial offset values:
    m_descHi.ofsX = (p_screenWidth / 2) - s_widthHi / 2;
    m_descHi.ofsY = (p_screenHeight / 2) - s_widthHi / 2;
    m_descHi.aspect = (float)m_descHi.width / (float)m_descHi.height;
}
CogFov::~CogFov() {
    assert(m_lo!=nullptr);
    assert(m_hi!=nullptr);

    ASSERT_DELETE(m_lo);
    ASSERT_DELETE(m_hi);
}

HRESULT CogFov::init(ID3D11Device* p_device, ID3D11DeviceContext* p_devcon) {
    m_lo = new Fov(m_descLo, p_device, p_devcon);
    HRESULT hr = m_lo->init();
    if(SUCCEEDED(hr)) {
        m_hi = new Fov(m_descHi, p_device, p_devcon);
        hr = m_hi->init();
    }
    return hr;
}

void CogFov::render(CogFx* p_cogFx, CogCb* p_cogCb, Cam* p_cam,
                    double p_eyePosX, double p_eyePosY, ID3D11Device* p_device,
                    ID3D11DeviceContext* p_devcon) {
    int pixelOfsX = p_eyePosX - m_descHi.width / 2;
    int pixelOfsY = p_eyePosY - m_descHi.height / 2;
    unsigned maxOfsX = m_screenWidth - m_descHi.width;
    unsigned maxOfsY = m_screenHeight - m_descHi.height;
    pixelOfsX = clip<int>(pixelOfsX, 0, maxOfsX);
    pixelOfsY = clip<int>(pixelOfsY, 0, maxOfsY);
    m_hi->setOfs(pixelOfsX, pixelOfsY);

    m_lo->renderToFov(p_cogFx, p_cogCb, p_cam, false);
    m_hi->renderToFov(p_cogFx, p_cogCb, p_cam, true);
}

void CogFov::combine(CogFx* p_cogFx, CogCb* p_cogCb,
                     ID3D11UnorderedAccessView* p_uavBackbuffer) {
    m_lo->renderToBackbuffer(p_cogFx, p_cogCb, p_uavBackbuffer);
    m_hi->renderToBackbuffer(p_cogFx, p_cogCb, p_uavBackbuffer);
}
