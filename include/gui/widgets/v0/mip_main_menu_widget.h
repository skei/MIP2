#ifndef mip_main_menu_widget_included
#define mip_main_menu_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/v0/mip_menu_widget.h"
#include "gui/widgets/v0/mip_menu_item_widget.h"
//#include "gui/widgets/v0/mip_panel_widget.h"
#include "gui/widgets/v0/mip_button_widget.h"

//----------------------------------------------------------------------

typedef MIP_Array<MIP_MenuWidget*> MIP_MenuWidgets;

//----------

class MIP_MainMenuWidget
: public MIP_MenuWidget {

//------------------------------
private:
//------------------------------

  MIP_MenuWidgets MMenus = {};

//------------------------------
public:
//------------------------------

  MIP_MainMenuWidget(MIP_FRect ARect)
  : MIP_MenuWidget(ARect) {
    //setName("MIP_MainMenuWidget");
    //setHint("mainmenu");
    flags.active = true;
    flags.visible = true;
    layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    MBorderEdges = MIP_EDGE_BOTTOM;
    setItemLayout(10,1);

    MAlwaysOpen = true;

    //setListener(this);

    //setFillBackground(false);
    //setDrawBorder(false);
    //layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    //layout.innerBorder = MIP_FRect(10,10,10,10);


  }

  //----------

  virtual ~MIP_MainMenuWidget() {
    //#ifndef MIP_NO_AUTODELETE
    //for (uint32_t i=0; i<MMenus.size(); i++) {
    //  delete MMenus[i];
    //}
    //#endif
  }

//------------------------------
public: // menu listener
//------------------------------

  void on_menuEvent(int32_t AIndex) override {
    MIP_Print("submenu: %i\n",AIndex);
//    MIP_MenuWidget* menu = (MIP_MenuWidget*)MChildren[AIndex];
//    if (menu) {
//      float x = menu->getRect().x;
//      float x = menu->getRect().y;
//      menu->open(this,x,y);
//    }
  }

//------------------------------
public:
//------------------------------

  virtual uint32_t appendMenu(const char* ALabel, MIP_MenuWidget* AMenu) {
    uint32_t index = getNumChildren();
    //MMenus.append(AMenu);
    MIP_MenuItemWidget* item;
    item = new MIP_MenuItemWidget( MIP_FRect(50,20) );
    item->layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
    item->setText(ALabel);
    appendWidget(item);
    MMenus.append(AMenu);
    return index;
  }

//------------------------------
public:
//------------------------------


  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
    MIP_MenuWidget::on_widget_mouseClick(AXpos,AYpos,AButton,AState,ATimeStamp);
    if (AButton == MIP_BUTTON_LEFT) {
      //if (getRect().contains(AXpos,AYpos)) {
      //  MIP_Print("inside\n");
      //}
      //else {
      if (!getRect().contains(AXpos,AYpos)) {
        // left-clicked outside of widget
        MSelectedItem = MPrevSelected;
//        if (MListener) MListener->on_menuEvent(MSelectedItem);
//        if (!MAlwaysOpen) close();
      }
    }
    if (AButton == MIP_BUTTON_RIGHT) {
      // right-clicked
      MSelectedItem = MPrevSelected;
//      if (MListener) MListener->on_menuEvent(MSelectedItem);
//      if (!MAlwaysOpen) close();
    }
  }

//------------------------------
public:
//------------------------------

  // item selected

  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) override {
    int32_t index = ASender->getWidgetIndex();
    MSelectedItem = index;
//    if (MListener) MListener->on_menuEvent(MSelectedItem);
//    if (!MAlwaysOpen) close();

    MIP_Print("menu %i\n",index);

    MIP_FRect rect = MChildren[index]->getRect();
    MIP_MenuWidget* menu = MMenus[index];
    if (menu) {
      float x = rect.x;
      float y = rect.y + rect.h;
      menu->open(this,x,y);
    }

    MIP_MenuWidget::do_widget_update(this,AMode);
    //MIP_Widget::do_widget_update(ASender);
  }

  //----------

  // (right button clicked)

  void do_widget_notify(MIP_Widget* AWidget, uint32_t AValue=0) override {
    if (AValue == MIP_MENU_NOTIFY_CLOSE) {
      // right clicked on menuitem
      MSelectedItem = MPrevSelected;

//      if (MListener) MListener->on_menuEvent(MSelectedItem);
//      if (!MAlwaysOpen) close();

    }
    //MIP_Widget::do_widget_notify(AWidget,AValue);
  }


};

//----------------------------------------------------------------------
#endif

