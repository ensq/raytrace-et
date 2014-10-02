#include <stdafx.h>

#include <Cam.h>
#include <CogFx.h>
#include <CogCb.h>
#include <CogFov.h>              

// The height of the fovea (in mm) may be computed using:
// x = tan(a) * 700mm
//                  - |
//               -    |
//           -        | x
//       -            |
//   - )a          |- |
//   ------------------
//          700mm
void getFovPixelDim(const int p_fovDegrees,
                    unsigned p_screenWidth,
                    unsigned p_screenHeight,
                    unsigned& io_fovWidth,
                    unsigned& io_fovHeight) {
    const int distToScreenMm = 700;
    const int screenWidthMm = 510;
    const int screenHeightMm = 287;
    const int screenResX = 2048;
    const int screenResY = 1152;
    const float pixelsPerMm = (float)screenResY / (float)screenHeightMm;

    float foveaDegrees_2 = (float)p_fovDegrees / 2.0f; // a
    float foveaMm = tan(RADIAN(foveaDegrees_2)) * 700.0f; // x
    float foveaPixels = foveaMm * pixelsPerMm;
    
    float foveaHeight = foveaPixels;
    float foveaWidth = foveaHeight * ((float)p_screenWidth / (float)p_screenHeight);

    io_fovWidth = ceil(foveaWidth);
    io_fovHeight = ceil(foveaHeight);
}

CogFov::CogFov(unsigned p_screenWidth, unsigned p_screenHeight,
               float p_fov) : m_screenWidth(p_screenWidth),
                              m_screenHeight(p_screenHeight) {
    static const int foveaDegrees = 2;
    static const int parafoveaDegrees = 5;
    
    unsigned m_widthLo;
    unsigned m_heightLo;
    //unsigned m_widthMi;
    //unsigned m_heightMi;
    unsigned m_widthHi;
    unsigned m_heightHi;
    getFovPixelDim(parafoveaDegrees, m_screenWidth, m_screenHeight,
                   m_widthHi, m_heightHi);
    //getFovPixelDim(parafoveaDegrees, m_screenWidth, m_screenHeight,
    //                m_widthMi, m_heightMi);

    m_widthLo = ceil((float)p_screenWidth * 0.25f);
    m_heightLo = ceil((float)p_screenHeight * 0.25f);

    m_descLo.fov = p_fov;
    m_descLo.width = m_widthLo;
    m_descLo.height = m_heightLo;
    m_descLo.widthUpscale = p_screenWidth;
    m_descLo.heightUpscale = p_screenHeight;
    m_descLo.ofsX = 0;
    m_descLo.ofsY = 0;
    m_descLo.aspect = (float)m_descLo.width / (float)m_descLo.height;

    // m_descMi.fov = p_fov;
    // m_descMi.width = ceil((float)m_widthMi * 0.5f);
    // m_descMi.height = ceil((float)m_heightMi * 0.5f);
    // m_descMi.widthUpscale = m_widthMi;
    // m_descMi.heightUpscale = m_heightMi;
    // m_descMi.ofsX = 0;
    // m_descMi.ofsY = 0;
    // m_descMi.aspect = (float)m_descMi.width / (float)m_descMi.height;

    m_descHi.fov = p_fov;
    m_descHi.width = m_widthHi;
    m_descHi.height = m_heightHi;
    m_descHi.widthUpscale = m_widthHi;
    m_descHi.heightUpscale = m_heightHi;
    // Only initial offset values:
    m_descHi.ofsX = (p_screenWidth / 2) - m_widthHi / 2;
    m_descHi.ofsY = (p_screenHeight / 2) - m_widthHi / 2;
    m_descHi.aspect = (float)m_descHi.width / (float)m_descHi.height;

    assert(p_screenWidth>=m_widthHi);
    assert(p_screenHeight>=m_heightHi);

    m_lo = nullptr;
    // m_mi = nullptr;
    m_hi = nullptr;
}
CogFov::~CogFov() {
    assert(m_lo!=nullptr);
    //assert(m_mi!=nullptr);
    assert(m_hi!=nullptr);

    ASSERT_DELETE(m_lo);
    //ASSERT_DELETE(m_mi);
    ASSERT_DELETE(m_hi);
}

HRESULT CogFov::init(ID3D11Device* p_device, ID3D11DeviceContext* p_devcon) {
    m_lo = new Fov(m_descLo, p_device, p_devcon);
    HRESULT hr = m_lo->init();
    // if(SUCCEEDED(hr)) {
    //     m_mi = new Fov(m_descMi, p_device, p_devcon);
    //     hr = m_mi->init();
    // }
    if(SUCCEEDED(hr)) {
        m_hi = new Fov(m_descHi, p_device, p_devcon);
        hr = m_hi->init();
    }
    return hr;
}

void getPixelOfs(double p_eyePosX, double p_eyePosY, unsigned p_fovWidth,
                 unsigned p_fovHeight, unsigned p_screenWidth,
                 unsigned p_screenHeight, int& io_pixelOfsX, int& io_pixelOfsY) {
    int pixelOfsX = p_eyePosX - p_fovWidth / 2;
    int pixelOfsY = p_eyePosY - p_fovHeight / 2;
    unsigned maxOfsX = p_screenWidth - p_fovWidth;
    unsigned maxOfsY = p_screenHeight - p_fovHeight;
    io_pixelOfsX = clip<int>(pixelOfsX, 0, maxOfsX);
    io_pixelOfsY = clip<int>(pixelOfsY, 0, maxOfsY);
}
void CogFov::render(CogFx* p_cogFx, CogCb* p_cogCb, Cam* p_cam,
                    double p_eyePosX, double p_eyePosY, ID3D11Device* p_device,
                    ID3D11DeviceContext* p_devcon) {
    int pixelOfsX, pixelOfsY;
    getPixelOfs(p_eyePosX, p_eyePosY, m_descHi.width, m_descHi.height,
                m_screenWidth, m_screenHeight, pixelOfsX, pixelOfsY);
    m_hi->setOfs(pixelOfsX, pixelOfsY);

    // getPixelOfs(p_eyePosX, p_eyePosY, m_descMi.widthUpscale, m_descMi.heightUpscale,
    //             m_screenWidth, m_screenHeight, pixelOfsX, pixelOfsY);
    // m_mi->setOfs(pixelOfsX, pixelOfsY);

    m_lo->renderToFov(p_cogFx, p_cogCb, p_cam, false);
    //m_mi->renderToFov(p_cogFx, p_cogCb, p_cam, true);
    m_hi->renderToFov(p_cogFx, p_cogCb, p_cam, true);
}

void CogFov::combine(CogFx* p_cogFx, CogCb* p_cogCb,
                     ID3D11UnorderedAccessView* p_uavBackbuffer) {
    m_lo->renderToBackbuffer(p_cogFx, p_cogCb, p_uavBackbuffer);
    // m_mi->renderToBackbuffer(p_cogFx, p_cogCb, p_uavBackbuffer);
    m_hi->renderToBackbuffer(p_cogFx, p_cogCb, p_uavBackbuffer);
}
