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

  bool        MDrawText       = true;
  char        MText[256]      = {0};
  MIP_Color   MTextColor      = MIP_Color(0.25);//MIP_COLOR_DARK_GRAY;
  uint32_t    MTextAlignment  = MIP_TEXT_ALIGN_CENTER;
  double      MTextSize       = 13.0;
  MIP_DPoint  MTextOffset     = MIP_DPoint(0,0);

//------------------------------
public:
//------------------------------

  MIP_TextWidget(MIP_DRect ARect, const char* AText)
  : MIP_PanelWidget(ARect) {
    MName = "MIP_TextWidget";
    setDrawRoundedCorners(false);
    strcpy(MText,AText);
    Options.autoSize = true;
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
  virtual void  setTextSize(double ASize)          { MTextSize = ASize; }
  virtual void  setTextOffset(MIP_DPoint AOffset) { MTextOffset = AOffset; }

  virtual const char* getText() { return MText; }

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

  virtual void updateTextRect(MIP_Painter* APainter) {
    if (APainter) {
      if (Options.autoSize) {
        float bounds[4];
        double textsize = MTextSize;
        if (textsize < 0) textsize = MRect.h * (- textsize);
        APainter->fontSize(textsize);
        float width = 0;
        float height = 0;
        if (MText) {
          APainter->textBounds(MRect.x,MRect.y,MText,nullptr,bounds);
          width  = bounds[2] - bounds[0];
          height = bounds[3] - bounds[1];
        }
        Layout.baseRect.w = width;
        Layout.baseRect.h = height;
      }
    }
  }

  //----------

  virtual void drawText(MIP_PaintContext* AContext) {
    if (MDrawText) {
      MIP_Painter* painter = AContext->painter;
      MIP_DRect rect = MRect;
      if (MRect.isEmpty()) return;
      //rect.shrink(2);
      rect.x += MTextOffset.x;
      rect.y += MTextOffset.y;
      double textsize = MTextSize;
      if (MTextSize < 0) { textsize = MRect.h * (- MTextSize); }
      painter->fontSize(textsize);
      painter->beginPath();
      painter->drawTextBox(rect,MText,MTextAlignment,MTextColor);
    }
  }

};

//----------------------------------------------------------------------
#endif


