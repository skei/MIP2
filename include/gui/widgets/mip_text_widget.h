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
  MIP_Color   MTextColor      = MIP_COLOR_DARK_GRAY;
  uint32_t    MTextAlignment  = MIP_TEXT_ALIGN_CENTER;
  float       MTextSize       = 13.0;
  MIP_DPoint  MTextOffset     = MIP_DPoint(0,0);

  MIP_Window* MOwnerWindow    = nullptr;

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
  virtual void  setTextOffset(MIP_DPoint AOffset) { MTextOffset = AOffset; }

  virtual const char* getText() { return MText; }

//------------------------------
public: // parent to child
//------------------------------

  /*
  void prepareForAlignment() override {
    MIP_PRINT;
    MIP_Window* window = (MIP_Window*)getOwnerWindow();
    if (window) {
      MIP_Painter* painter = window->getPainter();
      if (painter) {
        float bounds[4];;
        painter->textBounds(MRect.x,MRect.y,MText,nullptr,bounds);
        float xmin = bounds[0];
        //float ymin = bounds[1];
        float xmax = bounds[2];
        //float ymax = bounds[3];
        float width = xmax - xmin;
        MRect.w = width;
      }
    }
  }
  */

//------------------------------
public: // parent to child
//------------------------------

  void on_widget_config(MIP_Widget* AOwnerWindow) override {
    MOwnerWindow = (MIP_Window*)AOwnerWindow;
    MIP_PanelWidget::on_widget_config(AOwnerWindow);
  }

  //----------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    fillBackground(AContext);
    paintChildWidgets(AContext);
    drawText(AContext);
    drawBorder(AContext);
  }

//------------------------------
public:
//------------------------------

  // opening/resizing window, realignment,
  // widget with autoSize flag calls painter

  virtual void updateTextSize() {
    if (Options.autoSize) {
      /*
      MIP_Painter* painter = MOwnerWindow->getPainter();
      if (painter) {
        float bounds[4];;
        double textsize = MTextSize;
        if (textsize < 0) textsize = MRect.h * (- textsize);
        painter->fontSize(textsize);
        float width = 0;
        float height = 0;
        if (MText) {
          painter->textBounds(MRect.x,MRect.y,MText,nullptr,bounds);
          width  = bounds[2] - bounds[0];
          height = bounds[3] - bounds[1];
        }
        Layout.baseRect.w = width;
        Layout.baseRect.h = height;
      }
      */
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
      if (MTextSize < 0) {
        textsize = MRect.h * (- MTextSize);
      }
      painter->fontSize(textsize);
      painter->beginPath();
      painter->drawTextBox(rect,MText,MTextAlignment,MTextColor);
    }
  }

};

//----------------------------------------------------------------------
#endif


