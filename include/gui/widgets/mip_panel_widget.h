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

  bool      MDrawRoundedCorners   = false;//true;
  //float     MCornerRadius         = 4.0;
  float     MULCornerRadius       = 0.0;
  float     MURCornerRadius       = 0.0;
  float     MBRCornerRadius       = 0.0;
  float     MBLCornerRadius       = 0.0;

  bool      MFillBackground       = true;
  bool      MFillGradient         = false;//true;

  MIP_Color MBackgroundColor      = MIP_Color(0.5);
  MIP_Color MGradientColor1       = MIP_Color(0.6, 0.6, 0.6);
  MIP_Color MGradientColor2       = MIP_Color(0.4, 0.4, 0.4);

  bool      MDrawBorder           = true;
  float     MBorderWidth          = 1.0;
  MIP_Color MBorderColor          = MIP_Color(0.25);

  bool      MDrawDropShadow       = false;
  MIP_Color MDropShadowInnerColor = MIP_Color( 0.0, 0.0, 0.0, 1.0);
  MIP_Color MDropShadowOuterColor = MIP_Color( 0.0, 0.0, 0.0, 0.0);
  float     MDropShadowFeather    = 10;
  bool      MDropShadowInner      = false;

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

  virtual void setDrawRoundedCorners(bool ADraw=true)                   { MDrawRoundedCorners = ADraw; }
  virtual void setDrawDropShadow(bool ADraw=true)                       { MDrawDropShadow = ADraw; }
  virtual void setFillBackground(bool AFill=true)                       { MFillBackground = AFill; }
  virtual void setFillGradient(bool AFill=true)                         { MFillGradient = AFill; }
  virtual void setDrawBorder(bool ADraw=true)                           { MDrawBorder = ADraw; }

  virtual void setCornerRadius(float ul, float ur, float br, float bl)  { MULCornerRadius = ul; MURCornerRadius = ur; MBRCornerRadius = br; MBLCornerRadius = bl; }
  virtual void setCornerRadius(float r)                                 { MULCornerRadius = r; MURCornerRadius = r; MBRCornerRadius = r; MBLCornerRadius = r; }

  virtual void setBackgroundColor(MIP_Color AColor) { MBackgroundColor = AColor; }
  virtual void setGradientColor1(MIP_Color AColor) { MGradientColor1 = AColor; }
  virtual void setGradientColor2(MIP_Color AColor) { MGradientColor2 = AColor; }
  virtual void setBorderColor(MIP_Color AColor) { MBorderColor = AColor; }
  virtual void setBorderWidth(double AWidth) { MBorderWidth = AWidth; }

  virtual void setDropShadowInnerColor(MIP_Color AColor) { MDropShadowInnerColor = AColor; }
  virtual void setDropShadowOuterColor(MIP_Color AColor) { MDropShadowOuterColor = AColor; }
  virtual void setDropShadowFeather(float AFeather) {  MDropShadowFeather = AFeather; }
  virtual void setDropShadowInner(bool AInner=true) { MDropShadowInner = AInner; }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    fillBackground(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);
    //drawDropShadow(AContext);
  }

//------------------------------
public:
//------------------------------

  virtual void drawBorder(MIP_PaintContext* AContext) {
    if (MDrawBorder) {
      MIP_Painter* painter = AContext->painter;
      painter->beginPath();
      if (MDrawRoundedCorners) painter->roundedRectVarying(MRect.x,MRect.y,MRect.w,MRect.h, MULCornerRadius, MURCornerRadius, MBRCornerRadius, MBLCornerRadius);
      else painter->rect(MRect.x,MRect.y,MRect.w,MRect.h);
      painter->strokeColor(MBorderColor);
      painter->strokeWidth(MBorderWidth);
      painter->stroke();
    }
  }

  //----------

  virtual void fillBackground(MIP_PaintContext* AContext) {
    //if (MDrawDropShadow && MDropShadowInner) drawDropShadow(AContext);
    if (MFillBackground) {
      MIP_Painter* painter = AContext->painter;
      painter->beginPath();
      if (MDrawRoundedCorners) painter->roundedRectVarying(MRect.x,MRect.y,MRect.w,MRect.h, MULCornerRadius, MURCornerRadius, MBRCornerRadius, MBLCornerRadius);
      else painter->rect(MRect.x,MRect.y,MRect.w,MRect.h);
      if (MFillGradient) {
        //MIP_PaintSource paint = painter->linearGradient(MRect.x,MRect.y,MRect.x,MRect.y2(), MGradientColor1, MGradientColor2);
        //painter->fillPaint(paint);
        MIP_PaintSource paint = painter->linearGradient(MRect.x,MRect.y,MRect.x,MRect.y2(), MGradientColor1, MGradientColor2);
        painter->fillPaint(paint);
      }
      else {
        painter->fillColor(MBackgroundColor);
      }
      painter->fill();
    }
    //if (MDrawDropShadow && !MDropShadowInner) drawDropShadow(AContext);
  }

  //----------

  virtual void drawDropShadow(MIP_PaintContext* AContext) {
    if (MDrawDropShadow) {
      MIP_Painter* painter = AContext->painter;
      MIP_DRect rect = MRect;
      double rounded = 0.0;
      double feather = MDropShadowFeather;
      if (MDrawRoundedCorners) rounded = MULCornerRadius;
      painter->beginPath();
      if (MDropShadowInner) {
        MIP_PaintSource paint = painter->boxGradient(
          rect.x,
          rect.y,
          rect.w,
          rect.h,
          rounded,
          feather,
          MDropShadowOuterColor,
          MDropShadowInnerColor
        );

        rect.shrink(feather);
        painter->rect(MRect.x,MRect.y,MRect.w,MRect.h);
        painter->fillPaint(paint);
        painter->fill();
      }
      else {

        MIP_DRect cr = painter->getClipRect();
        painter->resetClip();

        MIP_PaintSource paint = painter->boxGradient(
          rect.x,
          rect.y,
          rect.w,
          rect.h,
          rounded,
          feather,
          MDropShadowInnerColor,
          MDropShadowOuterColor
        );

        rect.grow(feather);
        painter->rect(rect.x,rect.y,rect.w,rect.h);
        painter->fillPaint(paint);
        painter->fill();

        painter->setClip(cr);

      }
    }
  }

};

//----------------------------------------------------------------------
#endif


