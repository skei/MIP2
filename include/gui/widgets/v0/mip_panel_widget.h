#ifndef mip_panel_widget1_included
#define mip_panel_widget1_included
//----------------------------------------------------------------------

/*
  todo:
  - gradient
  - drop shadow?
*/

#include "gui/mip_widget.h"

//----------------------------------------------------------------------

class MIP_PanelWidget1
: public MIP_Widget {

//------------------------------
protected:
//------------------------------

  bool        MFillBackground       = true;
  MIP_Color   MBackgroundColor      = MIP_COLOR_GRAY;

  bool        MDrawBorder           = true;
  MIP_Color   MBorderColor          = MIP_COLOR_DARK_GRAY;
  uint32_t    MBorderEdges          = MIP_EDGES_ALL;
  float       MBorderThickness      = 1;

  uint32_t    MBorderRoundedCorners = MIP_CORNER_NONE;
  float       MBorderRoundedRadius  = 10.0;

//------------------------------
public:
//------------------------------

  MIP_PanelWidget1(MIP_FRect ARect=MIP_FRect())
  : MIP_Widget(ARect) {
    setName("MIP_PanelWidget");
    setHint("MIP_PanelWidget");
  }

  virtual ~MIP_PanelWidget1() {
  }

//------------------------------
public:
//------------------------------

  virtual void setFillBackground(bool AFill=true)         { MFillBackground = AFill; }
  virtual void setBackgroundColor(MIP_Color AColor)       { MBackgroundColor = AColor; }

  virtual void setDrawBorder(bool ADraw=true)             { MDrawBorder = ADraw; }
  virtual void setBorderColor(MIP_Color AColor)           { MBorderColor = AColor; }
  virtual void setBorderEdges(uint32_t AEdges)            { MBorderEdges = AEdges; }
  virtual void setBorderThickness(float AThickness)       { MBorderThickness = AThickness; }
  virtual void setBorderRoundedCorners(uint32_t ACorners) { MBorderRoundedCorners = ACorners; }
  virtual void setBorderRoundedRadius(float ARadius)      { MBorderRoundedRadius = ARadius; }

  //----------

  virtual void fillBackground(MIP_BasePainter* APainter, MIP_FRect ARect, uint32_t AMode) {
    if (MFillBackground) {
      //if (MBorderRoundedCorners == MIP_CORNER_NONE) {
      //  APainter->setColor(MBackgroundColor);
      //  APainter->rectangle(MRect);
      //}
      APainter->roundedRectangle(MRect,MBorderRoundedRadius,MBorderRoundedCorners);
      APainter->setColor(MBackgroundColor);
      //APainter->setLineWidth(MBorderThickness);
      APainter->fillPath();
    }
  }



  //----------

  virtual void drawBorder(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
    if (MDrawBorder) {

//      if (MBorderRoundedCorners == MIP_CORNER_NONE) {
//        MIP_FRect rect = getRect();
//        float x1 = rect.x;
//        float y1 = rect.y;
//        float x2 = rect.x2();
//        float y2 = rect.y2();
//        if (MBorderEdges & MIP_EDGE_TOP) {
//          APainter->moveTo(x1,y1);
//          APainter->lineTo(x2,y1);
//        }
//        if (MBorderEdges & MIP_EDGE_BOTTOM) {
//          APainter->moveTo(x1,y2);
//          APainter->lineTo(x2,y2);
//        }
//        if (MBorderEdges & MIP_EDGE_LEFT) {
//          APainter->moveTo(x1,y1);
//          APainter->lineTo(x1,y2);
//        }
//        if (MBorderEdges & MIP_EDGE_RIGHT) {
//          APainter->moveTo(x2,y1);
//          APainter->lineTo(x2,y2);
//        }
//      }
//
//      else {
        APainter->roundedRectangle(MIP_FRect(MRect.x,MRect.y,MRect.w,MRect.h),MBorderRoundedRadius,MBorderRoundedCorners);
//      }

      APainter->setColor(MBorderColor);
      APainter->setLineWidth(MBorderThickness);
      APainter->strokePath();


    } // draw border
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {

//    MIP_FRect mrect = getRect();
//    //APainter->resetClip();
//    APainter->setClip(mrect);

    fillBackground(APainter,ARect,AMode);
    //paintChildren(APainter,getRect(),AMode);
    paintWidgets(APainter,ARect,AMode);
    drawBorder(APainter,ARect,AMode);

//    APainter->resetClip();

  }

  //----------

};

//----------------------------------------------------------------------
#endif
