#include <stdafx.h>

#include <Timer.h>
#include <TimerDelta.h>

TimerDelta::TimerDelta() {
    m_timer = new Timer();

    m_ticking = true;
    m_timeDelta = 0;
    m_timePrev = 0;
    m_timeCur = 0;
}
TimerDelta::~TimerDelta() {
    ASSERT_DELETE( m_timer );
}

double TimerDelta::tick() {
    if( m_ticking==false ) {
        m_timeDelta = 0.0;
    } else {
        m_timer->stop();

        m_timeDelta = m_timer->getElapsedTimeSec();
        if( m_timeDelta < 0.0 ) { // This may indeed happen
            m_timeDelta = 0.0;
        }

        // Reset and start timer to measure next tick:
        m_timer->reset();
        m_timer->start();
    }
    return m_timeDelta;
}
void TimerDelta::reset() {
    m_timer->reset();
    m_timer->start();
    m_ticking = true;
}
void TimerDelta::start() {
    if( m_ticking==false ) {
        m_timer->reset();
        m_timer->start();
        m_ticking = true;
    }
}
void TimerDelta::stop() {
    if( m_ticking==true ) {
        m_ticking = false;
    }
}