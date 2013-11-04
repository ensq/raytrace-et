#include <stdafx.h>

#include <Dx.h>
#include <Win.h>
#include <Dv2520.h>

Dv2520::Dv2520( Win& p_win ) {
	m_win = &p_win;

	m_dx = nullptr;
}
Dv2520::~Dv2520() {
	ASSERT_DELETE( m_dx );
}

HRESULT Dv2520::init() {
	HRESULT hr = S_FALSE;
	
	m_dx = new Dx( *m_win );
	hr = m_dx->init();
	
	return hr;
}

int Dv2520::run() {
	m_timerDelta.reset();
	m_timerDelta.start();

	MSG msgWin = { 0 };
	while( WM_QUIT!=msgWin.message ) {
		if( PeekMessage( &msgWin, NULL, 0, 0, PM_REMOVE ) ) {
			TranslateMessage( &msgWin );
			DispatchMessage( &msgWin );
		} else {
			double delta = m_timerDelta.tick();
			HRESULT hr = m_dx->render( delta );
			if( FAILED( hr ) ) {
				ERR_HR( hr );
			}
		}
	}
	return (int)msgWin.wParam;
}