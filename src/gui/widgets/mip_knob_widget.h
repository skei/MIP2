#ifndef kode_knob_widget_included
#define kode_knob_widget_included
//----------------------------------------------------------------------

#include "kode.h"

//#include "base/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"

#include "gui/widgets/kode_drag_value_widget.h"

//----------------------------------------------------------------------

class KODE_KnobWidget
: public KODE_DragValueWidget {

//------------------------------
private:
//------------------------------

  //KODE_FPoint MKnobOffset   = KODE_FPoint(0,0);
  //KODE_FPoint MLabelOffset  = KODE_FPoint(0,0);
  //KODE_FPoint MValueOffset  = KODE_FPoint(0,0);

  bool        MDrawKnob             = true;
  KODE_Color  MKnobColor            = KODE_COLOR_LIGHT_GRAY;
  KODE_Color  MInteractiveKnobColor = KODE_COLOR_WHITE;
  KODE_Color  MKnobBack             = KODE_COLOR_DARK_GRAY;
  //uint32_t    MKnobSize             = 5.0f;

  bool        MBipolar              = false;

  float       MArcThickness         = 0.20;
  float       MNeedleLength         = 0.25;
  float       MNeedleThickness      = 0.10;
  KODE_Color  MNeedleColor          = KODE_COLOR_LIGHT_GRAY;

  bool        MDrawSteppedArc       = false;
  KODE_Color  MStepColor            = KODE_COLOR_GRAY;

//------------------------------
public:
//------------------------------

  KODE_KnobWidget(KODE_FRect ARect)
  : KODE_DragValueWidget(ARect) {
    setName("KODE_KnobWidget");
    setHint("knob");
    setFillBackground(false);
    setDrawBorder(false);
    //MKnobSize = ARect.w * 0.2f;
  }

  virtual ~KODE_KnobWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawKnob(bool ADraw=true)               { MDrawKnob = ADraw; }
  virtual void setKnobColor(KODE_Color AColor)            { MKnobColor = AColor; }
  virtual void setInteractiveKnobColor(KODE_Color AColor)            { MInteractiveKnobColor = AColor; }
  virtual void setKnobBackgroundColor(KODE_Color AColor)  { MKnobBack = AColor; }
  virtual void setBipolar(bool bi=true)                   { MBipolar = bi; }

  //void setArcColor(KODE_Color AColor)           { MArcColor = AColor; }
  //void setArcBackgroundColor(KODE_Color AColor) { MArcBackgroundColor = AColor; }
  void setArcThickness(float AThickness)        { MArcThickness = AThickness; }
  void setNeedleLength(float ALength)           { MNeedleLength = ALength; }
  void setNeedleThickness(float AThickness)     { MNeedleThickness = AThickness; }
  void setNeedleColor(KODE_Color AColor)        { MNeedleColor = AColor; }

  void setDrawSteppedArc(bool ADraw=true) { MDrawSteppedArc = ADraw; }
  void setStepColor(KODE_Color AColor)    { MStepColor = AColor; }

  //----------

//------------------------------
public:
//------------------------------

  virtual void drawKnob(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) {
    if (MDrawKnob) {

      float     value             = getValue();
      float     S                 = (float)getRect().w;
      int32_t   arc_thickness     = S * MArcThickness;
      float     step_start        = S / 2.0f - arc_thickness;
      float     step_end          = S / 2.0f + 1.0f;
      float     needle_length     = S * MNeedleLength; // / 4;
      float     needle_thickness  = S * MNeedleThickness; // arc_thickness * 0.33f;

      KODE_FRect r = getRect();
      //r.shrink(0.5f + MKnobSize * 0.5f);
      r.shrink((arc_thickness / 2.0f) + 1.0f);

      float x = getRect().x + ((float)getRect().w * 0.5f);
      float y = getRect().y + ((float)getRect().h * 0.5f);

      float a1;// = -0.4f;
      float a2;// = value * 0.8f;

      //APainter->setPenSize(arc_thickness);
      //APainter->setDrawColor(MKnobBack);
      //APainter->drawArc(r,-0.4f,0.8f);

      APainter->drawArc(r,-0.4f,0.8f,MKnobBack,arc_thickness);

      if (MBipolar) {
        float v = (value * 2.0f) - 1.0f;
        if (v < 0) {
          a1 = 0.4f * v;
          a2 = -a1;//0.4f * -v;
        }
        else {
          a1 = 0.0f;          // start
          a2 = 0.4f * v;      // length
        }
      }
      else {
        a1 = -0.4;            // start
        a2 =  0.8f * value;   // length
      }


      //APainter->setDrawColor(MKnobColor);
      //APainter->drawArc(r,a1,a2);

      //r.shrink(2.0f);
      if (isDragging()) {
        APainter->drawArc(r,a1,a2,MInteractiveKnobColor,arc_thickness);
      }
      else {
        APainter->drawArc(r,a1,a2,MKnobColor,arc_thickness);
      }

      // steps

      //KODE_Parameter* param;
      //if (MParameter) param = (KODE_Parameter*)MParameter;
      //else param = KODE_NULL;

      if (getQuantize() && MDrawSteppedArc) {
        //APainter->setDrawColor(MStepColor);
        //APainter->setPenSize(1);
        uint32_t num = getQuantizeSteps() + 1;//param->getNumSteps();
        if (num > 2) {
          float va = 1.0f / (float)(num - 1);
          float vv = va;
          for (uint32_t i=1; i<num-1; i++) {
            float v = ((1.0f - vv) * 0.8f) + 0.1f;
            float a = sinf( v * KODE_PI2  );
            float b = cosf( v * KODE_PI2 );
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

      //if (!MDrawSteppedNeedle) {
      //  value = getValue();
      //}

      float v = ((1.0f - value) * 0.8f) + 0.1f; // 0.1 = (1.0 - 0.8) / 2.0
      float a = sinf( v * KODE_PI2  );
      float b = cosf( v * KODE_PI2 );
      float x1  = x;
      float y1  = y;
      float x2  = x + (a * needle_length);
      float y2  = y + (b * needle_length);
      //APainter->setDrawColor(MNeedleColor);
      //APainter->setPenSize(arc_thickness/2);
      //APainter->drawLine(x1,y1,x2,y2);
      APainter->drawLine(x1,y1,x2,y2,MNeedleColor,needle_thickness);

      //

      /*
      APainter->setDrawColor(KODE_Red);
      APainter->setPenSize(1);
      r.shrink(arc_thickness);
      value = getDragValue();
      a2 = value * 0.8f;
      APainter->drawArc(r,-0.4f,a2);
      */

    }
  }




//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    drawKnob(APainter,ARect,AMode);
    //paintChildren(APainter,ARect,AMode);
    drawBorder(APainter,ARect,AMode);
  }

};

//----------------------------------------------------------------------
#endif
