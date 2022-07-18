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

  MIP_Color MColor = MIP_COLOR_BLACK;

//------------------------------
public:
//------------------------------

  virtual void setColor(MIP_Color AColor) { MColor = AColor; }

//------------------------------
public:
//------------------------------

  MIP_ColorWidget(MIP_DRect ARect, MIP_Color AColor)
  : MIP_Widget(ARect) {
    MColor = AColor;
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    MIP_Painter* painter = AContext->painter;
    painter->beginPath();
    painter->rect(MRect.x,MRect.y,MRect.w,MRect.h);
    painter->fillColor(MColor);
    painter->fill();
    paintChildWidgets(AContext);

  }

};

//----------------------------------------------------------------------
#endif

