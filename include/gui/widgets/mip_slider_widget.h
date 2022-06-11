#ifndef mip_slider_widget_included
#define mip_slider_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widget.h"
#include "gui/widgets/mip_drag_value_widget.h"

class MIP_SliderWidget
: public MIP_DragValueWidget {

//------------------------------
protected:
//------------------------------

  bool MDrawValueBar = true;
  MIP_Color MValueBarColor    = MIP_COLOR_LIGHT_GRAY;
  MIP_Color MModulationColor  = MIP_COLOR_DARK_RED;

//------------------------------
public:
//------------------------------

  MIP_SliderWidget(MIP_FRect ARect, const char* AName="")
  : MIP_DragValueWidget(ARect,AName) {
    setName("MIP_SliderWidget");
    setHint("slider");
  }

  virtual ~MIP_SliderWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void drawValueBar(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
    double value = getValue();
    double modvalue = 0.0;
    if (MDrawValueBar && MParameter) {
      //value = MParameter->getNormalizedValue();
      value = MParameter->getValue();
      modvalue = value + MParameter->getModulation();
      value = MParameter->normalizeValue(value);
      value = MIP_Clamp(value,0,1);
      modvalue = MParameter->normalizeValue(modvalue);
      modvalue = MIP_Clamp(modvalue,0,1);
    }
    if (MRect.w > 0.0) {
      // value
      float width = MRect.w * value;
      MIP_FRect rect = MRect;
      rect.w = width;
      APainter->setColor(MValueBarColor);
      APainter->rectangle(rect);
      APainter->fillPath();
      // modulation
      width = MRect.w * modvalue;
      rect.w = width;
      rect.h = 2;
      APainter->setColor(MModulationColor);
      APainter->rectangle(rect);
      APainter->fillPath();
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter, ARect, AMode);
    //paintWidgets(APainter,ARect,AMode);
    drawValueBar(APainter, ARect, AMode);
    drawText(APainter, ARect, AMode);
    drawValueText(APainter, ARect, AMode);
    drawBorder(APainter, ARect, AMode);
  }

};

//----------------------------------------------------------------------
#endif
