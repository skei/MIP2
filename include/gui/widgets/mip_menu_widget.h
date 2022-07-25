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
    //Flags.active = false;
    //Flags.visible = false;
  }

  //----------

  virtual ~MIP_MenuWidget() {
  }

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
public: // child to parent
//------------------------------

  void do_widget_notify(MIP_Widget* ASender, uint32_t AMode, int32_t AValue) override {
    //if (MParent) MParent->do_widget_update(ASender,AMode);
    if (ASender) {
      MIP_MenuItemWidget* menu_item = (MIP_MenuItemWidget*)ASender;
      int32_t index = AValue;//ASender->getWidgetIndex();
      if (index < 0) {
        MIP_Print("---\n");
      }
      else {
        const char* name = menu_item->getText();
        MIP_Print("%i (%s)\n",index,name);
      }
      do_widget_modal(nullptr);
    }
  }

//------------------------------
public:
//------------------------------

  virtual void open(double AXpos, double AYpos/*, uint32_t AMode=0*/) {
    Flags.visible = true;
    Flags.active = true;
    do_widget_modal(this);
  }

  //----------

  virtual void close(double AXpos, double AYpos/*, uint32_t AMode=0*/) {
    Flags.visible = false;
    Flags.active = false;
    //do_widget_update(this);
    do_widget_modal(nullptr);
  }

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



