#ifndef DV2520_UTIL_H
#define DV2520_UTIL_H

#define ZERO_MEM( arg ) ZeroMemory( &arg, sizeof( arg ) );
#define ASSERT_DELETE( arg ) assert( arg ); delete arg; arg = nullptr;
#define ASSERT_RELEASE( arg ) assert( arg ); arg->Release(); arg = nullptr;

class Util {
public:
	static void crtMemLeakDetectionIfDebug();
	
	static void msgBoxShow( std::string p_what );
	static void getLastErrorAndTerminateProcess( LPTSTR p_function );
	static void terminateProcess();
protected:
private:
};

#endif // DV2520_UTIL_H