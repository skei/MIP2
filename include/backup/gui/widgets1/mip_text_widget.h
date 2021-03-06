#ifndef mip_text_widget_included
#define mip_text_widget_included
//----------------------------------------------------------------------

//#include "gui/mip_widgets.h"
#include "gui/widgets/mip_panel_widget.h"

//----------------------------------------------------------------------

class MIP_TextWidget
: public MIP_PanelWidget {

//------------------------------
protected:
//------------------------------

  const char* MText           = "";
  float       MTextSize       = 11;
  uint32_t    MTextAlignment  = MIP_TEXT_ALIGN_LEFT;
  MIP_Color   MTextColor      = MIP_COLOR_BLACK;
  MIP_FRect   MTextOffset     = MIP_FRect(2,2,2,2);
  bool        MDrawText       = true;
  bool        MDrawParamName  = true;

//------------------------------
public:
//------------------------------

  MIP_TextWidget(MIP_FRect ARect, const char* AText="")
  : MIP_PanelWidget(ARect) {
    setName("MIP_TextWidget");
    setHint("text");
    MText = AText;
    MDrawBorder = false;
    MFillBackground = false;//true;
    //MBackgroundColor = MIP_COLOR_RED;
  }

  virtual ~MIP_TextWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setText(const char* AText)             { MText = AText; }
  virtual void setDrawText(bool ADraw)                { MDrawText = ADraw; }
  virtual void setDrawParamName(bool ADraw)           { MDrawParamName = ADraw; }
  virtual void setTextAlignment(uint32_t AAlignment)  { MTextAlignment = AAlignment; }
  virtual void setTextColor(MIP_Color AColor)         { MTextColor = AColor; }
  virtual void setTextOffset(MIP_FRect AOffset)       { MTextOffset = AOffset; }
  virtual void setTextOffset(float x)                 { MTextOffset = MIP_FRect(x,x,x,x); }
  virtual void setTextOffset(float x, float y)        { MTextOffset = MIP_FRect(x,y); }
  virtual void setTextSize(float ASize)               { MTextSize = ASize; }

  //----------

  virtual void drawText(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
    MIP_Parameter* param = getParameter();
    MIP_FRect rect = getRect();
    rect.shrink(MTextOffset);
    if (MDrawText) {
      const char* txt = MText;
      //if (MDrawParamName && (MParamIndex >= 0)) txt = MParamName;
      if (param && MDrawParamName) {
        txt = param->getName();
      }
      APainter->setFontSize(MTextSize);
      APainter->setColor(MTextColor);
      APainter->drawText(rect,txt,MTextAlignment);
    }
  }

//------------------------------
public:
//------------------------------

//  void attachWindow(MIP_Window* AWindow) override {
//    if (AWindow) {
//      if (flags.autoSize) {
//        MIP_Painter* painter = AWindow->getPainter();
//        if (painter) {
//          float w = painter->getTextWidth(MText) + 2;
//          float h = painter->getTextHeight(MText);// + 1;
//          setInitialSize(w,h);
//        }
//      }
//    }
//  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    //paintChildren(APainter,getRect(),AMode);
    drawText(APainter,ARect,AMode);
    drawBorder(APainter,ARect,AMode);
  }

//------------------------------

};

//----------------------------------------------------------------------
#endif

