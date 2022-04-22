#ifndef mip_knob_widget_included
#define mip_knob_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_painter.h"
#include "gui/mip_widget.h"
#include "gui/widgets/mip_drag_value_widget.h"

//----------------------------------------------------------------------

class MIP_KnobWidget
: public MIP_DragValueWidget {

//------------------------------
private:
//------------------------------

  bool        MDrawKnob             = true;
  MIP_Color   MKnobColor            = MIP_COLOR_LIGHT_GRAY;
  MIP_Color   MInteractiveKnobColor = MIP_COLOR_WHITE;
  MIP_Color   MKnobBack             = MIP_Color(0.4);
  bool        MBipolar              = false;
  float       MArcThickness         = 0.20;
  float       MNeedleLength         = 0.25;
  float       MNeedleThickness      = 0.10;
  MIP_Color   MNeedleColor          = MIP_COLOR_LIGHT_GRAY;
  bool        MDrawSteppedArc       = false;
  MIP_Color   MStepColor            = MIP_COLOR_GRAY;

//------------------------------
public:
//------------------------------

  MIP_KnobWidget(MIP_FRect ARect)
  : MIP_DragValueWidget(ARect) {
    setName("MIP_KnobWidget");
    setHint("knob");
    setFillBackground(true);
    setBackgroundColor(0.6);
    setDrawBorder(false);
  }

  virtual ~MIP_KnobWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawKnob(bool ADraw=true)               { MDrawKnob = ADraw; }
  virtual void setKnobColor(MIP_Color AColor)             { MKnobColor = AColor; }
  virtual void setInteractiveKnobColor(MIP_Color AColor)  { MInteractiveKnobColor = AColor; }
  virtual void setKnobBackgroundColor(MIP_Color AColor)   { MKnobBack = AColor; }
  virtual void setBipolar(bool bi=true)                   { MBipolar = bi; }
  virtual void setArcThickness(float AThickness)          { MArcThickness = AThickness; }
  virtual void setNeedleLength(float ALength)             { MNeedleLength = ALength; }
  virtual void setNeedleThickness(float AThickness)       { MNeedleThickness = AThickness; }
  virtual void setNeedleColor(MIP_Color AColor)           { MNeedleColor = AColor; }
  virtual void setDrawSteppedArc(bool ADraw=true)         { MDrawSteppedArc = ADraw; }
  virtual void setStepColor(MIP_Color AColor)             { MStepColor = AColor; }

  //----------

//------------------------------
public:
//------------------------------

  virtual void drawKnob(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
    if (MDrawKnob) {
      float     value             = getValue();
      float     modvalue          = getModValue();
      float     S                 = (float)getRect().w;
      int32_t   arc_thickness     = S * MArcThickness;
      float     step_start        = S / 2.0f - arc_thickness;
      float     step_end          = S / 2.0f + 1.0f;
      float     needle_length     = S * MNeedleLength;
      float     needle_thickness  = S * MNeedleThickness;
      if (MParameter) {
        value     = MParameter->to01(value);
        modvalue  = MParameter->to01(modvalue);
      }
      MIP_FRect r = getRect();
      MIP_FRect rr = r;
      r.shrink((arc_thickness / 2.0f) + 1.0f);
      rr.shrink(2);
      float x = getRect().x + ((float)getRect().w * 0.5f);
      float y = getRect().y + ((float)getRect().h * 0.5f);
      APainter->drawArc(r,-0.4f,0.8f,MKnobBack,arc_thickness);
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
      if (isDragging()) {
        APainter->drawArc(r,a1,a2,MInteractiveKnobColor,arc_thickness);
      }
      else {
        APainter->drawArc(r,a1,a2,MKnobColor,arc_thickness);
      }
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
      APainter->drawArc(rr,m1,m2,MIP_COLOR_BLACK,2);
      // steps
      if (getQuantize() && MDrawSteppedArc) {
        uint32_t num = getQuantizeSteps();
        if (num > 2) {
          float va = 1.0f / (float)(num - 1);
          float vv = va;
          for (uint32_t i=1; i<num-1; i++) {
            float v = ((1.0f - vv) * 0.8f) + 0.1f;
            float a = sinf( v * MIP_PI2  );
            float b = cosf( v * MIP_PI2 );
            float x1  = x + (int32_t)(a * step_start);
            float y1  = y + (int32_t)(b * step_start);
            float x2  = x + (int32_t)(a * step_end);
            float y2  = y + (int32_t)(b * step_end);
            APainter->drawLine(x1,y1,x2,y2,MStepColor);
            vv += va;
          }
        } // > 2
      }
      // value 'needle'
      float v = ((1.0f - value) * 0.8f) + 0.1f;
      float a = sinf( v * MIP_PI2  );
      float b = cosf( v * MIP_PI2 );
      float x1  = x;
      float y1  = y;
      float x2  = x + (a * needle_length);
      float y2  = y + (b * needle_length);
      APainter->drawLine(x1,y1,x2,y2,MNeedleColor,needle_thickness);
    }
  }




//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    drawKnob(APainter,ARect,AMode);
    //paintChildren(APainter,ARect,AMode);
    drawBorder(APainter,ARect,AMode);
  }

};

//----------------------------------------------------------------------
#endif
