#ifndef mip_knob_widget_included
#define mip_knob_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widget.h"
#include "gui/widgets/mip_drag_value_widget.h"

class MIP_KnobWidget
: public MIP_DragValueWidget {

//------------------------------
protected:
//------------------------------

  bool      MDrawKnob             = true;
  bool      MBipolar              = false;

  float     MArcThickness         = 10.0;
  MIP_Color MArcBackgroundColor   = MIP_Color(0.3);
  MIP_Color MArcValueColor        = MIP_COLOR_LIGHT_GRAY;
  MIP_Color MArcInteractiveColor  = MIP_COLOR_WHITE;
  MIP_Color MArcModulationColor   = MIP_COLOR_DARK_RED;

  float     MNeedleLength         = 10.0;
  float     MNeedleThickness      = 3.0;
  MIP_Color MNeedleColor          = MIP_COLOR_WHITE;

//------------------------------
public:
//------------------------------

  MIP_KnobWidget(MIP_FRect ARect, const char* AName="")
  : MIP_DragValueWidget(ARect,AName) {
    setName("MIP_KnobWidget");
    setHint("knob");
    //setDrawValueText(false);
    flags.redrawInteract = true;
  }

  //----------

  virtual ~MIP_KnobWidget() {
  }

//------------------------------
public:
//------------------------------

  void setDrawKnob(bool ADraw=true)             { MDrawKnob = ADraw; }
  void setBipolar(bool ABipolar=true)           { MBipolar  = ABipolar; }

  void setArcThickness(float AThickness)        { MArcThickness = AThickness; }

  void setArcBackgroundColor(MIP_Color AColor)  { MArcBackgroundColor = AColor; }
  void setArcValueColor(MIP_Color AColor)       { MArcValueColor = AColor; }
  void setArcInteractiveColor(MIP_Color AColor) { MArcInteractiveColor = AColor; }
  void setArcModulationColor(MIP_Color AColor)  { MArcModulationColor = AColor; }



  void setNeedleLength(float ALength)           { MNeedleLength = ALength; }
  void setNeedleThickness(float AThickness)     { MNeedleThickness = AThickness; }
  void setNeedleColor(MIP_Color AColor)         { MNeedleColor = AColor; }

//------------------------------
public:
//------------------------------

  virtual void drawKnob(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
    if (MDrawKnob) {

      double value = getValue();
      double modvalue = 0.0;

      if (MParameter && MDrawParamValue) {
        //value = MParameter->getNormalizedValue();
        value = MParameter->getValue();
        //MIP_Print("value %.2f\n",value);
        //MIP_Print("value %.2f\n",value);
        modvalue = MParameter->getModulation();
      }

      // rect, center

      MIP_FRect r = getRect();
      MIP_FRect rr = r;
      r.shrink((MArcThickness / 2.0f) + 1.0f);
      rr.shrink(2);

      float x = getRect().x + ((float)getRect().w * 0.5f);
      float y = getRect().y + ((float)getRect().h * 0.5f);

      // background

      APainter->drawArc(r,-0.4f,0.8f,MArcBackgroundColor,MArcThickness);

      // value

      float a1;
      float a2;
      if (MBipolar) {
        float v = (value * 2.0f) - 1.0f;
        if (v < 0) {
          a1 = 0.4f * v;
          a2 = -a1;
        }
        else {
          a1 = 0.0f;
          a2 = 0.4f * v;
        }
      }
      else {
        a1 = -0.4;
        a2 =  0.8f * value;
      }
      if (state.interacting) {
        APainter->drawArc(r,a1,a2,MArcInteractiveColor,MArcThickness);
      }
      else {
        APainter->drawArc(r,a1,a2,MArcValueColor,MArcThickness);
      }

      // modulation

      float mvalue = MIP_Clamp(value + modvalue,0,1);
      float m1;
      float m2;
      if (MBipolar) {
        float m = (mvalue * 2.0f) - 1.0f;
        if (m < 0) {
          m1 = 0.4f * m;
          m2 = -m1;
        }
        else {
          m1 = 0.0f;
          m2 = 0.4f * m;
        }
      }
      else {
        m1 = -0.4;
        m2 =  0.8f * mvalue;
      }
      APainter->drawArc(rr,m1,m2,MArcModulationColor,2);

      // needle

      float v = ((1.0f - value) * 0.8f) + 0.1f;
      float a = sinf( v * MIP_PI2  );
      float b = cosf( v * MIP_PI2 );
      float x1  = x;
      float y1  = y;
      float x2  = x + (a * MNeedleLength);
      float y2  = y + (b * MNeedleLength);
      APainter->drawLine(x1,y1,x2,y2,MNeedleColor,MNeedleThickness);

    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter, ARect, AMode);
    //paintWidgets(APainter,ARect,AMode);
    drawKnob(APainter, ARect, AMode);
    //drawText(APainter, ARect, AMode);
    //drawValueText(APainter, ARect, AMode);
    drawBorder(APainter, ARect, AMode);
  }

};

//----------------------------------------------------------------------
#endif
