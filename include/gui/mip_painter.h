#ifndef mip_painter_included
#define mip_painter_included
//----------------------------------------------------------------------

#include "mip.h"
//#include "base/types/mip_rect.h"
#include "base/types/mip_stack.h"
//#include "gui/mip_widget.h"
#include "gui/mip_drawable.h"
#include "gui/base/mip_base_painter.h"

typedef MIP_Stack<MIP_FRect,16> MIP_RectStack;

//----------------------------------------------------------------------

#ifdef MIP_NO_GUI
  typedef MIP_BasePainter MIP_ImplementedPainter;
#endif

#ifdef MIP_PAINTER_XCB
  #include "gui/xcb/mip_xcb_painter.h"
  typedef MIP_XcbPainter MIP_ImplementedPainter;
#endif

#ifdef MIP_PAINTER_CAIRO
  #include "gui/cairo/mip_cairo_painter.h"
  typedef MIP_CairoPainter MIP_ImplementedPainter;
#endif

//----------------------------------------------------------------------

class MIP_Painter
: public MIP_ImplementedPainter {

//------------------------------
private:
//------------------------------

  MIP_FRect      MClipRect;
  MIP_RectStack  MClipStack;

//------------------------------
public:
//------------------------------

  MIP_Painter(MIP_Drawable* ATarget)
  : MIP_ImplementedPainter(ATarget) {
    MClipRect = MIP_FRect( ATarget->getWidth()-1, ATarget->getHeight()-1 );
    //MClipStack.push(MClipRect);
  }

  //----------

  virtual ~MIP_Painter() {
  }

//------------------------------
public:
//------------------------------

  /*
    - push current clip rect
    - set new clip rect
  */

  void pushClip(MIP_FRect ARect) {
    MClipStack.push(MClipRect);
    MClipRect = ARect;
    //resetClip();
    setClip(MClipRect);
  }

  //----------

  /*
    - pop rect
    - set clip rect to popped rect
  */

  MIP_FRect popClip() {
    MClipRect = MClipStack.pop();
    //resetClip();
    setClip(MClipRect);
    return MClipRect;
  }

  //----------

  void resetClipStack() {
    MClipStack.reset();
  }

  //----------

  void setClipRect(MIP_FRect ARect) {
    MClipRect = ARect;
  }

  //----------

  MIP_FRect getClipRect() {
    return MClipRect;
  }

};

//----------------------------------------------------------------------
#endif















