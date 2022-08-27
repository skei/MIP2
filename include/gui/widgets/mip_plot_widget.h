#ifndef mip_plot_widget_included
#define mip_plot_widget_included
//----------------------------------------------------------------------

#define MAX_VALUES 1024

#include "gui/widgets/mip_panel_widget.h"
//#include "gui/mip_widget.h"

class MIP_PlotWidget
: public MIP_PanelWidget {

//------------------------------
private:
//------------------------------

  //bool      MDrawBackground     = true;
  //MIP_Color MBackColor          = MIP_COLOR_DARK_GRAY;

  MIP_Color MLineColor          = MIP_COLOR_LIGHT_GRAY;
  bool      MBipolar            = true;
  uint32_t  MNumValues          = 64;
  float     MValues[MAX_VALUES]  = {0};

//------------------------------
public:
//------------------------------

  MIP_PlotWidget(MIP_DRect ARect)
  : MIP_PanelWidget(ARect) {
    MName = "MIP_PlotWidget";
    //memset(MValues,0,sizeof(MValues));
    for (uint32_t i=0; i<MAX_VALUES; i++) MValues[i] = MIP_RandomSigned();
  }

  virtual ~MIP_PlotWidget() {
  }

//------------------------------
public:
//------------------------------

    //int32_t getWidth(void)   { return getRect().w; }
    //int32_t getHeight(void)  { return getRect().h; }

    void setNumValues(uint32_t ANum) {
      MNumValues = ANum;
    }

    void setValue(int32_t AIndex, float AValue) {
      MValues[AIndex] = AValue;
    }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    MIP_Painter* painter = AContext->painter;
    fillBackground(AContext);
    if (MRect.w > 0) {
      double h2 = MRect.h * 0.5f;
      double yprev = MRect.y2() - 1.0;
      if (MBipolar) yprev -= h2;
      double index = 0.0;
      double index_add = (double)MNumValues / MRect.w;
      double x = MRect.x;
      painter->beginPath();
      painter->moveTo(x,yprev);
      for (int32_t i=0; i<MRect.w; i++) {
        double v = MValues[(int)index];
        index += index_add;
        if (MBipolar) v = (v + 1.0) * 0.5;        // -1..1 -> 0..1
        double y = MRect.y2() - (v * MRect.h) - 1.0;
        x += 1.0;
        painter->lineTo(x,y);
        yprev = y;
      }
      painter->strokeColor(MLineColor);
      painter->stroke();
    }
    drawBorder(AContext);
  }

};

#undef MAX_WIDTH

//----------------------------------------------------------------------
#endif
