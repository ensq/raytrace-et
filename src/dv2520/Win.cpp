#include <stdafx.h>

#include <Win.h>

Win::Win( WinDesc p_desc ) {
	m_desc = p_desc;
}
Win::~Win() {
}

HRESULT Win::init() {
	HRESULT hr = E_FAIL;

	// Extract desc:
	LPCTSTR title = m_desc.title;
	LPCTSTR className = m_desc.className;
	unsigned width = m_desc.width;
	unsigned height = m_desc.height;
	HINSTANCE hInstance = m_desc.hInstance;
	int nCmdShow = m_desc.nCmdShow;

	WNDCLASSEX wcex;
	ZERO_MEM( wcex ); // Use ZeroMemory to save some lines of code, and make up for it with a redundant comment.
	wcex.cbSize			= sizeof( WNDCLASSEX ); 
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = Win::wWinProc;
	wcex.hInstance      = hInstance;
	wcex.hCursor        = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground  = (HBRUSH)( COLOR_WINDOW + 1 );
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = className;

	LPTSTR lastMethod = "RegisterClassEx";
	if( RegisterClassEx( &wcex )!=0 ) { // Registers a window class for subsequent use in calls to the CreateWindow or CreateWindowEx function.
		RECT rct = { 0, 0, width, height };
		AdjustWindowRect( &rct, WS_OVERLAPPEDWINDOW, FALSE );

		lastMethod = "CreateWindow";
		m_hWnd = CreateWindow(
			className,
			title,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rct.right	- rct.left,
			rct.bottom	- rct.top,
			NULL,
			NULL,
			hInstance,
			NULL);
		if( m_hWnd!=NULL ) { // Return value of CreateWindow is NULL if failed.
			ShowWindow( m_hWnd, nCmdShow );
			hr = S_OK;
		}
	}
	if( hr!=S_OK ) {
		Util::getLastErrorAndTerminateProcess( lastMethod );
	}
	return hr;
}

unsigned Win::getWidth() {
	RECT rct; GetClientRect( m_hWnd, &rct );
	return rct.right - rct.left;
}
unsigned Win::getHeight() {
	RECT rct; GetClientRect( m_hWnd, &rct );
	return rct.bottom - rct.top;
}
HWND Win::getHWnd() const {
	return m_hWnd;
}

LRESULT CALLBACK Win::wWinProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) {
	HDC hdc;
	PAINTSTRUCT ps;
	LRESULT lResult = 0;
	switch ( message ) {
	case WM_PAINT:
		hdc = BeginPaint( hWnd, &ps );
		EndPaint( hWnd, &ps );
		break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	case WM_KEYDOWN:
		if( wParam==VK_ESCAPE ) {
			PostQuitMessage( 0 );
		}
		break;
	default:
		lResult = DefWindowProc( hWnd, message, wParam, lParam );
		break;
	}
	return lResult;
}