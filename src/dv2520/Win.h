#ifndef DV2520_WIN_H
#define DV2520_WIN_H

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

	// This method is totally off limits. Hands off.
	static LRESULT CALLBACK wWinProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
protected:
private:
	HWND m_hWnd;
	WinDesc m_desc;
};

#endif // DV2520_WIN_H