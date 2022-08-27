#ifndef mip_main_menu_widget_included
#define mip_main_menu_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/mip_menu_widget.h"
#include "gui/widgets/mip_menu_item_widget.h"
//#include "gui/widgets/mip_panel_widget.h"
#include "gui/widgets/mip_button_widget.h"


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_MainMenuWidget
: public MIP_PanelWidget {
//, public MIP_MenuListener {

//------------------------------
private:
//------------------------------

  MIP_CharPtrArray  MMenus            = {};
  int32_t           MSelectedMainMenu = -1;
  MIP_MenuListener* MListener         = nullptr;

//  int32_t MSelectedItem = -1;
//  int32_t MPrevSelected = -1;

//------------------------------
public:
//------------------------------

  MIP_MainMenuWidget(MIP_FRect ARect, MIP_MenuListener* AListener=nullptr)
  : MIP_PanelWidget(ARect) {
    setName("MIP_MainMenuWidget");
    setHint("mainmenu");
    MListener = AListener;
    //flags.active = true;
    //flags.visible = true;
    MFillBackground = true;
    MBackgroundColor = MIP_COLOR_LIGHT_GRAY;
    MDrawBorder = true;
    MBorderColor = MIP_COLOR_DARK_GRAY;
    MBorderEdges = MIP_EDGE_BOTTOM;
    layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    layout.innerBorder = MIP_FRect(5,0,5,0);
    layout.spacing = MIP_FPoint(5,0);
    //setItemSize(40,16);
    //setItemLayout(10,1);
    //setAlwaysOpen();
  }

  //----------

  virtual ~MIP_MainMenuWidget() {
  }

//------------------------------
public:
//------------------------------

  //TODO

  virtual void appendMenu(const char* AText, MIP_MenuWidget* AMenu) {
    MIP_MenuItemWidget* menuitem = new MIP_MenuItemWidget(MIP_FRect(32,16));
    menuitem->setText(AText);
    menuitem->layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT_CENTER;
    appendWidget(menuitem);
  }

//------------------------------
public:
//------------------------------

//  virtual uint32_t appendMenu(const char* ALabel, MIP_MenuWidget* AMenu) {
//    uint32_t index = getNumChildren();
//    MIP_MenuItemWidget* item;
//    item = new MIP_MenuItemWidget( MIP_FRect(MItemWidth,MItemHeight) );
//    item->layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
//    item->setText(ALabel);
//    appendWidget(item);
//    MMenus.append(AMenu);
//    return index;
//  }

//------------------------------
public: // menu listener
//------------------------------

//  void on_menuEvent(int32_t AIndex) override {
//    int32_t index = (MSelectedMainMenu * 1000) + AIndex;
//    if (MListener) MListener->on_menuEvent(index);
//  }

//------------------------------
public:
//------------------------------


//  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
//    //MIP_MenuWidget::on_widget_mouseClick(AXpos,AYpos,AButton,AState,ATimeStamp);
//    if (AButton == MIP_BUTTON_LEFT) {
//      if (!getRect().contains(AXpos,AYpos)) {
//        // left-clicked outside of widget
//        MSelectedItem = MPrevSelected;
//        //if (MListener) MListener->on_menuEvent(MSelectedItem);
//        //if (!MAlwaysOpen) close();
//      }
//    }
//    if (AButton == MIP_BUTTON_RIGHT) {
//      // right-clicked
//      MSelectedItem = MPrevSelected;
//      //if (MListener) MListener->on_menuEvent(MSelectedItem);
//      //if (!MAlwaysOpen) close();
//    }
//  }

//------------------------------
public:
//------------------------------

  // item selected

//  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) override {
//    int32_t index = ASender->getWidgetIndex();
//    MSelectedItem = index;
//    MSelectedMainMenu = index;
//    MIP_FRect rect = MChildren[index]->getRect();
//    MIP_MenuWidget* menu = MMenus[index];
//    if (menu) {
//      float x = rect.x;
//      float y = rect.y + rect.h;
//      menu->open(this,x,y);
//    }
//    //MIP_PanelWidget::do_widget_update(this,AMode);
//  }

  //----------

//  void do_widget_notify(MIP_Widget* AWidget, uint32_t AValue=0) override {
//    if (AValue == MIP_MENU_NOTIFY_CLOSE) {
//      // right clicked on menuitem
//      MSelectedItem = MPrevSelected;
//      //if (MListener) MListener->on_menuEvent(MSelectedItem);
//      //if (!MAlwaysOpen) close();
//    }
//    //MIP_MenuWidget::do_widget_notify(AWidget,AValue);
//  }


};

//----------------------------------------------------------------------
#endif

