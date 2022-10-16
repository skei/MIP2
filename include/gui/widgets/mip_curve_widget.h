#ifndef mip_curve_widget_includedbutton
#define mip_curve_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/mip_drag_value_widget.h"

class MIP_CurveWidget
: public MIP_DragValueWidget {

//------------------------------
protected:
//------------------------------

    MIP_Color  MCurveColor     = MIP_Color(0.25);//MIP_COLOR_DARK_GRAY;
    MIP_Color  MInteractColor  = MIP_Color(1);//MIP_COLOR_WHITE;
    bool        MDecay          = false;
  //MIP_Point  MCoords[1024]   = {0};
    float       MXCoords[1024]   = {0};
    float       MYCoords[1024]   = {0};

//------------------------------
public:
//------------------------------

  MIP_CurveWidget(MIP_DRect ARect, bool ADecay=true)
  : MIP_DragValueWidget(ARect,"",0.5) {

    setWidgetName("MIP_CurveWidget");
    //setHint("curve");
    setText("curve");
    setBackgroundColor(MIP_Color(0.55f,0.55f,0.55f));

    MDecay = ADecay;

  }

  //----------

  virtual ~MIP_CurveWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setCurveColor(MIP_Color AColor)    { MCurveColor = AColor; }
  virtual void setInteractColor(MIP_Color AColor) { MInteractColor = AColor; }

//------------------------------
public:
//------------------------------


  void on_widget_paint(MIP_PaintContext* AContext) override {
    MIP_Painter* painter = AContext->painter;

    MIP_DRect mrect = getRect();
    //mrect.shrink(2);

    int32_t num = mrect.w;

    // calc coords

    if (num > 0) {
      float x,xadd;
      if (MDecay) {
        x = 1.0;
        xadd = -1.0f / num;
      }
      else {
        x = 0;
        xadd = 1.0f / num;
      }

      //float h = (float)mrect.h;
      //int32 y2 = mrect.y2() + 1;

      float value = getValue();
      for (int32_t i=0; i<num; i++) {
        float n = MIP_Curve(x,value);
        MXCoords[i] = mrect.x + i;
        MYCoords[i] = mrect.y2() + 1 - (n * (float)mrect.h);
        x += xadd;
      }
    }

    if (MDecay) {
      MXCoords[num] = mrect.x2();
      MYCoords[num] = mrect.y2() + 1;
      MXCoords[num+1] = mrect.x;
      MYCoords[num+1] = mrect.y2() + 1;
    }
    else {
      MXCoords[num] = mrect.x2();
      MYCoords[num] = mrect.y + 1;
      MXCoords[num+1] = mrect.x2();
      MYCoords[num+1] = mrect.y2() + 1;
    }

    // background

    //APainter->setFillColor(MBackgroundColor);

    painter->beginPath();
    painter->rect(mrect.x,mrect.y,mrect.w,mrect.h);
    painter->fillColor(MBackgroundColor);
    painter->fill();

    // curve

    //APainter->setPenSize(2);

    MIP_Color color = MCurveColor;
    //if (MIsInteractive) color = MInteractColor;

    painter->beginPath();
    painter->moveTo(MXCoords[0],MYCoords[0]);

    for (int32_t i=1; i<num; i++) {
      painter->lineTo(MXCoords[i],MYCoords[i]);
    }
    painter->strokeWidth(2);
    painter->strokeColor(color);
    painter->stroke();

    //APainter->setPenSize(1);
  }

};

//----------------------------------------------------------------------
#endif
