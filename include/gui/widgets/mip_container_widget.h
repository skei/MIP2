#ifndef mip_container_widget_included
#define mip_container_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widget.h"

class MIP_ContainerWidget
: public MIP_Widget {

//------------------------------
private:
//------------------------------

  MIP_Widget*  MContent = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ContainerWidget(MIP_FRect ARect)
  : MIP_Widget(ARect) {
    setName("MIP_ContainerWidget");
    setHint("container");
    flags.visible = false;
    flags.active = false;
    MContent = new MIP_Widget(ARect);

  }

  virtual ~MIP_ContainerWidget() {
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

  MIP_Widget* appendWidget(MIP_Widget* AWidget) override {
    return MContent->appendWidget(AWidget);
  }

//------------------------------
public:
//------------------------------

  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) override {
    MIP_Widget::do_widget_update(ASender);
    //if (ASender == MTitleBar) {
    //  if (MClosable) {
    //    if (MTitleBar->getValue() >= 0.5f) open();
    //    else close();
    //  }
    //}
  }

  //----------

//  void do_widget_redraw(MIP_Widget* ASender, MIP_FRect ARect, uint32_t AMode) override {
//    MIP_Widget::do_widget_redraw(ASender,ARect,AMode);
//  }

//  void do_widget_moved(MIP_Widget* ASender, float ADeltaX=0.0f, float ADeltaY=0.0f) override {
//    MIP_Widget::do_widget_moved(ASender,ADeltaX,ADeltaY);
//  }

//  void do_widget_resized(MIP_Widget* ASender) override {
//    MIP_Widget::do_widget_resized(ASender);
//  }

//  void do_widget_grabMouseCursor(MIP_Widget* ASender) override {
//    MIP_Widget::do_widget_grabMouseCursor(ASender);
//  }

//  void do_widget_grabKeyboard(MIP_Widget* ASender) override {
//    MIP_Widget::do_widget_grabKeyboard(ASender);
//  }

//  void do_widget_grabModal(MIP_Widget* ASender) override {
//    MIP_Widget::do_widget_grabModal(ASender);
//  }

//  void do_widget_setMouseCursor(MIP_Widget* ASender, int32_t ACursor) override {
//    MIP_Widget::do_widget_setMouseCursor(ASender,ACursor);
//  }

//  void do_widget_setMouseCursorPos(MIP_Widget* ASender, float AXpos, float AYpos) override {
//    MIP_Widget::do_widget_setMouseCursorPos(ASender,AXpos,AYpos);
//  }

//  void do_widget_setHint(MIP_Widget* ASender, const char* AHint) override {
//    MIP_Widget::do_widget_setHint(ASender,AHint);
//  }

//  void do_widget_notify(MIP_Widget* AWidget, uint32_t AValue=0) override {
//    MIP_Widget::do_widget_notify(AWidget,AValue);
//  }

};

//----------------------------------------------------------------------
#endif

