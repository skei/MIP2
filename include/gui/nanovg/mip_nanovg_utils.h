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

//void nvg_text_box(NVGcontext* nvg, MIP_DRect ARect, const char* AText, uint32_t AAlignment, MIP_Color AColor) {
//  float bounds[4];;
//  nvgTextBounds(nvg,ARect.x,ARect.y,AText,nullptr,bounds);
//  float xmin = bounds[0];
//  float ymin = bounds[1];
//  float xmax = bounds[2];
//  float ymax = bounds[3];
//  float width = xmax - xmin;
//  float height = ymax - ymin;
//  float x_avail = ARect.w - width;
//  float y_avail = ARect.h - height;
//  float x = ARect.x + (ARect.x - xmin);
//  float y = ARect.y + (ARect.y - ymin);
//  if (AAlignment & MIP_TEXT_ALIGN_LEFT) {}
//  else if (AAlignment & MIP_TEXT_ALIGN_RIGHT) { x += x_avail; }
//  else { x += (x_avail * 0.5); }
//  if (AAlignment & MIP_TEXT_ALIGN_TOP) {}
//  else if (AAlignment & MIP_TEXT_ALIGN_BOTTOM) { y += y_avail; }
//  else { y += (y_avail * 0.5); }
//  nvgFillColor(nvg,nvg_color(AColor));
//  nvgText(nvg,x,y,AText,nullptr);
//}


//----------------------------------------------------------------------
#endif
