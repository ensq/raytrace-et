#ifndef DV2520_BUFSTREAM_H
#define DV2520_BUFSTREAM_H

#define BUFSTREAMSIZE 16

struct ID3D11Buffer;

#include <Util.h>
#include <d3d11.h> // eww.

template<class T>
class BufStream {
  public:
    BufStream(D3D11_BIND_FLAG p_bindFlag,
              D3D11_RESOURCE_MISC_FLAG p_resourceMiscFlag) {
        m_bindFlag = p_bindFlag;
        m_resourceMiscFlag = p_resourceMiscFlag;

        m_elements.resize(BUFSTREAMSIZE);

        m_elementCntCur = 0;
        m_elementCntMax = 0;
        m_buffer = nullptr;
    }
    virtual ~BufStream() {
        // Buffer is not necessarily initialized when de-allocated:
        SAFE_RELEASE(m_buffer);
    }

    void reset() {
        m_elementCntCur = 0;
    }
    void pushElements(T* p_elements, unsigned p_elementsCnt) {
        m_elements.reserve(m_elements.size() + p_elementsCnt);
        m_elements.insert(m_elements.begin() + m_elementCntCur,
                          p_elements, p_elements + p_elementsCnt);
        m_elementCntCur += p_elementsCnt;
    }
    void pushElement(T p_element) {
        if(m_elementCntCur<m_elements.size()) {
            m_elements.at(m_elementCntCur) = p_element;
        } else {
            m_elements.push_back(p_element);
        }
        m_elementCntCur++;
    }

    HRESULT updateBufStream(ID3D11Device* p_device,
                            ID3D11DeviceContext* p_devcon) {
        HRESULT hr = S_OK;
        if(m_elementCntCur>0) {
            hr = streamToBuffer(p_device, p_devcon);
        }
        if(FAILED(hr)) {
            ERR_HR(hr);
        }
        return hr;
    }

    unsigned getElementCntCur() const {
        return m_elementCntCur;
    }
    unsigned getElementCntMax() const {
        return m_elementCntMax;
    }
    ID3D11Buffer* getBuffer() const {
        return m_buffer;
    }
  protected:
    unsigned getNewElementCntMax(unsigned p_maxCur,
                                 unsigned p_maxTarget) const {
        unsigned maxNew = (p_maxCur>0) ? p_maxCur : 1;
        while(maxNew<p_maxTarget) {
            maxNew *= 2;
        }
        return maxNew;
    }

    HRESULT streamToBuffer(ID3D11Device* p_device,
                           ID3D11DeviceContext* p_devcon) {
        HRESULT hr = S_OK;
        if(m_elementCntCur>m_elementCntMax) {
            hr = increaseStreamCapacity(p_device);
        }
        if(SUCCEEDED(hr)) {
            hr = mapToBuffer(p_devcon);
        }
        return hr;
    }
    HRESULT increaseStreamCapacity(ID3D11Device* p_device) {
        m_elementCntMax = getNewElementCntMax(m_elementCntMax, m_elementCntCur);
        return createStream(p_device);
    }
    HRESULT mapToBuffer(ID3D11DeviceContext* p_devcon) {
        D3D11_MAPPED_SUBRESOURCE map;
        HRESULT hr = p_devcon->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD,
                                   0, &map);
        if(SUCCEEDED(hr)) {
            memcpy(map.pData, &m_elements[0],
                   m_elementCntCur * sizeof(T));
            p_devcon->Unmap(m_buffer, 0);
        } else {
            ERR_HR(hr);
        }
        return hr;
    }
    virtual HRESULT createStream(ID3D11Device* p_device) {
        SAFE_RELEASE(m_buffer);
        return createBuffer(p_device);
    }
    HRESULT createBuffer(ID3D11Device* p_device) {
        D3D11_BUFFER_DESC desc;
        ZERO_MEM(desc);
        desc.BindFlags = m_bindFlag;
        desc.ByteWidth = sizeof(T) * m_elementCntMax;
        desc.MiscFlags = m_resourceMiscFlag;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.StructureByteStride = sizeof(T);

        HRESULT hr = p_device->CreateBuffer(&desc, NULL, &m_buffer);
        if(FAILED(hr)) {
            ERR_HR(hr);
        }
        SET_D3D_OBJECT_NAME(m_buffer, "BufStream::m_buffer");
        return hr;
    }

    unsigned m_elementCntCur;
    unsigned m_elementCntMax;
    std::vector<T> m_elements;
    ID3D11Buffer* m_buffer;

    D3D11_BIND_FLAG m_bindFlag;
    D3D11_RESOURCE_MISC_FLAG m_resourceMiscFlag;
  private:
};

#endif // DV2520_BUFSTREAM_H
