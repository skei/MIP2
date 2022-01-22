#ifndef mip_thread_included
#define mip_thread_included
//----------------------------------------------------------------------

#include "mip.h"

//----------

#ifdef MIP_LINUX
  #include "base/system/linux/mip_linux_thread.h"
#endif

#ifdef MIP_WIN32
  #include "base/system/win32/mip_win32_thread.h"
#endif

//----------------------------------------------------------------------
#endif
