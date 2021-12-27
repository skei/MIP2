#ifndef mip_window_included
#define mip_window_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_rect.h"
#include "gui/mip_widget.h"
#include "gui/base/mip_base_window.h"

//----------------------------------------------------------------------

#ifdef MIP_NO_GUI
  typedef MIP_BaseWindow MIP_ImplementedWindow;
#endif

#ifdef MIP_GUI_XCB
  #include "gui/xcb/mip_xcb_window.h"
  typedef MIP_XcbWindow MIP_ImplementedWindow;
#endif

//----------------------------------------------------------------------

class MIP_Window
: public MIP_ImplementedWindow {


//------------------------------
public:
//------------------------------

  MIP_Window(int32_t AWidth, int32_t AHeight, const char* ATitle="", void* AParentPtr=nullptr)
  : MIP_ImplementedWindow(AWidth,AHeight,ATitle,AParentPtr) {
    MIP_PRINT;
  }

  //----------

  virtual ~MIP_Window() {
    MIP_PRINT;
  }

//------------------------------
public: // MIP_BaseWindow
//------------------------------

  void on_window_move(int32_t AXpos, int32_t AYpos) override {
    MIP_Print("x %i y %i\n",AXpos,AYpos);
  }

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    MIP_Print("w %i h %i\n",AWidth,AHeight);
  }

  void on_window_open() override {
    MIP_Print("\n");
  }

  void on_window_close() override {
    MIP_Print("\n");
  }

  void on_window_keyPress(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) override {
    MIP_Print("k %i s %i ts %i\n",AKey,AState,ATimeStamp);
  }

  void on_window_keyRelease(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) override {
    MIP_Print("k %i s %i ts %i\n",AKey,AState,ATimeStamp);
  }

  void on_window_mouseClick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    MIP_Print("x %i y %i b %i s %i ts %i\n",AXpos,AYpos,AButton,AState,ATimeStamp);
  }

  void on_window_mouseRelease(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    MIP_Print("x %i y %i b %i s %i ts %i\n",AXpos,AYpos,AButton,AState,ATimeStamp);
  }

  void on_window_mouseMove(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATimeStamp) override {
    MIP_Print("x %i y %i s %i ts %i\n",AXpos,AYpos,AState,ATimeStamp);
  }

  void on_window_enter(int32_t AXpos, int32_t AYpos, uint32_t ATimeStamp) override {
    MIP_Print("x %i y %i ts %i\n",AXpos,AYpos,ATimeStamp);
  }

  void on_window_leave(int32_t AXpos, int32_t AYpos, uint32_t ATimeStamp) override {
    MIP_Print("x %i y %i ts %i\n",AXpos,AYpos,ATimeStamp);
  }

  void on_window_timer() override {
    MIP_Print("\n");
  }

  void on_window_idle() override {
    MIP_Print("\n");
  }

  void on_window_clientMessage(uint32_t AData, void* APtr) override {
    MIP_Print("data %i ptr %p\n",AData,APtr);
  }

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    MIP_Print("x %i y %i w %i h %i\n",AXpos,AYpos,AWidth,AHeight);
  }


//------------------------------
public: // MIP_Widget
//------------------------------

//  void do_widget_update(MIP_Widget* AWidget) override {
//    MIP_PRINT;
//  }
//
//  //----------
//
//  void do_widget_redraw(MIP_Widget* AWidget) override {
//    MIP_PRINT;
//    //TODO: widget->on_paint_..
//  }


};

//----------------------------------------------------------------------
#endif
