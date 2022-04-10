#ifndef mip_painter_included
#define mip_painter_included
//----------------------------------------------------------------------

#include "mip.h"
//#include "base/types/mip_rect.h"
#include "base/types/mip_stack.h"
//#include "gui/mip_widget.h"
#include "gui/mip_drawable.h"
#include "gui/base/mip_base_painter.h"

#define MIP_RECT_STACK_SIZE 1024

typedef MIP_Stack<MIP_FRect,MIP_RECT_STACK_SIZE> MIP_RectStack;

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
    //MIP_Print("cliprect: %.2f,%.2f,%.2f,%.2f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
    //MClipStack.push(MClipRect);
  }

  //----------

  virtual ~MIP_Painter() {
  }

//------------------------------
public: // resizing
//------------------------------

  void resize(uint32_t AWidth, uint32_t AHeight) override {
    //MIP_Print("%i,%i\n",AWidth,AHeight);
    MIP_ImplementedPainter::resize(AWidth,AHeight);
    MClipStack.reset();
    MClipRect = MIP_FRect( AWidth/*-1*/, AHeight/*-1*/ );
    MClipStack.push(MClipRect);
  }

//------------------------------
public: // clipping
//------------------------------

  /*
    - push current clip rect
    - set new clip rect
  */

  void pushClip(MIP_FRect ARect) {
    //MIP_Print("ARect = %.2f,%.2f,%.2f,%.2f\n",ARect.x,ARect.y,ARect.w,ARect.h);
    //MIP_Print("depth: %i\n",MClipStack.getNumItems());
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
    //MIP_Print("MClipRect = %.2f,%.2f,%.2f,%.2f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
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
    //MIP_Print("ARect = %.2f,%.2f,%.2f,%.2f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
  }

  //----------

  MIP_FRect getClipRect() {
    return MClipRect;
  }

//------------------------------
public: // painting
//------------------------------



};

//----------------------------------------------------------------------
#endif















