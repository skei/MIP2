#ifndef mip_paint_context_included
#define mip_paint_context_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_rect.h"
#include "gui/mip_painter.h"
#include "gui/mip_theme.h"
#include "gui/mip_widget.h"

//----------------------------------------------------------------------

struct MIP_PaintContext {
  uint32_t      mode        = MIP_WIDGET_PAINT_NORMAL;
  MIP_DRect     updateRect  = {};
  MIP_Painter*  painter     = nullptr;
  MIP_Theme*    theme       = &MIP_DefaultTheme;;
  //MIP_WIdget**  dirtyWidgets;
};

//----------------------------------------------------------------------
#endif
