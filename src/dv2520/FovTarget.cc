#include <stdafx.h>
#include <FovTarget.h>

FovTarget::FovTarget(const unsigned p_width,
                     const unsigned p_height) : m_texWidth(p_width), 
                                                m_texHeight(p_height) {
    assert(m_texWidth>0);
    assert(m_texHeight>0);

    m_tex = nullptr;
    m_srv = nullptr;
    m_uav = nullptr;
    m_rtv = nullptr;

    m_texAliasing = 1;
    m_texMips = 1;
    m_texFormat = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
    m_texUsage = D3D11_USAGE_DEFAULT;
    m_texBindFlags  = D3D11_BIND_RENDER_TARGET
        | D3D11_BIND_SHADER_RESOURCE
        | D3D11_BIND_UNORDERED_ACCESS;

}
FovTarget::~FovTarget() {
    assert(m_tex!=nullptr);
    assert(m_srv!=nullptr);
    assert(m_uav!=nullptr);
    assert(m_rtv!=nullptr);
    
    m_tex->Release();
    m_srv->Release();
    m_uav->Release();
    m_rtv->Release();
}

HRESULT FovTarget::init(ID3D11Device* p_device) {
    HRESULT hr = initTex(p_device);
    if(hr==S_OK) {
        hr = initSrv(p_device);
    }
    if(hr==S_OK) {
        hr = initRtv(p_device);
    }
    if(hr==S_OK) {
        hr = initUav(p_device);
    }
    return hr;
}

ID3D11Texture2D* FovTarget::getTex() {
    assert(m_tex!=nullptr);
    return m_tex;
}
ID3D11ShaderResourceView* FovTarget::getSrv() {
    assert(m_srv!=nullptr);
    return m_srv;
}
ID3D11RenderTargetView* FovTarget::getRtv() {
    assert(m_rtv!=nullptr);
    return m_rtv;
}
ID3D11UnorderedAccessView* FovTarget::getUav() {
    assert(m_uav!=nullptr);
    return m_uav;
}

HRESULT FovTarget::initTex(ID3D11Device* p_device) {
    D3D11_TEXTURE2D_DESC texDesc;
    ZeroMemory(&texDesc, sizeof(texDesc));
    texDesc.Width = m_texWidth;
    texDesc.Height = m_texHeight;
    texDesc.MipLevels = m_texMips;
    texDesc.ArraySize = 1;
    texDesc.Format = m_texFormat;
    texDesc.SampleDesc.Count = m_texAliasing;
    texDesc.Usage = m_texUsage;
    texDesc.BindFlags = m_texBindFlags;

    HRESULT hr = p_device->CreateTexture2D(&texDesc, NULL, &m_tex);
    return hr;
}
HRESULT FovTarget::initSrv(ID3D11Device* p_device) {
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = m_texFormat;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = m_texMips;

    HRESULT hr = p_device->CreateShaderResourceView(m_tex, &srvDesc, &m_srv);
    return hr;
}
HRESULT FovTarget::initRtv(ID3D11Device* p_device) {
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
    rtvDesc.Format = m_texFormat;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0;

    HRESULT hr = p_device->CreateRenderTargetView(m_tex, &rtvDesc, &m_rtv);
    return hr;
}
HRESULT FovTarget::initUav(ID3D11Device* p_device) {
    D3D11_UNORDERED_ACCESS_VIEW_DESC descUav;
    descUav.Format = m_texFormat;
    descUav.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    descUav.Texture2D.MipSlice = 0;
    
    HRESULT hr = p_device->CreateUnorderedAccessView(m_tex, &descUav, &m_uav);
    return hr;
}
