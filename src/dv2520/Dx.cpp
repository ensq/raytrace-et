#include <stdafx.h>

#include <Dx.h>
#include <Win.h>
#include <CogFx.h>
#include <CogD3d.h>
#include <BufUav.h>

Dx::Dx( Win& p_win ) {
	m_win = &p_win;

	m_cogD3d = nullptr;
	m_cogFx = nullptr;
}
Dx::~Dx() {
	ASSERT_DELETE( m_cogD3d );
	ASSERT_DELETE( m_cogFx );
}

HRESULT Dx::init() {
	// Initialize D3D first-of-all:
	HRESULT hr = initCogD3d();

	// ...then continue initializing rest of the cogs:
	ID3D11Device* device = m_cogD3d->getDevice();
	ID3D11DeviceContext* devcon = m_cogD3d->getDevcon();

	if( SUCCEEDED( hr ) ) {
		hr = initCogFx( device );
	}

	return hr;
}
HRESULT Dx::render( double p_delta ) {
	ID3D11Device* device = m_cogD3d->getDevice();
	ID3D11DeviceContext* devcon = m_cogD3d->getDevcon();
	IDXGISwapChain* swapchain = m_cogD3d->getSwapChain();

	ID3D11UnorderedAccessView* uav[] = { m_cogD3d->getBackbuffer() };
	devcon->CSSetUnorderedAccessViews( 0, 1, uav, NULL );

	m_cogFx->fxSet( devcon, Fxs_TEST );
	devcon->Dispatch( 25, 25, 1 );
	m_cogFx->fxUnset( devcon, Fxs_TEST );

	return swapchain->Present( 0, 0 );
}

HRESULT Dx::initCogD3d() {
	m_cogD3d = new CogD3d( *m_win );
	return m_cogD3d->init();
}
HRESULT Dx::initCogFx( ID3D11Device* p_device ) {
	m_cogFx = new CogFx();
	return m_cogFx->init( p_device );
}