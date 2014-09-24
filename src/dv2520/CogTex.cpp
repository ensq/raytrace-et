#include <stdafx.h>
#include <DDSTextureLoader.h>

#include <CogTex.h>

Tex::Tex(Texs p_id, ID3D11ShaderResourceView* p_srv) {
    m_id = p_id;
    m_srv = p_srv;
}
Tex::~Tex() {
    ASSERT_RELEASE(m_srv);
}

Texs Tex::getId() const {
    return m_id;
}
ID3D11ShaderResourceView* Tex::getSrv() const {
    return m_srv;
}

CogTex::CogTex() {
}
CogTex::~CogTex() {
    for(auto it = m_texs.begin(); it != m_texs.end(); it++) {
        ASSERT_DELETE(it->second);
    }
}

HRESULT CogTex::init(ID3D11Device* p_device) {
    return S_OK;
}

Tex* CogTex::getTex(ID3D11Device* p_device, Texs p_id) {
    Tex* tex = nullptr;

    auto texIt = m_texs.find(p_id);
    if(texIt!=m_texs.end()) {
        tex = texIt->second;
    } else {
        HRESULT hr = createTexFromFile(p_device, p_id, &tex);
        m_texs.insert(std::pair< Texs, Tex* >(p_id, tex));
    }
    return tex;
}

HRESULT CogTex::createTexFromFile(ID3D11Device* p_device, Texs p_id, Tex** io_tex) {
    HRESULT hr = S_FALSE;
    if((unsigned)p_id < (unsigned)Texs_last) {
        std::string filename = std::string(TexFilenames[(unsigned)p_id ]);    // Will crash if filename not existant.
        ID3D11ShaderResourceView* srv;
        hr = createSrvFromFile(p_device, TexFilepath + filename, &srv);
        *io_tex = new Tex(p_id, srv);
    } else {
        throw ExceptionDv2520("Encountered non-existant filename in CogTex::createTexFromFile!");
    }
    return hr;
}
HRESULT CogTex::createSrvFromFile(ID3D11Device* p_device, std::string p_file, ID3D11ShaderResourceView** io_srv) {
    wchar_t* wstr = Util::stringToWstr(p_file);
    HRESULT hr = DirectX::CreateDDSTextureFromFile(
                     p_device,
                     wstr,
                     nullptr,
                     io_srv);

    delete[] wstr;
    return hr;
}