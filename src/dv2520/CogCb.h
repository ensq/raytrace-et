#ifndef DV2520_COGCB_H
#define DV2520_COGCB_H

#include <Cb.h>
#include <Cbs.h>

class CogCb {
  public:
    CogCb();
    ~CogCb();

    HRESULT init(ID3D11Device* p_device);

    void setCbs(ID3D11DeviceContext* p_devcon);

    HRESULT mapCbPerInstance(ID3D11DeviceContext* p_devcon, CbPerInstance& p_new);
    HRESULT mapCbPerFrame(ID3D11DeviceContext* p_devcon, CbPerFrame& p_new);

    template < class T >
    HRESULT mapCb(ID3D11DeviceContext* p_devcon, Cb< T >* p_cb, T* p_new) {
        ID3D11Buffer* cb = p_cb->getBuf();
        D3D11_MAPPED_SUBRESOURCE mapSubRsrc;
        HRESULT hr = p_devcon->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapSubRsrc);
        if(SUCCEEDED(hr)) {
            T* old = (T*)mapSubRsrc.pData;
            memcpy(old, p_new, sizeof(T));
            p_devcon->Unmap(cb, 0);
        } else {
            ERR_HR(hr);
        }
        return hr;
    }
  protected:
  private:
    Cb< CbPerInstance >* m_cbPerInstance;
    Cb< CbPerFrame >* m_cbPerFrame;
};

#endif // DV2520_COGCB_H