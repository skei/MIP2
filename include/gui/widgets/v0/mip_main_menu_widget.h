#ifndef mip_main_menu_widget_included
#define mip_main_menu_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/v0/mip_menu_widget.h"
#include "gui/widgets/v0/mip_menu_item_widget.h"
//#include "gui/widgets/v0/mip_panel_widget.h"
#include "gui/widgets/v0/mip_button_widget.h"

typedef MIP_Array<MIP_MenuWidget*> MIP_MenuWidgets;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_MainMenuWidget
: public MIP_MenuWidget {

//------------------------------
private:
//------------------------------

  MIP_MenuWidgets MMenus            = {};
  int32_t         MSelectedMainMenu = -1;

//------------------------------
public:
//------------------------------

  MIP_MainMenuWidget(MIP_FRect ARect, MIP_MenuListener* AListener=nullptr)
  : MIP_MenuWidget(ARect) {
    setName("MIP_MainMenuWidget");
    setHint("mainmenu");
    MListener = AListener;
    flags.active = true;
    flags.visible = true;
    layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    MBorderEdges = MIP_EDGE_BOTTOM;
    setItemSize(40,16);
    setItemLayout(10,1);
    //layout.innerBorder = MIP_FRect(5,0,5,0);
    setAlwaysOpen();
  }

  //----------

  virtual ~MIP_MainMenuWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual uint32_t appendMenu(const char* ALabel, MIP_MenuWidget* AMenu) {
    uint32_t index = getNumChildren();
    MIP_MenuItemWidget* item;
    item = new MIP_MenuItemWidget( MIP_FRect(MItemWidth,MItemHeight) );
    item->layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
    item->setText(ALabel);
    appendWidget(item);
    MMenus.append(AMenu);
    return index;
  }

//------------------------------
public: // menu listener
//------------------------------

  void on_menuEvent(int32_t AIndex) override {
    int32_t index = (MSelectedMainMenu * 1000) + AIndex;
    if (MListener) MListener->on_menuEvent(index);
  }

//------------------------------
public:
//------------------------------


  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
    MIP_MenuWidget::on_widget_mouseClick(AXpos,AYpos,AButton,AState,ATimeStamp);
    if (AButton == MIP_BUTTON_LEFT) {
      if (!getRect().contains(AXpos,AYpos)) {
        // left-clicked outside of widget
        MSelectedItem = MPrevSelected;
        //if (MListener) MListener->on_menuEvent(MSelectedItem);
        //if (!MAlwaysOpen) close();
      }
    }
    if (AButton == MIP_BUTTON_RIGHT) {
      // right-clicked
      MSelectedItem = MPrevSelected;
      //if (MListener) MListener->on_menuEvent(MSelectedItem);
      //if (!MAlwaysOpen) close();
    }
  }

//------------------------------
public:
//------------------------------

  // item selected

  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) override {
    int32_t index = ASender->getWidgetIndex();
    MSelectedItem = index;
    MSelectedMainMenu = index;
    MIP_FRect rect = MChildren[index]->getRect();
    MIP_MenuWidget* menu = MMenus[index];
    if (menu) {
      float x = rect.x;
      float y = rect.y + rect.h;
      menu->open(this,x,y);
    }
    //MIP_PanelWidget::do_widget_update(this,AMode);
  }

  //----------

  void do_widget_notify(MIP_Widget* AWidget, uint32_t AValue=0) override {
    if (AValue == MIP_MENU_NOTIFY_CLOSE) {
      // right clicked on menuitem
      MSelectedItem = MPrevSelected;
      //if (MListener) MListener->on_menuEvent(MSelectedItem);
      //if (!MAlwaysOpen) close();
    }
    //MIP_MenuWidget::do_widget_notify(AWidget,AValue);
  }


};

//----------------------------------------------------------------------
#endif

