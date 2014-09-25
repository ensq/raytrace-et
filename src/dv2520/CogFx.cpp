#include <stdafx.h>

#include <FxCs.h>
#include <CogFx.h>
#include <TimerD3d.h>

CogFx::CogFx() {
    m_csRaysGenerate = nullptr;
    m_csRaysIntersect = nullptr;
    m_csLighting = nullptr;
    m_csCombine = nullptr;

    m_timer = nullptr;
}
CogFx::~CogFx() {
    ASSERT_DELETE(m_csRaysGenerate);
    ASSERT_DELETE(m_csRaysIntersect);
    ASSERT_DELETE(m_csLighting);
    ASSERT_DELETE(m_csCombine);

    ASSERT_DELETE(m_timer);
}

HRESULT CogFx::init(ID3D11Device* p_device) {
    HRESULT hr = S_FALSE;

    m_csRaysGenerate = new FxCs(gFxCsoPathCsRaysGenerate);
    hr = m_csRaysGenerate->init(p_device);
    if(SUCCEEDED(hr)) {
        m_csRaysIntersect = new FxCs(gFxCsoPathCsRaysIntersect);
        hr = m_csRaysIntersect->init(p_device);
    }
    if(SUCCEEDED(hr)) {
        m_csLighting = new FxCs(gFxCsoPathCsLighting);
        hr = m_csLighting->init(p_device);
    }
    if(SUCCEEDED(hr)) {
        m_csCombine = new FxCs(gFxCsoPathCsCombine);
        hr = m_csCombine->init(p_device);
    }

    if(SUCCEEDED(hr)) {
        m_timer = new TimerD3d();
        hr = m_timer->init(p_device);
    }

    return hr;
}

double CogFx::dispatch(ID3D11DeviceContext* p_devcon, Fxs p_fx,
                unsigned p_x, unsigned p_y) {
    fxSet(p_devcon, p_fx);
    m_timer->start(p_devcon);
    p_devcon->Dispatch(p_x, p_y, 1);
    m_timer->stop(p_devcon);
    fxUnset(p_devcon, p_fx);

    return m_timer->time(p_devcon);
}

void CogFx::fxSet(ID3D11DeviceContext* p_devcon, Fxs p_fx) {
    switch(p_fx) {
    case Fxs_CS_RAYSGENERATE:
        m_csRaysGenerate->set(p_devcon);
        break;
    case Fxs_CS_RAYSINTERSECT:
        m_csRaysIntersect->set(p_devcon);
        break;
    case Fxs_CS_LIGHTING:
        m_csLighting->set(p_devcon);
        break;
    case Fxs_CS_COMBINE:
        m_csCombine->set(p_devcon);
        break;
    default:
        throw ExceptionDv2520("Encountered unknown Fxs-type enum in CogFx::fxSet.");
        break;
    }
}
void CogFx::fxUnset(ID3D11DeviceContext* p_devcon, Fxs p_fx) {
    switch(p_fx) {
    case Fxs_CS_RAYSGENERATE:
        m_csRaysGenerate->unset(p_devcon);
        break;
    case Fxs_CS_RAYSINTERSECT:
        m_csRaysIntersect->unset(p_devcon);
        break;
    case Fxs_CS_LIGHTING:
        m_csLighting->unset(p_devcon);
        break;
    case Fxs_CS_COMBINE:
        m_csCombine->unset(p_devcon);
        break;
    default:
        throw ExceptionDv2520("Encountered unknown Fxs-type enum in CogFx::fxUnset.");
        break;
    }
}
