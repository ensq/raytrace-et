#ifndef DV2520_TIMER_H
#define DV2520_TIMER_H

#include <stdio.h>

// High precision time measurements are system specific:
#define TIMER timer ## __FILE__ ## __LINE__
#ifdef _WIN32
#include <windows.h>
#define TIMER_PRINT_ELAPSED(expr, string)                               \
    Timer TIMER;                                                        \
    TIMER ## .start();                                                  \
    expr;                                                               \
    TIMER ## .stop();                                                   \
    char str ## __FILE__ ## __LINE__ ## [256];                          \
    sprintf_s(str ## __FILE__ ## __LINE__ ,                             \
              256,                                                      \
              "%s: %f ms\n",                                            \
              string,                                                   \
              TIMER ## .getElapsedTimeMilliSec());                      \
    OutputDebugString(str ## __FILE__ ## __LINE__);   
#else
#include <sys/time.h>
#define TIMER_PRINT_ELAPSED(expr, string)                               \
    Timer TIMER;                                                        \
    TIMER ## .start();                                                  \
    expr;                                                               \
    TIMER ## .stop();                                                   \
    printf("%s: %f ms\n",                                               \
           string,                                                      \
               TIMER ## .getElapsedTimeMilliSec());                     
#endif // _WIN32



    
class Timer {
  public:
    Timer();
    virtual ~Timer();

    void start();
    void stop();
    void reset();

    double getElapsedTimeMicroSec();
    double getElapsedTimeMilliSec();
    double getElapsedTimeSec();

    double getStartTimeMicroSec() const;
    double getStartTimeMilliSec() const;
    double getStartTimeSec() const;

    double getEndTimeMicroSec() const;
    double getEndTimeMilliSec() const;
    double getEndTimeSec() const;
  protected:
  private:
    bool m_ticking;
    double m_startTimeMicroSec;
    double m_endTimeMicroSec;

#ifdef _WIN32
    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_startCount;
    LARGE_INTEGER m_endCount;
#else
    timeval m_startCount;
    timeval m_endCount;
#endif // _WIN32
};

#endif // DV2520_TIMER_H
