#include <stdafx.h>

#include <BufSrv.h>

BufSrv::BufSrv( unsigned p_tNum, unsigned p_tSize ) : Buf( p_tNum, p_tSize ) {
	m_data = nullptr;
}
BufSrv::BufSrv( unsigned p_tNum, unsigned p_tSize, void* p_data ) : Buf( p_tNum, p_tSize ) {
	m_data = p_data;
}
BufSrv::~BufSrv() {
	ASSERT_RELEASE( m_srv );
}

HRESULT BufSrv::init( ID3D11Device* p_device ) {
	HRESULT hr = initBuf( p_device );
	if( SUCCEEDED( hr )==true ) {
		hr = initSrv( p_device );
	}
	return hr;
}

ID3D11ShaderResourceView* BufSrv::getSrv() const {
	return m_srv;
}

HRESULT BufSrv::initBuf( ID3D11Device* p_device ) {
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = getTSize() * getTNum();
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.StructureByteStride = getTSize();
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	HRESULT hr = S_OK;
	if( m_data!=nullptr ) {
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = m_data;
		hr = Buf::initBuf( p_device, desc, initData );
	} else {
		hr = Buf::initBuf( p_device, desc );
	}
	return hr;
}
HRESULT BufSrv::initSrv( ID3D11Device* p_device ) {
	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	desc.BufferEx.FirstElement = 0;
	desc.BufferEx.Flags = 0;
	desc.BufferEx.NumElements = getTNum();

	ID3D11Buffer* buf = getBuf();
	return p_device->CreateShaderResourceView( buf, &desc, &m_srv );
}