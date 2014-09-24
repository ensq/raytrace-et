#ifndef DV2520_WINFO_H
#define DV2520_WINFO_H

class Winfo {
  public:
    Winfo(
        HINSTANCE p_hInstance,
        HINSTANCE p_hInstancePrev,
        LPWSTR p_lpCmdLine,
        int p_nCmdShow);
    ~Winfo();

    HINSTANCE getHInstance() const;
    HINSTANCE getHInstancePrev() const;
    LPWSTR getLpCmdLine() const;
    int getNCmdShow() const;
  protected:
  private:
    HINSTANCE m_hInstance;
    HINSTANCE m_hInstancePrev;
    LPWSTR m_lpCmdLine;
    int m_nCmdShow;
};

#endif // DV2520_WINFO_H