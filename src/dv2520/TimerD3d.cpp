#include <stdafx.h>

#include <TimerD3d.h>

TimerD3d::TimerD3d() {
    m_start = nullptr;
    m_stop = nullptr;
    m_disjoint = nullptr;
}
TimerD3d::~TimerD3d() {
    ASSERT_RELEASE(m_start);
    ASSERT_RELEASE(m_stop);
    ASSERT_RELEASE(m_disjoint);
}

HRESULT TimerD3d::init(ID3D11Device* p_device) {
    D3D11_QUERY_DESC desc;
    ZERO_MEM(desc);
    desc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
    HRESULT hr = p_device->CreateQuery(&desc, &m_disjoint);

    desc.Query = D3D11_QUERY_TIMESTAMP;
    if(SUCCEEDED(hr)) {
        p_device->CreateQuery(&desc, &m_start);
    }
    if(SUCCEEDED(hr)) {
        p_device->CreateQuery(&desc, &m_stop);
    }
    return hr;
}

void TimerD3d::start(ID3D11DeviceContext* p_devcon) {
    p_devcon->Begin(m_disjoint);
    p_devcon->End(m_start);
}
void TimerD3d::stop(ID3D11DeviceContext* p_devcon) {
    p_devcon->End(m_stop);
    p_devcon->End(m_disjoint);
}
double TimerD3d::time(ID3D11DeviceContext* p_devcon) {
    UINT64 timeStart = 0;
    while(p_devcon->GetData(m_start, &timeStart, sizeof(timeStart), 0)!=S_OK);

    UINT64 timeEnd = 0;
    while(p_devcon->GetData(m_stop, &timeEnd, sizeof(timeEnd), 0)!=S_OK);

    D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjointData;
    while(p_devcon->GetData(m_disjoint, &disjointData, sizeof(disjointData), 0)!=S_OK);

    double time = -1.0f;
    if(disjointData.Disjoint==FALSE) {
        UINT64 timeDelta = timeEnd - timeStart;
        double frequency = static_cast<double>(disjointData.Frequency);
        time = (timeDelta / frequency) * 1000.0f;
    }
    return time;
}