#ifndef mip_timer_included
#define mip_timer_included
//----------------------------------------------------------------------

#include "mip.h"

//----------

#ifdef MIP_LINUX
  #include "base/system/linux/mip_linux_timer.h"
#endif

#ifdef MIP_WIN32
  #include "base/system/win32/mip_win32_timer.h"
#endif

//----------------------------------------------------------------------
#endif
