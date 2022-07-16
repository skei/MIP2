#ifndef mip_window_included
#define mip_window_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"
#include "gui/mip_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifdef MIP_GUI_XCB
  #include "gui/xcb/mip_xcb_window.h"
  typedef MIP_XcbWindow MIP_ImplementedWindow;
#endif

//----------

typedef MIP_ImplementedWindow MIP_BasicWindow;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Window
: public MIP_ImplementedWindow
, public MIP_Widget {

//------------------------------
protected:
//------------------------------

  MIP_WidgetArray   MDirtyWidgets       = {};
  MIP_Widget*       MHoverWidget        = nullptr;
  MIP_Widget*       MModalWidget        = nullptr;
  MIP_Widget*       MMouseCaptureWidget = nullptr;
  MIP_Widget*       MKeyCaptureWidget   = nullptr;

  MIP_Painter*      MWindowPainter      = nullptr;
  MIP_PaintContext  MPaintContext       = {};

//------------------------------
public:
//------------------------------

  MIP_Window(uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false)
  : MIP_ImplementedWindow(AWidth,AHeight,AEmbedded)
  , MIP_Widget(MIP_DRect(0,0,AWidth,AHeight)) {
    //MIP_Print("MRect: %.0f,%.0f - %.0f,%.0f\n",MRect.x,MRect.y,MRect.w,MRect.h);
    MWindowPainter = new MIP_Painter(this,this);
    MPaintContext.painter = MWindowPainter;
    MRect.setPos(0.0);
  }

  //----------

  virtual ~MIP_Window() {
    delete MWindowPainter;
  }

//------------------------------
public:
//------------------------------

  MIP_Painter* getPainter() {
    return MWindowPainter;
  }

  MIP_PaintContext* getPaintContext() {
    return &MPaintContext;
  }

  MIP_Widget* findHoverWidget(int32_t AXpos, int32_t AYpos) {
    return nullptr;
  }

//------------------------------
public: // window
//------------------------------

  /*
    standalone: on_window_resize() will be called afterwards..
    TODO: check plugin..
  */

  void on_window_open() override {
    //MIP_PRINT;
    //MIP_Print("aligning..\n");
    //alignChildWidgets();
  }

  //----------

  void on_window_close() override {
    //MIP_PRINT;
  }

  //----------

  void on_window_move(int32_t AXpos, int32_t AYpos) override {
    //MIP_PRINT;
    //MRect.setPos(AXpos,AYpos);
    MRect.setPos(0,0); // because of alignChildWidgets
  }

  //----------

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    //MIP_PRINT;
    //int32_t new_width = MIP_NextPowerOfTwo(AWidth);
    //int32_t new_height = MIP_NextPowerOfTwo(AWidth);
    delete MWindowPainter;
    MWindowPainter = new MIP_Painter(this,this);
    MPaintContext.painter = MWindowPainter;
    MRect.setSize(AWidth,AHeight);
    //MIP_Print("aligning..\n");
    alignChildWidgets();
  }

  //----------

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    MPaintContext.mode        = MIP_WIDGET_PAINT_NORMAL;
    MPaintContext.theme       = &MIP_DefaultTheme;
    MPaintContext.updateRect  = MIP_DRect(AXpos,AYpos,AWidth,AHeight);
    MPaintContext.painter     = MWindowPainter;
  }

  //----------

  void on_window_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) override {
    //MIP_PRINT;
  }

  //----------

  void on_window_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) override {
    //MIP_PRINT;
  }

  //----------

  void on_window_mouse_press(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //MIP_PRINT;
  }

  //----------

  void on_window_mouse_release(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //MIP_PRINT;
  }

  //----------

  void on_window_mouse_move(uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //MIP_PRINT;
  }

  //----------

  void on_window_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //MIP_PRINT;
  }

  //----------

  void on_window_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //MIP_PRINT;
  }

  //----------

  void on_window_client_message(uint32_t AData) override {
    //MIP_PRINT;
  }

  //----------

  //void on_window_start_event_thread(uint32_t AMode=0) override {
  //  //MIP_PRINT;
  //}

  //----------

  //void on_window_stop_event_thread(uint32_t AMode=0) override {
  //  //MIP_PRINT;
  //}

//------------------------------
public: // parent to child
//------------------------------

//  void on_widget_move(MIP_DPoint APos) override {}
//  void on_widget_resize(MIP_DPoint ASize) override {}
//  void on_widget_align(bool ARecursive=true) override {}
//  void on_widget_paint(MIP_PaintContext* AContext) override {}
//  void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) override {}
//  void on_widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) override {}
//  void on_widget_mouse_press(uint32_t AButton, uint32_t AState, MIP_DPoint APos, uint32_t ATime) override {}
//  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, MIP_DPoint APos, uint32_t ATime) override {}
//  void on_widget_mouse_move(uint32_t AState, MIP_DPoint APos, uint32_t ATime) override {}
//  void on_widget_enter(MIP_DPoint APos, uint32_t ATime) override {}
//  void on_widget_leave(MIP_DPoint APos, uint32_t ATime) override {}
//  //void on_widget_connect(MIP_Parameter* AParameter) override {}

//------------------------------
public: // child to parent
//------------------------------

  /*
    editor can overload this, and catch the paramtere from the sender widget
  */

  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) override {
  }

  //----------

  void do_widget_redraw(MIP_Widget* ASender, uint32_t AMode=0) override {
    MDirtyWidgets.append(ASender);
    MIP_DRect rect = ASender->getWidgetRect();
    // should this ne called something less 'low-level'? redraw() ??
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

