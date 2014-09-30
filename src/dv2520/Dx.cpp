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
#include <CogFov.h>

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
    m_cogFov = nullptr;
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
    ASSERT_DELETE(m_cogFov);
}

HRESULT Dx::init() {
    HRESULT hr = S_FALSE;

    m_cogD3d = new CogD3d(*m_win);
    hr = m_cogD3d->init();
    D3d d3d = m_cogD3d->getD3d();
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
    if(SUCCEEDED(hr)) {
        Singleton<Ant>::get().init(d3d.device, m_win->getWidth(),
                                   m_win->getHeight());
    }

    if(SUCCEEDED(hr)) {
        m_cogFx = new CogFx();
        hr = m_cogFx->init(d3d.device);
    }
    if(SUCCEEDED(hr)) {
        m_cogCb = new CogCb();
        hr = m_cogCb->init(d3d.device);

        CbPerInstance cbPerInstance;
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
        hr = m_cogGeo->init(d3d.device, d3d.devcon);
    }
    if(SUCCEEDED(hr)) {
        m_cogFov = new CogFov(m_win->getWidth(), m_win->getHeight(),
                              (float)RADIAN(45.0f));
        hr = m_cogFov->init(d3d.device, d3d.devcon);
    }

    return hr;
}
HRESULT Dx::render(double p_delta, Cam* p_cam,
                   double p_eyePosX, double p_eyePosY) {
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

    m_cogFov->render(m_cogFx, m_cogCb, p_cam, p_eyePosX, p_eyePosY,
                     d3d.device, d3d.devcon);
    
    // Unset SRVs:
    memset(srvs, NULL, sizeof(ID3D11ShaderResourceView*) * NUM_SRVS);
    d3d.devcon->CSSetShaderResources(0, NUM_SRVS, srvs);

    m_cogFov->combine(m_cogFx, m_cogCb, m_uavBackbuffer);

    // Unset Samplerstates:
    ID3D11SamplerState* sssUnset[] = { NULL };
    d3d.devcon->CSSetSamplers(0, 1, sssUnset);

    d3d.devcon->OMSetRenderTargets(1, &m_rtvBackbuffer, NULL);
    Singleton<Ant>::get().render();
    d3d.devcon->OMSetRenderTargets(0, NULL, NULL);
    return d3d.swapChain->Present(0, 0);
}
