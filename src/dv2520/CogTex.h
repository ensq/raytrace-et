#ifndef DV2520_COGTEX_H
#define DV2520_COGTEX_H

const static std::string TexFilepath = "../../../tex/dv2520/";

enum Texs {
	Texs_default = 0,

	Texs_last = 1 // obs
};
static const char* TexFilenames[] = { "default.dds" };

class Tex {
public:
	Tex( Texs p_id, ID3D11ShaderResourceView* p_srv );
	~Tex();

	Texs getId() const;
	ID3D11ShaderResourceView* getSrv() const;
protected:
private:
	Texs m_id;
	ID3D11ShaderResourceView* m_srv;
};

class CogTex {
public:
	CogTex();
	~CogTex();

	HRESULT init( ID3D11Device* p_device );

	Tex* getTex( ID3D11Device* p_device, Texs p_id );
protected:
private:
	HRESULT createTexFromFile( ID3D11Device* p_device, Texs p_id, Tex** io_tex );
	HRESULT createSrvFromFile( ID3D11Device* p_device, std::string p_file, ID3D11ShaderResourceView** io_srv );

	std::map< Texs, Tex* > m_texs;
};

#endif // DV2520_COGTEX_H