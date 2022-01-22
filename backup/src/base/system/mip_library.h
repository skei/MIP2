#ifndef mip_library_included
#define mip_library_included
//----------------------------------------------------------------------

#include "mip.h"

//----------

#ifdef MIP_LINUX
  #include "base/system/linux/mip_linux_library.h"
#endif

#ifdef MIP_WIN32
  #include "base/system/win32/mip_win32_library.h"
#endif

//----------------------------------------------------------------------
#endif
