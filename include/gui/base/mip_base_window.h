#ifndef mip_base_window_included
#define mip_base_window_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_drawable.h"
//#include "gui/mip_window_listener.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_BaseWindow
/*: public MIP_WindowListener*/{

//------------------------------
protected:
//------------------------------

  //MIP_WindowListener* MWindowListener = this;

//------------------------------
public:
//------------------------------

  MIP_BaseWindow() {
  }

  //----------

  virtual ~MIP_BaseWindow() {
  }

//------------------------------
public:
//------------------------------

  //void setWindowListener(MIP_WindowListener* AListener) {
  //  MWindowListener = AListener;
  //}

//------------------------------
public:
//------------------------------

  virtual void on_window_open() {}
  virtual void on_window_close() {}
  virtual void on_window_move(int32_t AXpos, int32_t AYpos) {}
  virtual void on_window_resize(int32_t AWidth, int32_t AHeight) {}
  virtual void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void on_window_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_mouse_click(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_mouse_release(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_mouse_dblclick(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_mouse_move(uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_client_message(uint32_t AData) {}
  //virtual void on_window_start_event_thread(uint32_t AMode=0) { MIP_PRINT; }
  //virtual void on_window_stop_event_thread(uint32_t AMode=0) { MIP_PRINT; }

//------------------------------
public:
//------------------------------

  virtual void setWindowPos(uint32_t AXpos, uint32_t AYpos) {}
  virtual void setWindowSize(uint32_t AWidth, uint32_t AHeight) {}
  virtual void setWindowTitle(const char* ATitle) {}
  virtual void openWindow() {}
  virtual void closeWindow() {}
  virtual void flush(void) {}
  virtual void sync(void) {}
  virtual void eventLoop() {}
  virtual void startEventThread() {}
  virtual void stopEventThread() {}
  virtual void sendClientMessage(uint32_t AData, uint32_t AType) {}
  virtual void invalidateRegion(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void redrawRegion(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void reparentWindow(intptr_t AParent) {}
  virtual void beginPaint()  {}
  virtual void endPaint()  {}
  virtual void setEventThreadCallbacks( void (*AStart)(void* AUser), void (*AStop)(void* AUser) ) {}

//------------------------------
public: // mouse
//------------------------------

  virtual void setMouseCursor(int32_t ACursor) {}
  virtual void setMouseCursorPos(int32_t AXpos, int32_t AYpos) {}
  virtual void hideMouseCursor(void) {}
  virtual void showMouseCursor(void) {}
  virtual void grabMouseCursor(void) {}
  virtual void releaseMouseCursor(void) {}

//------------------------------
public: // paint
//------------------------------

  virtual void fillColor(uint32_t AColor) {}
  virtual void fillColor(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor) {}
  virtual void blitBuffer(int32_t ADstX, int32_t ADstY, void* AData, uint32_t AStride, int32_t ASrcW, int32_t ASrcH) {}
  //virtual void blitImage(int32_t ADstX, int32_t ADstY, /*xcb_image_t*/void* AImage) {}
  //virtual void blitDrawable(int32_t ADstX, int32_t ADstY, /*xcb_drawable_t*/intptr_t ADrawable, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) {}
  virtual void blitDrawable(int32_t ADstX, int32_t ADstY, MIP_Drawable* ADrawable, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) {}

};

//----------------------------------------------------------------------
#endif
