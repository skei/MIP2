#ifndef mip_menu_item_widget_included
#define mip_menu_item_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widget.h"
#include "gui/widgets/mip_text_widget.h"

#define MIP_MENU_NOTIFY_CLOSE 1

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_MenuItemWidget
: public MIP_TextWidget {

//------------------------------
private:
//------------------------------

  MIP_Color  MNormalBackgroundColor  = MIP_COLOR_LIGHT_GRAY;
  MIP_Color  MNormalTextColor        = MIP_COLOR_BLACK;

  MIP_Color  MHoverBackgroundColor  = MIP_COLOR_DARK_GRAY;
  MIP_Color  MHoverTextColor        = MIP_COLOR_WHITE;

//------------------------------
public:
//------------------------------

  MIP_MenuItemWidget(MIP_FRect ARect, const char* AText="")
  : MIP_TextWidget(ARect,AText) {
    setName("MIP_MenuItemWidget");
    setHint("menuitem");
    flags.autoCursor = false;
    setDrawText(true);
    setDrawBorder(false);
    setFillBackground(true);
    setBackgroundColor(MNormalBackgroundColor);
    setTextColor(MNormalTextColor);
    setMouseCursor(MIP_CURSOR_FINGER);
  }

  //----------

  virtual ~MIP_MenuItemWidget() {
  }

//------------------------------
public:
//------------------------------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == MIP_BUTTON_LEFT) {
      do_widget_update(this);
    }
    if (AButton == MIP_BUTTON_RIGHT) {
      do_widget_notify(this,MIP_MENU_NOTIFY_CLOSE);
    }
  }

  //----------

  void on_widget_mouseEnter(float AXpos, float AYpos, MIP_Widget* AFrom/*, uint32_t ATimeStamp=0*/) final {
    //MIsHovering = true;
    setBackgroundColor(MHoverBackgroundColor);
    setTextColor(MHoverTextColor);
    do_widget_redraw(this,getRect(),0);
    do_widget_setMouseCursor(this,MIP_CURSOR_FINGER);
    if (flags.autoHint) do_widget_setHint(this,getHint(),0);
  }

  //----------

  void on_widget_mouseLeave(float AXpos, float AYpos, MIP_Widget* ATo/*, uint32_t ATimeStamp=0*/) final {
    //MIsHovering = false;
    setBackgroundColor(MNormalBackgroundColor);
    setTextColor(MNormalTextColor);
    do_widget_redraw(this,getRect(),0);
    do_widget_setMouseCursor(this,MIP_CURSOR_DEFAULT);
    if (flags.autoHint) do_widget_setHint(this,"",0);
  }

  //----------

//  void on_widget_connect(MIP_Parameter* AParameter, uint32_t ASubIndex) final {
//    //MIsHovering = false;
//    setBackgroundColor(MIP_COLOR_DARK_GRAY);
//    setTextColor(MIP_COLOR_LIGHT_GRAY);
//    do_widget_redraw(this,getRect(),0);
//  }

};

//----------------------------------------------------------------------
#endif
