#ifndef mip_widget_included
#define mip_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_rect.h"
#include "gui/mip_paint_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Widget;
typedef MIP_Array<MIP_Widget*> MIP_WidgetArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Widget {

//------------------------------
protected:
//------------------------------

  MIP_Widget*     MParent     = nullptr;
  MIP_WidgetArray MChildren   = {};
  MIP_DRect       MRect       = {};
  MIP_DRect       MBaseRect   = {};

//------------------------------
public:
//------------------------------

  MIP_Widget(MIP_DRect ARect) {
    MBaseRect = ARect;
    MRect = ARect;
  }

  //----------

  virtual ~MIP_Widget() {
  }

//------------------------------
public:
//------------------------------

  MIP_Widget* getParent() { return MParent; }
  MIP_DRect   getRect()   { return MRect; }

//------------------------------
public:
//------------------------------

  void setPos(double AXpos, double AYpos) {
    MRect.x = AXpos;
    MRect.y = AYpos;
  }

//------------------------------
public:
//------------------------------

  MIP_Widget* findChildWidget(double AXpos, double AYpos) {
    return nullptr;
  }

  void paintChildren(MIP_DRect ARect, uint32_t AMode=0) {
  }

  void alignChildren(bool ARecursive=true) {
  }

//------------------------------
public:
//------------------------------

  virtual void on_widget_move(double AXpos, double AYpos) {}
  virtual void on_widget_resize(double AWidth, double AHeight) {}
  virtual void on_widget_paint(MIP_PaintContext* AContext, double AXpos, double AYpos, double AWidth, double AHeight) { MIP_PRINT; }
  virtual void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) {}
  virtual void on_widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) {}
  virtual void on_widget_mouse_press(uint32_t AButton, uint32_t AState, double AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {}
  virtual void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {}
  virtual void on_widget_enter(double AXpos, double AYpos, uint32_t ATime) {}
  virtual void on_widget_leave(double AXpos, double AYpos, uint32_t ATime) {}

//------------------------------
public:
//------------------------------

  virtual void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) {
    if (MParent) MParent->do_widget_update(ASender,AMode);
  }

  virtual void do_widget_redraw(MIP_Widget* ASender, uint32_t AMode=0) {
    if (MParent) MParent->do_widget_redraw(ASender,AMode);
  }

  virtual void do_widget_modal(MIP_Widget* ASender, uint32_t AMode=0) {
    if (MParent) MParent->do_widget_modal(ASender,AMode);
  }

  virtual void do_widget_cursor(MIP_Widget* ASender, uint32_t ACursor) {
    if (MParent) MParent->do_widget_cursor(ASender,ACursor);
  }

  virtual void do_widget_hint(MIP_Widget* ASender, const char* AHint) {
    if (MParent) MParent->do_widget_hint(ASender,AHint);
  }

  virtual void do_widget_notify(MIP_Widget* ASender, uint32_t AMode, int32_t AValue) {
    if (MParent) MParent->do_widget_notify(ASender,AMode,AValue);
  }

};

//----------------------------------------------------------------------
#endif
