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

};

//----------------------------------------------------------------------
#endif


