#include <stdafx.h>

#include <FxCs.h>
#include <CogFx.h>

CogFx::CogFx() {
	m_csLighting = nullptr;
}
CogFx::~CogFx() {
	ASSERT_DELETE( m_csLighting );
}

HRESULT CogFx::init( ID3D11Device* p_device ) {
	m_csLighting = new FxCs( gFxCsoPathCsLighting );
	return m_csLighting->init( p_device );
}

void CogFx::fxSet( ID3D11DeviceContext* p_devcon, Fxs p_fx ) {
	switch( p_fx ) {
	case Fxs_CS_LIGHTING:
		m_csLighting->set( p_devcon );
		break;
	default:
		throw ExceptionDv2520( "Encountered unknown Fxs-type enum in CogFx::fxSet." );
		break;
	}
}
void CogFx::fxUnset( ID3D11DeviceContext* p_devcon, Fxs p_fx ) {
	switch ( p_fx ) {
	case Fxs_CS_LIGHTING:
		m_csLighting->unset( p_devcon );
		break;
	default:
		throw ExceptionDv2520( "Encountered unknown Fxs-type enum in CogFx::fxUnset." );
		break;
	}
}