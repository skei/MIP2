#ifndef mip_included
#define mip_included
//----------------------------------------------------------------------

//------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <memory.h>
#include <string.h>

//#include <sys/types.h>
//#include <sys/stat.h>
#include <dirent.h>

#ifdef MIP_LINUX
  #include <dlfcn.h>
#endif

//------------------------------

/*
  we still have LOTS of unimplemented stub functions, so -Wextra prints
  gazillions of 'unused parameter' warnings..
  TODO: remove this when we have implemented everything
  OR: add __MIP_UNUSED everywhere...
*/

//#pragma GCC diagnostic push
//#pragma GCC diagnostic error "-Wuninitialized"
#pragma GCC diagnostic ignored "-Wunused-parameter"
//#pragma GCC diagnostic push

// stb_image
#pragma GCC diagnostic ignored "-Wshift-negative-value"
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"

//------------------------------

// be careful, ordering might be important..

#include "base/mip_config.h"
#include "base/mip_defines.h"
#include "base/mip_const.h"
#include "base/mip_enums.h"
#include "base/mip_attributes.h"

#include "base/debug/mip_debug.h"

//----------

#include "base/types/mip_types.h"
//#include "base/types/mip_color.h"
//#include "base/types/mip_point.h"
//#include "base/types/mip_rect.h"

//#include <vector>

//----------------------------------------------------------------------
#endif
