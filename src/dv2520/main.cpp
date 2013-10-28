#include <stdafx.h>

#include <Win.h>

static const LPCTSTR g_winTitle		= "dv2520";
static const LPCTSTR g_winClassName	= "dv2520Win";
static const unsigned g_winWidth	= 800;
static const unsigned g_winHeight	= 800;

int WINAPI wWinMain( HINSTANCE p_hInstance, HINSTANCE p_hInstancePrev, LPWSTR p_lpCmdLine, int p_nCmdShow ) {
	Util::crtMemLeakDetectionIfDebug();
	HRESULT hr = S_FALSE;

	// Create window:
	WinDesc desc; ZERO_MEM( desc );
	desc.title			= g_winTitle;
	desc.className		= g_winClassName;
	desc.width			= g_winWidth;
	desc.height			= g_winHeight;
	desc.hInstance		= p_hInstance;
	desc.hInstancePrev	= p_hInstancePrev;
	desc.lpCmdLine		= p_lpCmdLine;
	desc.nCmdShow		= p_nCmdShow;
	Win win( desc );
	hr = win.init();

	MSG msgWin = { 0 };
	while( WM_QUIT!=msgWin.message ) {
		if( PeekMessage( &msgWin, NULL, 0, 0, PM_REMOVE ) ) {
			TranslateMessage( &msgWin );
			DispatchMessage( &msgWin );
		} else {
			// Dispatch and things.
		}
	}
	return (int)msgWin.wParam;
}