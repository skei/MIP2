#ifndef mip_text_widget_included
#define mip_text_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets/mip_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_TextWidget
: public MIP_PanelWidget {

//------------------------------
protected:
//------------------------------

  bool      MDrawText       = true;
  char      MText[256]      = {0};
  MIP_Color MTextColor      = MIP_COLOR_DARK_GRAY;
  uint32_t  MTextAlignment  = MIP_TEXT_ALIGN_CENTER;
  float     MTextSize       = 13.0;

//------------------------------
public:
//------------------------------

  MIP_TextWidget(MIP_DRect ARect, const char* AText)
  : MIP_PanelWidget(ARect) {
    MName = "MIP_TextWidget";
    setDrawRoundedCorners(false);
    strcpy(MText,AText);
  }

  //----------

  virtual ~MIP_TextWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void  setDrawText(bool ADraw=true)      { MDrawText = ADraw; }
  virtual void  setText(const char* AText)        { strcpy(MText,AText); }
  virtual void  setTextColor(MIP_Color AColor)    { MTextColor = AColor; }
  virtual void  setTextAlignment(uint32_t AAlign) { MTextAlignment = AAlign; }
  virtual void  setTextSize(float ASize)          { MTextSize = ASize; }

//------------------------------
public: // parent to child
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    fillBackground(AContext);
    paintChildWidgets(AContext);
    drawText(AContext);
    drawBorder(AContext);
  }

//------------------------------
public:
//------------------------------

  virtual void drawText(MIP_PaintContext* AContext) {
    if (MDrawText) {
      MIP_Painter* painter = AContext->painter;
      MIP_DRect rect = MRect;
      rect.shrink(2);
      painter->fontSize(MTextSize);
      painter->beginPath();
      painter->drawTextBox(rect,MText,MTextAlignment,MTextColor);
    }
  }

};

//----------------------------------------------------------------------
#endif


