#ifndef mip_painter_included
#define mip_painter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_drawable.h"
#include "gui/base/mip_base_painter.h"
//#include "gui/mip_bitmap.h"
//#include "gui/mip_surface.h"
//#include "gui/mip_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifdef MIP_PAINTER_OPENGL
  #include "gui/opengl/mip_opengl_painter.h"
  typedef MIP_OpenGLPainter MIP_ImplementedPainter;
#endif

#ifdef MIP_PAINTER_NANOVG
  #include "gui/nanovg/mip_nanovg_painter.h"
  typedef MIP_NanoVGPainter MIP_ImplementedPainter;
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Painter
: public MIP_ImplementedPainter {

//------------------------------
protected:
//------------------------------


//------------------------------
public:
//------------------------------

  MIP_Painter(MIP_Drawable* ATarget, MIP_Drawable* ASource)
  : MIP_ImplementedPainter(ATarget,ASource) {
  }

  //----------

  virtual ~MIP_Painter() {
  }

//------------------------------
public:
//------------------------------

  virtual void drawTextBox(MIP_DRect ARect, const char* AText, uint32_t AAlignment, MIP_Color AColor) override {
    float bounds[4];;
    textBounds(ARect.x,ARect.y,AText,nullptr,bounds);
    float xmin = bounds[0];
    float ymin = bounds[1];
    float xmax = bounds[2];
    float ymax = bounds[3];
    float width = xmax - xmin;
    float height = ymax - ymin;
    float x_avail = ARect.w - width;
    float y_avail = ARect.h - height;
    float x = ARect.x + (ARect.x - xmin);
    float y = ARect.y + (ARect.y - ymin);
    if (AAlignment & MIP_TEXT_ALIGN_LEFT) {}
    else if (AAlignment & MIP_TEXT_ALIGN_RIGHT) { x += x_avail; }
    else { x += (x_avail * 0.5); }
    if (AAlignment & MIP_TEXT_ALIGN_TOP) {}
    else if (AAlignment & MIP_TEXT_ALIGN_BOTTOM) { y += y_avail; }
    else { y += (y_avail * 0.5); }
    fillColor(AColor);
    text(x,y,AText,nullptr);
  }

};

//----------------------------------------------------------------------
#endif


