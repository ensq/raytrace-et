#ifndef DV2520_DX_H
#define DV2520_DX_H

enum Fxs;

struct ID3D11ShaderResourceView;

class Win;
class Obj;
class CogFx;
class CogCb;
class CogD3d;
class BufUav;
class TimerD3d;

#include <MathSimple.h>

class Dx {
public:
	Dx( Win& p_win );
	~Dx();

	HRESULT init();
	HRESULT render( double p_delta, Mat4F& p_view, Mat4F& p_proj );
	double dispatch( ID3D11DeviceContext* p_devcon, Fxs p_fx );
protected:
private:
	HRESULT initObjTest( ID3D11Device* p_device );

	ID3D11UnorderedAccessView* m_uavBackbuffer;
	ID3D11RenderTargetView* m_rtvBackbuffer;

	BufUav* m_uavRays;
	BufUav* m_uavIntersections;

	Obj* m_objTest;

	CogD3d* m_cogD3d;
	CogFx* m_cogFx;
	CogCb* m_cogCb;

	TimerD3d* m_timer;

	Win* m_win;
};

#endif // DV2520_DX_H