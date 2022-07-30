#ifndef mip_knob_widget_included
#define mip_knob_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets/mip_drag_value_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_KnobWidget
: public MIP_DragValueWidget {

//------------------------------
protected:
//------------------------------

  bool      MDrawArc      = true;
  MIP_Color MArcColor     = MIP_COLOR_LIGHT_GRAY;
  MIP_Color MArcBackColor = MIP_COLOR_DARK_GRAY;
  double    MArcWidth     = 8.0;

//------------------------------
public:
//------------------------------

  MIP_KnobWidget(MIP_DRect ARect, const char* AText, double AValue)
  : MIP_DragValueWidget(ARect,AText,AValue) {
    MName = "MIP_KnobWidget";
  }

  //----------

  virtual ~MIP_KnobWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void  setDrawArc(bool ADraw=true)       { MDrawArc = ADraw; }
  virtual void  setArcColor(MIP_Color AColor)     { MArcColor = AColor; }
  virtual void  setArcBackColor(MIP_Color AColor) { MArcBackColor = AColor; }
  virtual void  setArcWidth(double AWidth)        { MArcWidth = AWidth; }

//------------------------------
public: // parent to child
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    fillBackground(AContext);
    paintChildWidgets(AContext);
    drawKnob(AContext);
    //drawText(AContext);
    //drawValue(AContext);
    //drawModulation(AContext);
    drawBorder(AContext);
  }

//------------------------------
public:
//------------------------------

  virtual void drawKnob(MIP_PaintContext* AContext) {
    if (MDrawArc) {
      MIP_Painter* painter = AContext->painter;
      double value = getValue();
      double minval = getMinValue();
      double maxval = getMaxValue();
      if (MDrawParameterValue) {
        MIP_Parameter* parameter = getParameter();
        if (parameter) {
          value  = parameter->getValue();
          minval = parameter->getMinValue();
          maxval = parameter->getMaxValue();
        }
      }

      painter->strokeWidth(MArcWidth);

      double range = maxval - minval;
      if (range > 0) {
        double v = value / range; // 0..1

        double cx = MRect.x + (MRect.w * 0.5);
        double cy = MRect.y + (MRect.h * 0.5);
        double r  = (MRect.w * 0.5) - (MArcWidth * 0.5);
        double a1 = (0.0 + 0.25 + 0.1) * MIP_PI2;
        double a2 = a1 + (v * 0.8 * MIP_PI2);

        painter->beginPath();
        painter->strokeColor(MArcBackColor);
        painter->arc(cx,cy,r,a1,a1 + (0.8 * MIP_PI2),NVG_CW);
        painter->stroke();

        painter->beginPath();
        painter->strokeColor(MArcColor);
        painter->arc(cx,cy,r,a1,a2,NVG_CW);
        painter->stroke();

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




