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

    m_lo = nullptr;
    m_hi = nullptr;
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

    ASSERT_DELETE(m_hi);
    ASSERT_DELETE(m_lo);
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

        CbPerInstance cbPerInstance; // Per instance CB only needs to be updated once:
        cbPerInstance.screenWidth = m_win->getWidth();
        cbPerInstance.screenHeight = m_win->getHeight();
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

    // Initialize the low fidelity fov:
    if(SUCCEEDED(hr)) {
        float aspect = ((float)m_win->getWidth()) / ((float)m_win->getHeight());
        float fov = (float)RADIAN(45.0f);

        unsigned w, h;
        w = (unsigned)ceil((float)m_win->getWidth() / 4.0f); // dimensions are temp
        h = (unsigned)ceil((float)m_win->getHeight() / 4.0f);
        m_lo = new Fov(w, h, m_win->getWidth(), m_win->getHeight(), 0, 0,
                       fov, aspect, d3d.device, d3d.devcon);
        hr = m_lo->init();
    }
    // Initialize the high fidelity fov:
    if(SUCCEEDED(hr)) {
        float aspect = ((float)m_win->getWidth()) / ((float)m_win->getHeight());
        float fov = (float)RADIAN(11.25f);

        unsigned w, h;
        w = (unsigned)ceil((float)m_win->getWidth() / 4.0f);
        h = (unsigned)ceil((float)m_win->getHeight() / 4.0f);
        m_hi = new Fov(w, h, w, h, 300, 300,
                       fov, aspect, d3d.device, d3d.devcon);
        hr = m_hi->init();
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
        m_cogTex->getTex(d3d.device, Texs_default)->getSrv() // ought be array
    };
    d3d.devcon->CSSetShaderResources(0, NUM_SRVS, srvs);

    // Set common Samplerstates:
    ID3D11SamplerState* sss[] = {m_cogSS->getSamplerState(SSs_default)};
    d3d.devcon->CSSetSamplers(0, 1, sss);

    m_lo->renderToFov(m_cogFx, m_cogCb, p_cam);
    m_hi->renderToFov(m_cogFx, m_cogCb, p_cam);
    
    // Unset SRVs:
    memset(srvs, NULL, sizeof(ID3D11ShaderResourceView*) * NUM_SRVS);
    d3d.devcon->CSSetShaderResources(0, NUM_SRVS, srvs);

    m_lo->renderToBackbuffer(m_cogFx, m_cogCb, m_uavBackbuffer);
    m_hi->renderToBackbuffer(m_cogFx, m_cogCb, m_uavBackbuffer);

    // Unset Samplerstates:
    ID3D11SamplerState* sssUnset[] = { NULL };
    d3d.devcon->CSSetSamplers(0, 1, sssUnset);

    d3d.devcon->OMSetRenderTargets(1, &m_rtvBackbuffer, NULL);
    Singleton<Ant>::get().render();
    d3d.devcon->OMSetRenderTargets(0, NULL, NULL);
    return d3d.swapChain->Present(0, 0);
}
