#ifndef mip_menu_item_widget_included
#define mip_menu_item_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets/mip_text_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_MenuItemWidget
: public MIP_TextWidget {

//------------------------------
protected:
//------------------------------

  MIP_Color MHighlightBackgroundColor = MIP_COLOR_DARK_GRAY;
  MIP_Color MHighlightTextColor       = MIP_COLOR_LIGHT_GRAY;
  MIP_Color MNormalBackgroundColor    = MBackgroundColor;
  MIP_Color MNormalTextColor          = MTextColor;

//------------------------------
public:
//------------------------------

  MIP_MenuItemWidget(MIP_DRect ARect, const char* AText)
  : MIP_TextWidget(ARect,AText) {
    MName = "MIP_MenuItemWidget";
    MDrawBorder = false;
    MFillBackground = true;
    MFillGradient = false;
    MNormalBackgroundColor = MBackgroundColor;
    MNormalTextColor = MTextColor;
    MMouseCursor = MIP_CURSOR_FINGER;
  }

  //----------

  virtual ~MIP_MenuItemWidget() {
  }

//------------------------------
public:
//------------------------------


//------------------------------
public: // parent to child
//------------------------------

  //void on_widget_paint(MIP_PaintContext* AContext) override {
  //  MIP_TextWidget::on_widget_paint(AContext);
  //}

  //----------

  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    switch (AButton) {
      case MIP_BUTTON_LEFT:
        do_widget_notify(this,0,MIndex);
        break;
      case MIP_BUTTON_RIGHT:
        do_widget_notify(this,0,-1);
        break;
    }
    //do_widget_update(this);
  }

  //----------

  void on_widget_enter(MIP_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) override {
    MIP_TextWidget::on_widget_enter(AFrom,AXpos,AYpos,ATime);
    MNormalBackgroundColor = MBackgroundColor;
    MNormalTextColor = MTextColor;
    MBackgroundColor = MHighlightBackgroundColor;
    MTextColor = MHighlightTextColor;
    do_widget_redraw(this);
  }

  //----------

  void on_widget_leave(MIP_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) override {
    MIP_TextWidget::on_widget_leave(ATo,AXpos,AYpos,ATime);
    MBackgroundColor = MNormalBackgroundColor;
    MTextColor = MNormalTextColor;
    do_widget_redraw(this);
  }


//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif
