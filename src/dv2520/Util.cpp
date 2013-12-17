#include <stdafx.h>

void Util::crtMemLeakDetectionIfDebug() {
#ifdef DV2520_DEBUG
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
#endif // DV2520_DEBUG
}

void Util::errHr( HRESULT p_hr ) {
	_com_error comError( p_hr );
	LPCTSTR errorString = comError.ErrorMessage();

	msgBoxShowAndTerminateProcess( errorString );
}

void Util::msgBoxShowAndTerminateProcess( const char* p_what ) {
	MessageBoxA(
		NULL,
		p_what,
		"dv2520",
		MB_OK | MB_ICONEXCLAMATION );
	Util::terminateProcess();
}
void Util::msgBoxShowAndTerminateProcess( std::string p_what ) {
	msgBoxShowAndTerminateProcess( p_what.c_str() );
}
//static void msgBoxShowAndTerminateProcess( LPCTSTR p_what ) {
//	msgBoxShowAndTerminateProcess( p_what );
//}

void Util::getLastErrorAndTerminateProcess( LPTSTR p_function ) {
	// Retrieve the system error message for the last-error code:
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		(LPTSTR)&lpMsgBuf,
		0, 
		NULL );

	// Display the error message:
	lpDisplayBuf = (LPVOID)LocalAlloc(
		LMEM_ZEROINIT, 
		( lstrlen( (LPCTSTR)lpMsgBuf) + lstrlen( (LPCTSTR)p_function) + 40 ) * sizeof( TCHAR ) ); 
	StringCchPrintf( (LPTSTR)lpDisplayBuf, 
		LocalSize( lpDisplayBuf ) / sizeof( TCHAR ),
		TEXT( "%s failed with error %d: %s" ),
		p_function, 
		dw, 
		lpMsgBuf ); 
	MessageBox( NULL, (LPCTSTR)lpDisplayBuf, TEXT( "Error" ), MB_OK ); 

	// Don't forget to clean up:
	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);

	// Finally, exit the responsible process:
	ExitProcess(dw); 
}
void Util::terminateProcess() {
	DWORD dw = GetLastError(); 
	ExitProcess(dw); 
}
// http://msdn.microsoft.com/en-us/library/windows/desktop/ms680582%28v=vs.85%29.aspx

wchar_t* Util::stringToWstr( std::string string ) {
	int wchars_num = MultiByteToWideChar(
		CP_UTF8, 
		0, 
		string.c_str(), 
		-1, 
		NULL, 
		0 );
	
	wchar_t* wstr = new wchar_t[ wchars_num ];
	MultiByteToWideChar(
		CP_UTF8, 
		0,
		string.c_str(),
		-1, 
		wstr, 
		wchars_num);
	return wstr;
}