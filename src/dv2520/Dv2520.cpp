#include <stdafx.h>

#include <Dx.h>
#include <Et.h>
#include <Win.h>
#include <Cam.h>
#include <Ant.h>
#include <Timer.h>
#include <Dv2520.h>
#include <Printer.h>

#define TRACK_EYES

Dv2520::Dv2520(Win& p_win) {
    m_win = &p_win;

#define Z_NEAR 1
#define Z_FAR 1000
    Vec3F pos = Vec3F(0.0f, 8.86f, -10.42f); //Vec3F(0.0f, 0.0f, -45.0f);
    Vec3F look = Vec3F(0.0f, -0.81f, 0.59f); // Vec3F(0.0f, 0.0f, 1.0f);
    m_cam = new Cam(pos, look, Z_NEAR, Z_FAR);

    m_isTracking = false;
#ifdef TRACK_EYES
    m_isTracking = true;
#endif // TRACK_EYES
    m_state.x = 576;
    m_state.y = 576; // note that these specify eye position for the
                     // entire screen; independant of where the window
                     // is actually positioned.

    m_printer = new Printer("res.txt");

    m_dx = nullptr;
    m_et = nullptr;
}
Dv2520::~Dv2520() {
    ASSERT_DELETE(m_dx);
    ASSERT_DELETE(m_et);
    ASSERT_DELETE(m_cam);
}

HRESULT Dv2520::init() {
    HRESULT hr = S_FALSE;

    // Initialize EyeX first, as it is probably more prone to failure
    // than the graphics context:
    m_et = new Et();
    bool success = m_et->init();
    hr = success==true ? S_OK : S_FALSE;

    if(SUCCEEDED(hr)) {
        m_dx = new Dx(*m_win);
        hr = m_dx->init();
    }

    return hr;
}

int Dv2520::run() {
    m_timerDelta.reset();
    m_timerDelta.start();

int numFrames = 1000;
    MSG msgWin = { 0 };
while(WM_QUIT!=msgWin.message && numFrames>0) {
        if(PeekMessage(&msgWin, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msgWin);
            DispatchMessage(&msgWin);
        } else {
            double delta = m_timerDelta.tick();
            gameloop(delta);
numFrames--;
        }
    }
    return (int)msgWin.wParam;
}

void Dv2520::gameloop(double p_delta) {
    InputQueue& inputQueue = m_win->getInputQueue();

    // Keyboard input:
    InputKey key;
    while(inputQueue.keyPop(key)==false) {
        switch(key.getType()) {
        case InputKeyTypes_W:
            m_cam->walk(VELOCITY_WALK);
            break;
        case InputKeyTypes_A:
            m_cam->strafe(-VELOCITY_WALK/2);
            break;
        case InputKeyTypes_S:
            m_cam->walk(-VELOCITY_WALK);
            break;
        case InputKeyTypes_D:
            m_cam->strafe(VELOCITY_WALK/2);
            break;

        // Because I can't be bothered.
        case InputKeyTypes_UP:
            m_cam->pitch(-VELOCITY_ROTATE);
            break;
        case InputKeyTypes_LEFT:
            m_cam->yaw(-VELOCITY_ROTATE);
            break;
        case InputKeyTypes_DOWN:
            m_cam->pitch(VELOCITY_ROTATE);
            break;
        case InputKeyTypes_RIGHT:
            m_cam->yaw(VELOCITY_ROTATE);
            break;

        case InputKeyTypes_SPACE:
            m_isTracking = !m_isTracking;
            break;
        }
    }

    // Mouse input:
    InputMouse mouse;
    inputQueue.mousePop(mouse);
    m_cam->yaw(mouse.getDX());
    m_cam->pitch(mouse.getDY());

    // Clear input:
    inputQueue.empty(); // Remember to clear the input.

    if(m_isTracking==true) {
        m_state = m_et->getState();
    }
    Singleton<Ant>::get().setEyeFixation(m_state.x, m_state.y);

    Vec3F pos = m_cam->getPos();
    Vec3F look = m_cam->getLook();
    Singleton<Ant>::get().setPos(pos.x, pos.y, pos.z);
    Singleton<Ant>::get().setLook(look.x, look.y, look.z);

    double lX, lY;
    m_win->getLocalPos(m_state.x, m_state.y, lX, lY);
    Singleton<Ant>::get().setEyeFixation(lX, lY);
    
    Singleton<Ant>::get().setTimeRaysGenerate(0.0);
    Singleton<Ant>::get().setTimeRaysInterect(0.0);
    Singleton<Ant>::get().setTimeLighting(0.0);
    HRESULT hr = m_dx->render(p_delta, m_cam, lX, lY);
    if(FAILED(hr)) {
        ERR_HR(hr);
    }
float dtRaysGenerate = Singleton<Ant>::get().getTimeRaysGenerate();
float dtRaysLighting = Singleton<Ant>::get().getTimeRaysIntersect();
float dtLighting = Singleton<Ant>::get().getTimeLighting();
m_printer->printFrame(dtRaysGenerate, dtRaysLighting, dtLighting);
}
