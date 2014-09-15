#include <stdafx.h>

#include <Timer.h>

Timer::Timer() {
    m_ticking = true;
    m_startTimeMicroSec = 0;
    m_endTimeMicroSec = 0;

#ifdef _WIN32
    QueryPerformanceFrequency( &m_frequency );
    m_startCount.QuadPart = 0;
    m_endCount.QuadPart = 0;
#else
    m_startCount.tv_sec = 0;
    m_startCount.tv_usec = 0;
    m_endCount.tv_sec = 0;
    m_endCount.tv_usec = 0;
#endif // _WIN32
}
Timer::~Timer() {
}

void Timer::start() {
    m_ticking = true;
#ifdef _WIN32
    QueryPerformanceCounter( (LARGE_INTEGER*)&m_startCount );
    m_startTimeMicroSec = m_startCount.QuadPart * ( 1000000.0 / m_frequency.QuadPart );
#else
    gettimeofday( &m_startCount, NULL );
    m_startTimeMicroSec = ( m_startCount.tv_sec * 1000000.0 ) + m_startCount.tv_usec;
#endif // _WIN32
}
void Timer::stop() {
    m_ticking = false;
#ifdef _WIN32
    QueryPerformanceCounter( &m_endCount );
    m_endTimeMicroSec = m_endCount.QuadPart * ( 1000000.0 / m_frequency.QuadPart );
#else
    gettimeofday( &m_endCount, NULL );
    m_endTimeMicroSec = ( m_endCount.tv_sec * 1000000.0 ) + m_endCount.tv_usec;
#endif // _WIN32
}
void Timer::reset() {
    m_ticking = true;
    m_startTimeMicroSec = 0;
    m_endTimeMicroSec = 0;
#ifdef _WIN32
    m_startCount.QuadPart = 0;
    m_endCount.QuadPart = 0;
#else
    m_startCount.tv_sec = 0;
    m_startCount.tv_usec = 0;
    m_endCount.tv_sec = 0;
    m_endCount.tv_usec = 0;
#endif // _WIN32
}

double Timer::getElapsedTimeMicroSec() {
    if( m_ticking ) {
        stop();
    }
    return m_endTimeMicroSec - m_startTimeMicroSec;
}
double Timer::getElapsedTimeMilliSec() {
    return getElapsedTimeMicroSec() * 0.001;
}
double Timer::getElapsedTimeSec() {
    return getElapsedTimeMicroSec() * 0.000001;
}

double Timer::getStartTimeMicroSec() const {
    return m_startTimeMicroSec;
}
double Timer::getStartTimeMilliSec() const {
    return m_startTimeMicroSec * 0.001;
}
double Timer::getStartTimeSec() const {
    return m_startTimeMicroSec * 0.000001;
}

double Timer::getEndTimeMicroSec() const { 
    return m_endTimeMicroSec;
}
double Timer::getEndTimeMilliSec() const {
    return m_endTimeMicroSec * 0.001;
}
double Timer::getEndTimeSec() const {
    return m_endTimeMicroSec * 0.000001;
}