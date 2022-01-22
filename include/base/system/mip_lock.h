#ifndef mip_lock_included
#define mip_lock_included
//----------------------------------------------------------------------

#include "mip.h"

//----------

#ifdef MIP_LINUX
  #include "base/system/linux/mip_linux_lock.h"
#endif

#ifdef MIP_WIN32
  #include "base/system/win32/mip_win32_lock.h"
#endif

//----------------------------------------------------------------------
#endif
