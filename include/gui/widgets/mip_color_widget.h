#ifndef mip_color_widget_included
#define mip_color_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widget.h"
//#include "gui/nanovg/mip_nanovg_utils.h"

//----------------------------------------------------------------------

class MIP_ColorWidget
: public MIP_Widget {

//------------------------------
private:
//------------------------------

  MIP_Color MColor = MIP_COLOR_BLACK;

//------------------------------
public:
//------------------------------

  virtual void setColor(MIP_Color AColor) { MColor = AColor; }
  virtual MIP_Color getColor() { return MColor; }

//------------------------------
public:
//------------------------------

  MIP_ColorWidget(MIP_DRect ARect, MIP_Color AColor)
  : MIP_Widget(ARect) {
    MName = "MIP_ColorWidget";
    MColor = AColor;
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    MIP_Painter* painter = AContext->painter;
    MIP_DRect rect = MRect;
    //rect.overlap(AContext->updateRect);
    painter->beginPath();
    //painter->rect(MRect.x,MRect.y,MRect.w,MRect.h);
    painter->rect(rect.x,rect.y,rect.w,rect.h);
    painter->fillColor(MColor);
    painter->fill();

//    painter->beginPath();
//    painter->moveTo(0,0);
//    for (uint32_t i=0; i<30000; i++) {
//      double x = MIP_RandomRange(0,MRect.w);
//      double y = MIP_RandomRange(0,MRect.h);
//      painter->lineTo(x,y);
//    }
//    painter->strokeColor(MIP_Color(0,0,0,0.1));
//    painter->strokeWidth(1);
//    painter->stroke();

    paintChildWidgets(AContext);
  }

};

//----------------------------------------------------------------------
#endif

