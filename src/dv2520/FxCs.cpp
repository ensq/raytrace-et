#include <stdafx.h>

#include <FxCs.h>

FxCs::FxCs( LPCWSTR p_kernelPath ) : Fx( p_kernelPath ) {
	m_cs = nullptr;
}
FxCs::~FxCs() {
	ASSERT_RELEASE( m_cs );
}

HRESULT FxCs::init( ID3D11Device* p_device ) {
	HRESULT hr = Fx::init( p_device );
	if( SUCCEEDED( hr ) ) {
		hr = p_device->CreateComputeShader( 
			m_blob->GetBufferPointer(),
			m_blob->GetBufferSize(),
			nullptr,
			&m_cs );
	} else {
		ERR_HR( hr );
	}
	return hr;
}

void FxCs::set( ID3D11DeviceContext* p_devcon ) {
	p_devcon->CSSetShader( m_cs, nullptr, 0 );
}
void FxCs::unset( ID3D11DeviceContext* p_devcon ) {
	p_devcon->CSSetShader( nullptr, nullptr, 0 );
}