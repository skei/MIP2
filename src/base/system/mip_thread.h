#ifndef mip_thread_included
#define mip_thread_included
//----------------------------------------------------------------------

#include <pthread.h>
//#include <unistd.h> // sleep

#include "base/system/mip_time.h"

class MIP_Thread {

private:

  bool        MThreadRunning;
  int32_t     MThreadSleep;
  void*       MUsr;
  pthread_t   MThreadHandle;

//------------------------------
public:
//------------------------------

  MIP_Thread() {
    MUsr = nullptr;
    MThreadRunning = false;
    MThreadSleep = -1;
    MThreadHandle = 0;
  }

  //----------

  virtual ~MIP_Thread() {
    if (MThreadRunning) stop();
  }

//------------------------------
public:
//------------------------------

  virtual void on_threadFunc(void* usr) { /*MIP_Trace("on_threadFunc\n");^*/ };

//------------------------------
private:
//------------------------------

  static
  void* threadProc(void* data) {
    MIP_Print("threadProc\n");
    MIP_Thread* thr = (MIP_Thread*)data;
    if (thr) {
      if (thr->MThreadSleep >= 0) {
        while (thr->MThreadRunning) {
          thr->on_threadFunc(thr->MUsr);
          //usleep(thr->MThreadSleep*1000); //ms*1000;
          MIP_Sleep(thr->MThreadSleep);
        }
      }
      else {
        thr->on_threadFunc(thr->MUsr);
      }
    }
    return nullptr;
  }

//------------------------------
public:
//------------------------------

  void start(void* usr, int32_t ms=-1) { // -1 = no timer
    MIP_Print("start\n");
    MUsr = usr;
    MThreadSleep = ms;
    MThreadRunning = true;
    pthread_create(&MThreadHandle,nullptr,&threadProc,this);
  }

  //----------

  void stop(void) {
    MIP_Print("stop\n");
    MThreadRunning = false;
    void* ret;
    pthread_join(MThreadHandle,&ret);
  }

//------------------------------

};

//----------------------------------------------------------------------
#endif
