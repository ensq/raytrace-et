#include <stdafx.h>

#include <AntTweakBar.h>

#include <Ant.h>

Ant::Ant() {
    m_timeRaysGenerate = 0;
    m_timeRaysInterect = 0;
    m_timeLighting = 0;

    m_eyeFixPosX = -1;
    m_eyeFixPosY = -1;
}
Ant::~Ant() {
    TwTerminate();
}

void Ant::init(ID3D11Device* p_device, unsigned p_screenWidth,
               unsigned p_screenHeight) {
    TwInit(TW_DIRECT3D11, p_device);
    TwWindowSize(p_screenWidth, p_screenHeight);

    TwBar *bar = TwNewBar("AntTweakBar");
    TwDefine("GLOBAL help='This example shows how to integrate AntTweakBar"
             " into a DirectX11 application.'");
    int barSize[2] = { 224, 320 };
    TwSetParam(bar, NULL, "size", TW_PARAM_INT32, 2, barSize);

    // Add variables to the tweak bar
    TwAddVarRW(bar, "Ms - RaysGenerate", TW_TYPE_DOUBLE,
               &m_timeRaysGenerate, "");
    TwAddVarRW(bar, "Ms - RaysInterect", TW_TYPE_DOUBLE,
               &m_timeRaysInterect, "");
    TwAddVarRW(bar, "Ms - Lighting", TW_TYPE_DOUBLE,
               &m_timeLighting, "");

    TwAddVarRW(bar, "Eye fix X", TW_TYPE_DOUBLE,
               &m_eyeFixPosX, "");
    TwAddVarRW(bar, "Eye fix Y", TW_TYPE_DOUBLE,
               &m_eyeFixPosY, "");

    TwAddVarRW(bar, "Pos X", TW_TYPE_FLOAT,
               &m_posX, "");
    TwAddVarRW(bar, "Pos Y", TW_TYPE_FLOAT,
               &m_posY, "");
    TwAddVarRW(bar, "Pos Z", TW_TYPE_FLOAT,
               &m_posZ, "");

    TwAddVarRW(bar, "Look X", TW_TYPE_FLOAT,
               &m_lookX, "");
    TwAddVarRW(bar, "Look Y", TW_TYPE_FLOAT,
               &m_lookY, "");
    TwAddVarRW(bar, "Look Z", TW_TYPE_FLOAT,
               &m_lookZ, "");
}
void Ant::render() {
    TwDraw();
}
int Ant::eventWin(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return TwEventWin(hWnd, message, wParam, lParam);
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

void Ant::setTimeRaysGenerate(double p_time) {
    m_timeRaysGenerate = p_time;
}
void Ant::setTimeRaysInterect(double p_time) {
    m_timeRaysInterect = p_time;
}
void Ant::setTimeLighting(double p_time) {
    m_timeLighting = p_time;
}
void Ant::setEyeFixation(double p_x, double p_y) {
    m_eyeFixPosX = p_x;
    m_eyeFixPosY = p_y;

}
void Ant::setPos(float p_x, float p_y, float p_z) {
    m_posX = p_x;
    m_posY = p_y;
    m_posZ = p_z;
}
void Ant::setLook(float p_x, float p_y, float p_z) {
    m_lookX = p_x;
    m_lookY = p_y;
    m_lookZ = p_z;
}
