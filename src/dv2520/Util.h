#ifndef DV2520_UTIL_H
#define DV2520_UTIL_H

#define ZERO_MEM( arg ) ZeroMemory( &arg, sizeof( arg ) );
#define ASSERT_DELETE( arg ) assert( arg ); delete arg; arg = nullptr;
#define ASSERT_RELEASE( arg ) assert( arg ); arg->Release(); arg = nullptr;

#ifdef DV2520_DEBUG
#define ERR_HR( hr ) Util::errHr( hr );
#elif
#define ERR_HR( hr ) ;
#endif // DV2520_DEBUG

class Util {
public:
	static void crtMemLeakDetectionIfDebug();

	static void errHr( HRESULT p_hr ); // Make this unaccessible.

	static void msgBoxShowAndTerminateProcess( const char* p_what );
	static void msgBoxShowAndTerminateProcess( std::string p_what );
	//static void msgBoxShowAndTerminateProcess( LPCTSTR p_what ); // LPCTSTR defined as const char*

	static void getLastErrorAndTerminateProcess( LPTSTR p_function );
	static void terminateProcess();
protected:
private:
};

#endif // DV2520_UTIL_H