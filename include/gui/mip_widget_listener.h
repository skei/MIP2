#ifndef mip_widget_listener_included
#define mip_widget_listener_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_painter.h"

class MIP_WidgetListener {
public:
  virtual void on_widget_move(int32_t AXpos, int32_t AYpos) {}
  virtual void on_widget_resize(int32_t AWidth, int32_t AHeight) {}
  virtual void on_widget_paint(MIP_Painter* APainter, int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) { MIP_PRINT; }
  virtual void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) {}
  virtual void on_widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) {}
  virtual void on_widget_mouse_press(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_widget_mouse_release(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_widget_mouse_move(uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_widget_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_widget_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
public:
  virtual void do_widget_update(MIP_WidgetListener* ASender) {}
  virtual void do_widget_redraw(MIP_WidgetListener* ASender) {}
  virtual void do_widget_notify(MIP_WidgetListener* ASender, int32_t AMessage) {}
};


//----------------------------------------------------------------------
#endif
