#ifndef mip_slider_widget_included
#define mip_slider_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/mip_drag_value_widget.h"

//----------------------------------------------------------------------

class MIP_SliderWidget
: public MIP_DragValueWidget {

//------------------------------
protected:
//------------------------------

  bool      MDrawValueBar             = true;
  MIP_Color MValueBarColor            = MIP_Color(0.7);
  uint32_t  MValueBarDirection        = MIP_RIGHT;
  MIP_FRect MValueBarOffset           = MIP_FRect(1,1,1,1);
  MIP_Color MInteractiveValueBarColor = MIP_Color(0.9);
  bool      MDrawQuantized            = false;
  MIP_Color MQuantizedColor           = MIP_Color(0.3);
  bool      MDrawSnap                 = false;
  MIP_Color MSnapColor                = MIP_Color(0.3);

  //bool      MDrawValueBarQuantized    = false;
  //MIP_Color MValueBarQuantizedColor   = MIP_Color(0.3);



//------------------------------
public:
//------------------------------

  MIP_SliderWidget(MIP_FRect ARect, const char* AText="", float AValue=0.0)
  : MIP_DragValueWidget(ARect) {
    setName("MIP_SliderWidget");
    setHint("slider");
    MText = AText;
    MValue = AValue;
    MDrawBorder       = true;
    MBackgroundColor  = MIP_Color(0.7);
    MTextColor        = MIP_Color(0);
    MValueTextColor   = MIP_Color(0);
    MTextOffset       = MIP_FRect(6,0,0,0);
  }

  virtual ~MIP_SliderWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawValueBar(bool ADraw=true) {
    MDrawValueBar = ADraw;
  }

  virtual void setValueBarColor(MIP_Color AColor) {
    MValueBarColor = AColor;
  }

  virtual void setValueBarDirection(uint32_t ADirection) {
    MValueBarDirection = ADirection;
  }

  virtual void setDrawQuantized(bool ADraw=true) {
    MDrawQuantized = ADraw;
  }

  virtual void setQuantizedColor(MIP_Color AColor) {
    MQuantizedColor = AColor;
  }

  virtual void setDrawSnap(bool ADraw=true) {
    MDrawSnap = ADraw;
  }

  virtual void setSnapColor(MIP_Color AColor) {
    MSnapColor = AColor;
  }

  //----------

  virtual void drawValueBar(MIP_BasePainter* APainter, MIP_FRect ARect, uint32_t AMode) {
    if (MDrawValueBar) {
      MIP_FRect  rect  = getRect();
      float       value = getValue();
      rect.shrink(MValueBarOffset);
      float w = rect.w * value;
      float h = rect.h * value;
      switch (MValueBarDirection) {
        case MIP_LEFT:
          rect.x = rect.x2() - w;
          rect.w = w;
          break;
        case MIP_RIGHT:
          rect.w = w;
          break;
        case MIP_UP:
          rect.y = rect.y2() - h;
          rect.h = h;
          break;
        case MIP_DOWN:
          rect.h = h;
          break;
      }
      if (isDragging()) {
        APainter->fillRectangle(rect,MInteractiveValueBarColor);
      }
      else {
        APainter->fillRectangle(rect,MValueBarColor);
      }
      //if (getQuantize()) {
        if (MDrawQuantized) {
          uint32_t num = getQuantizeSteps();
          if (num > 0) {
            MIP_FRect r = getRect();
            float step = r.w / (float)(num-1);
            float x = r.x + step;
            for (uint32_t i=0; i<num-1; i++) {
//              APainter->setColor(MQuantizedColor);
//              APainter->setLineWidth(1);
//              APainter->moveTo(x,rect.y);
//              APainter->lineTo(x,rect.y2());
//              APainter->strokePath();
              APainter->drawLine(x,rect.y,x,rect.y2(),MQuantizedColor,1);
              x += step;
            } // for
          } // num > 0
        } // draw q
      //} // quantize
      if (MDrawSnap) {
        MIP_FRect r = getRect();
        float x = r.x + (r.w * getSnapPos());
//        APainter->setColor(MSnapColor);
//        APainter->setLineWidth(1);
//        APainter->moveTo(x,rect.y);
//        APainter->lineTo(x,rect.y2());
//        APainter->strokePath();
        APainter->drawLine(x,rect.y,x,rect.y2(),MSnapColor,1);
      } // draw snap

    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    //paintChildren(APainter,getRect(),AMode);
    drawValueBar(APainter,ARect,AMode);
    drawText(APainter,ARect,AMode);
    drawValueText(APainter,ARect,AMode);
    drawBorder(APainter,ARect,AMode);
  }

};

//----------------------------------------------------------------------
#endif
