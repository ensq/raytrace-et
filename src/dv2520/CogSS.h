#ifndef DV2520_COGSS_H
#define DV2520_COGSS_H

enum SSs {
	SSs_default
};

class CogSS {
public:
	CogSS();
	~CogSS();

	HRESULT init( ID3D11Device* p_device );

	ID3D11SamplerState* getSamplerState( SSs p_ss ) const;
protected:
private:
	std::map< SSs, ID3D11SamplerState* > m_sss;
};

#endif // DV2520_COGSS_H