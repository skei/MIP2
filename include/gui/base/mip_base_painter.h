#ifndef mip_base_painter_included
#define mip_base_painter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_drawable.h"

//----------------------------------------------------------------------


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_BasePainter {

//------------------------------
public:
//------------------------------

  MIP_BasePainter(MIP_Drawable* ASurface, MIP_Drawable* ATarget) {
  }

  //----------

  virtual ~MIP_BasePainter() {
  }

  //----------

  virtual void beginPaint(int32_t AWidth, int32_t AHeight) {}
  virtual void endPaint() {}

};

//----------------------------------------------------------------------
#endif
