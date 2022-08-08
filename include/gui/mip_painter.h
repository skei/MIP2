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

//----------

#if 0
#define MIP_RECT_STACK_SIZE 1024
typedef MIP_Stack<MIP_DRect,MIP_RECT_STACK_SIZE> MIP_DRectStack;
#endif // 0

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Painter
: public MIP_ImplementedPainter {

//------------------------------
private:
//------------------------------

  #if 0
  MIP_DRect      MClipRect;
  MIP_DRectStack  MClipStack;
  #endif

//------------------------------
public:
//------------------------------

  MIP_Painter(MIP_Drawable* ATarget, MIP_Drawable* ASource)
  : MIP_ImplementedPainter(ATarget,ASource) {

    #if 0
    MClipRect = MIP_FRect( ATarget->getWidth()-1, ATarget->getHeight()-1 );
    #endif

  }

  //----------

  virtual ~MIP_Painter() {
  }

//------------------------------
public: // clipping
//------------------------------

  #if 0

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

  #endif // 0

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


