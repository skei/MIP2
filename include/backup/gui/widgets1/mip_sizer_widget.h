#ifndef mip_sizer_widget_included
#define mip_sizer_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widget.h"
//#include "gui/widgets/v0/mip_panel_widget.h"
#include "gui/mip_widgets.h"


#define MIP_SIZER_NONE   0
#define MIP_SIZER_LEFT   1
#define MIP_SIZER_RIGHT  2
#define MIP_SIZER_TOP    3
#define MIP_SIZER_BOTTOM 4
#define MIP_SIZER_WINDOW 5

//----------------------------------------------------------------------

class MIP_SizerWidget
: public MIP_Widget {

//------------------------------
  protected:
//------------------------------

    float       prevx = 0.0f;
    float       prevy = 0.0f;
    bool        MIsDragging =false;
    MIP_Widget* MTarget = nullptr;
    uint32_t    MMode = MIP_SIZER_NONE;
    MIP_Color   MFillColor = MIP_Color( 0.45 );

//------------------------------
public:
//------------------------------

  MIP_SizerWidget(MIP_FRect ARect,uint32_t AMode=MIP_SIZER_NONE,MIP_Widget* ATarget=nullptr)
  : MIP_Widget(ARect) {
    setName("MIP_SizerWidget");
    setHint("sizer");
    setMode(AMode);
    setTarget(ATarget);
    switch (AMode) {
      case MIP_SIZER_NONE:
      case MIP_SIZER_LEFT:
        layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
        break;
      case MIP_SIZER_RIGHT:
        layout.alignment = MIP_WIDGET_ALIGN_FILL_RIGHT;
        break;
      case MIP_SIZER_TOP:
        layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        break;
      case MIP_SIZER_BOTTOM:
        layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;
        break;
      case MIP_SIZER_WINDOW:
        //layout.alignment = MIP_WIDGET_ALIGN_LEFT_BOTTOM;
        break;
    }
  }

  virtual ~MIP_SizerWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setMode(uint32_t AMode) {
    MMode = AMode;
    switch(MMode) {
      case MIP_SIZER_NONE:     setCursor(MIP_CURSOR_DEFAULT);         break;
      case MIP_SIZER_LEFT:     setCursor(MIP_CURSOR_ARROWLEFTRIGHT);  break;
      case MIP_SIZER_RIGHT:    setCursor(MIP_CURSOR_ARROWLEFTRIGHT);  break;
      case MIP_SIZER_TOP:      setCursor(MIP_CURSOR_ARROWUPDOWN);     break;
      case MIP_SIZER_BOTTOM:   setCursor(MIP_CURSOR_ARROWUPDOWN);     break;
      case MIP_SIZER_WINDOW:   setCursor(MIP_CURSOR_ARROWDIAGRIGHT);   break;
    }
  }

  virtual void setTarget(MIP_Widget* ATarget) {
    MTarget = ATarget;
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) final {
    if (MMode == MIP_SIZER_WINDOW) {
      APainter->fillTriangle( MRect.x2(), MRect.y, MRect.x2(), MRect.y2(), MRect.x, MRect.y2(), MFillColor );
    }
    else {
      APainter->fillRectangle(MRect,MFillColor);
    }

  }

  //----------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == MIP_BUTTON_LEFT) {
      MIsDragging = true;
      prevx = AXpos;
      prevy = AYpos;
    }
  }

  //----------
  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == MIP_BUTTON_LEFT) MIsDragging = false;
  }

  //----------

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (MIsDragging) {
      float deltax = AXpos - prevx;
      float deltay = AYpos - prevy;
      switch(MMode) {
        case MIP_SIZER_LEFT:
          deltay = 0;
          break;
        case MIP_SIZER_RIGHT:
          deltay = 0;
          deltax = -deltax;
          break;
        case MIP_SIZER_TOP:
          deltax = 0;
          break;
        case MIP_SIZER_BOTTOM:
          deltax = 0;
          deltay = -deltay;
          break;
        case MIP_SIZER_WINDOW:
          break;

      } // switch mode

      //if FMode = ksm_Horizontal then deltax := 0;
      //if FMode = ksm_Vertical then deltay := 0;
      //if (MMode == MIP_SIZER_WINDOW) {
      //  if (MParent) MParent->do_widget_resized(this,deltax,deltay);
      //}
      //else {

        if (MTarget) {

          if (MMode == MIP_SIZER_WINDOW) {
            MTarget->do_widget_resized(this,deltax,deltay);
          }
          else {
            float tw = MTarget->getRect().w;
            float th = MTarget->getRect().h;
            // todo: check flags.sizePercent
            tw += deltax;
            th += deltay;
            MIP_FPoint tmin = MTarget->layout.minSize;
            MIP_FPoint tmax = MTarget->layout.maxSize;
            if ( (tw > tmin.w) && (tw < tmax.w)
              && (th > tmin.h) && (th < tmax.h) ) {
              MTarget->do_widget_resized(this,deltax,deltay);
            }
          }
        } // sizer
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
