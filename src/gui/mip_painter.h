#ifndef mip_painter_included
#define mip_painter_included
//----------------------------------------------------------------------

#include "mip.h"
//#include "base/types/mip_rect.h"
//#include "gui/mip_widget.h"
#include "gui/mip_drawable.h"
#include "gui/base/mip_base_painter.h"

//----------------------------------------------------------------------

#ifdef MIP_NO_GUI
  typedef MIP_BasePainter MIP_ImplementedPainter;
#endif

#ifdef MIP_GUI_XCB
  #include "gui/xcb/mip_xcb_painter.h"
  typedef MIP_XcbPainter MIP_ImplementedPainter;
#endif

//----------------------------------------------------------------------

class MIP_Painter
: public MIP_ImplementedPainter {


//------------------------------
public:
//------------------------------

  MIP_Painter(MIP_Drawable* ATarget)
  : MIP_ImplementedPainter(ATarget) {
    MIP_PRINT;
  }

  //----------

  virtual ~MIP_Painter() {
    MIP_PRINT;
  }


};

//----------------------------------------------------------------------
#endif
