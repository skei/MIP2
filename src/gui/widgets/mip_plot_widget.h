#ifndef kode_plot_widget_included
#define kode_plot_widget_included
//----------------------------------------------------------------------

#define MAX_WIDTH 4096

#include "gui/kode_widget.h"

class KODE_PlotWidget
: public KODE_Widget {

//------------------------------
private:
//------------------------------

  KODE_Color  MBackColor          = KODE_COLOR_DARK_GRAY;
  KODE_Color  MLineColor          = KODE_COLOR_LIGHT_GRAY;
  bool        MDrawBackground     = true;
  bool        MBipolar            = true;
  float       MValues[MAX_WIDTH]  = {0};


//------------------------------
public:
//------------------------------

  KODE_PlotWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    setName("KODE_PlotWidget");
    setHint("plot");
    //memset(MValues,0,sizeof(MValues));
  }

  virtual ~KODE_PlotWidget() {
  }

//------------------------------
public:
//------------------------------

    int32_t getWidth(void)   { return getRect().w; }
    int32_t getHeight(void)  { return getRect().h; }

    void setValue(int32_t AIndex, float AValue) {
      MValues[AIndex] = AValue;
    }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    KODE_FRect mrect = getRect();
    if (MDrawBackground) {
      APainter->fillRectangle(mrect,MBackColor);
    }
    float h2 = (float)mrect.h * 0.5f;
    int32_t yprev = mrect.y2() - 1;
    if (MBipolar) yprev -= h2;
    int32_t x = mrect.x;
    for (int32_t i=0; i<mrect.w; i++) {
      float v = MValues[i];
      if (MBipolar) v = (v+1) * 0.5;        // -1..1 -> 0..1
      //int32 y = mrect.y2() - (v*h2) - 1;
      int32_t y = mrect.y2() - (v*mrect.h) - 1;
      APainter->drawLine(x,yprev,x+1,y,MLineColor);
      x++;
      yprev = y;
    }
    KODE_Widget::on_widget_paint(APainter,ARect,AMode);
  }

};

#undef MAX_WIDTH

//----------------------------------------------------------------------
#endif
