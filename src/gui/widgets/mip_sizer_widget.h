#ifndef kode_sizer_widget_included
#define kode_sizer_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"

#define KODE_SIZER_NONE   0
#define KODE_SIZER_LEFT   1
#define KODE_SIZER_RIGHT  2
#define KODE_SIZER_TOP    3
#define KODE_SIZER_BOTTOM 4
#define KODE_SIZER_WINDOW 5

//----------------------------------------------------------------------

class KODE_SizerWidget
: public KODE_Widget {

//------------------------------
  protected:
//------------------------------

    float         prevx = 0.0f;
    float         prevy = 0.0f;
    bool          MIsDragging =false;
    KODE_Widget*  MTarget = KODE_NULL;
    uint32_t      MMode = KODE_SIZER_NONE;
    KODE_Color    MFillColor = KODE_Color( 0.40f );

//------------------------------
public:
//------------------------------

  KODE_SizerWidget(KODE_FRect ARect,uint32_t AMode=KODE_SIZER_NONE,KODE_Widget* ATarget=KODE_NULL)
  : KODE_Widget(ARect) {
    setName("KODE_SizerWidget");
    setHint("sizer");
    setMode(AMode);
    setTarget(ATarget);
    switch (AMode) {
      case KODE_SIZER_NONE:
      case KODE_SIZER_LEFT:
        layout.alignment = KODE_WIDGET_ALIGN_FILL_LEFT;
        break;
      case KODE_SIZER_RIGHT:
        layout.alignment = KODE_WIDGET_ALIGN_FILL_RIGHT;
        break;
      case KODE_SIZER_TOP:
        layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
        break;
      case KODE_SIZER_BOTTOM:
        layout.alignment = KODE_WIDGET_ALIGN_FILL_BOTTOM;
        break;
      case KODE_SIZER_WINDOW:
        //layout.alignment = KODE_WIDGET_ALIGN_LEFT_BOTTOM;
        break;
    }
  }

  virtual ~KODE_SizerWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setMode(uint32_t AMode) {
    MMode = AMode;
    switch(MMode) {
      case KODE_SIZER_NONE:     setCursor(KODE_CURSOR_DEFAULT);         break;
      case KODE_SIZER_LEFT:     setCursor(KODE_CURSOR_ARROWLEFTRIGHT);  break;
      case KODE_SIZER_RIGHT:    setCursor(KODE_CURSOR_ARROWLEFTRIGHT);  break;
      case KODE_SIZER_TOP:      setCursor(KODE_CURSOR_ARROWUPDOWN);     break;
      case KODE_SIZER_BOTTOM:   setCursor(KODE_CURSOR_ARROWUPDOWN);     break;
      case KODE_SIZER_WINDOW:   setCursor(KODE_CURSOR_ARROWDIAGLEFT);   break;
    }
  }

  virtual void setTarget(KODE_Widget* ATarget) {
    MTarget = ATarget;
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) final {
    //if (MMode == KODE_SIZER_WINDOW) {
    //  APainter->fillRectangle( KODE_FRect( MRect.x2()-1, MRect.y, MRect.x2()-1, MRect.y2()-1, MRect.x, MRect.y2()-1 );
    //}
    //else {
    //  APainter->fillRectangle(getRect(),MFillColor);
    //}
//    KODE_FRect rect = getRect();
//    rect.overlap(ARect);
//    APainter->fillRectangle(rect,MFillColor);
    //APainter->fillRectangle(getRect(),MFillColor);
    APainter->fillRectangle(ARect,MFillColor);
  }

  //----------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == KODE_BUTTON_LEFT) {
      MIsDragging = true;
      prevx = AXpos;
      prevy = AYpos;
    }
  }

  //----------
  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == KODE_BUTTON_LEFT) MIsDragging = false;
  }

  //----------
  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (MIsDragging) {
      float deltax = AXpos - prevx;
      float deltay = AYpos - prevy;
      switch(MMode) {
        case KODE_SIZER_LEFT:
          deltay = 0;
          break;
        case KODE_SIZER_RIGHT:
          deltay = 0;
          deltax = -deltax;
          break;
        case KODE_SIZER_TOP:
          deltax = 0;
          break;
        case KODE_SIZER_BOTTOM:
          deltax = 0;
          deltay = -deltay;
          break;
        case KODE_SIZER_WINDOW:
          break;

      } // switch mode

      //if FMode = ksm_Horizontal then deltax := 0;
      //if FMode = ksm_Vertical then deltay := 0;
      //if (MMode == KODE_SIZER_WINDOW) {
      //  if (MParent) MParent->do_widget_resized(this,deltax,deltay);
      //}
      //else {

        if (MTarget) {


          float tw = MTarget->getRect().w;
          float th = MTarget->getRect().h;
          // todo: check flags.sizePercent
          tw += deltax;
          th += deltay;
          KODE_FPoint tmin = MTarget->layout.minSize;
          KODE_FPoint tmax = MTarget->layout.maxSize;
          if ( (tw > tmin.w) && (tw < tmax.w)
            && (th > tmin.h) && (th < tmax.h) ) {
            MTarget->do_widget_resized(this,deltax,deltay);
          }
        }
      //  else self.do_widget_resized(self,deltax,deltay,FMode);
      //}
      prevx = AXpos;
      prevy = AYpos;
    } // dragging
    //axWidget::doMouseMove(aXpos,aYpos,aButton);
  }


};

//----------------------------------------------------------------------
#endif
