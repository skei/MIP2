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

#ifdef MIP_PAINTER_CAIRO
  #include "gui/cairo/mip_cairo_painter.h"
  typedef MIP_CairoPainter MIP_ImplementedPainter;
#endif

#ifdef MIP_PAINTER_GDI
  #include "gui/gdi/mip_gdi_painter.h"
  typedef MIP_GdiPainter MIP_ImplementedPainter;
#endif

#ifdef MIP_PAINTER_GLX
  #include "gui/opengl/mip_opengl_painter.h"
  typedef MIP_GlxPainter MIP_ImplementedPainter;
#endif

#ifdef MIP_PAINTER_NANOVG
  #include "gui/nanovg/mip_nanovg_painter.h"
  typedef MIP_NanoVGPainter MIP_ImplementedPainter;
#endif

#ifdef MIP_PAINTER_WGL
  #include "gui/wgl/mip_wgl_painter.h"
  typedef MIP_WglPainter MIP_ImplementedPainter;
#endif

#ifdef MIP_PAINTER_XCB
  #include "gui/xcb/mip_xcb_painter.h"
  typedef MIP_XcbPainter MIP_ImplementedPainter;
#endif


//----------

// clip stack

#define MIP_RECT_STACK_SIZE 1024
typedef MIP_Stack<MIP_DRect,MIP_RECT_STACK_SIZE> MIP_DRectStack;

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

  MIP_DRect       MClipRect;
  MIP_DRectStack  MClipStack;

//------------------------------
public:
//------------------------------

  MIP_Painter(MIP_Drawable* ATarget, MIP_Drawable* ASource)
  : MIP_ImplementedPainter(ATarget,ASource) {

    MClipRect = MIP_DRect( ATarget->drawable_getWidth()-1, ATarget->drawable_getHeight()-1 );

  }

  //----------

  virtual ~MIP_Painter() {
  }

//------------------------------
public: // clipping
//------------------------------

  /*
    - push current clip rect
    - set new clip rect
  */

  //virtual void pushClip(MIP_DRect ARect) {
  void pushClip(MIP_DRect ARect) override {
    MClipStack.push(MClipRect);
    MClipRect = ARect;
    resetClip();
    setClip(MClipRect);
  }

  //virtual void pushOverlapClip(MIP_DRect ARect) {
  void pushOverlapClip(MIP_DRect ARect) override {
    MIP_DRect r = ARect;
    r.overlap(MClipRect);
    pushClip(r);
  }

  //----------

  /*
    - pop rect
    - set clip rect to popped rect
  */

  //virtual MIP_DRect popClip() {
  MIP_DRect popClip() override {
    MClipRect = MClipStack.pop();
    resetClip();
    setClip(MClipRect);
    return MClipRect;
  }

  //----------

  //virtual void resetClipStack() {
  void resetClipStack() override {
    MClipStack.reset();
  }

  //----------

  //virtual void setClipRect(MIP_DRect ARect) {
  void setClipRect(MIP_DRect ARect) override {
    MClipRect = ARect;
  }

  //----------

  //virtual MIP_DRect getClipRect() {
  MIP_DRect getClipRect() override {
    return MClipRect;
  }

//------------------------------
public:
//------------------------------

  void drawTextBox(MIP_DRect ARect, const char* AText, uint32_t AAlignment, MIP_Color AColor) override {
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

  //----------

  void triangle(double x1, double y1, double x2, double y2, double x3, double y3) override {
    moveTo(x1,y1);
    lineTo(x2,y2);
    lineTo(x3,y3);
    lineTo(x1,y1);
  }

  //----------

  void rectangle(MIP_DRect r) override {
    rect(r.x,r.y,r.w,r.h);
  }

  //----------

  void ellipse(MIP_DRect r) override {
    float cx = r.x + (r.w * 0.5);
    float cy = r.y + (r.h * 0.5);
    float rx = (r.w * 0.5);
    float ry = (r.h * 0.5);
    MIP_ImplementedPainter::ellipse(cx,cy,rx,ry);
  }

  //----------


};

//----------------------------------------------------------------------
#endif


