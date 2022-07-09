#ifndef mip_painter_included
#define mip_painter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_paint_source.h"
#include "gui/mip_paint_target.h"
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
  typedef MIP_OpenGLPainter MIP_BasePainter;
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Painter
: public MIP_BasePainter {

//------------------------------
protected:
//------------------------------


//------------------------------
public:
//------------------------------

  MIP_Painter(MIP_PaintTarget* ATarget, MIP_PaintSource* ASource)
  : MIP_BasePainter(ATarget,ASource) {
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


