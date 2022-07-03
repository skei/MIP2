#ifndef mip_value_widget_included
#define mip_value_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widget.h"
#include "gui/widgets/mip_text_widget.h"

class MIP_ValueWidget
: public MIP_TextWidget {

//------------------------------
protected:
//------------------------------


  //const char* MValueText          = "0.0";
  bool        MDrawValueText      = true;
  MIP_Color   MValueTextColor     = MIP_COLOR_BLACK;
  double      MValueTextSize      = 12.0;
  uint32_t    MValueTextAlignment = MIP_TEXT_ALIGN_RIGHT;
  MIP_FRect   MValueTextOffset    = MIP_FRect(0,0,2,0);
  bool        MDrawParamValue     = true;
  uint32_t    MDigits             = 2;

  char        MDisplayText[32]    = {0};

//------------------------------
public:
//------------------------------

  MIP_ValueWidget(MIP_FRect ARect, const char* AText="")
  : MIP_TextWidget(ARect,AText) {
    setName("MIP_TemplateWidget");
    setHint("template");
    setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    setTextOffset(MIP_FRect(2,0,0,0));
  }

  virtual ~MIP_ValueWidget() {
  }

//------------------------------
public:
//------------------------------

//  virtual void setDrawParamName(bool ADraw=true) {
//    MDrawParamName = ADrawName;
//  }
//
//  virtual void setDrawParamValue(bool ADraw=true) {
//    MDrawParamValue = ADraw;
//  }

  //virtual void setDrawParamName(bool AParam=true) {
  //  MDrawParamName = AParam;
  //}

  virtual void setDrawValueText(bool ADraw=true) {
    MDrawValueText = ADraw;
  }

  virtual void setDigits(uint32_t ADigits) {
    MDigits = ADigits;
  }

//------------------------------
public:
//------------------------------

  virtual void drawValueText(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
    if (MDrawValueText) {
      MIP_FRect rect = MRect;
      rect.shrink(MValueTextOffset);
      const char* text = MDisplayText;
      if (MDrawParamValue && MParameter) {
        double value = MParameter->getValue();
        if (MParameter->isStepped()) {
          sprintf(MDisplayText,"%i",(int)value);
          text = MDisplayText;
        }
        else {
          sprintf(MDisplayText,"%.2f",value);
          text = MDisplayText;
        }
      }
      else {
        double value = getValue();
        if (MDigits == 0) {
          sprintf(MDisplayText,"%i",(int)value);
          text = MDisplayText;
        }
        else {
          char format[8] = {0};
          format[0] = '%';
          format[1] = '.';
          format[2] = '0' + MDigits;
          format[3] = 'f';
          format[4] = 0;
          MIP_Print("format: '%s'\n",format);
          sprintf(MDisplayText,"%.2f",value);
          text = MDisplayText;
        }
      }
      APainter->setColor(MValueTextColor);
      APainter->drawText(rect,text,MValueTextAlignment);
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter, ARect, AMode);
    paintWidgets(APainter,ARect,AMode);
    drawText(APainter, ARect, AMode);
    drawValueText(APainter, ARect, AMode);
    drawBorder(APainter, ARect, AMode);
  }

};

//----------------------------------------------------------------------
#endif
