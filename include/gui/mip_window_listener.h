#ifndef mip_window_listener_included
#define mip_window_listener_included
//----------------------------------------------------------------------

#include "mip.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_WindowListener {
public:
  virtual void on_window_open() {}
  virtual void on_window_close() {}
  virtual void on_window_move(int32_t AXpos, int32_t AYpos) {}
  virtual void on_window_resize(int32_t AWidth, int32_t AHeight) {}
  virtual void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) { MIP_PRINT; }
  virtual void on_window_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_mouse_press(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_mouse_release(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_mouse_move(uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_client_message(uint32_t AData) {}
};

//----------------------------------------------------------------------
#endif
