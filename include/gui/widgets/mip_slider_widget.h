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
  bool      MDrawValueBarGradient     = true;

  bool      MDrawValueBarBorder       = true;
  MIP_Color MValueBarBorderColor      = MIP_Color(0.2);
  uint32_t  MValueBarBorderEdges      = MIP_EDGE_RIGHT;//ALL;
  float     MValueBarBorderThickness  = 1;
  uint32_t  MValueBarRoundedCorners   = MIP_CORNER_NONE;
  float     MValueBarRoundedRadius    = 6.0;


  MIP_Color MValueBarColor            = MIP_COLOR_LIGHT_GRAY;
  float     MValueBarGradient         = 0.2;
  uint32_t  MValueBarDirection        = MIP_RIGHT;
  MIP_FRect MValueBarOffset           = MIP_FRect(1,1,1,1);
  MIP_Color MInteractiveValueBarColor = MIP_COLOR_WHITE;
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
    MText             = AText;
    MValue            = AValue;
    MDrawBorder       = true;
    MRoundedCorners   = MIP_CORNER_NONE;
    MRoundedRadius    = 6;
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

  virtual void setDrawValueBarGradient(bool AGradient=true) {
    MDrawValueBarGradient = AGradient;
  }

  virtual void setDrawValueBarBorder(bool ABorder=true) {
    MDrawValueBarBorder = ABorder;
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

//  virtual void fillBackground(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
//    if (MFillBackground) {
//      APainter->roundedRectangle(MRect,MRoundedRadius,MRoundedCorners,MIP_EDGE_ALL/*MBorderEdges*/);
//      if (MFillGradient) {
//        MIP_Color c1 = MBackgroundColor;
//        MIP_Color c2 = MBackgroundColor;
//        c1.blend(MIP_COLOR_WHITE,0.2);
//        c2.blend(MIP_COLOR_BLACK,0.2);
//        //APainter->fillPathGradient(MRect.x,MRect.y,MRect.x2(),MRect.y2(),MGradientColor1,MGradientColor2,MGradientVertical);
//        APainter->fillPathGradient(MRect.x,MRect.y,MRect.x2(),MRect.y2(),c1,c2,MGradientVertical);
//      }
//      else {
//        APainter->setColor(MBackgroundColor);
//        APainter->fillPath();
//      }
//    }
//  }


  virtual void drawValueBar(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {


    MIP_Parameter* param = getParameter();

    bool stepped = false;
    float value = getValue();

//    if (param) {
//      MIP_DPrint("getValue: %.3f param.getValue: %.3f param.getNormalizedValue: %.3f\n",getValue(),param->getValue(),param->getNormalizedValue());
//      stepped = (param->isStepped());
//      //value = param->getNormalizedValue();
//    }

//    MIP_Print("value %f\n",value);

    if (MDrawValueBar) {

      MIP_FRect rect = getRect();
      rect.shrink(MValueBarOffset);


      //MIP_Print("value %f\n",value);

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

      MIP_Color c;
      if (isDragging()) c = MInteractiveValueBarColor;
      else c = MValueBarColor;

      APainter->roundedRectangle(rect,MRoundedRadius,MRoundedCorners,MIP_EDGE_ALL);

      if (MDrawValueBarGradient) {
        MIP_Color c1 = c;
        MIP_Color c2 = c;
        c1.blend(MIP_COLOR_WHITE,0.2);
        c2.blend(MIP_COLOR_BLACK,0.2);
        APainter->fillPathGradient(MRect.x,MRect.y,MRect.x2(),MRect.y2(),c1,c2,MGradientVertical);
      }
      else {
        APainter->setColor(c);
        //APainter->rectangle(rect);
        APainter->fillPath();
      }

      if (MDrawValueBarBorder) {
        APainter->setColor(MValueBarBorderColor);
        APainter->setLineWidth(MValueBarBorderThickness);
        APainter->roundedRectangle(rect,MValueBarRoundedRadius,MValueBarRoundedCorners,MValueBarBorderEdges);
        APainter->strokePath();
      }

      //if (MDrawQuantized & stepped) {
      if (stepped) {

        //uint32_t num = getQuantizeSteps();
        uint32_t num = param->getMaxValue() - param->getMinValue() + 1;
        MIP_Print("num %i\n",num);

        if (num > 0) {
          MIP_FRect r = getRect();
          //float step = r.w / (float)(num-1);
          float step = r.w / (float)(num);
          float x = r.x + step;
          for (uint32_t i=0; i<num-1; i++) {
            APainter->setColor(MQuantizedColor);
            APainter->setLineWidth(1);
            APainter->moveTo(x,rect.y);
            APainter->lineTo(x,rect.y2());
            APainter->strokePath();
            //APainter->drawLine(x,rect.y,x,rect.y2(),MQuantizedColor,1);
            x += step;
          } // for
        } // num > 0
      } // draw quantized

      if (MDrawSnap) {
        MIP_FRect r = getRect();
        float x = r.x + (r.w * getSnapPos());
        APainter->setColor(MSnapColor);
        APainter->setLineWidth(1);
        APainter->moveTo(x,rect.y);
        APainter->lineTo(x,rect.y2());
        APainter->strokePath();
        //APainter->drawLine(x,rect.y,x,rect.y2(),MSnapColor,1);
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
