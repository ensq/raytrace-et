#ifndef DV2520_FOVTARGET_H
#define DV2520_FOVTARGET_H

typedef long HRESULT;
typedef unsigned int UINT;

enum DXGI_FORMAT;
enum D3D11_USAGE;

struct ID3D11Device;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11UnorderedAccessView;
struct ID3D11RenderTargetView;

class FovTarget {
 public:
    FovTarget(unsigned p_width, unsigned p_height);
    ~FovTarget();

    HRESULT init(ID3D11Device* p_device);

    ID3D11Texture2D* getTex();
    ID3D11ShaderResourceView* getSrv();
    ID3D11RenderTargetView* getRtv();
    ID3D11UnorderedAccessView* getUav();
 protected:
 private:
    HRESULT initTex(ID3D11Device* p_device);
    HRESULT initSrv(ID3D11Device* p_device);
    HRESULT initRtv(ID3D11Device* p_device);
    HRESULT initUav(ID3D11Device* p_device);
    
    const unsigned m_texWidth;
    const unsigned m_texHeight;

    unsigned m_texAliasing;
    unsigned m_texMips;
    DXGI_FORMAT m_texFormat;
    D3D11_USAGE m_texUsage;
    unsigned m_texBindFlags;
    
    ID3D11Texture2D* m_tex;
    ID3D11ShaderResourceView* m_srv;
    ID3D11RenderTargetView* m_rtv;
    ID3D11UnorderedAccessView* m_uav;
};

#endif // DV2520_FOVTARGET_H

/*
DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT;
DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
DXGI_FORMAT::DXGI_FORMAT_R8_UNORM;
DXGI_FORMAT::DXGI_FORMAT_R16_FLOAT;
*/
