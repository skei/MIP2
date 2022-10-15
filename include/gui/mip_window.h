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
#include "gui/mip_surface.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifdef MIP_GUI_WIN32
  #include "gui/win32/mip_win32_window.h"
  typedef MIP_Win32Window MIP_ImplementedWindow;
#endif

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
//, public MIP_TimerListener
, public MIP_Widget {

//------------------------------
protected:
//------------------------------

  MIP_WidgetArray   MDirtyWidgets       = {};

  MIP_Widget*       MHoverWidget        = nullptr;
  MIP_Widget*       MClickedWidget      = nullptr;
  MIP_Widget*       MModalWidget        = nullptr;
  MIP_Widget*       MLockedWidget       = nullptr;

  uint32_t          MCurrentCursor      = MIP_CURSOR_DEFAULT;

  MIP_PaintContext  MPaintContext       = {};
  MIP_Painter*      MWindowPainter      = nullptr;

  #ifdef MIP_WINDOW_BUFFERED
  MIP_Painter*      MBufferPainter      = nullptr;
  MIP_Surface*      MBufferSurface      = nullptr;
  #endif

  // click
  MIP_Widget*       MMouseClickedW      = nullptr;
  int32_t           MMouseClickedX      = 0;
  int32_t           MMouseClickedY      = 0;
  uint32_t          MMouseClickedB      = 0;
  uint32_t          MMouseClickedS      = 0;
  uint32_t          MMouseClickedT      = 0;
  uint32_t          MPrevClickTime      = 0;
  int32_t           MMousePrevX         = 0;
  int32_t           MMousePrevY         = 0;
  int32_t           MMouseDragX         = 0;
  int32_t           MMouseDragY         = 0;
//bool              MScaleWindow        = false;
  double            MWindowScaleX       = 1.0;
  double            MWindowScaleY       = 1.0;

//------------------------------
public:
//------------------------------

//  MIP_Window(uint32_t AWidth, uint32_t AHeight)
//  : MIP_ImplementedWindow(AWidth,AHeight)
//  , MIP_Widget(MIP_DRect(0,0,AWidth,AHeight)) {
//    setup(AWidth,AHeight);
//  }

  //----------

  MIP_Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : MIP_ImplementedWindow(AWidth,AHeight,AParent)
  , MIP_Widget(MIP_DRect(0,0,AWidth,AHeight)) {
    //MWindowType = "MIP_Window";
    //MIP_Print("%i,%i\n",AWidth,AHeight);
    MOwnerWindow = this;
    //setup(AWidth,AHeight);
    MName = "MIP_Window";
    MWindowPainter = new MIP_Painter(this,this);
    #ifdef MIP_WINDOW_BUFFERED
      createBuffer(AWidth,AHeight);
      MPaintContext.painter = MBufferPainter;
    #else
      MPaintContext.painter = MWindowPainter;
    #endif
    MParent = nullptr;
    MRect.setPos(0.0);
    Layout.initialRect = MRect;
    Layout.baseRect = MRect;
    MIndex = -1;

  }

  //----------

  virtual ~MIP_Window() {
    //cleanup();
    delete MWindowPainter;
    #ifdef MIP_WINDOW_BUFFERED
      //delete MBufferPainter;
      //delete MBufferSurface;
      deleteBuffer();
    #endif
  }

//------------------------------
public:
//------------------------------

//  void setup(uint32_t AWidth, uint32_t AHeight) {
//    MName = "MIP_Window";
//    MWindowPainter = new MIP_Painter(this,this);
//    #ifdef MIP_WINDOW_BUFFERED
//      createBuffer(AWidth,AHeight);
//      MPaintContext.painter = MBufferPainter;
//    #else
//      MPaintContext.painter = MWindowPainter;
//    #endif
//    MParent = nullptr;
//    MRect.setPos(0.0);
//    Layout.initialRect = MRect;
//    Layout.baseRect = MRect;
//    MIndex = -1;
//  }

  //----------

//  void cleanup() {
//    delete MWindowPainter;
//    #ifdef MIP_WINDOW_BUFFERED
//      //delete MBufferPainter;
//      //delete MBufferSurface;
//      deleteBuffer();
//    #endif
//  }

//------------------------------
public:
//------------------------------

  #ifdef MIP_WINDOW_BUFFERED

  void createBuffer(uint32_t AWidth, uint32_t AHeight) {
    MBufferSurface = new MIP_Surface(this,AWidth,AHeight);
    MBufferPainter = new MIP_Painter(this,MBufferSurface);
  }

  //----------

  void deleteBuffer() {
    delete MBufferPainter;
    delete MBufferSurface;
  }

  //----------

  void resizeBuffer(uint32_t AWidth, uint32_t AHeight) {
    if (MBufferSurface) delete MBufferSurface;
    MBufferSurface = new MIP_Surface(this,AWidth,AHeight);
    if (MBufferPainter) delete MBufferPainter;
    MBufferPainter = new MIP_Painter(this,MBufferSurface);
  }

  //----------

  //void resizeWindow(uint32_t AWidth, uint32_t AHeight) {
  //  on_resize_window()
  //}

  #endif

//------------------------------
public:
//------------------------------

  //void setScaleWindow(bool AScale) {
  //  MScaleWindow = AScale;
  //}

  //----------

  //bool getScaleWindow() {
  //  return MScaleWindow;
  //}

  //----------

  MIP_Painter* getWindowPainter() {
    return MWindowPainter;
  }

  //----------

  MIP_Painter* getBufferPainter() {
    #ifdef MIP_WINDOW_BUFFERED
      return MBufferPainter;
    #else
      return nullptr;
    #endif
  }

  //----------

  MIP_Painter* getPainter() {
    #ifdef MIP_WINDOW_BUFFERED
      return MBufferPainter;
    #else
      return MWindowPainter;
    #endif
  }

  //----------

  MIP_PaintContext* getPaintContext() {
    return &MPaintContext;
  }

//------------------------------
public:
//------------------------------

  void unmodal() {
    if (MModalWidget) MModalWidget->on_widget_unmodal();
  }

  //----------

  //void alignWidgets() {
  //  alignChildWidgets(true);
  //  scaleWidget(MWindowScaleX,MWindowScaleY,true);
  //}

  //----------

  void updateHoverWidget(int32_t AXpos, int32_t AYpos, uint32_t ATime) {
    MIP_Widget* hover = nullptr;
    // find hover
    if (MModalWidget) {
      hover = MModalWidget->findChildWidget(AXpos,AYpos);
    } else {
      hover = findChildWidget(AXpos,AYpos);
    }
    // enter/leave
    if (hover != MHoverWidget) {
      if (!MClickedWidget) {
        if (MHoverWidget) MHoverWidget->on_widget_leave(hover,AXpos,AYpos,ATime);
        if (hover) hover->on_widget_enter(MHoverWidget,AXpos,AYpos,ATime);
      }
      MHoverWidget = hover;
    }
    /*
    // edges/resizable
    if (MHoverWidget) {
      MIP_DRect r = MHoverWidget->getRect();
      if (AXpos >= (r.x2() - 5)) {
        setMouseCursor(MIP_CURSOR_ARROW_RIGHT);
      }
      else if (AXpos <= (r.x + 5)) {
        setMouseCursor(MIP_CURSOR_ARROW_LEFT);
      }
      else if (AYpos >= (r.y2() - 5)) {
        setMouseCursor(MIP_CURSOR_ARROW_DOWN);
      }
      else if (AYpos <= (r.y + 5)) {
        setMouseCursor(MIP_CURSOR_ARROW_UP);
      }
      else {
        setMouseCursor(MIP_CURSOR_DEFAULT);
      }
    }
    */

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

  /*
  void on_timerCallback(MIP_Timer* ATimer) override {
    MIP_PRINT;
  }
  */

//------------------------------
public: // window
//------------------------------

  /*
    standalone: on_window_resize() will be called afterwards..
    TODO: check plugin..
  */

  void on_window_open() override {
    //MIP_Print("MRect: %.0f,%.0f)\n",MRect.w,MRect.h);
    //setOwnerWindow(this);
    //MRect.setSize(AWidth,AHeight);
    on_widget_config(this);

    //freezeLayout();

  }

  //----------

  // alignChildWidgets assumes 0,0 is upper corner..

  void on_window_move(int32_t AXpos, int32_t AYpos) override {
    //MIP_PRINT;
    MRect.setPos(0,0); // still needed?
  }

  //----------

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    //MIP_Print("%i,%i\n",AWidth,AHeight);
    if (MWindowPainter) delete MWindowPainter;
    MWindowPainter = new MIP_Painter(this,this);
    MWindowPainter->setClipRect(MIP_DRect(0,0,AWidth,AHeight));
    #ifdef MIP_WINDOW_BUFFERED
      resizeBuffer(AWidth,AHeight);
      MPaintContext.painter = MBufferPainter;
    #else
      MPaintContext.painter = MWindowPainter;
    #endif
    MRect.setSize(AWidth,AHeight);
    on_widget_config(this);

    /*
    double xscale = AWidth / Layout.initialRect.w;
    double yscale = AHeight / Layout.initialRect.h;
    MIP_Print("xscale %f yscale %f\n",xscale,yscale);
    */
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

    MIP_Print("%i,%i - %i,%i\n",AXpos,AYpos,AWidth,AHeight);

    MIP_Painter* painter;
    #ifdef MIP_WINDOW_BUFFERED
      painter = MBufferPainter;
    #else
      painter = MWindowPainter;
    #endif

    // -> beginPaint
    //#ifdef MIP_WIN32
    //  painter->setup( hdc() );
    //#endif

    MPaintContext.mode        = MIP_WIDGET_PAINT_NORMAL;
    MPaintContext.theme       = &MIP_DefaultTheme;
    MPaintContext.updateRect  = MIP_DRect(AXpos,AYpos,AWidth,AHeight);
    MPaintContext.painter     = painter;

    if (painter) {

      //painter->beginPaint(AXpos,AYpos,AWidth,AHeight);
      //painter->beginPaint(0,0,AWidth,AHeight);

      painter->beginPaint(0,0,MRect.w,MRect.h);
      painter->resetClip();
      painter->setClipRect(MIP_DRect(AXpos,AYpos,AWidth,AHeight));
      paintChildWidgets(&MPaintContext);
      painter->resetClip();
      painter->endPaint();
    }

    #ifdef MIP_WINDOW_BUFFERED
      blitDrawable(AXpos,AYpos,MBufferSurface,AXpos,AYpos,AWidth,AHeight);
    #endif

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

  void on_window_mouse_click(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    MMouseClickedX  = AXpos;
    MMouseClickedY  = AYpos;
    MMouseClickedB  = AButton;
    MMouseClickedS  = AState;
    MMouseClickedT  = ATime;
    MMousePrevX     = AXpos;
    MMousePrevY     = AYpos;
    MMouseDragX     = AXpos;
    MMouseDragY     = AYpos;
    int32_t elapsed = (ATime - MPrevClickTime);
    bool dblclick = (elapsed < MIP_WINDOW_DBLCLICK_MS);
    MPrevClickTime = ATime;
    if (MHoverWidget && (MHoverWidget != this)) {
      if (dblclick && MHoverWidget->Options.doubleClick) {
        MClickedWidget = MHoverWidget;
        MHoverWidget->on_widget_mouse_dblclick(AButton,AState,AXpos,AYpos,ATime);
      }
      else {
        MClickedWidget = MHoverWidget;
        MHoverWidget->on_widget_mouse_click(AButton,AState,AXpos,AYpos,ATime);
      }
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
    if (MLockedWidget) { // todo: also add if mouse_clicked?
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
      if (MClickedWidget) {
        MClickedWidget->on_widget_mouse_move(AState,AXpos,AYpos,ATime);
      }
      else if (MHoverWidget) {
        if (MHoverWidget->Options.wantHoverEvents) {
          MHoverWidget->on_widget_mouse_move(AState,AXpos,AYpos,ATime);
        }
      }
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
//  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, MIP_DPoint APos, uint32_t ATime) override {}
//  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, MIP_DPoint APos, uint32_t ATime) override {}
//  void on_widget_mouse_move(uint32_t AState, MIP_DPoint APos, uint32_t ATime) override {}
//  void on_widget_enter(MIP_DPoint APos, uint32_t ATime) override {}
//  void on_widget_leave(MIP_DPoint APos, uint32_t ATime) override {}
//  //void on_widget_connect(MIP_Parameter* AParameter) override {}

//  void on_widget_unmodal() override {
//    MIP_PRINT;
//  }

//------------------------------
public: // child to parent
//------------------------------

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
    //if (MModalWidget) MModalWidget->on_widget_modal(false);
    MModalWidget = ASender;
    //if (MModalWidget) MModalWidget->on_widget_modal(true);
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

  //----------

  void do_widget_resized(MIP_Widget* ASender, float ADeltaX=0.0f, float ADeltaY=0.0f, uint32_t AMode=0) override {
  }

  //----------

  //MIP_Widget* do_widget_get_owner_window(MIP_Widget* ASender) override {
  //  return this;
  //}

//------------------------------
public:
//------------------------------

//  void setupPaintContext(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
//    MPaintContext.mode        = MIP_WIDGET_PAINT_NORMAL;
//    MPaintContext.theme       = &MIP_DefaultTheme;
//    MPaintContext.updateRect  = MIP_DRect(AXpos,AYpos,AWidth,AHeight);
//    #ifdef MIP_WINDOW_BUFFERED
//      MPaintContext.painter = MBufferPainter;
//    #else
//      MPaintContext.painter = MWindowPainter;
//    #endif
//  }

  //----------

//  MIP_Widget* getOwnerWindow() override {
//    return this;
//  }

  //----------

//  void redrawEditor() {
//    invalidateRegion(0,0,getWindowWidth(),getWindowHeight());
//  }

};


//----------------------------------------------------------------------
#endif

