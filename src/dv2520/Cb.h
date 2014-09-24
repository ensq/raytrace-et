#ifndef DV2520_CB_H
#define DV2520_CB_H

// At the moment, we only use dynamic constant buffers.
// Be sure to add support for constant ones as well.
template < class T >
class Cb {
  public:
    Cb() {
        m_buf = nullptr;
    }
    ~Cb() {
        ASSERT_RELEASE(m_buf);
    }

    HRESULT init(ID3D11Device* p_device) {
        unsigned cbSize = sizeof(T);
        if((cbSize % 16)!=0) {
            throw ExceptionDv2520("Constant buffer not multiple of 16!");
        }

        D3D11_BUFFER_DESC desc;
        ZERO_MEM(desc);
        desc.BindFlags        = D3D11_BIND_CONSTANT_BUFFER;
        desc.ByteWidth        = cbSize;
        desc.CPUAccessFlags    = D3D11_CPU_ACCESS_WRITE;
        desc.Usage            = D3D11_USAGE_DYNAMIC;
        return p_device->CreateBuffer(&desc, NULL, &m_buf);
    }

    ID3D11Buffer* getBuf() const {
        return m_buf;
    }
  protected:
  private:
    ID3D11Buffer* m_buf;
};

#endif // DV2520_CB_H