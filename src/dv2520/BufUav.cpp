#include <stdafx.h>

#include <BufUav.h>

BufUav::BufUav(unsigned p_tNum, unsigned p_tSize, DXGI_FORMAT p_tFormat) : Buf(p_tNum, p_tSize) {
    m_tFormat = p_tFormat;
    m_uav = nullptr;
}
BufUav::~BufUav() {
    ASSERT_RELEASE(m_uav);
}

HRESULT BufUav::init(ID3D11Device* p_device) {
    HRESULT hr = initBuf(p_device);
    if(SUCCEEDED(hr)) {
        hr = initUav(p_device);
    }

    return hr;
}

ID3D11UnorderedAccessView* BufUav::getUav() const {
    return m_uav;
}

HRESULT BufUav::initBuf(ID3D11Device* p_device) {
    D3D11_BUFFER_DESC desc;
    ZERO_MEM(desc);
    desc.ByteWidth    = getByteWidth();
    desc.BindFlags    = D3D11_BIND_UNORDERED_ACCESS;
    desc.Usage        = D3D11_USAGE_DEFAULT;
    desc.MiscFlags    = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    desc.StructureByteStride = getTSize();

    return Buf::initBuf(p_device, desc);
}
HRESULT BufUav::initUav(ID3D11Device* p_device) {
    D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
    ZERO_MEM(desc);
    desc.Format                    = m_tFormat;
    desc.ViewDimension            = D3D11_UAV_DIMENSION_BUFFER;
    desc.Buffer.FirstElement    = 0;
    desc.Buffer.NumElements        = getTNum();

    ID3D11Buffer* buf = getBuf();
    return p_device->CreateUnorderedAccessView(buf, &desc, &m_uav);
}