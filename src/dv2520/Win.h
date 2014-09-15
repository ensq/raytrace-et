#ifndef DV2520_WIN_H
#define DV2520_WIN_H

#define WinKeyW 0x57
#define WinKeyA 0x41
#define WinKeyS 0x53
#define WinKeyD 0x44

#include <InputQueue.h>

struct WinDesc {
    LPCTSTR title;
    LPCTSTR className;
    unsigned width;
    unsigned height;

    HINSTANCE hInstance;
    HINSTANCE hInstancePrev;
    LPWSTR lpCmdLine;
    int nCmdShow;
};

class Win {
public:
    Win( WinDesc p_desc );
    ~Win();

    HRESULT init();

    unsigned getWidth();
    unsigned getHeight();
    HWND getHWnd() const;
    InputQueue& getInputQueue();

    // This method is totally off limits. Hands off.
    static LRESULT CALLBACK wWinProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
    static void wWinKeyProc( WPARAM wParam );
    static void wWinMouseProc( WPARAM wParam, LPARAM lParam );
protected:
private:
    HWND m_hWnd;
    WinDesc m_desc;
    static InputQueue m_inputQueue;
};

#endif // DV2520_WIN_H