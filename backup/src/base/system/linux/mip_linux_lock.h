#ifndef mip_linux_lock_included
#define mip_linux_lock_included
//----------------------------------------------------------------------

#include "mip.h"

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

//----------------------------------------------------------------------
#endif
