#ifndef DV2520_COGD3D_H
#define DV2520_COGD3D_H

class Win;

struct D3d {
    D3d();
    ~D3d();

    ID3D11Device* device;
    ID3D11DeviceContext* devcon;
    IDXGISwapChain* swapChain;
};

class CogD3d {
  public:
    CogD3d(Win& p_win);
    ~CogD3d();

    HRESULT init();

    D3d getD3d();
  protected:
  private:
    HRESULT initD3d();

    D3d m_d3d;
    Win* m_win;
};

#endif // DV2520_COGD3D_H
