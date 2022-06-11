#ifndef mip_text_widget_included
#define mip_text_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widget.h"
#include "gui/widgets/mip_panel_widget.h"

//----------------------------------------------------------------------

class MIP_TextWidget
: public MIP_PanelWidget {

//------------------------------
protected:
//------------------------------

  const char* MText           = "text";
  bool        MDrawText       = true;
  MIP_Color   MTextColor      = MIP_COLOR_BLACK;
  double      MTextSize       = 11.0;
  uint32_t    MTextAlignment  = MIP_TEXT_ALIGN_CENTER;
  bool        MDrawParamName  = true;//false;
  MIP_FRect   MTextOffset     = MIP_FRect(2,0,2,0);

//------------------------------
public:
//------------------------------

  MIP_TextWidget(MIP_FRect ARect, const char* AText="")
  : MIP_PanelWidget(ARect) {
    setName("MIP_TextWidget");
    setHint("text");
    setText(AText);
    setDrawBorder(false);
  }

  virtual ~MIP_TextWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawText(bool ADraw=true) {
    MDrawText = ADraw;
  }

  virtual void setText(const char* AText) {
    MText = AText;
  }

  virtual void setTextAlignment(uint32_t AAlignment) {
    MTextAlignment = AAlignment;
  }

  virtual void setTextColor(MIP_Color AColor) {
    MTextColor = AColor;
  }

  virtual void setTextSize(double ASize) {
    MTextSize = ASize;
  }

  virtual void setTextOffset(MIP_FRect AOffset) {
    MTextOffset = AOffset;
  }

  virtual void setDrawParamName(bool ADraw=true) {
    MDrawParamName = ADraw;
  }

//------------------------------
public:
//------------------------------

  virtual void drawText(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
    if (MDrawText) {
      MIP_FRect rect = MRect;
      rect.shrink(MTextOffset);
      const char* text = MText;
      if (MDrawParamName && MParameter) {
        text = MParameter->getName();
      }
      APainter->setColor(MTextColor);
      APainter->setFontSize(MTextSize);
      APainter->drawText(rect,text,MTextAlignment);
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    paintWidgets(APainter,ARect,AMode);
    drawText(APainter,ARect,AMode);
    drawBorder(APainter,ARect,AMode);
  }

};

//----------------------------------------------------------------------
#endif
