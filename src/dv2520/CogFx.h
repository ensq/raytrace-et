#ifndef DV2520_COGFX_H
#define DV2520_COGFX_H

class Fx;

enum Fxs {
	Fxs_CS_RAYSGENERATE,
	Fxs_CS_RAYSINTERSECT,
	Fxs_CS_LIGHTING
};

static const LPCWSTR gFxCsoPathCsLighting = L"CsLighting.cso";

class CogFx {
public:
	CogFx();
	~CogFx();

	HRESULT init( ID3D11Device* p_device );

	void fxSet( ID3D11DeviceContext* p_devcon, Fxs p_fx );
	void fxUnset( ID3D11DeviceContext* p_devcon, Fxs p_fx );
protected:
private:
	Fx* m_csLighting;
};

#endif // DV2520_COGFX_H