#ifndef DV2520_DV2520_H
#define DV2520_DV2520_H

#define VELOCITY_WALK 0.1f
#define VELOCITY_ROTATE 0.05f

#include <TimerDelta.h>

class Dx;
class Win;
class Cam;

class Dv2520 {
public:
	Dv2520( Win& p_win );
	~Dv2520();

	HRESULT init();
	int run();
	void gameloop( double p_delta );
protected:
private:
	Dx* m_dx;
	Cam* m_cam;
	TimerDelta m_timerDelta;

	Win* m_win;
};

#endif // DV2520_DV2520_H