#include <stdafx.h>

#include <Win.h>
#include <CogD3d.h>

D3D_FEATURE_LEVEL g_dxFeatureLevels[] = {
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0
};
unsigned g_dxFeatureLevelsNum = sizeof( g_dxFeatureLevels ) / sizeof( D3D_FEATURE_LEVEL );
D3D_DRIVER_TYPE g_dxDriverTypes[] = {
	D3D_DRIVER_TYPE_HARDWARE,
	D3D_DRIVER_TYPE_WARP,
	D3D_DRIVER_TYPE_REFERENCE
};
unsigned g_dxDriverTypesNum = sizeof( g_dxDriverTypes ) / sizeof( D3D_DRIVER_TYPE );

D3d::D3d() {
	ZERO_MEM( *(this) );
}
D3d::~D3d() {
}

CogD3d::CogD3d( Win& p_win ) {
	m_win = &p_win;
}
CogD3d::~CogD3d() {
	ASSERT_RELEASE( m_d3d.device );
	ASSERT_RELEASE( m_d3d.devcon );
	ASSERT_RELEASE( m_d3d.swapChain );
}

HRESULT CogD3d::init() {
	return initD3d();
}

D3d CogD3d::getD3d() {
	return m_d3d;
}

HRESULT CogD3d::initD3d() {
	HWND hWnd = m_win->getHWnd();
	unsigned hWndWidth = m_win->getWidth();
	unsigned hWndHeight = m_win->getHeight();

	UINT deviceSwapChainFlags = 0;
#ifdef DV2520_DEBUG
	deviceSwapChainFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // DV2520_DEBUG

	DXGI_SWAP_CHAIN_DESC scd; ZERO_MEM( scd );
	scd.BufferCount			= 1;
	scd.OutputWindow		= hWnd;
	scd.Windowed			= TRUE;
	scd.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	scd.SampleDesc.Count	= 1;
	scd.SampleDesc.Quality	= 0;
	scd.BufferDesc.Width	= hWndWidth;
	scd.BufferDesc.Height	= hWndHeight;
	scd.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator	= 60;
	scd.BufferDesc.RefreshRate.Denominator	= 1;

	HRESULT hr = S_FALSE;
	D3D_FEATURE_LEVEL featureLevel;
	D3D_DRIVER_TYPE driverType;
	for( unsigned i = 0; i < g_dxDriverTypesNum && hr!=S_OK; i++ ) {
		driverType = g_dxDriverTypes[ i ];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			driverType,
			NULL,
			deviceSwapChainFlags,
			g_dxFeatureLevels,
			g_dxFeatureLevelsNum,
			D3D11_SDK_VERSION,
			&scd,
			&( m_d3d.swapChain ),
			&( m_d3d.device ),
			&featureLevel,
			&( m_d3d.devcon ) );
	}
	if( !SUCCEEDED( hr ) ) {
		ERR_HR( hr );
	} else {
		// Log instantiated device type and driver.
	}

	return hr;
}