#ifndef mip_shm_included
#define mip_shm_included
//----------------------------------------------------------------------

#include "mip.h"

//----------

#ifdef MIP_LINUX
  #include "base/system/linux/mip_linux_shm.h"
#endif

#ifdef MIP_WIN32
  #include "base/system/win32/mip_win32_shm.h"
#endif

//----------------------------------------------------------------------
#endif
