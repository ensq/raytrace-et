#include <stdafx.h>

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow ) {
	Util::crtMemLeakDetectionIfDebug();
	return 0;
}