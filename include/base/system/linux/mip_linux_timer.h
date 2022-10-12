#ifndef mip_linux_timer_included
#define mip_linux_timer_included
//----------------------------------------------------------------------

// -lrt

#include <time.h>   // timer
#include <signal.h> // sigval

//#include <sys/time.h>
//#include <errno.h>      // errno


#include "base/utils/mip_math.h"

//----------------------------------------------------------------------

class MIP_Timer;

//----------

class MIP_TimerListener {
  public:
    virtual void on_timerCallback(MIP_Timer* ATimer) {}
};

//----------------------------------------------------------------------

//static void mip_timer_callback(sigval val) {
//  //MIP_TimerListener* listener = (MIP_TimerListener*)val.sival_ptr;
//  //if (listener) listener->on_timerCallback(val.sival_int);
//  MIP_Timer* timer = (MIP_Timer*)val.sival_ptr;
//  if (timer) {
//    timer->on_timer();
//    //if (listener) listener->on_timerCallback(val.sival_int);
//  }
//}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Timer {

private:

  //friend
  //static void mip_timer_callback(sigval val);

  static void mip_timer_callback(sigval val) {
    //MIP_TimerListener* listener = (MIP_TimerListener*)val.sival_ptr;
    //if (listener) listener->on_timerCallback(val.sival_int);
    MIP_Timer* timer = (MIP_Timer*)val.sival_ptr;
    if (timer) {
      timer->on_timer();
      //if (listener) listener->on_timerCallback(val.sival_int);
    }
  }

private:

  sigevent            MSigEvent;
  timer_t             MTimer;
  itimerspec          MTimerSpec;
  bool                MRunning  = false;
  MIP_TimerListener*  MListener = nullptr;

public:

  MIP_Timer(MIP_TimerListener* AListener, void* AUserPtr) {

    MListener = AListener;

    MSigEvent.sigev_notify            = SIGEV_THREAD;
    MSigEvent.sigev_notify_function   = mip_timer_callback;
    MSigEvent.sigev_notify_attributes = nullptr;
    MSigEvent.sigev_value.sival_ptr   = this;//AListener;
    //int res =
    timer_create(CLOCK_REALTIME, &MSigEvent, &MTimer);
    //MIP_Print("MTimer %i\n",MTimer);
    /*if (res == -1) {
      switch(errno) {
        case EAGAIN:
          MIP_Print("timer_create error: Temporary error during kernel allocation of timer structures.\n");
          break;
        case EINVAL:
          MIP_Print("timer_create error: Clock ID, sigev_notify, sigev_signo, or sigev_notify_thread_id is invalid.\n");
          break;
        case ENOMEM:
          MIP_Print("timer_create error: Could not allocate memory.\n");
          break;
      }
    }*/
  }

  //----------

  // timer_delete() deletes the timer whose ID is given in timerid.  If
  // the timer was armed at the time of this call, it is disarmed before
  // being deleted.  The treatment of any pending signal generated by the
  // deleted timer is unspecified.

  ~MIP_Timer() {
    timer_delete(MTimer);
  }

  //----------

  void start(float ms, bool oneshot=false) {
    if (!MRunning) {
      float s = ms * 0.001f;
      float sec = MIP_Trunc(s);
      float nsec = (s-sec) * 1000000000; // 1000000.0f * (sec - MIP_Fract(s));
      time_t isec = sec;
      long insec = nsec;
      //MIP_Print("s %.4f sec %.4f nsec %.4f isec %i insec %i\n",s,sec,nsec,isec,insec);
      if (oneshot) {
        MTimerSpec.it_interval.tv_sec   = 0;
        MTimerSpec.it_interval.tv_nsec  = 0;
      }
      else {
        MTimerSpec.it_interval.tv_sec   = isec;
        MTimerSpec.it_interval.tv_nsec  = insec;
      }
      MTimerSpec.it_value.tv_sec      = isec;
      MTimerSpec.it_value.tv_nsec     = insec;
      //int res =
      timer_settime(MTimer, 0, &MTimerSpec, 0);
      /*if (res == -1) {
        switch(errno) {
          case EFAULT:
            MIP_Print("timer_settime error: new_value, old_value, or curr_value is not a valid pointer.\n");
            break;
          //case EINVAL:
          //  MIP_Print("timer_settime error: timerid is invalid.\n");
          //  break;
          case EINVAL:
            MIP_Print("timer_settime: new_value.it_value is negative; or new_value.it_value.tv_nsec is negative or greater than 999,999,999.\n");
            break;
        }
      }*/
      MRunning = true;
    }
  }

  //----------

  // If new_value->it_value specifies a nonzero value (i.e., either sub‐
  // field is nonzero), then timer_settime() arms (starts) the timer, set‐
  // ting it to initially expire at the given time.  (If the timer was
  // already armed, then the previous settings are overwritten.)  If
  // new_value->it_value specifies a zero value (i.e., both subfields are
  // zero), then the timer is disarmed.

  void stop() {
    if (MRunning) {
      MTimerSpec.it_interval.tv_sec   = 0;
      MTimerSpec.it_interval.tv_nsec  = 0;
      MTimerSpec.it_value.tv_sec      = 0;
      MTimerSpec.it_value.tv_nsec     = 0;
      /*int res =*/ timer_settime(MTimer, 0, &MTimerSpec, 0);
      //if (res != 0) { MIP_Print("error stopping timer\n"); }
      MRunning = false;
    }
  }

  //----------

  void on_timer() {
    if (MListener) MListener->on_timerCallback(this);
  }

};

//----------------------------------------------------------------------
#endif

