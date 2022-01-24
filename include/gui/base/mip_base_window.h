#ifndef mip_base_window_included
#define mip_base_window_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_color.h"
#include "gui/mip_drawable.h"
//#include "gui/mip_widget.h"

//----------------------------------------------------------------------

class MIP_BaseWindow {

//------------------------------
public:
//------------------------------

  MIP_BaseWindow(int32_t AWidth, int32_t AHeight, const char* ATitle, void* AParentPtr) {
  }

  //----------

  virtual ~MIP_BaseWindow() {
  }

//------------------------------
public:
//------------------------------

  //virtual MIP_Painter* getPainter() { return nullptr; }

  virtual void setWindowPos(uint32_t AXpos, uint32_t AYpos) {}
  virtual void setWindowSize(uint32_t AWidth, uint32_t AHeight) {}
  virtual void setWindowTitle(const char* ATitle) {}

  virtual void open() {}
  virtual void close() {}
  virtual void eventLoop() {}
  virtual void reparent(void* AParent) {}
  virtual void startTimer(uint32_t ms) {}
  virtual void stopTimer(void) {}
  virtual void setMouseCursor(uint32_t ACursor) {}
  virtual void setMouseCursorPos(int32_t AXpos, int32_t AYpos) {}
  virtual void hideMouseCursor(void) {}
  virtual void showMouseCursor(void) {}
  virtual void grabMouseCursor(void) {}
  virtual void releaseMouseCursor(void) {}
  virtual void beginPaint() {}
  virtual void paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void endPaint() {}
  virtual void invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void sendEvent(uint32_t AData, uint32_t AType) {}
  virtual void flush(void) {}
  virtual void sync(void) {}

  virtual void fill(MIP_Color AColor) {}
  virtual void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, MIP_Color AColor) {}
  virtual void blit(int32_t ADstX, int32_t ADstY, MIP_Drawable* ASource) {}
  virtual void blit(int32_t ADstX, int32_t ADstY, MIP_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) {}

//------------------------------
public:
//------------------------------

  //virtual void on_window_open() {}
  //virtual void on_window_close() {}
  virtual void on_window_move(int32_t AXpos, int32_t AYpos) {}
  virtual void on_window_resize(int32_t AWidth, int32_t AHeight) {}
  virtual void on_window_keyPress(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_window_keyRelease(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_window_mouseClick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_window_mouseRelease(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_window_mouseMove(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_window_mouseEnter(int32_t AXpos, int32_t AYpos, uint32_t ATimeStamp) {}
  virtual void on_window_mouseLeave(int32_t AXpos, int32_t AYpos, uint32_t ATimeStamp) {}
  virtual void on_window_timer() {}
  virtual void on_window_idle() {}
  virtual void on_window_clientMessage(uint32_t AData, void* APtr) {}
  virtual void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}

};

//----------------------------------------------------------------------
#endif
