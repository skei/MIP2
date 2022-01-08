#ifndef kode_button_widget_included
#define kode_button_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_text_widget.h"

//----------------------------------------------------------------------

class KODE_ButtonWidget
: public KODE_TextWidget {

//------------------------------
protected:
//------------------------------

  bool        MIsToggle           = false;
  bool        MDrawTriangle       = false;

  const char* MOnText             = "On";
  KODE_Color  MOnBackgroundColor  = KODE_Color(0.7f);
  KODE_Color  MOnBorderColor      = KODE_Color(0.3f);
  KODE_Color  MOnTextColor        = KODE_Color(0.3f);
  uint32_t    MOnTextAlignment    = KODE_TEXT_ALIGN_CENTER;
  KODE_FRect  MOnTextOffset       = KODE_FRect(2,2,2,2);

  const char* MOffText            = "Off";
  KODE_Color  MOffBackgroundColor = KODE_Color(0.3f);
  KODE_Color  MOffBorderColor     = KODE_Color(0.7f);
  KODE_Color  MOffTextColor       = KODE_Color(0.7f);
  uint32_t    MOffTextAlignment   = KODE_TEXT_ALIGN_CENTER;
  KODE_FRect  MOffTextOffset      = KODE_FRect(2,2,2,2);

//------------------------------
public:
//------------------------------

  KODE_ButtonWidget(KODE_FRect ARect)
  : KODE_TextWidget(ARect) {
    setName("KODE_ButtonWidget");
    setHint("button");
    setCursor(KODE_CURSOR_FINGER);
    setFillBackground(true);
  }

  virtual ~KODE_ButtonWidget() {
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

  virtual void setTextColor(KODE_Color AOff, KODE_Color AOn) {
    MOnTextColor = AOn;
    MOffTextColor = AOff;
  }

  virtual void setBackgroundColor(KODE_Color AOff, KODE_Color AOn) {
    MOnBackgroundColor = AOn;
    MOffBackgroundColor = AOff;
  }

  virtual void setDrawTriangle(bool ADraw=true) {
    MDrawTriangle = ADraw;
  }


//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    KODE_FRect mrect = getRect();
    if (getValue() >= 0.5) { // on
      KODE_TextWidget::setText(MOnText);
      KODE_TextWidget::setTextColor(MOnTextColor);
      KODE_TextWidget::setTextAlignment(MOnTextAlignment);
      KODE_TextWidget::setBackgroundColor(MOnBackgroundColor);
      //APainter->drawText(mrect,MOnText,MOnTextAlignment,MOnTextColor);
      //APainter->fillRectangle(getRect(),MOnBackgroundColor);
      //KODE_FRect rect = getRect();
      //rect.shrink(MOnTextOffset);
      //APainter->drawText(rect,MOnText,MOnTextAlignment,MOnTextColor);
      //APainter->drawRectangle(getRect(),MOnBorderColor);
    }
    else { // off
      KODE_TextWidget::setText(MOffText);
      KODE_TextWidget::setTextColor(MOffTextColor);
      KODE_TextWidget::setTextAlignment(MOffTextAlignment);
      KODE_TextWidget::setBackgroundColor(MOffBackgroundColor);
      //APainter->fillRectangle(getRect(),MOffBackgroundColor);
      //KODE_FRect rect = getRect();
      //rect.shrink(MOffTextOffset);
      //APainter->drawText(rect,MOffText,MOffTextAlignment,MOffTextColor);
      //APainter->drawRectangle(getRect(),MOffBorderColor);
    }

    fillBackground(APainter,ARect,AMode);
    drawText(APainter,ARect,AMode);
    if (MDrawTriangle) {
      if (getValue() >= 0.5) {
        APainter->fillTriangle( mrect.x2() - 12, mrect.y2() - 9,   mrect.x2() - 5, mrect.y2() - 9,   mrect.x2() - 9, mrect.y2() - 5,   KODE_COLOR_LIGHT_GRAY);
      }
      else {
        APainter->fillTriangle( mrect.x2() - 9, mrect.y2() - 13,   mrect.x2() - 5, mrect.y2() - 9,   mrect.x2() - 9, mrect.y2() - 5,   KODE_COLOR_LIGHT_GRAY);
      }
    }
    drawBorder(APainter,ARect,AMode);

  }

  //----------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == KODE_BUTTON_LEFT) {
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
    if (AButton == KODE_BUTTON_LEFT) {
      if (!MIsToggle) {
        setValue(0.0f);
        update();
        redraw();
      }
    }
  }

};

//----------------------------------------------------------------------
#endif

