#ifndef mip_button_widget_included
#define mip_button_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/mip_text_widget.h"

//----------------------------------------------------------------------

class MIP_ButtonWidget
: public MIP_TextWidget {

//------------------------------
protected:
//------------------------------

  bool        MIsToggle             = false;
  bool        MDrawTriangle         = false;

  bool        MClicked              = false;

  const char* MOnText               = "On";
  MIP_Color   MOnBackgroundColor    = MIP_COLOR_LIGHT_YELLOW;
  MIP_Color   MOnBorderColor        = MIP_COLOR_BLACK;
  MIP_Color   MOnTextColor          = MIP_COLOR_BLACK;
  uint32_t    MOnTextAlignment      = MIP_TEXT_ALIGN_CENTER;
  MIP_FRect   MOnTextOffset         = MIP_FRect(2,2,2,2);

  const char* MOffText              = "Off";
  MIP_Color   MOffBackgroundColor   = MIP_Color(0.6);//MIP_COLOR_GRAY;
  MIP_Color   MOffBorderColor       = MIP_COLOR_DARK_GRAY;
  MIP_Color   MOffTextColor         = MIP_COLOR_DARK_GRAY;
  uint32_t    MOffTextAlignment     = MIP_TEXT_ALIGN_CENTER;
  MIP_FRect   MOffTextOffset        = MIP_FRect(2,2,2,2);



//------------------------------
public:
//------------------------------

  MIP_ButtonWidget(MIP_FRect ARect)
  : MIP_TextWidget(ARect) {
    setName("MIP_ButtonWidget");
    setHint("button");
    setCursor(MIP_CURSOR_FINGER);
    setDrawBorder(true);
    setRoundedCorners(MIP_CORNER_ALL);
    setRoundedRadius(6);
    setFillBackground(true);
    setFillGradient(true);
    setColors();
 }

  virtual ~MIP_ButtonWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setIsToggle(bool AIsToggle=true) {
    MIsToggle = AIsToggle;
  }

  virtual void setText(const char* AOff, const char* AOn) {
    MOnText = AOn;
    MOffText = AOff;
  }

  virtual void setTextAlignment(uint32_t AOff, uint32_t AOn) {
    MOnTextAlignment = AOn;
    MOffTextAlignment = AOff;
  }

  virtual void setTextColors(MIP_Color AOff, MIP_Color AOn) {
    MOnTextColor = AOn;
    MOffTextColor = AOff;
  }

  virtual void setBackgroundColors(MIP_Color AOff, MIP_Color AOn) {
    MOnBackgroundColor = AOn;
    MOffBackgroundColor = AOff;
  }

  //virtual void setOnGradientColors(MIP_Color AColor1, MIP_Color AColor2) {
  //  MOnGradientColor1 = AColor1;
  //  MOnGradientColor2 = AColor2;
  //}

  //virtual void setOffGradientColors(MIP_Color AColor1, MIP_Color AColor2) {
  //  MOffGradientColor1 = AColor1;
  //  MOffGradientColor2 = AColor2;
  //}

  virtual void setDrawTriangle(bool ADraw=true) {
    MDrawTriangle = ADraw;
  }


//------------------------------
public:
//------------------------------

  void setColors() {
    if (getValue() >= 0.5) { // on
      MIP_TextWidget::setText(MOnText);
      MIP_TextWidget::setTextColor(MOnTextColor);
      MIP_TextWidget::setTextAlignment(MOnTextAlignment);
      MIP_TextWidget::setBackgroundColor(MOnBackgroundColor);
      MBorderColor = MOnBorderColor;
    }
    else { // off
      MIP_TextWidget::setText(MOffText);
      MIP_TextWidget::setTextColor(MOffTextColor);
      MIP_TextWidget::setTextAlignment(MOffTextAlignment);
      MIP_TextWidget::setBackgroundColor(MOffBackgroundColor);
      MBorderColor = MOffBorderColor;
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    MIP_FRect mrect = getRect();
    setColors();
    fillBackground(APainter,ARect,AMode);
    drawText(APainter,ARect,AMode);

    if (MDrawTriangle) {
      if (getValue() >= 0.5) {
        APainter->fillTriangle( mrect.x2() - 12, mrect.y2() - 9,   mrect.x2() - 5, mrect.y2() - 9,   mrect.x2() - 9, mrect.y2() - 5,   MIP_COLOR_LIGHT_GRAY);
      }
      else {
        APainter->fillTriangle( mrect.x2() - 9, mrect.y2() - 13,   mrect.x2() - 5, mrect.y2() - 9,   mrect.x2() - 9, mrect.y2() - 5,   MIP_COLOR_LIGHT_GRAY);
      }
    }

    drawBorder(APainter,ARect,AMode);

  }

  //----------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == MIP_BUTTON_LEFT) {
      MClicked = true;
      if (MIsToggle) {
        setValue(1.0f - getValue());
      }
      else {
        setValue(1.0f);
      }
      update();
      redraw();
    }
  }

  //----------

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == MIP_BUTTON_LEFT) {
      MClicked = false;
      if (MIsToggle) {
      }
      else {
        setValue(0.0f);
        update();
        redraw();
      }
    }
  }

};

//----------------------------------------------------------------------
#endif

