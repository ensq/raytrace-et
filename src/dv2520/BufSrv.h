#ifndef DV2520_BUFSRV_H
#define DV2520_BUFSRV_H

#include <Buf.h>

class BufSrv : public Buf {
public:
	BufSrv( unsigned p_tNum, unsigned p_tSize );
	BufSrv( unsigned p_tNum, unsigned p_tSize, void* p_data );
	virtual ~BufSrv();

	HRESULT init( ID3D11Device* p_device );

	ID3D11ShaderResourceView* getSrv() const;
protected:
private:
	HRESULT initBuf( ID3D11Device* p_device );
	HRESULT initSrv( ID3D11Device* p_device );

	void* m_data;
	ID3D11ShaderResourceView* m_srv;
};

#endif // DV2520_BUFSRV_H