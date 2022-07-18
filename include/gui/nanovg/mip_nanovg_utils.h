#ifndef mip_nanovg_utils_included
#define mip_nanovg_utils_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/nanovg/mip_nanovg.h"

//----------------------------------------------------------------------

NVGcolor nvg_color(MIP_Color AColor) {
  NVGcolor color;
  color.r = AColor.r;
  color.g = AColor.g;
  color.b = AColor.b;
  color.a = AColor.a;
  return color;
}

//----------------------------------------------------------------------
#endif
