#ifndef kode_menu_item_widget2_included
#define kode_menu_item_widget2_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_text_widget.h"

#define KODE_MENU_NOTIFY_CLOSE2 1

class KODE_MenuItemWidget2
: public KODE_PanelWidget {

//------------------------------
private:
//------------------------------

  KODE_TextWidget*  MTextWidget = KODE_NULL;

  //KODE_Color  MNormalBackgroundColor  = KODE_COLOR_GRAY;
  //KODE_Color  MNormalTextColor        = KODE_COLOR_BLACK;
  //KODE_Color  MHoverBackgroundColor  = KODE_COLOR_DARK_GRAY;
  //KODE_Color  MHoverTextColor        = KODE_COLOR_WHITE;

//------------------------------
public:
//------------------------------

  KODE_MenuItemWidget2(KODE_FRect ARect)
  : KODE_PanelWidget(ARect) {
    setName("KODE_MenuItemWidget2");
    setHint("menuitem2");
    flags.autoCursor = false;
    setDrawBorder(false);
    setFillBackground(false);
    //setBackgroundColor(MNormalBackgroundColor);
    //setTextColor(MNormalTextColor);
    setCursor(KODE_CURSOR_FINGER);
  }

  //----------

  virtual ~KODE_MenuItemWidget2() {
  }

//------------------------------
public:
//------------------------------

//  virtual void setLabel(const char* AText) {
//    //MLabel = AText;
//    KODE_TextWidget* text = new KODE_TextWidget( KODE_FRect(60,20),AText );
//    //text->layout.alignment = KODE_WIDGET_ALIGN_FILL_LEFT;
//    appendWidget(text);
//  }

//------------------------------
public:
//------------------------------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == KODE_BUTTON_LEFT) {
      do_widget_update(this);
    }
    if (AButton == KODE_BUTTON_RIGHT) {
      do_widget_notify(this,KODE_MENU_NOTIFY_CLOSE);
    }
  }

  //----------

  void on_widget_enter(float AXpos, float AYpos, KODE_Widget* AFrom, uint32_t ATimeStamp=0) final {
    //MIsHovering = true;
    //setBackgroundColor(MHoverBackgroundColor);
    //setTextColor(MHoverTextColor);
    //do_widget_redraw(this,getRect(),0);
    do_widget_setMouseCursor(this,KODE_CURSOR_FINGER);
    if (flags.autoHint) do_widget_setHint(this,getHint());
  }

  //----------

  void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo, uint32_t ATimeStamp=0) final {
    //MIsHovering = false;
    //setBackgroundColor(MNormalBackgroundColor);
    //setTextColor(MNormalTextColor);
    //do_widget_redraw(this,getRect(),0);
    do_widget_setMouseCursor(this,KODE_CURSOR_DEFAULT);
    if (flags.autoHint) do_widget_setHint(this,"");
  }

  //----------

  //void on_widget_connect(KODE_Parameter* AParameter, uint32_t ASubIndex) final {
  //  //MIsHovering = false;
  //  setBackgroundColor(KODE_COLOR_DARK_GRAY);
  //  setTextColor(KODE_COLOR_LIGHT_GRAY);
  //  do_widget_redraw(this,getRect(),0);
  //}

};

//----------------------------------------------------------------------
#endif
