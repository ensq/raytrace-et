#ifndef DV2520_UTIL_H
#define DV2520_UTIL_H

#define ZERO_MEM( arg ) ZeroMemory( &arg, sizeof( arg ) );
#define ASSERT_DELETE( arg ) assert( arg ); delete arg; arg = nullptr;
#define ASSERT_RELEASE( arg ) assert( arg ); arg->Release(); arg = nullptr;
#define SAFE_DELETE( arg ) if( arg!=nullptr ) { ASSERT_DELETE( arg ) };
#define SAFE_RELEASE( arg ) if( arg!=nullptr ) { ASSERT_RELEASE( arg ) };

#ifdef DV2520_DEBUG
#define ERR_HR( hr ) Util::errHr( hr );
#define SET_D3D_OBJECT_NAME( resource, name ) SetD3dObjectName( resource, name );
#else
#define ERR_HR( hr ) ;
#define SET_D3D_OBJECT_NAME( resource, name ) ;
#endif // DV2520_DEBUG

// Make this inaccessible.
template<UINT TNameLength>
inline void SetD3dObjectName(
    _In_ ID3D11DeviceChild* p_resource,
    _In_z_ const char(&name)[ TNameLength ]) {
    p_resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
}

class Util {
  public:
    static void crtMemLeakDetectionIfDebug();

    static void errHr(HRESULT p_hr);   // Make this unaccessible.

    static void msgBoxShowAndTerminateProcess(const char* p_what);
    static void msgBoxShowAndTerminateProcess(std::string p_what);
    //static void msgBoxShowAndTerminateProcess( LPCTSTR p_what ); // LPCTSTR defined as const char*

    static void getLastErrorAndTerminateProcess(LPTSTR p_function);
    static void terminateProcess();

    static wchar_t* stringToWstr(std::string string);
  protected:
  private:
};

#endif // DV2520_UTIL_H