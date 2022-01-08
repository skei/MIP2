#ifndef mip_template_widget_included
#define mip_template_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widget.h"

class MIP_TemplateWidget
: public MIP_Widget {

//------------------------------
public:
//------------------------------

  MIP_TemplateWidget(MIP_FRect ARect)
  : MIP_Widget(ARect) {
    setName("MIP_TemplateWidget");
    setHint("template");
  }

  virtual ~MIP_TemplateWidget() {
  }

//------------------------------
public:
//------------------------------

  void on_widget_move(float AXpos, float AYpos) override {
  }

  void on_widget_resize(float AWidth, float AHeight) override {
  }

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
  }

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
  }

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
  }

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) override {
  }

  void on_widget_keyPress(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp=0) override {
  }

  void on_widget_keyRelease(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp=0) override {
  }

  void on_widget_mouseEnter(float AXpos, float AYpos, MIP_Widget* AFrom/*, uint32_t ATimeStamp=0*/) override {
    //if (flags.autoCursor) do_widget_setMouseCursor(this,MCursor);
    //if (flags.autoHint) do_widget_setHint(this,MHint);
  }

  void on_widget_mouseLeave(float AXpos, float AYpos, MIP_Widget* ATo/*, uint32_t ATimeStamp=0*/) override {
    //if (flags.autoCursor) do_widget_setMouseCursor(this,MIP_CURSOR_DEFAULT);
    //if (flags.autoHint) do_widget_setHint(this,"");
  }

  //void on_widget_connect(MIP_Parameter* AParameter, uint32_t ASubIndex) override {
  void on_widget_connect(int32_t AParameterIndex) override {
  }

//------------------------------
public:
//------------------------------

  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) final {
    MIP_Widget::do_widget_update(ASender);
  }

  void do_widget_redraw(MIP_Widget* ASender, MIP_FRect ARect, uint32_t AMode) final {
    MIP_Widget::do_widget_redraw(ASender,ARect,AMode);
  }

  //void do_widget_moved(MIP_Widget* ASender, float ADeltaX=0.0f, float ADeltaY=0.0f) final {
  //  MIP_Widget::do_widget_moved(ASender,ADeltaX,ADeltaY);
  //}

  //void do_widget_resized(MIP_Widget* ASender) final {
  //  MIP_Widget::do_widget_resized(ASender);
  //}

  //void do_widget_grabMouseCursor(MIP_Widget* ASender) final {
  //  MIP_Widget::do_widget_grabMouseCursor(ASender);
  //}

  //void do_widget_grabKeyboard(MIP_Widget* ASender) final {
  //  MIP_Widget::do_widget_grabKeyboard(ASender);
  //}

  //void do_widget_grabModal(MIP_Widget* ASender) final {
  //  MIP_Widget::do_widget_grabModal(ASender);
  //}

  void do_widget_setMouseCursor(MIP_Widget* ASender, int32_t ACursor) final {
    MIP_Widget::do_widget_setMouseCursor(ASender,ACursor);
  }

  void do_widget_setMouseCursorPos(MIP_Widget* ASender, float AXpos, float AYpos) final {
    MIP_Widget::do_widget_setMouseCursorPos(ASender,AXpos,AYpos);
  }

  void do_widget_setHint(MIP_Widget* ASender, const char* AHint) final {
    MIP_Widget::do_widget_setHint(ASender,AHint);
  }

  void do_widget_notify(MIP_Widget* AWidget, uint32_t AValue=0) final {
    MIP_Widget::do_widget_notify(AWidget,AValue);
  }

};

//----------------------------------------------------------------------
#endif
