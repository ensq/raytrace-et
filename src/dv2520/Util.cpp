#include <stdafx.h>

void Util::crtMemLeakDetectionIfDebug() {
#ifdef DV2520_DEBUG
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
#endif // DV2520_DEBUG
}