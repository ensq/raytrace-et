#include <stdafx.h>

#include <Buf.h>

Buf::Buf( unsigned p_tNum, unsigned p_tSize ) {
    m_tNum = p_tNum;
    m_tSize = p_tSize;

    m_buf = nullptr;
}
Buf::~Buf() {
    ASSERT_RELEASE( m_buf );
}

HRESULT Buf::initBuf( ID3D11Device* p_device, D3D11_BUFFER_DESC& p_desc ) {
    return p_device->CreateBuffer( &p_desc, NULL, &m_buf );
}
HRESULT Buf::initBuf( ID3D11Device* p_device, D3D11_BUFFER_DESC& p_desc, D3D11_SUBRESOURCE_DATA& p_initialData ) {
    return p_device->CreateBuffer( &p_desc, &p_initialData, &m_buf );
}

unsigned Buf::getTNum() const {
    return m_tNum;
}
unsigned Buf::getTSize() const {
    return m_tSize;
}
unsigned Buf::getByteWidth() const {
    return m_tSize * m_tNum;
}
ID3D11Buffer* Buf::getBuf() const {
    return m_buf;
}