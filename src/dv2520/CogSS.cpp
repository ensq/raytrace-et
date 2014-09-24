#include <stdafx.h>

#include <CogSS.h>

CogSS::CogSS() {

}
CogSS::~CogSS() {
    for(auto it = m_sss.begin(); it != m_sss.end(); it++) {
        ASSERT_RELEASE(it->second);
    }
}

HRESULT CogSS::init(ID3D11Device* p_device) {
    HRESULT hr = S_OK;

    // Create default samplerstate.
    D3D11_SAMPLER_DESC desc;
    ZERO_MEM(desc);
    desc.Filter            = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    desc.AddressU        = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.AddressV        = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.AddressW        = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    desc.MinLOD            = 0;
    desc.MaxLOD            = D3D11_FLOAT32_MAX;

    ID3D11SamplerState* ss;
    hr = p_device->CreateSamplerState(&desc, &ss);
    if(SUCCEEDED(hr)) {
        m_sss[ SSs_default ] = ss;
    } else {
        Util::msgBoxShowAndTerminateProcess("CogSS::init CreateSamplerState failed!");
    }
    return hr;
}

ID3D11SamplerState* CogSS::getSamplerState(SSs p_ss) const {
    ID3D11SamplerState* ss = nullptr;

    auto ssIt = m_sss.find(p_ss);
    if(ssIt!=m_sss.end()) {
        ss = ssIt->second;
    } else {
        throw ExceptionDv2520("CogSS::getSamplerState Requested samplerstate is not initialized!");
    }
    return ss;
}