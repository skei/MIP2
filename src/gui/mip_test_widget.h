#ifndef mip_test_widget_included
#define mip_test_widget_included
//----------------------------------------------------------------------

#include "mip.h"
//#include "base/types/mip_array.h"
//#include "base/types/mip_rect.h"
//#include "gui/mip_painter.h"
#include "gui/mip_widget.h"

//----------------------------------------------------------------------

class MIP_TestWidget
: public MIP_Widget {

//------------------------------
private:
//------------------------------

  bool MIsDragging    = false;
  float MClickedX     = 0.0;
  float MClickedY     = 0.0;
  float MClickedValue = 0.0;
  float MSensitivity  = 0.01;

//------------------------------
public:
//------------------------------

  MIP_TestWidget(MIP_FRect ARect, const char* AName, float AValue)
  : MIP_Widget(ARect) {
    MName = AName;
    MValue = AValue;
  }

  //----------

  virtual ~MIP_TestWidget() {
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect) override {
    MIP_FRect rect = MRect;
    if (MIsDragging) APainter->fillRectangle(rect,0.75);
    else APainter->fillRectangle(rect,0.45);
    float w = (float)MRect.w;
    float val_w = w * MValue;
    float mod_w = w * MModValue;
    rect.w = val_w;
    APainter->fillRectangle(rect,MIP_COLOR_RED);
    rect.w = mod_w;
    rect.h = 2;
    APainter->fillRectangle(rect,MIP_COLOR_LIGHT_GREEN);
    //rect.x += val_w;
    //rect.w = MRect.w - val_w;
    //if (MIsDragging) {
    //  APainter->fillRectangle(rect,0.75);
    //}
    //else {
    //  APainter->fillRectangle(rect,0.45);
    //}
  }

  //----------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    //MIP_Print("%s : x %.2f y %.2f b %i s %i\n",MName,AXpos,AYpos,AButton,AState);
    if (AButton == MIP_BUTTON_LEFT) {
      MIsDragging   = true;
      MClickedX     = AXpos;
      MClickedY     = AYpos;
      MClickedValue = MValue;
      //do_widget_update(this);
      do_widget_redraw(this);
    }
  }

  //----------

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp) override {
    //MIP_Print("%s : x %.2f y %.2f s %i\n",MName,AXpos,AYpos,AState);
    if (MIsDragging) {
      //float deltax = MClickedX - AXpos;
      float deltay = MClickedY - AYpos;
      float value = MClickedValue + (deltay * MSensitivity);
      value = MIP_Clamp(value,0,1);
      if (value != MValue) { // todo: MIP_NearlyEqual
        MValue = value;
        do_widget_update(this);
        do_widget_redraw(this);
      }
    }
  }

  //----------

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    //MIP_Print("%s : x %.2f y %.2f b %i s %i\n",MName,AXpos,AYpos,AButton,AState);
    MIsDragging = false;
    do_widget_redraw(this);
  }


};


//----------------------------------------------------------------------
#endif
