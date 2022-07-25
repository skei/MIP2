#ifndef mip_waveform_widget_included
#define mip_waveform_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_paint_context.h"
#include "gui/widgets/mip_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_WaveformWidget
: public MIP_PanelWidget {

//------------------------------
protected:
//------------------------------

  bool      MDrawWaveform   = true;
  double    MWaveformWidth  = 1;
  MIP_Color MWaveformColor  = MIP_COLOR_DARK_GRAY;

  bool      MDrawZeroLine   = true;
  double    MZeroLineWidth  = 0.5;
  MIP_Color MZeroLineColor  = MIP_COLOR_BLACK;

  float*    MBuffer       = nullptr;
  uint32_t  MNumSamples   = 0;

//------------------------------
public:
//------------------------------

  MIP_WaveformWidget(MIP_DRect ARect)
  : MIP_PanelWidget(ARect) {
  }

  //----------

  virtual ~MIP_WaveformWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawWaveform(bool ADraw=true) { MDrawWaveform = ADraw; }
  virtual void setWaveformWidth(double AWidth) { MWaveformWidth = AWidth; }
  virtual void setWaveformColor(MIP_Color AColor) { MWaveformColor = AColor; }

  virtual void setBuffer(float* ABuffer, uint32_t ASamples) {
    MBuffer       = ABuffer;
    MNumSamples   = ASamples;
  }

//------------------------------
public: // parent to child
//------------------------------

//  void on_widget_move(double AXpos, double AYpos)  override {}
//  void on_widget_resize(double AWidth, double AHeight) override {}
//  void on_widget_align(bool ARecursive=true) override {}

  void on_widget_paint(MIP_PaintContext* AContext) override {
    fillBackground(AContext);
    drawWaveform(AContext);
    //paintChildWidgets(AContext);
    drawBorder(AContext);
  }

//  void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) override {}
//  void on_widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) override {}
//  void on_widget_mouse_press(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_enter(MIP_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_leave(MIP_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) override {}
//  //void on_widget_connect(MIP_Parameter* AParameter) override {}

//------------------------------
public:
//------------------------------

  virtual void drawWaveform(MIP_PaintContext* AContext) {
    if (MRect.w <= 0.0)   return;
    uint32_t width = MRect.w;
    double half = MRect.h * 0.5;
    double x = MRect.x;
    double y = MRect.y + half;
    MIP_Painter* painter = AContext->painter;
    if (MDrawWaveform) {
      if (!MBuffer) return;
      if (MNumSamples == 0) return;
      double pos = 0;
      double step = (double)MNumSamples / MRect.w;
      painter->beginPath();
      painter->strokeColor(MWaveformColor);
      painter->strokeWidth(MWaveformWidth);
      for (uint32_t i=0; i<width; i++) {
        uint32_t p = (uint32_t)pos;
        float v = MBuffer[p] * half;
        float y2 = y - v;
        if (i==0) painter->moveTo(x,y2);
        else painter->lineTo(x+i,y2);
        pos += step;
      }
      painter->stroke();
    }

    if (MDrawZeroLine) {
      painter->beginPath();
      painter->strokeColor(MZeroLineColor);
      painter->strokeWidth(MZeroLineWidth);
      painter->moveTo(MRect.x,y);
      painter->lineTo(MRect.x2(),y);
      painter->stroke();
    }

  }

};

//----------------------------------------------------------------------
#endif


