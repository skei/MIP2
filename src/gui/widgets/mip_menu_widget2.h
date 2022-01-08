#ifndef kode_menu_widget2_included
#define kode_menu_widget2_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_panel_widget.h"
#include "gui/widgets/kode_menu_item_widget2.h"

#define KODE_MENU_MAIN      0
#define KODE_MENU_DROPDOWN  1

//----------------------------------------------------------------------

//class KODE_MenuListener2 {
//  public:
//    virtual void on_menuEvent(int32_t AIndex) {}
//};

//----------------------------------------------------------------------

class KODE_MenuWidget2
: public KODE_PanelWidget
/*, public KODE_MenuListener2*/ {

//------------------------------
protected:
//------------------------------

  int32_t   MSelectedItem   = -1;
  int32_t   MPrevSelected   = -1;

//------------------------------
public:
//------------------------------

  KODE_MenuWidget2(KODE_FRect ARect)
  : KODE_PanelWidget(ARect) {

    setName("KODE_MenuWidget");
    setHint("menu");
    flags.active = true;
    flags.visible = true;
    layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    setDrawBorder(false);
    setFillBackground(false);
    //setBackgroundColor(KODE_COLOR_LIGHT_GRAY);
  }

  //----------

  virtual ~KODE_MenuWidget2() {
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
  virtual KODE_Widget* getItem(uint32_t AIndex) {
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

  void alignChildren(bool ARecursive=true) override {
    KODE_PanelWidget::alignChildren(ARecursive);
  }

//------------------------------
public:
//------------------------------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    KODE_PanelWidget::on_widget_mouseClick(AXpos,AYpos,AButton,AState);
    if (AButton == KODE_BUTTON_LEFT) {
      if (!getRect().contains(AXpos,AYpos)) {
        // left-clicked outside of widget
        MSelectedItem = MPrevSelected;
//        if (MListener) MListener->on_menuEvent(MSelectedItem);
        close();
      }
    }
    if (AButton == KODE_BUTTON_RIGHT) {
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

  void do_widget_update(KODE_Widget* ASender) final {
    int32_t index = ASender->getIndex();
    MSelectedItem = index;
//    if (MListener) MListener->on_menuEvent(MSelectedItem);
    close();
    KODE_PanelWidget::do_widget_update(this);
  }

  //----------

  // (right button clicked)

  void do_widget_notify(KODE_Widget* AWidget, uint32_t AValue=0) final {
    if (AValue == KODE_MENU_NOTIFY_CLOSE) {
      // right clicked on menuitem
      MSelectedItem = MPrevSelected;
//      if (MListener) MListener->on_menuEvent(MSelectedItem);
      close();
    }
    else {
      KODE_PanelWidget::do_widget_notify(AWidget,AValue);
    }
  }

};

//----------------------------------------------------------------------
#endif
