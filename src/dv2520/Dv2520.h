#ifndef DV2520_DV2520_H
#define DV2520_DV2520_H

#include <TimerDelta.h>

class Dx;
class Win;

class Dv2520 {
public:
	Dv2520( Win& p_win );
	~Dv2520();

	HRESULT init();
	int run();
protected:
private:
	Dx* m_dx;
	Win* m_win;
	TimerDelta m_timerDelta;
};

#endif // DV2520_DV2520_H