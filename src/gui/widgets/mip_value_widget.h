#ifndef mip_value_widget_included
#define mip_value_widget_included
//----------------------------------------------------------------------

#include "base/utils/mip_convert.h"
#include "gui/widgets/mip_text_widget.h"

#define MIP_VALUE_WIDGET_LABEL_SPACE 4.0f

//----------------------------------------------------------------------

class MIP_ValueWidget
: public MIP_TextWidget {

//------------------------------
private:
//------------------------------

  char        MValueText[32]      = {0};

//------------------------------
protected:
//------------------------------

  uint32_t    MDigits             = 3;

  // value text
  bool        MDrawValueText      = true;
  MIP_Color  MValueTextColor     = MIP_COLOR_WHITE;
  uint32_t    MValueTextAlignment = MIP_TEXT_ALIGN_RIGHT;
  MIP_FRect  MValueTextOffset    = MIP_FRect(2,2,2,2);

  // label
  bool        MDrawLabel          = false;
  const char* MLabel              = "";
  MIP_Color  MLabelColor         = MIP_Color(0.7f);

//------------------------------
public:
//------------------------------

  MIP_ValueWidget(MIP_FRect ARect, const char* AText="")
  : MIP_TextWidget(ARect) {
    setName("MIP_ValueWidget");
    setHint("value");
    MText = AText;
  }

  virtual ~MIP_ValueWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDigits(uint32_t ADigits) {
    MDigits = ADigits;
  }

  virtual void setDrawValueText(bool ADraw=true) {
    MDrawValueText = ADraw;
  }

  virtual void setValueTextAlignment(uint32_t AAlignment) {
    MValueTextAlignment = AAlignment;
  }

  virtual void setValueTextColor(MIP_Color AColor) {
    MValueTextColor = AColor;
  }

  virtual void setValueTextOffset(MIP_FRect AOffset) {
    MValueTextOffset = AOffset;
  }

  virtual void setDrawLabel(bool ADraw=true) {
    MDrawLabel = ADraw;
  }

  virtual void setLabel(const char* ALabel) {
    MLabel = ALabel;
  }

  virtual void setLabelColor(MIP_Color AColor) {
    MLabelColor = AColor;
  }

//------------------------------
public:
//------------------------------

  virtual void drawValueText(MIP_BasePainter* APainter, MIP_FRect ARect, uint32_t AMode) {

    if (MDrawValueText) {

      float value = getValue();
      const char*     label = "";

      MIP_Parameter* param = getParameter();

      if (param) {
        label = param->getLabel();
        value = param->from01(value);
        param->displayText(MValueText,value);
      }
      else {
        label = MLabel;
        MIP_FloatToString(MValueText,value,MDigits);
      }

      MIP_FRect value_rect = getRect();
      MIP_FRect label_rect = getRect();

      value_rect.shrink(MValueTextOffset);
      label_rect.shrink(MValueTextOffset);

      if (MDrawLabel) {
        float width = APainter->getTextWidth(label);
        label_rect.x = value_rect.x2() - width;
        label_rect.w = width;
        value_rect.w -= (width + MIP_VALUE_WIDGET_LABEL_SPACE);
      }

      if (MDrawValueText) {
        APainter->drawText(value_rect,MValueText,MValueTextAlignment,MValueTextColor);
      }
      if (MDrawLabel) {
        APainter->drawText(label_rect,label,MValueTextAlignment,MLabelColor);
      }
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    //paintChildren(APainter,getRect(),AMode);
    drawText(APainter,ARect,AMode);
    drawValueText(APainter,ARect,AMode);
    drawBorder(APainter,ARect,AMode);
  }

};

//----------------------------------------------------------------------
#endif
