#ifndef mip_menu_widget2_included
#define mip_menu_widget2_included
//----------------------------------------------------------------------

#include "gui/widgets/mip_panel_widget.h"
#include "gui/widgets/mip_menu_item_widget2.h"

#define MIP_MENU_MAIN      0
#define MIP_MENU_DROPDOWN  1

//----------------------------------------------------------------------

//class MIP_MenuListener2 {
//  public:
//    virtual void on_menuEvent(int32_t AIndex) {}
//};

//----------------------------------------------------------------------

class MIP_MenuWidget2
: public MIP_PanelWidget
/*, public MIP_MenuListener2*/ {

//------------------------------
protected:
//------------------------------

  int32_t   MSelectedItem   = -1;
  int32_t   MPrevSelected   = -1;

//------------------------------
public:
//------------------------------

  MIP_MenuWidget2(MIP_FRect ARect)
  : MIP_PanelWidget(ARect) {

    setName("MIP_MenuWidget");
    setHint("menu");
    flags.active = true;
    flags.visible = true;
    layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    setDrawBorder(false);
    setFillBackground(false);
    //setBackgroundColor(MIP_COLOR_LIGHT_GRAY);
  }

  //----------

  virtual ~MIP_MenuWidget2() {
  }

//------------------------------
public:
//------------------------------

  virtual int32_t getSelectedItem() {
    return MSelectedItem;
  }

  virtual uint32_t getNumItems() {
    return getNumChildren();
  }
//
  virtual MIP_Widget* getItem(uint32_t AIndex) {
    return getChild(AIndex);
  }

//------------------------------
public:
//------------------------------

  virtual void open(int32_t AXpos, int32_t AYpos) {
  }

  virtual void close() {
  }

//------------------------------
public:
//------------------------------

  void alignWidgets(bool ARecursive=true) override {
    MIP_PanelWidget::alignWidgets(ARecursive);
  }

//------------------------------
public:
//------------------------------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    MIP_PanelWidget::on_widget_mouseClick(AXpos,AYpos,AButton,AState,ATimeStamp);
    if (AButton == MIP_BUTTON_LEFT) {
      if (!getRect().contains(AXpos,AYpos)) {
        // left-clicked outside of widget
        MSelectedItem = MPrevSelected;
//        if (MListener) MListener->on_menuEvent(MSelectedItem);
        close();
      }
    }
    if (AButton == MIP_BUTTON_RIGHT) {
      // right-clicked
      MSelectedItem = MPrevSelected;
//      if (MListener) MListener->on_menuEvent(MSelectedItem);
      close();
    }
  }

//------------------------------
public:
//------------------------------

  // item selected

  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) final {
    int32_t index = ASender->getWidgetIndex();
    MSelectedItem = index;
//    if (MListener) MListener->on_menuEvent(MSelectedItem);
    close();
    MIP_PanelWidget::do_widget_update(this);
  }

  //----------

  // (right button clicked)

  void do_widget_notify(MIP_Widget* AWidget, uint32_t AValue=0) final {
    if (AValue == MIP_MENU_NOTIFY_CLOSE) {
      // right clicked on menuitem
      MSelectedItem = MPrevSelected;
//      if (MListener) MListener->on_menuEvent(MSelectedItem);
      close();
    }
    else {
      MIP_PanelWidget::do_widget_notify(AWidget,AValue);
    }
  }

};

//----------------------------------------------------------------------
#endif
