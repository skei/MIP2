#ifndef mip_base_painter_included
#define mip_base_painter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_drawable.h"

//----------------------------------------------------------------------

//typedef NVGpaint MIP_PaintSource;
//struct MIP_PaintSource;
//typedef NVGpaint MIP_PaintSource;

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

//------------------------------
public:
//------------------------------

  virtual void beginPaint(int32_t AWidth, int32_t AHeight) {}
  virtual void endPaint() {}

  virtual void drawTextBox(MIP_DRect ARect, const char* AText, uint32_t AAlignment, MIP_Color AColor) {}



};

//----------------------------------------------------------------------
#endif
