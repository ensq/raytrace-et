#ifndef DV2520_DX_H
#define DV2520_DX_H

class Win;
class CogFx;
class CogD3d;
class BufUav;

class Dx {
public:
	Dx( Win& p_win );
	~Dx();

	HRESULT init();
	HRESULT render( double p_delta );
protected:
private:
	HRESULT initCogD3d();
	HRESULT initCogFx( ID3D11Device* p_device );

	CogD3d* m_cogD3d;
	CogFx* m_cogFx;

	Win* m_win;
};

#endif // DV2520_DX_H