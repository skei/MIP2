#ifndef mip_slider_bank_widget_includedbutton
#define mip_slider_bank_widget_included
//----------------------------------------------------------------------

#include "base/utils/mip_random.h"
#include "gui/mip_widget.h"

#define MIP_MAX_NUM_SLIDERS 256

class MIP_SliderBankWidget
: public MIP_PanelWidget {

//------------------------------
private:
//------------------------------

    int32_t     MNumSliders   = 0;
    int32_t     MHoverSlider  = -1;
    MIP_Color  MBarColor     = MIP_COLOR_DARK_GRAY;
    MIP_Color  MHoverColor   = MIP_COLOR_LIGHT_GRAY;
    bool        MIsDragging   = false;

    float       MSliderValues[MIP_MAX_NUM_SLIDERS] = {0};

//------------------------------
public:
//------------------------------

  MIP_SliderBankWidget(MIP_FRect ARect, int32_t ANum=16)
  : MIP_PanelWidget(ARect) {

    setName("MIP_SliderBankWidget");
    setHint("aliderbank");
    setCursor(MIP_CURSOR_FINGER);

    setFillBackground(true);
    setBackgroundColor(MIP_Color(0.55f));
    setDrawBorder(false);

    MNumSliders = ANum;

    //setFlag(MIP_WIDGET_HOVER);
    //KMemset(MSliderValues,0,sizeof(MSliderValues));

    for (int32_t i=0; i<MIP_MAX_NUM_SLIDERS; i++) {
      float v = MIP_RandomRange(0,1);
      //MIP_Print("%i = %.2f\n",i,v);
      MSliderValues[i] = v;
    }

  }

  virtual ~MIP_SliderBankWidget() {
  }

//------------------------------
public:
//------------------------------

    void setNumSliders(int32_t ANum)     { MNumSliders = ANum; }
    void setBarColor(MIP_Color AColor)  { MBarColor = AColor; }

  //public:
  //
  //  void drawBar(SPainter* APainter, int32 AIndex) {
  //    float w = ( (float)MRect.w / (float)MNumSliders );
  //    float x = (float)MRect.x + (w * AIndex);
  //    float y = MRect.y2() - floorf(MSliderValues[AIndex] * MRect.h);
  //    APainter->fillRectangle( floorf(x), y/*MRect.y*/, floorf(x+w)-2, MRect.y2() );
  //  }

//------------------------------
public:
//------------------------------


  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    MIP_FRect mrect = getRect();

    fillBackground(APainter,ARect,AMode);

    //STrace("painting %s\n",MName);
    if (MNumSliders > 1) {
      float w = ( (float)mrect.w / (float)MNumSliders );
      int32_t iw2 = floorf(w) - 2;
      float x = (float)mrect.x /*+ sliderwidth - 1*/;
      for (int32_t i=0; i<MNumSliders; i++) {
        float v = MSliderValues[i] * mrect.h;
        float y = mrect.y2() - v;
        float h = v;//mrect.h - v;
        int32_t ix = floorf(x);
        if (i==MHoverSlider) APainter->fillRectangle( MIP_FRect(ix,y,iw2,h),MHoverColor );
        else APainter->fillRectangle( MIP_FRect(ix,y,iw2,h), MBarColor );
        x += w;
      }
    }

    drawBorder(APainter,ARect,AMode);

  }

  //----------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
    if (AButton == MIP_BUTTON_LEFT) {
      MIsDragging = true;
      MIP_FRect mrect = getRect();
      int32_t i = (AXpos - mrect.x) * MNumSliders / mrect.w;
      float v = (mrect.y2() - AYpos) / mrect.h;

      MSliderValues[i] = MIP_Clamp(v,0,1);
      do_widget_redraw(this,mrect,0);
    }
  }

  //----------

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
    if (AButton == MIP_BUTTON_LEFT) {
      MIsDragging = false;
    }
  }

  //----------

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) override {
    MIP_FRect mrect = getRect();
    //SWidget::on_mouseMove(AXpos,AYpos,AState);
    float w = (float)(AXpos - mrect.x) / (float)mrect.w; // 0..1
    int32_t index = (w * (float)MNumSliders);
    if (MIsDragging) {
      float v = (mrect.y2() - AYpos) / mrect.h;
      MSliderValues[index] = MIP_Clamp(v,0,1);
      if (index != MHoverSlider) MHoverSlider = index;
      do_widget_redraw(this,mrect,0);
    }
    else {
      if (index != MHoverSlider) MHoverSlider = index;
      do_widget_redraw(this,mrect,0);
    }
  }

  //----------

  void on_widget_mouseLeave(float AXpos, float AYpos, MIP_Widget* ATo/*, uint32_t ATimeStamp=0*/) override {
    MIP_Widget::on_widget_mouseLeave(AXpos,AYpos,ATo/*,ATimeStamp*/);
    MIP_FRect mrect = getRect();
    //SWidget::on_leave(AWidget);
    if (MHoverSlider >= 0) {
      MHoverSlider = -1;
      do_widget_redraw(this,mrect,0);
    }
    //if (flags.autoCursor) do_widget_setMouseCursor(this,MIP_CURSOR_DEFAULT);
    //if (flags.autoHint) do_widget_setHint(this,"");
  }

};

#undef MIP_MAX_NUM_SLIDERS


//----------------------------------------------------------------------
#endif
