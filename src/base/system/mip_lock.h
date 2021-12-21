#ifndef mip_lock_included
#define mip_lock_included
//----------------------------------------------------------------------

#include "mip.h"

#ifdef MIP_LINUX

// -lpthread
#include <pthread.h>

class MIP_Lock {

//------------------------------
private:
//------------------------------

  pthread_mutex_t MMutex; // = PTHREAD_MUTEX_INITIALIZER;

//------------------------------
public:
//------------------------------

  MIP_Lock() {
    pthread_mutex_init(&MMutex,nullptr);
  }

  //----------

  ~MIP_Lock() {
    pthread_mutex_destroy(&MMutex);
  }

  //----------

  void lock(void) {
    pthread_mutex_lock(&MMutex);
  }

  //----------

  bool tryLock(void) {
    return (pthread_mutex_trylock(&MMutex) == 0);
  }

  //----------

  void unlock(void) {
    pthread_mutex_unlock(&MMutex);
  }

//------------------------------
};

#endif // MIP_LINUX

//----------------------------------------------------------------------

//#ifdef MIP_WIN32
//
//  //#include <windows.h>
//
//  class MIP_Lock {
//    private:
//      CRITICAL_SECTION  MCriticalSection;
//
//    public:
//
//      MIP_Lock() {
//        InitializeCriticalSection(&MCriticalSection);
//      }
//
//      ~MIP_Lock() {
//        DeleteCriticalSection(&MCriticalSection);
//      }
//
//      void lock(void) {
//        EnterCriticalSection(&MCriticalSection);
//      }
//
//      bool tryLock(void) {
//        return TryEnterCriticalSection( &MCriticalSection );
//      }
//
//      void unlock(void) {
//        LeaveCriticalSection(&MCriticalSection);
//      }
//
//  };
//
//#endif // MIP_WIN32

//----------------------------------------------------------------------
#endif
