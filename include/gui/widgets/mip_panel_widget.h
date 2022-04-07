#ifndef mip_panel_widget_included
#define mip_panel_widget_included
//----------------------------------------------------------------------

/*
  todo:
  - gradient
  - drop shadow?
*/

#include "gui/mip_widget.h"

//----------------------------------------------------------------------

class MIP_PanelWidget
: public MIP_Widget {

//------------------------------
protected:
//------------------------------

  bool        MFillBackground       = true;
  MIP_Color   MBackgroundColor      = MIP_COLOR_GRAY;

  bool        MFillGradient         = false;
  MIP_Color   MGradientColor1       = MIP_Color(0.6);
  MIP_Color   MGradientColor2       = MIP_Color(0.4);
  bool        MGradientVertical     = true;

  bool        MDrawBorder           = false;
  MIP_Color   MBorderColor          = MIP_COLOR_DARK_GRAY;
  uint32_t    MBorderEdges          = MIP_EDGE_ALL;
  float       MBorderThickness      = 1;

  uint32_t    MRoundedCorners = MIP_CORNER_NONE;
  float       MRoundedRadius  = 10.0;

//------------------------------
public:
//------------------------------

  MIP_PanelWidget(MIP_FRect ARect=MIP_FRect())
  : MIP_Widget(ARect) {
    setName("MIP_PanelWidget");
    setHint("MIP_PanelWidget");
  }

  virtual ~MIP_PanelWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setFillBackground(bool AFill=true)         { MFillBackground = AFill; }
  virtual void setBackgroundColor(MIP_Color AColor)       { MBackgroundColor = AColor; }

  virtual void setFillGradient(bool AFill=true)           { MFillGradient = AFill; }
  virtual void setGradientColor1(MIP_Color AColor)        { MGradientColor1 = AColor; }
  virtual void setGradientColor2(MIP_Color AColor)        { MGradientColor2 = AColor; }
  virtual void setGradientVertical(bool AVertical)        { MGradientVertical = AVertical; }

  virtual void setDrawBorder(bool ADraw=true)             { MDrawBorder = ADraw; }
  virtual void setBorderColor(MIP_Color AColor)           { MBorderColor = AColor; }
  virtual void setBorderEdges(uint32_t AEdges)            { MBorderEdges = AEdges; }
  virtual void setBorderThickness(float AThickness)       { MBorderThickness = AThickness; }
  virtual void setRoundedCorners(uint32_t ACorners) { MRoundedCorners = ACorners; }
  virtual void setRoundedRadius(float ARadius)      { MRoundedRadius = ARadius; }

  //----------

  virtual void fillBackground(MIP_BasePainter* APainter, MIP_FRect ARect, uint32_t AMode) {
    if (MFillBackground) {
      APainter->setColor(MBackgroundColor);
      APainter->roundedRectangle(MRect,MRoundedRadius,MRoundedCorners,MIP_EDGE_ALL/*MBorderEdges*/);
      if (MFillGradient) {
        APainter->fillPathGradient(MRect.x,MRect.y,MRect.x2(),MRect.y2(),MGradientColor1,MGradientColor2,MGradientVertical);
      }
      else {
        APainter->fillPath();
      }
    }
  }



  //----------

  virtual void drawBorder(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
    if (MDrawBorder) {
      APainter->setColor(MBorderColor);
      APainter->setLineWidth(MBorderThickness);
      APainter->roundedRectangle(MIP_FRect(MRect.x,MRect.y,MRect.w,MRect.h),MRoundedRadius,MRoundedCorners,MBorderEdges);
      APainter->strokePath();
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    paintWidgets(APainter,ARect,AMode);
    drawBorder(APainter,ARect,AMode);
  }

  //----------

};

//----------------------------------------------------------------------
#endif


