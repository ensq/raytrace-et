#ifndef DV2520_DX_H
#define DV2520_DX_H

struct ID3D11ShaderResourceView;

class Win;
class CogFx;
class CogCb;
class CogD3d;
class BufUav;

#include <MathSimple.h>

class Dx {
public:
	Dx( Win& p_win );
	~Dx();

	HRESULT init();
	HRESULT render( double p_delta, Mat4F& p_view, Mat4F& p_proj );
protected:
private:
	HRESULT initTempVertexBuffer( ID3D11Device* p_device );
	HRESULT initTempIndexBuffer( ID3D11Device* p_device );

	BufUav* m_uavRays;
	BufUav* m_uavIntersections;

	ID3D11ShaderResourceView* m_srvTempVertices;
	ID3D11ShaderResourceView* m_srvTempIndices;

	CogD3d* m_cogD3d;
	CogFx* m_cogFx;
	CogCb* m_cogCb;

	Win* m_win;
};

#endif // DV2520_DX_H