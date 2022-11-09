#ifndef mip_win32_time_included
#define mip_win32_time_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/mip_const.h"
#include "base/utils/mip_math.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

double MIP_GetTimeMS(void) {
  return 0.0;
}

//----------

struct MIP_CurrentTime {
  int32_t year;
  int32_t month;
  int32_t day;
  int32_t hour;
  int32_t minutes;
  int32_t seconds;
};

//----------

void MIP_GetLocalTime(MIP_CurrentTime* ATime) {
  ATime->year    = 0;
  ATime->month   = 0;
  ATime->day     = 0;
  ATime->hour    = 0;
  ATime->minutes = 0;
  ATime->seconds = 0;
}

//----------

// usleep is deprecated

//void MIP_Sleep(MIP_ui32 ms) {
//  usleep(ms*1000); // ms*1000;
//}

int MIP_Sleep(long ms) {
  return 0;
}

//----------

//----------------------------------------------------------------------
// timer
//----------------------------------------------------------------------

//class MIP_TimerListener {
//  public:
//    virtual void on_timerCallback(void) {}
//};

//----------

//void mip_timer_callback(sigval val) {
//  MIP_TimerListener* listener = (MIP_TimerListener*)val.sival_ptr;
//  if (listener) listener->on_timerCallback();
//}

//----------

//class MIP_Timer {
//
//private:
//
//
//public:
//
//  MIP_Timer() {
//  }
//
//  //----------
//
//  ~MIP_Timer() {
//  }
//
//  //----------
//
//  void start(float ms, bool oneshot=false) {
//  }
//
//  //----------
//
//  void stop(void) {
//  }
//
//};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

//class MIP_Time {
//
//  public:
//
//    MIP_Time(bool AStart=true) {
//      if (AStart) start();
//    }
//
//    ~MIP_Time() {}
//
//  public:
//
//    double start(void) {
//      MStartTime = clock();
//      return MStartTime;
//    }
//
//    /*
//      wraparound? negative?
//      if clock has wrapped around since start() was called, we would
//      clock_t -> long int
//    */
//
//    double sec(void) {
//      clock_t currenttime = clock();
//      double elapsed = currenttime - MStartTime;
//      return elapsed / CLOCKS_PER_SEC; // 1000000;
//    }
//
//    double ms(void) {
//      return sec() * 1000.0;
//    }
//
//    double us(void) {
//      return sec() * 1000000.0;
//    }
//
//  private:
//      clock_t MStartTime = 0; // cppcheck
//};

//----------------------------------------------------------------------
#endif
