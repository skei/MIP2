#ifndef mip_paint_context_included
#define mip_paint_context_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_rect.h"
#include "gui/mip_painter.h"

//----------------------------------------------------------------------

struct MIP_PaintContext {
  MIP_FRect     updateRect;
  MIP_Painter*  painter;
  //MIP_WIdget**  dirtyWidgets;
};

//----------------------------------------------------------------------
#endif
