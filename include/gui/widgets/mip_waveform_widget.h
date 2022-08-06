#ifndef mip_waveform_widget_included
#define mip_waveform_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_paint_context.h"
#include "gui/widgets/mip_panel_widget.h"

#define NUM_MARKERS 16
#define NUM_AREAS   16

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

  //

  float*    MLeftBuffer   = nullptr;
  float*    MRightBuffer  = nullptr;

  float*    MBuffer       = nullptr;
  uint32_t  MNumSamples   = 0;

  uint32_t  MNumGrid      = 0;
  uint32_t  MNumSubGrid   = 0;

  MIP_Color MGridColor    = MIP_COLOR_BLACK;
  MIP_Color MSubGridColor = MIP_COLOR_DARK_GRAY;

  bool      MMarkerActive[NUM_MARKERS]  = {false};
  uint32_t  MMarkerPos[NUM_MARKERS]     = {0};
  MIP_Color MMarkerColor[NUM_MARKERS]   = {MIP_COLOR_DARK_GRAY};
  double    MMarkerWidth[NUM_MARKERS]   = {1};

  bool      MAreaActive[NUM_AREAS]    = {false};
  uint32_t  MAreaStart[NUM_AREAS]     = {0};
  uint32_t  MAreaLength[NUM_AREAS]    = {0};
  MIP_Color MAreaColor[NUM_AREAS]     = {MIP_COLOR_GREEN};

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

  virtual void setNumGrid(uint32_t ANum)    { MNumGrid = ANum; }
  virtual void setNumSubGrid(uint32_t ANum) { MNumSubGrid = ANum; }

  virtual void setBuffer(float* ABuffer, uint32_t ASamples) {
    MBuffer       = ABuffer;
    MNumSamples   = ASamples;
  }

  virtual void setBuffers(float* ALeftBuffer, float* ARightBuffer, uint32_t ASamples) {
    MLeftBuffer   = ALeftBuffer;
    MRightBuffer  = ARightBuffer;
    MNumSamples   = ASamples;
  }

  virtual void setMarkerActive(uint32_t AIndex, bool AActive) {
    MMarkerActive[AIndex] = AActive;
  }

  virtual void setMarkerPos(uint32_t AIndex, uint32_t APos) {
    MMarkerPos[AIndex]    = APos;
  }

  virtual void setMarkerColor(uint32_t AIndex, MIP_Color AColor) {
    MMarkerColor[AIndex]  = AColor;
  }

  virtual void setMarkerWidth(uint32_t AIndex, double AWidth) {
    MMarkerWidth[AIndex]  = AWidth;
  }

  virtual void setAreaActive(uint32_t AIndex, bool AActive) {
    MAreaActive[AIndex] = AActive;
  }

  virtual void setAreaStart(uint32_t AIndex, uint32_t AStart) {
    MAreaStart[AIndex]  = AStart;
  }

  virtual void setAreaLength(uint32_t AIndex, uint32_t ALength) {
    MAreaLength[AIndex] = ALength;
  }

  virtual void setAreaColor(uint32_t AIndex, MIP_Color AColor) {
    MAreaColor[AIndex]  = AColor;
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
//  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_enter(MIP_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_leave(MIP_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) override {}
//  //void on_widget_connect(MIP_Parameter* AParameter) override {}

//------------------------------
public:
//------------------------------

  virtual void drawWaveform(MIP_PaintContext* AContext) {
    //MIP_Print("MRect.w %.2f MBuffer %p MNumSamples %i\n",MRect.w,MBuffer,MNumSamples);
    double widget_width = MRect.w;
    if (widget_width <= 0.0) return;
    double half_height = MRect.h * 0.5;
    double mid_y = MRect.y + half_height;
    MIP_Painter* painter = AContext->painter;

    // waveform

    if (MDrawWaveform) {
      if (MBuffer && (MNumSamples > 0)) {
        double pos = 0;
        double step = (double)MNumSamples / MRect.w;
        painter->beginPath();
        painter->strokeColor(MWaveformColor);
        painter->strokeWidth(MWaveformWidth);
        for (uint32_t i=0; i<widget_width; i++) {
          uint32_t p = (uint32_t)pos;
          double v = MBuffer[p];
          v = MIP_Clamp(v,-1,1);
          v *= half_height;
          double y = mid_y - v;
          if (i==0) painter->moveTo(MRect.x,y);
          else painter->lineTo(MRect.x+i,y);
          pos += step;
        }
        painter->stroke();
      }
    }

    // zero-line

    if (MDrawZeroLine) {
      painter->beginPath();
      painter->strokeColor(MZeroLineColor);
      painter->strokeWidth(MZeroLineWidth);
      painter->moveTo(MRect.x,mid_y);
      painter->lineTo(MRect.x2(),mid_y);
      painter->stroke();
    }

    // sub grid

    uint32_t num = MNumGrid * MNumSubGrid;
    if (num > 0) {
      double width = MRect.w / (double)num;
      double x = MRect.x + width;
      painter->beginPath();
      for (uint32_t i=0; i<(num-1); i++) {
        painter->moveTo(x,MRect.y);
        painter->lineTo(x,MRect.y2());
        x += width;
      }
      painter->strokeColor(MSubGridColor);
      painter->stroke();
    }

    // grid

    num = MNumGrid;
    if (num > 0) {
      double width = MRect.w / (double)num;
      double x = MRect.x + width;
      painter->beginPath();
      for (uint32_t i=0; i<(num-1); i++) {
        painter->moveTo(x,MRect.y);
        painter->lineTo(x,MRect.y2());
        x += width;
      }
      painter->strokeColor(MGridColor);
      painter->stroke();
    }

    // areas

    for (uint32_t i=0; i<NUM_AREAS; i++) {
      if (MAreaActive[i]) {
        double x = (double)MAreaStart[i]  * widget_width / (double)MNumSamples;
        double w = (double)MAreaLength[i] * widget_width / (double)MNumSamples;
        painter->beginPath();
        painter->rect(MRect.x + x,MRect.y, w,MRect.h);
        painter->fillColor(MAreaColor[i]);
        painter->fill();
      }
    }

    // markers

    for (uint32_t i=0; i<NUM_MARKERS; i++) {
      if (MMarkerActive[i]) {
        double x = (double)MMarkerPos[i] * widget_width / (double)MNumSamples;
        painter->beginPath();
        painter->moveTo(MRect.x + x,MRect.y);
        painter->lineTo(MRect.x + x,MRect.y2());
        painter->strokeWidth(MMarkerWidth[i]);
        painter->strokeColor(MMarkerColor[i]);
        painter->stroke();
      }
    }


  }

};

//----------------------------------------------------------------------
#endif


