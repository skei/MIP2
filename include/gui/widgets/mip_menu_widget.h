#ifndef mip_menu_widget_included
#define mip_menu_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets/mip_panel_widget.h"
#include "gui/widgets/mip_menu_item_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_MenuWidget
: public MIP_PanelWidget {

//------------------------------
protected:
//------------------------------

//------------------------------
public:
//------------------------------

  MIP_MenuWidget(MIP_DRect ARect)
  : MIP_PanelWidget(ARect) {
    MName = "MIP_MenuWidget";
  }

  //----------

  virtual ~MIP_MenuWidget() {
  }

//------------------------------
public:
//------------------------------

//------------------------------
public: // parent to child
//------------------------------

  //void on_widget_paint(MIP_PaintContext* AContext) override {
  //  fillBackground(AContext);
  //  paintChildWidgets(AContext);
  //  drawMenu(AContext);
  //  drawBorder(AContext);
  //}

//------------------------------
public:
//------------------------------

  //virtual void drawMenu(MIP_PaintContext* AContext) {
  //  MIP_Painter* painter = AContext->painter;
  //  MIP_DRect rect = MRect;
  //  rect.shrink(2);
  //  painter->fontSize(MTextSize);
  //  painter->beginPath();
  //  painter->drawTextBox(rect,MText,MTextAlignment,MTextColor);
  //}

};

//----------------------------------------------------------------------
#endif



