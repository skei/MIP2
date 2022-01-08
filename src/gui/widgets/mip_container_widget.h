#ifndef kode_container_widget_included
#define kode_container_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"

class KODE_ContainerWidget
: public KODE_Widget {

//------------------------------
private:
//------------------------------

  KODE_Widget*  MContent = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_ContainerWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    setName("KODE_ContainerWidget");
    setHint("container");
    flags.visible = false;
    flags.active = false;
    MContent = new KODE_Widget(ARect);

  }

  virtual ~KODE_ContainerWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void showContainer(void) {
    if (!MContent->flags.visible) {
      MContent->flags.active = true;
      MContent->flags.visible = true;
//      setHeight(MOpenSize);
//      setInitialHeight(MOpenSize);
//      do_widget_resized(this);
    }
  }

  //----------

  virtual void hideContainer(void) {
    if (MContent->flags.visible) {
      MContent->flags.active = false;
      MContent->flags.visible = false;
//      setHeight(MClosedSize);
//      setInitialHeight(MClosedSize);
//      do_widget_resized(this);
    }
  }

//------------------------------
public:
//------------------------------

  KODE_Widget* appendWidget(KODE_Widget* AWidget) override {
    return MContent->appendWidget(AWidget);
  }

//------------------------------
public:
//------------------------------

  void do_widget_update(KODE_Widget* ASender) override {
    KODE_Widget::do_widget_update(ASender);
    //if (ASender == MTitleBar) {
    //  if (MClosable) {
    //    if (MTitleBar->getValue() >= 0.5f) open();
    //    else close();
    //  }
    //}
  }

  //----------

//  void do_widget_redraw(KODE_Widget* ASender, KODE_FRect ARect, uint32_t AMode) override {
//    KODE_Widget::do_widget_redraw(ASender,ARect,AMode);
//  }

//  void do_widget_moved(KODE_Widget* ASender, float ADeltaX=0.0f, float ADeltaY=0.0f) override {
//    KODE_Widget::do_widget_moved(ASender,ADeltaX,ADeltaY);
//  }

//  void do_widget_resized(KODE_Widget* ASender) override {
//    KODE_Widget::do_widget_resized(ASender);
//  }

//  void do_widget_grabMouseCursor(KODE_Widget* ASender) override {
//    KODE_Widget::do_widget_grabMouseCursor(ASender);
//  }

//  void do_widget_grabKeyboard(KODE_Widget* ASender) override {
//    KODE_Widget::do_widget_grabKeyboard(ASender);
//  }

//  void do_widget_grabModal(KODE_Widget* ASender) override {
//    KODE_Widget::do_widget_grabModal(ASender);
//  }

//  void do_widget_setMouseCursor(KODE_Widget* ASender, int32_t ACursor) override {
//    KODE_Widget::do_widget_setMouseCursor(ASender,ACursor);
//  }

//  void do_widget_setMouseCursorPos(KODE_Widget* ASender, float AXpos, float AYpos) override {
//    KODE_Widget::do_widget_setMouseCursorPos(ASender,AXpos,AYpos);
//  }

//  void do_widget_setHint(KODE_Widget* ASender, const char* AHint) override {
//    KODE_Widget::do_widget_setHint(ASender,AHint);
//  }

//  void do_widget_notify(KODE_Widget* AWidget, uint32_t AValue=0) override {
//    KODE_Widget::do_widget_notify(AWidget,AValue);
//  }

};

//----------------------------------------------------------------------
#endif

