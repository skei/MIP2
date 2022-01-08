#ifndef kode_curve_widget_includedbutton
#define kode_curve_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_drag_value_widget.h"

class KODE_CurveWidget
: public KODE_DragValueWidget {

//------------------------------
protected:
//------------------------------

    KODE_Color  MCurveColor     = KODE_COLOR_DARK_GRAY;
    KODE_Color  MInteractColor  = KODE_COLOR_WHITE;
    bool        MDecay          = false;
  //KODE_Point  MCoords[1024]   = {0};
    float       MXCoords[1024]   = {0};
    float       MYCoords[1024]   = {0};

//------------------------------
public:
//------------------------------

  KODE_CurveWidget(KODE_FRect ARect, bool ADecay=true)
  : KODE_DragValueWidget(ARect) {

    setName("KODE_CurveWidget");
    setHint("curve");
    setText("curve");
    setBackgroundColor(KODE_Color(0.55f,0.55f,0.55f));

    MDecay = ADecay;

  }

  //----------

  virtual ~KODE_CurveWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setCurveColor(KODE_Color AColor)    { MCurveColor = AColor; }
  virtual void setInteractColor(KODE_Color AColor) { MInteractColor = AColor; }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    KODE_FRect mrect = getRect();
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
          float n = KODE_Curve(x,value);
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
      APainter->fillRectangle(mrect,MBackgroundColor);

      // curve

      //APainter->setPenSize(2);

      KODE_Color color = MCurveColor;
      //if (MIsInteractive) color = MInteractColor;

      for (int32_t i=0; i<num; i++) {
        APainter->drawLine(MXCoords[i],MYCoords[i],MXCoords[i+1],MYCoords[i+1], color, 2);
      }

      //APainter->setPenSize(1);
  }

};

//----------------------------------------------------------------------
#endif
