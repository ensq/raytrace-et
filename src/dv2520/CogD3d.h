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
	CogD3d( Win& p_win );
	~CogD3d();

	HRESULT init();

	// Instead of returning these individually, consider returning D3d as a struct.
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDevcon();
	IDXGISwapChain* getSwapChain();
	ID3D11UnorderedAccessView* getBackbuffer() { return m_uavBackbuffer; } // Temp, make this better.
protected:
private:
	HRESULT initD3d();
	HRESULT initBackbuffer( ID3D11Device* p_device );

	D3d m_d3d;
	ID3D11UnorderedAccessView* m_uavBackbuffer;

	Win* m_win;
};

#endif // DV2520_COGD3D_H