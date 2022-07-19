#ifndef mip_panel_widget_included
#define mip_panel_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widget.h"

//----------------------------------------------------------------------

class MIP_PanelWidget
: public MIP_Widget {

//------------------------------
protected:
//------------------------------

  bool      MDrawRoundedCorners   = true;
  float     MRoundedCorners       = 5;

  bool      MFillBackground       = true;
  bool      MFillGradient         = true;

  MIP_Color MBackgroundColor      = MIP_COLOR_GRAY;
  MIP_Color MGradientColor1       = MIP_Color(0.6, 0.6, 0.6);
  MIP_Color MGradientColor2       = MIP_Color(0.4, 0.4, 0.4);

  bool      MDrawBorder           = true;
  float     MBorderWidth          = 1.0;
  MIP_Color MBorderColor          = MIP_COLOR_DARK_GRAY;

//  bool      MDrawDropShadow       = false;
//  MIP_Color MDropShadowInnerColor = MIP_Color(0,0,0,0.5);
//  MIP_Color MDropShadowOuterColor = MIP_Color(0,0,0,0.1);
//  float     MDropShadowRounded    = 5;
//  float     MDropShadowFeather    = 10;

//------------------------------
public:
//------------------------------

  MIP_PanelWidget(MIP_DRect ARect)
  : MIP_Widget(ARect) {
    MName = "MIP_PanelWidget";
  }

  //----------

  virtual ~MIP_PanelWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawRoundedCorners(bool ADraw=true) { MDrawRoundedCorners = ADraw; }
  //virtual void setDrawDropShadow(bool ADraw=true)     { MDrawDropShadow = ADraw; }
  virtual void setFillBackground(bool AFill=true)     { MFillBackground = AFill; }
  virtual void setFillGradient(bool AFill=true)       { MFillGradient = AFill; }
  virtual void setDrawBorder(bool ADraw=true)         { MDrawBorder = ADraw; }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    //drawDropShadow(AContext);
    fillBackground(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);
  }

//------------------------------
public:
//------------------------------

  virtual void fillBackground(MIP_PaintContext* AContext) {
    if (MFillBackground) {
      MIP_Painter* painter = AContext->painter;
      painter->beginPath();
      if (MDrawRoundedCorners) painter->roundedRect(MRect.x,MRect.y,MRect.w,MRect.h, MRoundedCorners);
      else painter->rect(MRect.x,MRect.y,MRect.w,MRect.h);
      if (MFillGradient) {
        NVGpaint paint = painter->linearGradient(MRect.x,MRect.y,MRect.x,MRect.y + MRect.y2(), MGradientColor1, MGradientColor2);
        painter->fillPaint(paint);
      }
      else {
        painter->fillColor(MBackgroundColor);
      }
      painter->fill();
    }
  }

  //----------

  virtual void drawBorder(MIP_PaintContext* AContext) {
    if (MDrawBorder) {
      MIP_Painter* painter = AContext->painter;
      painter->beginPath();
      if (MDrawRoundedCorners) painter->roundedRect(MRect.x,MRect.y,MRect.w,MRect.h, MRoundedCorners);
      else painter->rect(MRect.x,MRect.y,MRect.w,MRect.h);
      painter->strokeColor(MBorderColor);
      painter->strokeWidth(MBorderWidth);
      painter->stroke();
    }
  }

  //----------

//  virtual void drawDropShadow(MIP_PaintContext* AContext) {
//    if (MDrawDropShadow) {
//      MIP_Painter* painter = AContext->painter;
//      MIP_DRect rect = MRect;
//      //bndDropShadow(nvg,rect.x+3,rect.y+3,rect.w,rect.h,MDropShadowRounded,MDropShadowFeather,0.5); // radius, feather, alpha
//      //rect.grow(MRoundedCorners);
//      painter->beginPath();
//      //nvgRect(nvg,rect.x+8,rect.y+8,rect.w,rect.h);
//      painter->roundedRect(MRect.x+8,MRect.y+8,MRect.w,MRect.h, MRoundedCorners);
//      NVGpaint paint = painter->boxGradient(
//        rect.x,
//        rect.y,
//        rect.w,
//        rect.h,
//        MDropShadowRounded,
//        MDropShadowFeather,
//        nvg_color(MDropShadowInnerColor),
//        nvg_color(MDropShadowOuterColor)
//      );
//      painter->fillPaint(paint);
//      painter->fill();
//
//    }
//  }

};

//----------------------------------------------------------------------
#endif


