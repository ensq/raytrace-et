#include <stdafx.h>

#include <Ant.h>
#include <Fov.h>
#include <Cam.h>
#include <CogFx.h>
#include <CogCb.h>
#include <BufUav.h>
#include <structs.h>
#include <FovTarget.h>

Fov::Fov(DescFov p_desc, ID3D11Device* p_device,
         ID3D11DeviceContext* p_devcon)
    : m_device(p_device), m_devcon(p_devcon) {
    m_width = p_desc.width;
    m_height = p_desc.height;
    m_widthUpscale = p_desc.widthUpscale;
    m_heightUpscale = p_desc.heightUpscale;

    m_ofsX = p_desc.ofsX;
    m_ofsY = p_desc.ofsY;
    m_fov = p_desc.fov;
    m_aspect = p_desc.aspect;

    m_uavRays = nullptr;
    m_uavIntersections = nullptr;
    m_uavColor = nullptr;
    m_target = nullptr;
}
Fov::~Fov() {
    assert(m_uavRays!=nullptr);
    assert(m_uavIntersections!=nullptr);
    assert(m_uavColor!=nullptr);
    assert(m_target!=nullptr);

    delete m_uavRays;
    delete m_uavIntersections;
    delete m_uavColor;
    delete m_target;
}

HRESULT Fov::init() {
    unsigned resolution = m_width * m_height;
    assert(resolution>0);

    size_t tsizes[3] = {sizeof(Ray), sizeof(Intersection), sizeof(Vec4F)};
    BufUav** bufs[3] = {&m_uavRays, &m_uavIntersections, &m_uavColor};
    for(size_t i = 0; i<3; i++) {
        *(bufs[i]) = new BufUav(resolution, tsizes[i], DXGI_FORMAT_UNKNOWN);
    }
    assert(m_uavRays!=nullptr);
    assert(m_uavIntersections!=nullptr);
    assert(m_uavColor!=nullptr);

    HRESULT hr = S_OK;
    for(size_t i = 0; i<3 && hr==S_OK; i++) {
        BufUav* buf = *(bufs[i]);
        hr = buf->init(m_device);
    }

    m_target = new FovTarget(m_width, m_height);
    if(hr==S_OK) {
        hr = m_target->init(m_device);
    }
    return hr;
}

void Fov::renderToFov(CogFx* p_cogFx, CogCb* p_cogCb,
                      Cam* p_cam, bool p_isOffset) {
    m_devcon->ClearRenderTargetView(m_target->getRtv(), DxClearColor::Black);

    Mat4F proj, projInv;
    p_cam->getProj(m_fov, m_aspect, proj);
    projInv = proj;
    projInv.inverse();
    CbPerFov cbPerFov;
    cbPerFov.proj = proj;
    cbPerFov.projInv = projInv;
    cbPerFov.fovWidth = m_width;
    cbPerFov.fovHeight = m_height;
    cbPerFov.fovOfsX = m_ofsX;
    cbPerFov.fovOfsY = m_ofsY;
    cbPerFov.fov = m_fov;
    cbPerFov.aspect = m_aspect;
    p_cogCb->mapCbPerFov(m_devcon, cbPerFov);
    p_cogCb->setCbs(m_devcon);

#define NUM_UAVS 5
    ID3D11UnorderedAccessView* uavs[] = {
        m_uavRays->getUav(),
        m_uavIntersections->getUav(),
        m_uavColor->getUav(),
        m_target->getUav(),
        NULL
    };
    m_devcon->CSSetUnorderedAccessViews(0, NUM_UAVS, uavs, NULL);

    unsigned dX = (unsigned)ceil((float)m_width  / (float)BLOCK_SIZE);
    unsigned dY = (unsigned)ceil((float)m_height / (float)BLOCK_SIZE);
    double dtRays, dtIntersect, dtLighting;
    dtRays = dtIntersect = dtLighting = 0;
    if(p_isOffset==true) {
        dtRays = p_cogFx->dispatch(m_devcon, Fxs_CS_RAYSGENERATEOFFSET, dX, dY);
    } else {
        dtRays = p_cogFx->dispatch(m_devcon, Fxs_CS_RAYSGENERATE, dX, dY);
    }
    #define NUM_BOUNCES 3
    for(unsigned i = 0; i<NUM_BOUNCES; i++) {
        dtIntersect += p_cogFx->dispatch(m_devcon, Fxs_CS_RAYSINTERSECT,
                                         dX, dY);
        dtLighting += p_cogFx->dispatch(m_devcon, Fxs_CS_LIGHTING, dX, dY);
    }
    
    dtRays += Singleton<Ant>::get().getTimeRaysGenerate();
    dtIntersect += Singleton<Ant>::get().getTimeRaysIntersect();
    dtLighting += Singleton<Ant>::get().getTimeLighting();

    Singleton<Ant>::get().setTimeRaysGenerate(dtRays);
    Singleton<Ant>::get().setTimeRaysInterect(dtIntersect);
    Singleton<Ant>::get().setTimeLighting(dtLighting);

    memset(uavs, 0, sizeof(ID3D11UnorderedAccessView*) * NUM_UAVS);
    m_devcon->CSSetUnorderedAccessViews(0, NUM_UAVS, uavs, NULL);
}
void Fov::renderToBackbuffer(CogFx* p_cogFx, CogCb* p_cogCb,
                             ID3D11UnorderedAccessView* p_uavBackbuffer) {
    CbPerFov cbPerFov;
    cbPerFov.fovWidth = m_widthUpscale;
    cbPerFov.fovHeight = m_heightUpscale;
    cbPerFov.fovOfsX = m_ofsX;
    cbPerFov.fovOfsY = m_ofsY;
    p_cogCb->mapCbPerFov(m_devcon, cbPerFov);
    p_cogCb->setCbs(m_devcon);

#define NUM_SRVS 6
#define NUM_UAVS 5
    ID3D11ShaderResourceView* srvs[] = {NULL, NULL, NULL, NULL, NULL,
                                        m_target->getSrv()};
    ID3D11UnorderedAccessView* uavs[] = {NULL, NULL, NULL, NULL,
                                         p_uavBackbuffer};
    m_devcon->CSSetShaderResources(0, NUM_SRVS, srvs);
    m_devcon->CSSetUnorderedAccessViews(0, NUM_UAVS, uavs, NULL);

    unsigned dX = (unsigned)ceil((float)m_widthUpscale  / (float)BLOCK_SIZE);
    unsigned dY = (unsigned)ceil((float)m_heightUpscale / (float)BLOCK_SIZE);
    p_cogFx->dispatch(m_devcon, Fxs_CS_COMBINE, dX, dY);

    memset(srvs, 0, sizeof(ID3D11ShaderResourceView*) * NUM_SRVS);
    m_devcon->CSSetShaderResources(0, NUM_SRVS, srvs);

    memset(uavs, 0, sizeof(ID3D11UnorderedAccessView*) * NUM_UAVS);
    m_devcon->CSSetUnorderedAccessViews(0, NUM_UAVS, uavs, NULL);
}
