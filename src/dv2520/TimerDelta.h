#ifndef DV2520_TIMERDELTA_H
#define DV2520_TIMERDELTA_H

class Timer;

class TimerDelta {
public:
	TimerDelta();
	~TimerDelta();

	double tick();
	void reset();
	void start();
	void stop();
protected:
private:
	bool m_ticking;
	Timer* m_timer;

	double m_timeDelta;
	double m_timePrev;
	double m_timeCur;
};

#endif // DV2520_TIMERDELTA_H