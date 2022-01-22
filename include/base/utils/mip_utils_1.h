#ifndef mip_utils_included
#define mip_utils_included
//----------------------------------------------------------------------

#include "mip.h"

// MIP_GetThreadId
#include <sys/syscall.h>
#include <sys/unistd.h>
#define gettid() syscall(SYS_gettid)

// sleep
// -lrt
#include <time.h>     // timer
//#include <sys/time.h>
//#include <signal.h>     // sigval
//#include <errno.h>      // errno


//----------------------------------------------------------------------

//TODO: -> MIP_StrUtils.h

// https://stackoverflow.com/questions/7666509/hash-function-for-string

uint32_t MIP_HashString(const char* buffer) {
  char* ptr = (char*)buffer;
  unsigned long h = 5381;
  int c;
  while ((c = *ptr++)) {
    h = ((h << 5) + h) + c; // h * 33 + c
  }
  return h;
}

//----------------------------------------------------------------------

// uint32_t thread_id = MIP_GetThreadId();

pid_t MIP_GetProcessId() {
  return getpid();
}

//----------

pid_t MIP_GetThreadId() {
  return gettid();
}

//----------------------------------------------------------------------

// usleep is deprecated

//void MIP_Sleep(uint32_t ms) {
//  usleep(ms*1000); // ms*1000;
//}

int MIP_Sleep(long ms) {
  struct timespec req, rem;
  if (ms > 999) {
    req.tv_sec = (int)(ms / 1000);                            // Must be Non-Negative
    req.tv_nsec = (ms - ((long)req.tv_sec * 1000)) * 1000000; // Must be in range of 0 to 999999999
  }
  else {
    req.tv_sec = 0;               // Must be Non-Negative
    req.tv_nsec = ms * 1000000;   // Must be in range of 0 to 999999999
  }
  return nanosleep(&req , &rem);
}

//----------------------------------------------------------------------
#endif
