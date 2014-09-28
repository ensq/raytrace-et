#include <stdafx.h>

#include <Dx.h>
#include <Ant.h>
#include <Win.h>
#include <Fov.h>
#include <Cam.h>
#include <CogFx.h>
#include <CogCb.h>
#include <CogSS.h>
#include <CogD3d.h>
#include <CogTex.h>
#include <CogGeo.h>

#include <FovTarget.h> // TEMP

Dx::Dx(Win& p_win) {
    m_win = &p_win;

    m_uavBackbuffer = nullptr;
    m_rtvBackbuffer = nullptr;

    m_cogD3d = nullptr;
    m_cogFx = nullptr;
    m_cogCb = nullptr;
    m_cogSS = nullptr;
    m_cogTex = nullptr;
    m_cogGeo = nullptr;
}
Dx::~Dx() {
    ASSERT_RELEASE(m_uavBackbuffer);
    ASSERT_RELEASE(m_rtvBackbuffer);

    ASSERT_DELETE(m_cogD3d);
    ASSERT_DELETE(m_cogFx);
    ASSERT_DELETE(m_cogCb);
    ASSERT_DELETE(m_cogSS);
    ASSERT_DELETE(m_cogTex);
    ASSERT_DELETE(m_cogGeo);

    ASSERT_DELETE(m_fov);
    ASSERT_DELETE(m_fov2);
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
        HRESULT hr = d3d.swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), 
                                              (LPVOID*)&backbufferTex);
        if(SUCCEEDED(hr)) {
            hr = d3d.device->CreateUnorderedAccessView(backbufferTex, NULL,
                                                       &m_uavBackbuffer);
        }
        if(SUCCEEDED(hr)) {
            hr = d3d.device->CreateRenderTargetView(backbufferTex, NULL,
                                                    &m_rtvBackbuffer);
        }
        ASSERT_RELEASE(backbufferTex);
    }

    // Initialize AntTweakBar
    if(SUCCEEDED(hr)) {
        Singleton<Ant>::get().init(d3d.device, m_win->getWidth(),
                                   m_win->getHeight());
    }

    // ...then continue initializing rest of the cogs:
    if(SUCCEEDED(hr)) {
        m_cogFx = new CogFx();
        hr = m_cogFx->init(d3d.device);
    }
    if(SUCCEEDED(hr)) {
        m_cogCb = new CogCb();
        hr = m_cogCb->init(d3d.device);

        // Rubbish, get rid of this:
        //float aspect = ((float)m_win->getWidth()) / ((float)m_win->getHeight());
        //float fov = (float)PI/4;
        float zFar = 1000;
        float zNear = 1;
        // Per instance CB only needs to be updated once:
        CbPerInstance cbPerInstance;
        cbPerInstance.screenWidth = m_win->getWidth();
        cbPerInstance.screenHeight = m_win->getHeight();
        //cbPerInstance.aspect = aspect;
        //cbPerInstance.fov = fov;
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

    if(SUCCEEDED(hr)) {
        float aspect = ((float)m_win->getWidth()) / ((float)m_win->getHeight());
        float fov = (float)RADIAN(45.0f);
        float fov2 = (float)RADIAN(11.25f);

        unsigned w, h;
        w = (unsigned)ceil((float)m_win->getWidth() / 4.0f);
        h = (unsigned)ceil((float)m_win->getHeight() / 4.0f);
        m_fov = new Fov(w, h, 0, 0, fov, aspect, d3d.device, d3d.devcon);
        hr = m_fov->init();

        w = (unsigned)ceil((float)m_win->getWidth() / 4.0f);
        h = (unsigned)ceil((float)m_win->getHeight() / 4.0f);
        m_fov2 = new Fov(w, h, 300, 300, fov2, aspect, d3d.device, d3d.devcon);
        hr = m_fov2->init();
    }

    return hr;
}
HRESULT Dx::render(double p_delta, Cam* p_cam) {
    D3d d3d = m_cogD3d->getD3d();
    d3d.devcon->ClearRenderTargetView(m_rtvBackbuffer,
                                      DxClearColor::Black);

    // Update scene geomatry:
    m_cogGeo->update(d3d.device, d3d.devcon);

    // Update per-frame constant buffer:
    Mat4F view, viewInv;
    p_cam->getView(view);
    viewInv = view;
    viewInv.inverse();
    CbPerFrame cbPerFrame;
    cbPerFrame.view = view;
    cbPerFrame.viewInv = viewInv;
    cbPerFrame.pos = p_cam->getPos();
    cbPerFrame.instancesCnt = m_cogGeo->getInstancesCnt();
    cbPerFrame.lightsCnt = m_cogGeo->getLightsCnt();
    m_cogCb->mapCbPerFrame(d3d.devcon, cbPerFrame);
    m_cogCb->setCbs(d3d.devcon);

#define NUM_SRVS 6
    ID3D11ShaderResourceView* srvs[] = {
        m_cogGeo->getSrvVertices(),
        m_cogGeo->getSrvIndices(),
        m_cogGeo->getSrvInstances(),
        m_cogGeo->getSrvLights(),
        m_cogGeo->getSrvNodes(),
        m_cogTex->getTex(d3d.device, Texs_default)->getSrv() // Make this more general.
    };
    d3d.devcon->CSSetShaderResources(0, NUM_SRVS, srvs);

    // Set common Samplerstates:
    ID3D11SamplerState* sss[] = {m_cogSS->getSamplerState(SSs_default)};
    d3d.devcon->CSSetSamplers(0, 1, sss);

    m_fov->render(m_cogFx, m_cogCb, p_cam);
    m_fov2->render(m_cogFx, m_cogCb, p_cam);
    
    // Unset SRVs:
    memset(srvs, NULL, sizeof(ID3D11ShaderResourceView*) * NUM_SRVS);
    d3d.devcon->CSSetShaderResources(0, NUM_SRVS, srvs);

    // TEST
    CbPerFov cbPerFov;
    cbPerFov.fovWidth = m_win->getWidth();
    cbPerFov.fovHeight = m_win->getHeight();
    cbPerFov.fovOfsX = 0;
    cbPerFov.fovOfsY = 0;
    m_cogCb->mapCbPerFov(d3d.devcon, cbPerFov);
    m_cogCb->setCbs(d3d.devcon);

    ID3D11ShaderResourceView* srvsext[] = {NULL, NULL, NULL, NULL, NULL, NULL, m_fov->getFovTarget()->getSrv()};
    ID3D11UnorderedAccessView* uavs[] = {NULL, NULL, NULL, NULL, m_uavBackbuffer};
    d3d.devcon->CSSetShaderResources(0, 7, srvsext);
    d3d.devcon->CSSetUnorderedAccessViews(0, 5, uavs, NULL);
    unsigned dX = (unsigned)ceil((float)m_win->getWidth()  / (float)BLOCK_SIZE);
    unsigned dY = (unsigned)ceil((float)m_win->getHeight() / (float)BLOCK_SIZE);
    m_cogFx->dispatch(d3d.devcon, Fxs_CS_COMBINE,
                      dX, dY);
    
    srvsext[6] = NULL;
    uavs[4] = NULL;
    d3d.devcon->CSSetShaderResources(0, 7, srvsext);
    d3d.devcon->CSSetUnorderedAccessViews(0, 5, uavs, NULL);    
    // TEST

    // TEST2
    cbPerFov.fovWidth = 200;
    cbPerFov.fovHeight = 200;
    cbPerFov.fovOfsX = 300;
    cbPerFov.fovOfsY = 300;
    m_cogCb->mapCbPerFov(d3d.devcon, cbPerFov);
    m_cogCb->setCbs(d3d.devcon);

    srvsext[6] = m_fov2->getFovTarget()->getSrv();
    d3d.devcon->CSSetShaderResources(0, 7, srvsext);
    uavs[4] = m_uavBackbuffer;
    d3d.devcon->CSSetUnorderedAccessViews(0, 5, uavs, NULL);
    dX = (unsigned)ceil((float)200  / (float)BLOCK_SIZE);
    dY = (unsigned)ceil((float)200 / (float)BLOCK_SIZE);
    m_cogFx->dispatch(d3d.devcon, Fxs_CS_COMBINE,
                      dX, dY);
    
    srvsext[6] = NULL;
    uavs[4] = NULL;
    d3d.devcon->CSSetShaderResources(0, 7, srvsext);
    d3d.devcon->CSSetUnorderedAccessViews(0, 5, uavs, NULL);    
    // TEST2

    // Unset Samplerstates:
    ID3D11SamplerState* sssUnset[] = { NULL };
    d3d.devcon->CSSetSamplers(0, 1, sssUnset);

    // Render GUI
    d3d.devcon->OMSetRenderTargets(1, &m_rtvBackbuffer, NULL);
    Singleton<Ant>::get().render();
    d3d.devcon->OMSetRenderTargets(0, NULL, NULL);

    return d3d.swapChain->Present(0, 0);
}
