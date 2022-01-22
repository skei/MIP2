#ifndef mip_time_included
#define mip_time_included
//----------------------------------------------------------------------

#include "mip.h"

//----------

#ifdef MIP_LINUX
  #include "base/system/linux/mip_linux_time.h"
#endif

#ifdef MIP_WIN32
  #include "base/system/win32/mip_win32_time.h"
#endif

//----------------------------------------------------------------------
#endif
