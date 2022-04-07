#ifndef mip_panel_widget0_included
#define mip_panel_widget0_included
//----------------------------------------------------------------------

#include "gui/mip_widget.h"

//----------------------------------------------------------------------

class MIP_PanelWidget0
: public MIP_Widget {

//------------------------------
protected:
//------------------------------

  bool        MFillBackground   = true;
  MIP_Color   MBackgroundColor  = MIP_Color(0.6f);

  bool        MDrawBorder       = true;
  MIP_Color   MBorderThickness  = 1;
  MIP_Color   MBorderColor      = MIP_Color(0.3f);
  uint32_t    MBorderEdges      = MIP_EDGES_ALL;

//------------------------------
public:
//------------------------------

  MIP_PanelWidget0(MIP_FRect ARect=MIP_FRect())
  : MIP_Widget(ARect) {
    setName("MIP_PanelWidget");
    setHint("panel");
  }

  virtual ~MIP_PanelWidget0() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawBorder(bool ADraw=true) {
    MDrawBorder = ADraw;
  }

  virtual void setBorderThickness(float AThickness) {
    MBorderThickness = AThickness;
  }

  virtual void setBorderColor(MIP_Color AColor) {
    MBorderColor = AColor;
  }

  virtual void setBorderEdges(uint32_t AEdges) {
    MBorderEdges = AEdges;
  }

  virtual void setFillBackground(bool AFill=true) {
    MFillBackground = AFill;
  }

  virtual void setBackgroundColor(MIP_Color AColor) {
    MBackgroundColor = AColor;
  }

  //----------

  virtual void drawBorder(MIP_BasePainter* APainter, MIP_FRect ARect, uint32_t AMode) {
//    APainter->setLineWidth(1);
    if (MDrawBorder) {

      switch (MBorderEdges) {
      }
      if (MBorderEdges == MIP_EDGES_ALL) {
        APainter->drawRectangle(getRect(),MBorderColor);
      }
      else {
        MIP_FRect rect = getRect();
        float x1 = rect.x;
        float y1 = rect.y;
        float x2 = rect.x2();
        float y2 = rect.y2();
        if (MBorderEdges & MIP_EDGE_TOP)     APainter->drawLine(x1,y1,x2,y1,MBorderColor);
        if (MBorderEdges & MIP_EDGE_BOTTOM)  APainter->drawLine(x1,y2,x2,y2,MBorderColor);
        if (MBorderEdges & MIP_EDGE_LEFT)    APainter->drawLine(x1,y1,x1,y2,MBorderColor);
        if (MBorderEdges & MIP_EDGE_RIGHT)   APainter->drawLine(x2,y1,x2,y2,MBorderColor);
      }
    }
  }

  virtual void fillBackground(MIP_BasePainter* APainter, MIP_FRect ARect, uint32_t AMode) {
    if (MFillBackground) {
      //APainter->fillRectangle(MRect,MBackgroundColor);

      MIP_FRect rect = MRect;
      rect.overlap(ARect);

      if (rect.isNotEmpty()) {
        APainter->fillRectangle(rect,MBackgroundColor);
      }
    }
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




//#ifndef mip_panel_widget0_included
//#define mip_panel_widget0_included
////----------------------------------------------------------------------
//
///*
//  todo:
//  - gradient
//  - drop shadow?
//*/
//
//#include "gui/mip_widget.h"
//
////----------------------------------------------------------------------
//
//class MIP_PanelWidget0
//: public MIP_Widget {
//
////------------------------------
//protected:
////------------------------------
//
//  bool        MFillBackground       = true;
//  MIP_Color   MBackgroundColor      = MIP_COLOR_GRAY;
//
//  bool        MDrawBorder           = true;
//  MIP_Color   MBorderColor          = MIP_COLOR_DARK_GRAY;
//  uint32_t    MBorderEdges          = MIP_EDGES_ALL;
//  float       MBorderThickness      = 1;
//
//  uint32_t    MBorderRoundedCorners = MIP_CORNER_NONE;
//  float       MBorderRoundedRadius  = 10.0;
//
////------------------------------
//public:
////------------------------------
//
//  MIP_PanelWidget0(MIP_FRect ARect=MIP_FRect())
//  : MIP_Widget(ARect) {
//    setName("MIP_PanelWidget");
//    setHint("MIP_PanelWidget");
//  }
//
//  virtual ~MIP_PanelWidget0() {
//  }
//
////------------------------------
//public:
////------------------------------
//
//  virtual void setFillBackground(bool AFill=true)         { MFillBackground = AFill; }
//  virtual void setBackgroundColor(MIP_Color AColor)       { MBackgroundColor = AColor; }
//
//  virtual void setDrawBorder(bool ADraw=true)             { MDrawBorder = ADraw; }
//  virtual void setBorderColor(MIP_Color AColor)           { MBorderColor = AColor; }
//  virtual void setBorderEdges(uint32_t AEdges)            { MBorderEdges = AEdges; }
//  virtual void setBorderThickness(float AThickness)       { MBorderThickness = AThickness; }
//  virtual void setBorderRoundedCorners(uint32_t ACorners) { MBorderRoundedCorners = ACorners; }
//  virtual void setBorderRoundedRadius(float ARadius)      { MBorderRoundedRadius = ARadius; }
//
//  //----------
//
//  virtual void fillBackground(MIP_BasePainter* APainter, MIP_FRect ARect, uint32_t AMode) {
//    if (MFillBackground) {
//      //if (MBorderRoundedCorners == MIP_CORNER_NONE) {
//      //  APainter->setColor(MBackgroundColor);
//      //  APainter->rectangle(MRect);
//      //}
//      APainter->roundedRectangle(MRect,MBorderRoundedRadius,MBorderRoundedCorners);
//      APainter->setColor(MBackgroundColor);
//      //APainter->setLineWidth(MBorderThickness);
//      APainter->fillPath();
//    }
//  }
//
//
//
//  //----------
//
//  virtual void drawBorder(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
//    if (MDrawBorder) {
//
////      if (MBorderRoundedCorners == MIP_CORNER_NONE) {
////        MIP_FRect rect = getRect();
////        float x1 = rect.x;
////        float y1 = rect.y;
////        float x2 = rect.x2();
////        float y2 = rect.y2();
////        if (MBorderEdges & MIP_EDGE_TOP) {
////          APainter->moveTo(x1,y1);
////          APainter->lineTo(x2,y1);
////        }
////        if (MBorderEdges & MIP_EDGE_BOTTOM) {
////          APainter->moveTo(x1,y2);
////          APainter->lineTo(x2,y2);
////        }
////        if (MBorderEdges & MIP_EDGE_LEFT) {
////          APainter->moveTo(x1,y1);
////          APainter->lineTo(x1,y2);
////        }
////        if (MBorderEdges & MIP_EDGE_RIGHT) {
////          APainter->moveTo(x2,y1);
////          APainter->lineTo(x2,y2);
////        }
////      }
////
////      else {
//        APainter->roundedRectangle(MIP_FRect(MRect.x,MRect.y,MRect.w,MRect.h),MBorderRoundedRadius,MBorderRoundedCorners);
////      }
//
//      APainter->setColor(MBorderColor);
//      APainter->setLineWidth(MBorderThickness);
//      APainter->strokePath();
//
//
//    } // draw border
//  }
//
////------------------------------
//public:
////------------------------------
//
//  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
//
////    MIP_FRect mrect = getRect();
////    //APainter->resetClip();
////    APainter->setClip(mrect);
//
//    fillBackground(APainter,ARect,AMode);
//    //paintChildren(APainter,getRect(),AMode);
//    paintWidgets(APainter,ARect,AMode);
//    drawBorder(APainter,ARect,AMode);
//
////    APainter->resetClip();
//
//  }
//
//  //----------
//
//};
//
////----------------------------------------------------------------------
//#endif
