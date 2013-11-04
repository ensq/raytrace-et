#ifndef DV2520_BUF_H
#define DV2520_BUF_H

class Buf {
public:
	Buf( unsigned p_tNum, unsigned p_tSize );
	virtual ~Buf();

	virtual HRESULT init( ID3D11Device* p_device ) = 0;
protected:
	HRESULT initBuf( ID3D11Device* p_device, D3D11_BUFFER_DESC& p_desc );
	HRESULT initBuf( ID3D11Device* p_device, D3D11_BUFFER_DESC& p_desc, D3D11_SUBRESOURCE_DATA& p_initialData );

	unsigned getTNum() const;
	unsigned getTSize() const;
	unsigned getByteWidth() const;
	ID3D11Buffer* getBuf() const;
private:
	unsigned m_tNum;
	unsigned m_tSize;

	ID3D11Buffer* m_buf;
};

#endif // DV2520_BUF_H