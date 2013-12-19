#ifndef DV2520_DX_H
#define DV2520_DX_H

#define BLOCK_SIZE 16

enum Fxs;

struct ID3D11RenderTargetView;
struct ID3D11UnorderedAccessView;

class Win;
class Obj;
class CogFx;
class CogCb;
class CogSS;
class CogD3d;
class CogTex;
class BufUav;
class TimerD3d;

#include <Vertex.h>
#include <MathSimple.h>
#include <ObjInstance.h>
#include <BufStreamSrv.h>

class Dx {
public:
	Dx( Win& p_win );
	~Dx();

	HRESULT init();
	HRESULT render( double p_delta, Mat4F& p_view, Mat4F& p_proj );
protected:
private:
	double dispatch( ID3D11DeviceContext* p_devcon, Fxs p_fx );
	bool initObjTest( ID3D11Device* p_device );

	// These ought to be put into some sort of class:
	ID3D11UnorderedAccessView* m_uavBackbuffer;
	ID3D11RenderTargetView* m_rtvBackbuffer;
	
	BufStreamSrv< Vertex >* m_srvStreamVertices;
	BufStreamSrv< unsigned >* m_srvStreamIndices;
	BufStreamSrv< ObjInstance >* m_srvStreamInstances;

	BufUav* m_uavRays;
	BufUav* m_uavIntersections;

	Obj* m_objTest;

	CogD3d* m_cogD3d;
	CogFx* m_cogFx;
	CogCb* m_cogCb;
	CogSS* m_cogSS;
	CogTex* m_cogTex;

	TimerD3d* m_timer;

	Win* m_win;
};

#endif // DV2520_DX_H