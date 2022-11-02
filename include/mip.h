#ifndef mip_included
#define mip_included
//----------------------------------------------------------------------

//#define MIP_IMPLEMENTATION


//------------------------------
//
// system includes
//
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

#ifdef __gnu_linux__
  #include <dlfcn.h>
#endif

//------------------------------
//
// compiler warnings
//
//------------------------------

//#pragma GCC diagnostic push
//#pragma GCC diagnostic push

/*
  -Wall -Wextra
  we still have lots of unimplemented stub functions, so -Wextra prints
  gazillions of 'unused parameter' warnings..
  TODO: remove this when we have implemented everything
  OR: add __MIP_UNUSED everywhere?
*/

#pragma GCC diagnostic ignored "-Wunused-parameter"

/*
  stb_image
  todo: move this closer to stb_image, and pop the setting afterwards
*/

#pragma GCC diagnostic ignored "-Wshift-negative-value"
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"

/*
  opengl
*/

#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wcast-function-type"

/*
  vst3
*/

// -> compiler option: -Wno-unknown-pragmas
//#ifdef MIP_WIN32
//  #pragma GCC diagnostic ignored "-Wunknown-pragmas"
//#endif

//------------------------------
//
// mip framework
//
//------------------------------

// be careful, ordering might be important..

#include "base/mip_config.h"
#include "base/mip_defines.h"
#include "base/mip_const.h"
#include "base/mip_enums.h"

#include "base/mip_attributes.h"
#include "base/debug/mip_debug.h"
#include "base/types/mip_types.h"

//------------------------------
//
//
//
//------------------------------

#ifdef MIP_LINUX
  #include "base/system/mip_linux.h"
#endif

#ifdef MIP_WIN32
  #include "base/system/mip_win32.h"
#endif

//----------------------------------------------------------------------
#endif
