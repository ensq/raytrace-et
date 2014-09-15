#include <stdafx.h>

#include <AntTweakBar.h>

#include <Ant.h>

Ant::Ant() {
    m_timeRaysGenerate = 0;
    m_timeRaysInterect = 0;
    m_timeLighting = 0;
}
Ant::~Ant() {
    TwTerminate();
}

void Ant::init( ID3D11Device* p_device, unsigned p_screenWidth, unsigned p_screenHeight ) {
    TwInit( TW_DIRECT3D11, p_device );
    TwWindowSize( p_screenWidth, p_screenHeight );

    TwBar *bar = TwNewBar("AntTweakBar");
    TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' ");
    int barSize[2] = { 224, 320 };
    TwSetParam(bar, NULL, "size", TW_PARAM_INT32, 2, barSize);

    // Add variables to the tweak bar
    TwAddVarRW(bar, "Kernel Time: RaysGenerate", TW_TYPE_DOUBLE, &m_timeRaysGenerate, "");
    TwAddVarRW(bar, "Kernel Time: RaysInterect", TW_TYPE_DOUBLE, &m_timeRaysInterect, "");
    TwAddVarRW(bar, "Kernel Time: Lighting", TW_TYPE_DOUBLE, &m_timeLighting, "");
}
void Ant::render() {
    TwDraw();
}
int Ant::eventWin( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) {
    return TwEventWin( hWnd, message, wParam, lParam );
}

double Ant::getTimeRaysGenerate() const {
    return m_timeRaysGenerate;
}
double Ant::getTimeRaysIntersect() const {
    return m_timeRaysInterect;
}
double Ant::getTimeLighting() const {
    return m_timeLighting;
}

void Ant::setTimeRaysGenerate( double p_time ) {
    m_timeRaysGenerate = p_time;
}
void Ant::setTimeRaysInterect( double p_time ) {
    m_timeRaysInterect = p_time;
}
void Ant::setTimeLighting( double p_time ) {
    m_timeLighting = p_time;
}