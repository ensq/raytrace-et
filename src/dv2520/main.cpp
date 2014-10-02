#include <stdafx.h>

#include <Win.h>
#include <Dv2520.h>

static const LPCTSTR g_winTitle = "dv2520";
static const LPCTSTR g_winClassName = "dv2520Win";
static const unsigned g_winWidth = 800;
static const unsigned g_winHeight = 800;
static const int g_isWindowed = 1;

int WINAPI wWinMain(HINSTANCE p_hInstance, HINSTANCE p_hInstancePrev,
                    LPWSTR p_lpCmdLine, int p_nCmdShow) {
    Util::crtMemLeakDetectionIfDebug();
    HRESULT hr = S_FALSE;

    WinDesc desc;
    ZERO_MEM(desc);
    desc.title = g_winTitle;
    desc.className = g_winClassName;
    desc.width = g_winWidth;
    desc.height = g_winHeight;
    desc.hInstance = p_hInstance;
    desc.hInstancePrev = p_hInstancePrev;
    desc.lpCmdLine = p_lpCmdLine;
    desc.nCmdShow = p_nCmdShow;
    desc.isWindowed = g_isWindowed;
    Win win(desc);
    hr = win.init();

    Dv2520 dv2520(win);
    if(SUCCEEDED(hr)) {
        hr = dv2520.init();
    }
    int retVal = 1;
    if(SUCCEEDED(hr)) {
        retVal = dv2520.run();
    } else {
        retVal = hr;
    }
    return retVal;
}
