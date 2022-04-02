#ifndef mip_menu_item_widget2_included
#define mip_menu_item_widget2_included
//----------------------------------------------------------------------

#if 0

#include "gui/widgets/v0/mip_text_widget.h"

#define MIP_MENU_NOTIFY_CLOSE2 1

class MIP_MenuItemWidget2
: public MIP_PanelWidget {

//------------------------------
private:
//------------------------------

  MIP_TextWidget*  MTextWidget = nullptr;

  const char* MText = "";
  MIP_Color   MTextColor = MIP_COLOR_BLACK;

  //MIP_Color  MNormalBackgroundColor  = MIP_COLOR_GRAY;
  //MIP_Color  MNormalTextColor        = MIP_COLOR_BLACK;
  //MIP_Color  MHoverBackgroundColor  = MIP_COLOR_DARK_GRAY;
  //MIP_Color  MHoverTextColor        = MIP_COLOR_WHITE;

//------------------------------
public:
//------------------------------

  MIP_MenuItemWidget2(MIP_FRect ARect)
  : MIP_PanelWidget(ARect) {
    setName("MIP_MenuItemWidget2");
    setHint("menuitem2");
    flags.autoCursor = false;
    setDrawBorder(false);
    setFillBackground(false);
    //setBackgroundColor(MNormalBackgroundColor);
    //setTextColor(MNormalTextColor);
    setCursor(MIP_CURSOR_FINGER);
  }

  //----------

  virtual ~MIP_MenuItemWidget2() {
  }

//------------------------------
public:
//------------------------------

//  virtual void setLabel(const char* AText) {
//    //MLabel = AText;
//    MIP_TextWidget* text = new MIP_TextWidget( MIP_FRect(60,20),AText );
//    //text->layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
//    appendWidget(text);
//  }

  virtual void setText(const char* AText) { MText = AText; }
  virtual void setTextColor(MIP_Color AColor) { MTextColor = AColor; }

  //virtual void setDrawText(bool ADraw)                { MDrawText = ADraw; }


//------------------------------
public:
//------------------------------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == MIP_BUTTON_LEFT) {
      do_widget_update(this);
    }
    if (AButton == MIP_BUTTON_RIGHT) {
      do_widget_notify(this,MIP_MENU_NOTIFY_CLOSE2);
    }
  }

  //----------

  void on_widget_mouseEnter(float AXpos, float AYpos, MIP_Widget* AFrom/*, uint32_t ATimeStamp=0*/) final {
    //MIsHovering = true;
    //setBackgroundColor(MHoverBackgroundColor);
    //setTextColor(MHoverTextColor);
    //do_widget_redraw(this,getRect(),0);
    do_widget_setMouseCursor(this,MIP_CURSOR_FINGER);
    if (flags.autoHint) do_widget_setHint(this,getHint(),0);
  }

  //----------

  void on_widget_mouseLeave(float AXpos, float AYpos, MIP_Widget* ATo/*, uint32_t ATimeStamp=0*/) final {
    //MIsHovering = false;
    //setBackgroundColor(MNormalBackgroundColor);
    //setTextColor(MNormalTextColor);
    //do_widget_redraw(this,getRect(),0);
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

#endif // 0



//----------------------------------------------------------------------
#endif
