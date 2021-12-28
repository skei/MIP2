#ifndef mip_utils_included
#define mip_utils_included
//----------------------------------------------------------------------

//#include "base/utils/mip_int.h"
//#include "base/utils/mip_math.h"
//#include "base/utils/mip_mem.h"
//#include "base/utils/mip_str.h"

//#include "base/utils/mip_convert.h"
//#include "base/utils/mip_fast_math.h"

//#include "base/utils/mip_easing.h"
//#include "base/utils/mip_interpolation.h"
//#include "base/utils/mip_windowing.h"

//#include "base/utils/mip_isaac.h"
//#include "base/utils/mip_tcc.h"
//#include "base/utils/mip_vm.h"

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

//int MIP_Sleep(long ms) {
//  struct timespec req, rem;
//  if (ms > 999) {
//    req.tv_sec = (int)(ms / 1000);                            // Must be Non-Negative
//    req.tv_nsec = (ms - ((long)req.tv_sec * 1000)) * 1000000; // Must be in range of 0 to 999999999
//  }
//  else {
//    req.tv_sec = 0;               // Must be Non-Negative
//    req.tv_nsec = ms * 1000000;   // Must be in range of 0 to 999999999
//  }
//  return nanosleep(&req , &rem);
//}

//----------------------------------------------------------------------
#endif
