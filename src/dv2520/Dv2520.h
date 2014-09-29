#ifndef DV2520_DV2520_H
#define DV2520_DV2520_H

#define VELOCITY_WALK 1.2f
#define VELOCITY_ROTATE 2.0f

#include <TimerDelta.h>

class Dx;
class Et;
class Win;
class Cam;

class Dv2520 {
  public:
    Dv2520(Win& p_win);
    ~Dv2520();

    HRESULT init();
    int run();
    void gameloop(double p_delta);
  protected:
  private:
    Et* m_et;
    Dx* m_dx;
    Cam* m_cam;
    TimerDelta m_timerDelta;

    Win* m_win;
};

#endif // DV2520_DV2520_H
