#include <stdafx.h>

#include <Fx.h>

Fx::Fx(LPCWSTR p_kernelPath) {
    m_path = p_kernelPath;

    m_blob = nullptr;
}
Fx::~Fx() {
    ASSERT_RELEASE(m_blob);
}

HRESULT Fx::init(ID3D11Device* p_device) {
    HRESULT hr = S_OK;
    hr = D3DReadFileToBlob(m_path, &m_blob);
    if(hr!=S_OK) {
        ERR_HR(hr);
    }

    return hr;
}