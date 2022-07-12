#ifndef mip_window_included
#define mip_window_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"
#include "gui/mip_widget.h"

//----------

#ifdef MIP_GUI_XCB
  #include "gui/xcb/mip_xcb_window.h"
  typedef MIP_XcbWindow MIP_ImplementedWindow;
#endif

typedef MIP_ImplementedWindow MIP_BasicWindow;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Window;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Window
: public MIP_ImplementedWindow
, public MIP_Widget {

//------------------------------
private:
//------------------------------

  MIP_WidgetArray MDirtyWidgets = {};
  MIP_Widget*     MHoverWidget  = nullptr;
  MIP_Widget*     MModalWidget  = nullptr;

//------------------------------
public:
//------------------------------

  MIP_Window(uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false)
  : MIP_ImplementedWindow(AWidth,AHeight,AEmbedded)
  , MIP_Widget(MIP_DRect(0,0,AWidth,AHeight)) {
  }

  //----------

  virtual ~MIP_Window() {
  }

//------------------------------
public:
//------------------------------

  MIP_Widget* findHoverWidget(int32_t AXpos, int32_t AYpos) {
    return nullptr;
  }

//------------------------------
public: // widget
//------------------------------

  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) override {
  }

  //----------

  void do_widget_redraw(MIP_Widget* ASender, uint32_t AMode=0) override {
    MDirtyWidgets.append(ASender);
    MIP_DRect rect = ASender->getRect();
    invalidate(rect.x,rect.y,rect.w,rect.h);
  }

  //----------

  void do_widget_modal(MIP_Widget* ASender, uint32_t AMode=0) override {
  }

  //----------

  void do_widget_cursor(MIP_Widget* ASender, uint32_t ACursor) override {
    switch (ACursor) {
      case MIP_CURSOR_GRAB:     break;
      case MIP_CURSOR_RELEASE:  break;
      case MIP_CURSOR_SHOW:     break;
      case MIP_CURSOR_HIDE:     break;
      default:                  break;
    }
  }

  //----------

  void do_widget_hint(MIP_Widget* ASender, const char* AHint) override {
  }

  //----------

  void do_widget_notify(MIP_Widget* ASender, uint32_t AMode, int32_t AValue) override {
  }

//------------------------------
public:
//------------------------------

};


//----------------------------------------------------------------------
#endif

