#ifndef mip_win32_lock_included
#define mip_win32_lock_included
//----------------------------------------------------------------------

#include "mip.h"

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
