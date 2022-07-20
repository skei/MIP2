#ifndef mip_slider_widget_included
#define mip_slider_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets/mip_drag_value_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_SliderWidget
: public MIP_DragValueWidget {

//------------------------------
protected:
//------------------------------

  bool MDrawSlider        = true;
  MIP_Color MSliderColor  = MIP_COLOR_LIGHT_GRAY;

//------------------------------
public:
//------------------------------

  MIP_SliderWidget(MIP_DRect ARect, const char* AText, double AValue)
  : MIP_DragValueWidget(ARect,AText,AValue) {
    MName = "MIP_SliderWidget";
  }

  //----------

  virtual ~MIP_SliderWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void  setDrawSlider(bool ADraw=true)    { MDrawSlider = ADraw; }
  virtual void  setSliderColor(MIP_Color AColor)  { MSliderColor = AColor; }

//------------------------------
public: // parent to child
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    fillBackground(AContext);
    paintChildWidgets(AContext);
    drawSlider(AContext);
    drawText(AContext);
    drawValue(AContext);
    drawModulation(AContext);
    drawBorder(AContext);
  }

//------------------------------
public:
//------------------------------

  virtual void drawSlider(MIP_PaintContext* AContext) {
    if (MDrawSlider) {
      MIP_Painter* painter = AContext->painter;
      double value = MValue;
      double minval = MMinValue;
      double maxval = MMaxValue;
      if (MDrawParameterValue) {
        MIP_Parameter* parameter = getParameter();
        if (parameter) {
          value  = parameter->getValue();
          minval = parameter->getMinValue();
          maxval = parameter->getMaxValue();
        }
      }
      double range = maxval - minval;
      if (range > 0) {
        double v = value / range; // 0..1
        painter->beginPath();
        painter->fillColor(MSliderColor);
        painter->rect(MRect.x,MRect.y,MRect.w * v, MRect.h);
        painter->fill();
        //painter->drawTextBox(rect,temp,MValueAlignment,MValueColor);
      }
    }
  }

//------------------------------
public:
//------------------------------


};

//----------------------------------------------------------------------
#endif




