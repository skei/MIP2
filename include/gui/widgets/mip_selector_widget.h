#ifndef mip_selector_widget_included
#define mip_selector_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets/mip_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_SelectorWidget
: public MIP_TextWidget {

//------------------------------
protected:
//------------------------------

  bool            MDrawArrow  = true;
  MIP_Color       MArrowColor = MIP_COLOR_DARK_GRAY;
  MIP_MenuWidget* MMenu       = nullptr;

//------------------------------
public:
//------------------------------

  MIP_SelectorWidget(MIP_DRect ARect, const char* AText, MIP_MenuWidget* AMenu)
  : MIP_TextWidget(ARect,AText) {
    MName = "MIP_SelectorWidget";
    MMenu = AMenu;
    MMouseCursor = MIP_CURSOR_FINGER;
  }

  //----------

  virtual ~MIP_SelectorWidget() {
  }

//------------------------------
public: // parent to child
//------------------------------

  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    if (AButton == MIP_BUTTON_LEFT) {
      MMenu->open(AXpos,AYpos,true);
    }
  }

  //----------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    fillBackground(AContext);
    paintChildWidgets(AContext);
    drawText(AContext);
    drawArrow(AContext);
    drawBorder(AContext);
  }



//------------------------------
public:
//------------------------------

  virtual void drawArrow(MIP_PaintContext* AContext) {
    if (MDrawArrow) {
      MIP_Painter* painter = AContext->painter;
      //MIP_DRect rect = MRect;
      //rect.shrink(2);
      //painter->fontSize(MTextSize);
      double x1 = MRect.x2() - 5 - 6;
      double x2 = x1 + 3;
      double x3 = x1 + 6;
      double y1 = MRect.y2() - 5 - 5;
      double y2 = y1 + 5;
      painter->beginPath();
      //painter->drawTextBox(rect,MText,MTextAlignment,MTextColor);
      painter->moveTo(x1,y1);
      painter->lineTo(x3,y1);
      painter->lineTo(x2,y2);
      painter->closePath();
      painter->fillColor(MArrowColor);
      painter->fill();
    }
  }

};

//----------------------------------------------------------------------
#endif



