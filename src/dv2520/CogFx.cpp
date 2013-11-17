#include <stdafx.h>

#include <FxCs.h>
#include <CogFx.h>

CogFx::CogFx() {
	m_csRaysGenerate = nullptr;
	m_csRaysIntersect = nullptr;
	m_csLighting = nullptr;
}
CogFx::~CogFx() {
	ASSERT_DELETE( m_csRaysGenerate );
	ASSERT_DELETE( m_csRaysIntersect );
	ASSERT_DELETE( m_csLighting );
}

HRESULT CogFx::init( ID3D11Device* p_device ) {
	HRESULT hr = S_FALSE;

	m_csRaysGenerate = new FxCs( gFxCsoPathCsRaysGenerate );
	hr = m_csRaysGenerate->init( p_device );
	if( SUCCEEDED( hr ) ) {
		m_csRaysIntersect = new FxCs( gFxCsoPathCsRaysIntersect );
		hr = m_csRaysIntersect->init( p_device );
	}
	if( SUCCEEDED( hr ) ) {
		m_csLighting = new FxCs( gFxCsoPathCsLighting );
		hr = m_csLighting->init( p_device );
	}

	return hr;
}

void CogFx::fxSet( ID3D11DeviceContext* p_devcon, Fxs p_fx ) {
	switch( p_fx ) {
	case Fxs_CS_RAYSGENERATE:
		m_csRaysGenerate->set( p_devcon );
		break;
	case Fxs_CS_RAYSINTERSECT:
		m_csRaysIntersect->set( p_devcon );
		break;
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
	case Fxs_CS_RAYSGENERATE:
		m_csRaysGenerate->unset( p_devcon );
		break;
	case Fxs_CS_RAYSINTERSECT:
		m_csRaysIntersect->unset( p_devcon );
		break;
	case Fxs_CS_LIGHTING:
		m_csLighting->unset( p_devcon );
		break;
	default:
		throw ExceptionDv2520( "Encountered unknown Fxs-type enum in CogFx::fxUnset." );
		break;
	}
}