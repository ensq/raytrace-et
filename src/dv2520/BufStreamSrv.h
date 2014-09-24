#ifndef DV2520_BUFSTREAMSRV_H
#define DV2520_BUFSTREAMSRV_H

#include <BufStream.h>

template<class T>
class BufStreamSrv : public BufStream<T> {
  public:
    BufStreamSrv() : BufStream(D3D11_BIND_SHADER_RESOURCE, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED) {
        m_srv = nullptr;
    }
    ~BufStreamSrv() {
        SAFE_RELEASE(m_srv);
    }

    ID3D11ShaderResourceView* getSrv() const {
        return m_srv;
    }
  protected:
    HRESULT createStream(ID3D11Device* p_device) {
        SAFE_RELEASE(m_buffer);
        SAFE_RELEASE(m_srv);

        HRESULT hr = createBuffer(p_device);
        if(SUCCEEDED(hr)) {
            hr = createSrv(p_device);
        }
        return hr;
    }
    HRESULT createSrv(ID3D11Device* p_device) {
        D3D11_BUFFER_DESC descBuf;
        ZERO_MEM(descBuf);
        m_buffer->GetDesc(&descBuf);

        D3D11_SHADER_RESOURCE_VIEW_DESC descSrv;
        ZERO_MEM(descSrv);
        descSrv.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
        descSrv.BufferEx.FirstElement = 0;
        descSrv.Format = DXGI_FORMAT_UNKNOWN;
        descSrv.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;

        HRESULT hr = p_device->CreateShaderResourceView(m_buffer, &descSrv, &m_srv);
        if(FAILED(hr)) {
            ERR_HR(hr);
        }
        SET_D3D_OBJECT_NAME(m_srv, "BufStreamSrv::m_srv");
        return hr;
    }
  private:
    ID3D11ShaderResourceView* m_srv;
};

#endif // DV2520_BUFSTREAMSRV_H