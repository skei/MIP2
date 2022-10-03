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
  MIP_Color MArcColor     = MIP_Color(0.75);//MIP_COLOR_LIGHT_GRAY;
  MIP_Color MArcBackColor = MIP_Color(0.25);//MIP_COLOR_DARK_GRAY;
  double    MArcWidth     = 25.0;

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
          //value  = parameter->getValue();
          minval = parameter->getMinValue();
          maxval = parameter->getMaxValue();
        }
      }

      double range = maxval - minval;
      if (range > 0) {
        double v = value / range; // 0..1

        double arcwidth,cx,cy,r;

        if (MRect.w > MRect.h) {
          arcwidth = MRect.h * MArcWidth / 100.0;
          cx = MRect.x + (MRect.w * 0.5);
          cy = MRect.y + (MRect.h * 0.5) + (arcwidth * 0.25);
          r  = (MRect.h * 0.5) - (arcwidth * 0.5);
        }
        else {
          arcwidth = MRect.w * MArcWidth / 100.0;
          cx = MRect.x + (MRect.w * 0.5);
          cy = MRect.y + (MRect.h * 0.5) + (arcwidth * 0.25);
          r  = (MRect.w * 0.5) - (arcwidth * 0.5);
        }
        //double a1 = (0.0 - 0.25 - 0.1) * MIP_PI2;
        //double a2 = a1 - (v * 0.8 * MIP_PI2);
        double a1 = (0.35    * MIP_PI2);
        double a2 = (v * 0.8 * MIP_PI2);

        painter->strokeWidth( arcwidth );

        painter->beginPath();
        painter->strokeColor(MArcBackColor);
        painter->arc(cx,cy,r,a1,(0.8*MIP_PI2),MIP_WINDING_COUNTER_CLOCKWISE);
        painter->stroke();

        painter->beginPath();
        painter->strokeColor(MArcColor);
        painter->arc(cx,cy,r,a1,a2,MIP_WINDING_COUNTER_CLOCKWISE);
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




