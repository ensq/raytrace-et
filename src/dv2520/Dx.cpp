#include <stdafx.h>

#include <Dx.h>
#include <Ant.h>
#include <Win.h>
#include <Obj.h>
#include <CogFx.h>
#include <CogCb.h>
#include <CogSS.h>
#include <CogD3d.h>
#include <CogTex.h>
#include <CogGeo.h>
#include <BufUav.h>
#include <structs.h>
#include <TimerD3d.h>

Dx::Dx(Win& p_win) {
    m_win = &p_win;

    m_uavBackbuffer = nullptr;
    m_rtvBackbuffer = nullptr;
    m_uavRays = nullptr;
    m_uavIntersections = nullptr;
    m_uavColor = nullptr;
    m_cogD3d = nullptr;
    m_cogFx = nullptr;
    m_cogCb = nullptr;
    m_cogSS = nullptr;
    m_cogTex = nullptr;
    m_cogGeo = nullptr;
    m_timer = nullptr;
}
Dx::~Dx() {
    ASSERT_RELEASE(m_uavBackbuffer);
    ASSERT_RELEASE(m_rtvBackbuffer);

    ASSERT_DELETE(m_uavRays);
    ASSERT_DELETE(m_uavIntersections);
    ASSERT_DELETE(m_uavColor);
    ASSERT_DELETE(m_cogD3d);
    ASSERT_DELETE(m_cogFx);
    ASSERT_DELETE(m_cogCb);
    ASSERT_DELETE(m_cogSS);
    ASSERT_DELETE(m_cogTex);
    ASSERT_DELETE(m_cogGeo);
    ASSERT_DELETE(m_timer);
}

HRESULT Dx::init() {
    HRESULT hr = S_FALSE;

    // Initialize D3D first-of-all:
    m_cogD3d = new CogD3d(*m_win);
    hr = m_cogD3d->init();
    D3d d3d = m_cogD3d->getD3d();

    // Initialize backbuffer
    if(SUCCEEDED(hr)) {
        ID3D11Texture2D* backbufferTex;
        HRESULT hr = d3d.swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbufferTex);
        if(SUCCEEDED(hr)) {
            hr = d3d.device->CreateUnorderedAccessView(backbufferTex, NULL, &m_uavBackbuffer);
        }
        if(SUCCEEDED(hr)) {
            hr = d3d.device->CreateRenderTargetView(backbufferTex, NULL, &m_rtvBackbuffer);
        }
        ASSERT_RELEASE(backbufferTex);
    }

    // Initialize AntTweakBar
    if(SUCCEEDED(hr)) {
        Singleton<Ant>::get().init(d3d.device, m_win->getWidth(), m_win->getHeight());
    }

    // ...then continue initializing rest of the cogs:
    if(SUCCEEDED(hr)) {
        m_cogFx = new CogFx();
        hr = m_cogFx->init(d3d.device);
    }
    if(SUCCEEDED(hr)) {
        m_cogCb = new CogCb();
        hr = m_cogCb->init(d3d.device);

        float aspect = ((float)m_win->getWidth()) / ((float)m_win->getHeight());     // Rubbish, get rid of this:
        float fov = (float)PI/4;
        float zFar = 1000;
        float zNear = 1;
        // Per instance CB only needs to be updated once:
        CbPerInstance cbPerInstance;
        cbPerInstance.screenWidth = m_win->getWidth();
        cbPerInstance.screenHeight = m_win->getHeight();
        cbPerInstance.aspect = aspect;
        cbPerInstance.fov = fov;
        m_cogCb->mapCbPerInstance(d3d.devcon, cbPerInstance);
    }
    if(SUCCEEDED(hr)) {
        m_cogSS = new CogSS();
        hr = m_cogSS->init(d3d.device);
    }
    if(SUCCEEDED(hr)) {
        m_cogTex = new CogTex();
        hr = m_cogTex->init(d3d.device);
    }
    if(SUCCEEDED(hr)) {
        m_cogGeo = new CogGeo();
        hr = m_cogGeo->init();
    }

    // Initialize UAVs:
    unsigned numPixels = m_win->getWidth() * m_win->getHeight();
    if(SUCCEEDED(hr)) {
        m_uavRays = new BufUav(numPixels, sizeof(Ray), DXGI_FORMAT_UNKNOWN);
        hr = m_uavRays->init(d3d.device);
    }
    if(SUCCEEDED(hr)) {
        m_uavIntersections = new BufUav(numPixels, sizeof(Intersection), DXGI_FORMAT_UNKNOWN);
        hr = m_uavIntersections->init(d3d.device);
    }
    if(SUCCEEDED(hr)) {
        m_uavColor = new BufUav(numPixels, sizeof(Vec4F), DXGI_FORMAT_UNKNOWN);
        hr = m_uavColor->init(d3d.device);
    }

    // Initialize timer
    if(SUCCEEDED(hr)) {
        m_timer = new TimerD3d();
        hr = m_timer->init(d3d.device);
    }

    return hr;
}
HRESULT Dx::render(double p_delta, Vec3F& p_pos, Mat4F& p_view, Mat4F& p_proj) {
    D3d d3d = m_cogD3d->getD3d();
    d3d.devcon->ClearRenderTargetView(m_rtvBackbuffer,
                                      DxClearColor::Black);

    m_cogGeo->update(d3d.device, d3d.devcon); // Update scene geometry.

    // Update per-frame CB:
    Mat4F viewInv = p_view;
    viewInv.inverse();
    Mat4F projInv = p_proj;
    projInv.inverse();

    CbPerFrame cbPerFrame;
    cbPerFrame.view = p_view;
    cbPerFrame.viewInv = viewInv;
    cbPerFrame.proj = p_proj;
    cbPerFrame.projInv = projInv;
    cbPerFrame.pos = p_pos;
    cbPerFrame.instancesCnt = m_cogGeo->getInstancesCnt();
    cbPerFrame.lightsCnt = m_cogGeo->getLightsCnt();
    m_cogCb->mapCbPerFrame(d3d.devcon, cbPerFrame);

    // Set CBs:
    m_cogCb->setCbs(d3d.devcon);

    // Set SRVs:
    ID3D11ShaderResourceView* srvs[] = {
        m_cogGeo->getSrvVertices(),
        m_cogGeo->getSrvIndices(),
        m_cogGeo->getSrvInstances(),
        m_cogGeo->getSrvLights(),
        m_cogGeo->getSrvNodes(),
        m_cogTex->getTex(d3d.device, Texs_default)->getSrv() // Make this more general.
    };
    d3d.devcon->CSSetShaderResources(0, 6, srvs);

    // Set UAVs:
    ID3D11UnorderedAccessView* uavs[] = {
        m_uavRays->getUav(),
        m_uavIntersections->getUav(),
        m_uavColor->getUav(),
        m_uavBackbuffer
    };
    d3d.devcon->CSSetUnorderedAccessViews(0, 4, uavs, NULL);

    // Set Samplerstates:
    ID3D11SamplerState* sss[] = { m_cogSS->getSamplerState(SSs_default) };
    d3d.devcon->CSSetSamplers(0, 1, sss);

    Singleton<Ant>::get().setTimeRaysGenerate(dispatch(d3d.devcon, Fxs_CS_RAYSGENERATE));
#define NUM_BOUNCES 2
    for(unsigned i = 0; i<2; i++) {
        Singleton<Ant>::get().setTimeRaysInterect(dispatch(d3d.devcon, Fxs_CS_RAYSINTERSECT));
        Singleton<Ant>::get().setTimeLighting(dispatch(d3d.devcon, Fxs_CS_LIGHTING));
    }

    // Unset SRVs:
    ID3D11ShaderResourceView* srvsUnset[] = { NULL, NULL, NULL, NULL, NULL, NULL };
    d3d.devcon->CSSetShaderResources(0, 6, srvsUnset);

    // Unset UAVs:
    ID3D11UnorderedAccessView* uavsUnset[] = { NULL, NULL, NULL, NULL };
    d3d.devcon->CSSetUnorderedAccessViews(0, 4, uavsUnset, NULL);

    // Unset Samplerstates:
    ID3D11SamplerState* sssUnset[] = { NULL };
    d3d.devcon->CSSetSamplers(0, 1, sssUnset);

    // Render GUI
    d3d.devcon->OMSetRenderTargets(1, &m_rtvBackbuffer, NULL);
    Singleton<Ant>::get().render();
    d3d.devcon->OMSetRenderTargets(0, NULL, NULL);

    return d3d.swapChain->Present(0, 0);
}
double Dx::dispatch(ID3D11DeviceContext* p_devcon, Fxs p_fx) {
    unsigned dX = (unsigned)ceil((float)m_win->getWidth() / (float)BLOCK_SIZE);
    unsigned dY = (unsigned)ceil((float)m_win->getHeight() / (float)BLOCK_SIZE);

    m_cogFx->fxSet(p_devcon, p_fx);
    m_timer->start(p_devcon);
    p_devcon->Dispatch(dX, dY, 1);
    m_timer->stop(p_devcon);
    m_cogFx->fxUnset(p_devcon, p_fx);

    return m_timer->time(p_devcon);
}
