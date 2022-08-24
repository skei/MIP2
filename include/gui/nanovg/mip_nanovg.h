#ifndef mip_nanovg_included
#define mip_nanovg_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/glx/mip_glx.h"

#define NANOVG_GL3_IMPLEMENTATION

#include "extern/nanovg/nanovg.h"
#include "extern/nanovg/nanovg_gl.h"
#include "extern/nanovg/nanovg_gl_utils.h"

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wmisleading-indentation"
    #include "extern/nanovg/nanovg.c"
  #pragma GCC diagnostic pop

//----------------------------------------------------------------------
#endif
