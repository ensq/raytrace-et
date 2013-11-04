#include <stdafx.h>

#include <FxCs.h>
#include <CogFx.h>

CogFx::CogFx() {
	m_fxTest = nullptr;
}
CogFx::~CogFx() {
	ASSERT_DELETE( m_fxTest );
}

HRESULT CogFx::init( ID3D11Device* p_device ) {
	m_fxTest = new FxCs( L"Test.cso" );
	return m_fxTest->init( p_device );
}

void CogFx::fxSet( ID3D11DeviceContext* p_devcon, Fxs p_fx ) {
	switch( p_fx ) {
	case Fxs_TEST:
		m_fxTest->set( p_devcon );
		break;
	default:
		throw ExceptionDv2520( "Encountered unknown Fxs-type enum in CogFx::fxSet." );
		break;
	}
}
void CogFx::fxUnset( ID3D11DeviceContext* p_devcon, Fxs p_fx ) {
	switch ( p_fx ) {
	case Fxs_TEST:
		m_fxTest->unset( p_devcon );
		break;
	default:
		throw ExceptionDv2520( "Encountered unknown Fxs-type enum in CogFx::fxUnset." );
		break;
	}
}