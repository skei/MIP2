#ifndef mip_paths_included
#define mip_paths_included
//----------------------------------------------------------------------

#include "mip.h"

//----------

#ifdef MIP_LINUX
  #include "base/system/linux/mip_linux_paths.h"
#endif

#ifdef MIP_WIN32
  #include "base/system/win32/mip_win32_paths.h"
#endif

//----------------------------------------------------------------------
#endif
