#ifndef mip_color_widget_included
#define mip_color_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widget.h"
#include "gui/nanovg/mip_nanovg_utils.h"

//----------------------------------------------------------------------

class MIP_ColorWidget
: public MIP_Widget {

//------------------------------
private:
//------------------------------

  MIP_Color MFillColor = MIP_COLOR_BLACK;

//------------------------------
public:
//------------------------------

  MIP_ColorWidget(MIP_DRect ARect, MIP_Color AColor)
  : MIP_Widget(ARect) {
    MFillColor = AColor;
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    MIP_Painter* painter = AContext->painter;
    painter->beginPath();
    painter->rect(MRect.x,MRect.y,MRect.w,MRect.h);
    painter->fillColor(nvg_color(MFillColor) );
    painter->fill();
    paintChildWidgets(AContext);

  }

};

//----------------------------------------------------------------------
#endif

