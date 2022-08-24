#ifndef mip_glx_painter_included
#define mip_glx_painter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_drawable.h"
#include "gui/base/mip_base_painter.h"
#include "gui/wgl/mip_wgl.h"
#include "gui/wgl/mip_wgl_utils.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_WglPainter
: public MIP_BasePainter {

//------------------------------
public:
//------------------------------

  // ASurface = pixmap
  // ATarget  = window

  MIP_WglPainter(MIP_Drawable* ASurface, MIP_Drawable* ATarget)
  : MIP_BasePainter(ASurface,ATarget) {
  }

  virtual ~MIP_WglPainter() {
  }

}

//----------------------------------------------------------------------
#endif
