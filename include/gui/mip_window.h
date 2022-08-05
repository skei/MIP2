#ifndef mip_window_included
#define mip_window_included
//----------------------------------------------------------------------

/*
  todo:
    resize window: un-modal (like right-clicking)
*/

#include "mip.h"
#include "base/system/mip_timer.h"
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

#define MIP_WINDOW_DBLCLICK_MS 300

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Window
: public MIP_ImplementedWindow
, public MIP_TimerListener
, public MIP_Widget {

//------------------------------
protected:
//------------------------------

  MIP_WidgetArray   MDirtyWidgets       = {};
  MIP_Widget*       MHoverWidget        = nullptr;
  MIP_Widget*       MClickedWidget      = nullptr;
  MIP_Widget*       MModalWidget        = nullptr;
  MIP_Widget*       MLockedWidget       = nullptr;
  MIP_Painter*      MWindowPainter      = nullptr;
  MIP_PaintContext  MPaintContext       = {};
  uint32_t          MCurrentCursor      = MIP_CURSOR_DEFAULT;

  // click

  int32_t           MMouseClickedX      = 0;
  int32_t           MMouseClickedY      = 0;
  uint32_t          MMouseClickedB      = 0;
  uint32_t          MMouseClickedS      = 0;
  uint32_t          MMouseClickedT      = 0;

  // drag

  int32_t           MMousePrevX         = 0;
  int32_t           MMousePrevY         = 0;
  int32_t           MMouseDragX         = 0;
  int32_t           MMouseDragY         = 0;

//------------------------------
public:
//------------------------------

  MIP_Window(uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false)
  : MIP_ImplementedWindow(AWidth,AHeight,AEmbedded)
  , MIP_Widget(MIP_DRect(0,0,AWidth,AHeight)) {

    MName = "MIP_Window";
    MWindowPainter = new MIP_Painter(this,this);
    MPaintContext.painter = MWindowPainter;
    MParent = nullptr;
    MRect.setPos(0.0);
    Layout.baseRect = MRect;
    MIndex = -1;

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

  //----------

  MIP_PaintContext* getPaintContext() {
    return &MPaintContext;
  }

  //----------

  void updateHoverWidget(int32_t AXpos, int32_t AYpos, uint32_t ATime) {
    MIP_Widget* hover = nullptr;
    if (MModalWidget) {
      hover = MModalWidget->findChildWidget(AXpos,AYpos);
    } else {
      hover = findChildWidget(AXpos,AYpos);
    }
    if (hover != MHoverWidget) {
      if (!MClickedWidget) {
        if (MHoverWidget) MHoverWidget->on_widget_leave(hover,AXpos,AYpos,ATime);
        if (hover) hover->on_widget_enter(MHoverWidget,AXpos,AYpos,ATime);
      }
      MHoverWidget = hover;
    }
  }

  //----------

  // when releasing mouse cursor after dragging
  // and entering window

  void updateHoverWidgetFrom(MIP_Widget* AFrom, int32_t AXpos, int32_t AYpos, uint32_t ATime) {
    if (MHoverWidget != AFrom) {
      if (AFrom) AFrom->on_widget_leave(MHoverWidget,AXpos,AYpos,ATime);
      if (MHoverWidget) MHoverWidget->on_widget_enter(AFrom,AXpos,AYpos,ATime);
    }
  }

//------------------------------
public: // timer
//------------------------------

//  void on_timerCallback(MIP_Timer* ATimer) override {
//  }

//------------------------------
public: // window
//------------------------------

  /*
    standalone: on_window_resize() will be called afterwards..
    TODO: check plugin..
  */

  void on_window_open() override {
    //MIP_Print("aligning..\n");
  }

  //----------

  void on_window_close() override {
    //MIP_PRINT;
  }

  //----------

  // alignChildWidgets assumes 0,0 is upper corner..ren

  void on_window_move(int32_t AXpos, int32_t AYpos) override {
    //MIP_PRINT;
    MRect.setPos(0,0);
  }

  //----------

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    //MIP_Print("%i,%i\n",AWidth,AHeight);
    delete MWindowPainter;
    MWindowPainter = new MIP_Painter(this,this);
    MPaintContext.painter = MWindowPainter;
    MRect.setSize(AWidth,AHeight);
    alignChildWidgets();
  }

  //----------

  /*
    not all platforms support partially updates=.. :-/
    (scissor taken into account in swapBuffers)

    if we must, we could render to a background pixmap, and blit just the
    updated part from this.. but if so, we need to jeep track of, and update
    the pixmap size when the window resizes..

  */

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    //MIP_Print("%i,%i - %i,%i\n",AXpos,AYpos,AWidth,AHeight);
    setupContext(AXpos,AYpos,AWidth,AHeight);
    MIP_NanoVGPainter* painter = (MIP_NanoVGPainter*)getPainter();
    painter->beginPaint(MRect.w,MRect.h);
    painter->scissor(AXpos,AYpos,AWidth,AHeight);
    paintChildWidgets(getPaintContext());
    painter->resetScissor();
    painter->endPaint();
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
    MMouseClickedX  = AXpos;
    MMouseClickedY  = AYpos;
    MMouseClickedB  = AButton;
    MMouseClickedS  = AState;
    MMouseClickedT  = ATime;
    MMousePrevX     = AXpos;
    MMousePrevY     = AYpos;
    MMouseDragX     = AXpos;
    MMouseDragY     = AYpos;
    if (MHoverWidget != this) {
      MClickedWidget = MHoverWidget;
      MHoverWidget->on_widget_mouse_press(AButton,AState,AXpos,AYpos,ATime);
    }
  }

  //----------

  void on_window_mouse_release(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //MIP_PRINT;
    if (MClickedWidget) {
      MClickedWidget->on_widget_mouse_release(AButton,AState,AXpos,AYpos,ATime);
      updateHoverWidgetFrom(MClickedWidget,AXpos,AYpos,ATime);
    }
    MClickedWidget = nullptr;
  }

  //----------

  void on_window_mouse_move(uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    if (MLockedWidget) {
      if ((AXpos == MMouseClickedX) && (AYpos == MMouseClickedY)) {
        MMousePrevX = AXpos;
        MMousePrevY = AYpos;
        return;
      }
      int32_t deltax = AXpos - MMouseClickedX;
      int32_t deltay = AYpos - MMouseClickedY;
      MMouseDragX += deltax;
      MMouseDragY += deltay;
      MClickedWidget->on_widget_mouse_move(AState,MMouseDragX,MMouseDragY,ATime);
      setMouseCursorPos(MMouseClickedX,MMouseClickedY);
    }
    else {
      updateHoverWidget(AXpos,AYpos,ATime);
      if (MClickedWidget) MClickedWidget->on_widget_mouse_move(AState,AXpos,AYpos,ATime);

    }
    MMousePrevX = AXpos;
    MMousePrevY = AYpos;
  }

  //----------

  void on_window_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //MIP_PRINT;
    //updateHoverWidgetFrom(nullptr,AXpos,AYpos,ATime);
  }

  //----------

  void on_window_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //MIP_PRINT;
    //updateHoverWidgetTo(nullptr,AXpos,AYpos,ATime);
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
    editor can overload this, and catch the parameter from the sender widget
  */

  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) override {
  }

  //----------

  void do_widget_redraw(MIP_Widget* ASender, uint32_t AMode=0) override {
    //MDirtyWidgets.append(ASender);
    MIP_DRect rect = ASender->getWidgetRect();
    invalidateRegion(rect.x,rect.y,rect.w,rect.h);
  }

  //----------

  // ignore event outside of modal widget (menus, etc)

  void do_widget_modal(MIP_Widget* ASender, uint32_t AMode=0) override {
    MModalWidget = ASender;
    if (ASender == nullptr) {
      // close menu..
    }
    updateHoverWidget(MMousePrevX,MMousePrevY,0);
  }

  //----------

  void do_widget_cursor(MIP_Widget* ASender, uint32_t ACursor) override {
    switch (ACursor) {
      case MIP_CURSOR_LOCK:
        MLockedWidget = ASender;
        break;
      case MIP_CURSOR_UNLOCK:
        MLockedWidget = nullptr;
        break;
      case MIP_CURSOR_SHOW:
        showMouseCursor();
        setMouseCursor(MCurrentCursor);
        break;
      case MIP_CURSOR_HIDE:
        hideMouseCursor();
        break;
      default:
        if (ACursor != MCurrentCursor) {
          setMouseCursor(ACursor);
          MCurrentCursor = ACursor;
        }
        break;
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

  void setupContext(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
    MPaintContext.mode        = MIP_WIDGET_PAINT_NORMAL;
    MPaintContext.theme       = &MIP_DefaultTheme;
    MPaintContext.updateRect  = MIP_DRect(AXpos,AYpos,AWidth,AHeight);
    MPaintContext.painter     = MWindowPainter;
  }

//  //----------

  //void redrawEditor() {
  //  invalidateRegion(0,0,getWindowWidth(),getWindowHeight());
  //}

};


//----------------------------------------------------------------------
#endif

