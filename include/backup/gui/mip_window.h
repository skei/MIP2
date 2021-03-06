#ifndef mip_window_included
#define mip_window_included
//----------------------------------------------------------------------


/*
  handles:
  - painting
  - buffering
  - resizing
  - mouse handling



*/

// don't include windows/surface/etc stuff..
//#define MIP_NO_GUI

// use xcb for lowlevel gui (window, surface)
//#define MIP_GUI_XCB

// draw directly to screen (no double buffering)
//#define MIP_NO_WINDOW_BUFFERING

// use bitmap (instead of surface) as backbuffer
//#define MIP_WINDOW_BUFFER_BITMAP

// scale window (events) instead of realign widgets
// (not working yet)
//#define MIP_WINDOW_SCALE_WINDOW

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_rect.h"
#include "gui/mip_painter.h"
#include "gui/mip_surface.h"
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
//
//----------------------------------------------------------------------



class MIP_WindowListener {
public:
  virtual void on_window_listener_updateWidget(MIP_Widget* AWidget) {}
  virtual void on_window_listener_resize(uint32_t AWidth, uint32_t AHeight) {}
};

//----------------------------------------------------------------------
//
// window
//
//----------------------------------------------------------------------

class MIP_Window
: public MIP_ImplementedWindow {

//------------------------------
private:
//------------------------------

  MIP_WindowListener* MListener               = nullptr;
  MIP_Painter*        MWindowPainter          = nullptr;

  #ifndef MIP_NO_WINDOW_BUFFERING
    uint32_t            MBufferWidth            = 0;
    uint32_t            MBufferHeight           = 0;
    MIP_Painter*        MBufferPainter          = nullptr;
    #ifdef MIP_WINDOW_BUFFER_BITMAP
      MIP_Bitmap*       MBufferBitmap           = nullptr;
    #else
      MIP_Surface*      MBufferSurface          = nullptr;
    #endif
  #endif

  MIP_Widget*         MMouseHoverWidget       = nullptr;
  MIP_Widget*         MModalWidget            = nullptr;
  MIP_Widget*         MMouseClickedWidget     = nullptr;
  MIP_Widget*         MMouseCapturedWidget    = nullptr;
  MIP_Widget*         MMouseLockedWidget      = nullptr;
  MIP_Widget*         MKeyInputWidget         = nullptr;

  bool                MFillWindowBackground   = false;
  MIP_Color           MWindowBackgroundColor  = MIP_Color(0.5);

  int32_t             MMouseX                 = 0;
  int32_t             MMouseY                 = 0;
  int32_t             MMousePrevX             = 0;
  int32_t             MMousePrevY             = 0;
  int32_t             MMouseClickedX          = 0;
  int32_t             MMouseClickedY          = 0;
  int32_t             MMouseDragX             = 0;
  int32_t             MMouseDragY             = 0;
  uint32_t            MResizingWidth          = 0;
  uint32_t            MResizingHeight         = 0;

  float               MWindowXScale           = 1.0;
  float               MWindowYScale           = 1.0;

//------------------------------
public:
//------------------------------

  MIP_Window(uint32_t AWidth, uint32_t AHeight, MIP_WindowListener* AListener, bool AEmbedded)
  : MIP_ImplementedWindow(AWidth,AHeight,AEmbedded) {
    MName = "MIP_Window";
    MListener = AListener;
    // move these to open() ?
    MWindowPainter = new MIP_Painter(this);
    MIP_Assert(MWindowPainter);
    #ifndef MIP_NO_WINDOW_BUFFERING
      createBuffer(AWidth,AHeight);
    #endif
    //

  }

  //----------

  virtual ~MIP_Window() {
    // to close() ?
    if (MWindowPainter) delete MWindowPainter;
    #ifndef MIP_NO_WINDOW_BUFFERING
      deleteBuffer();
    #endif
    //
  }

//------------------------------
public:
//------------------------------

  virtual bool isBuffered() {
    #ifdef MIP_NO_WINDOW_BUFFERING
      return false;
    #else
      return true;
    #endif
  }

  MIP_Painter* getWindowPainter() {
    return MWindowPainter;
  }

  MIP_Painter* getBufferPainter() {
    #ifdef MIP_NO_WINDOW_BUFFERING
      return MWindowPainter;
    #else
      return nullptr;
    #endif
  }

  MIP_Painter* getPainter() {
    #ifdef MIP_NO_WINDOW_BUFFERING
      return MWindowPainter;
    #else
      return MBufferPainter;
    #endif
  }

  MIP_Widget* getMouseHoverWidget()       { return MMouseHoverWidget; }
  MIP_Widget* getModalWidget()            { return MModalWidget; }
  MIP_Widget* getMouseClickedWidget()     { return MMouseClickedWidget; }
  MIP_Widget* getMouseMouseLockedWidget() { return MMouseLockedWidget; }
  MIP_Widget* getKeyInputWidget()         { return MKeyInputWidget; }

  void setFillWindowBackground(bool s=true)     { MFillWindowBackground = s; }
  void setWindowBackgroundColor(MIP_Color c)    { MWindowBackgroundColor = c; }

//------------------------------
public:
//------------------------------

  /*
    find widget the mouse cursoring is hovering above
    sets MMouseHoverWidget to foiund widget, or null if none..
  */

  void updateHoverWidget(uint32_t AXpos, uint32_t AYpos, uint32_t ATimeStamp=0) {
    MIP_Widget* hover = nullptr;
    if (MModalWidget) {
      hover = MModalWidget->findWidget(AXpos,AYpos);
    }
    else {
      hover = findWidget(AXpos,AYpos);
    }
    if (hover) {
      if (hover != MMouseHoverWidget) {
        if (MMouseHoverWidget) {
          MMouseHoverWidget->on_widget_mouseLeave(AXpos,AYpos,hover);
        }
        if (hover->state.active) {
          MMouseHoverWidget = hover;
          MMouseHoverWidget->on_widget_mouseEnter(AXpos,AYpos,MMouseHoverWidget);
        }
      }
    }
    else {
      if (MMouseHoverWidget) {
        MMouseHoverWidget->on_widget_mouseLeave(AXpos,AYpos,hover);
        MMouseHoverWidget = nullptr;
      }
    }
  }

  //----------

  // assume no prev widget, only prev clicked
  // called after mouse release

  void releaseHoverWidget(MIP_Widget* AClicked, uint32_t AXpos, uint32_t AYpos, uint32_t ATimeStamp=0) {
    MIP_Widget* hover = nullptr;
    if (MModalWidget) {
      hover = MModalWidget->findWidget(AXpos,AYpos);
    }
    else {
      hover = findWidget(AXpos,AYpos);
    }
    if (AClicked && (hover != AClicked)) {
      AClicked->on_widget_mouseLeave(AXpos,AYpos,hover);
    }
    if (hover) {
      if (hover->state.active) {
        hover->on_widget_mouseEnter(AXpos,AYpos,MMouseHoverWidget);
        MMouseHoverWidget = hover;
      }
    }
    else {
      MMouseHoverWidget = nullptr;
    }
  }

//------------------------------
public: // paint
//------------------------------

  void paint() {
    #ifdef MIP_NO_WINDOW_BUFFERING
      paintWindow(MRect);
    #else
      paintBuffer(MRect);
    #endif
  }

  //----------

  void paint(MIP_FRect ARect) {
    #ifdef MIP_NO_WINDOW_BUFFERING
      paintWindow(ARect);
    #else
      paintBuffer(ARect);
    #endif
  }

  //----------

  void paint(MIP_Widget* AWidget) {
    MIP_FRect rect = AWidget->getRect();
    #ifdef MIP_NO_WINDOW_BUFFERING
      paintWindow(rect);
    #else
      paintBuffer(rect);
    #endif
  };

//------------------------------
public: // window
//------------------------------

  void resizeWindow(uint32_t AWidth, uint32_t AHeight) {
    MWindowXScale = AWidth / MRect.w;
    MWindowYScale = AHeight / MRect.h;
    //MIP_Print("%i,%i scale %.2f,%.2f\n",AWidth,AHeight,MWindowXScale,MWindowYScale);
    //paint();
    #ifndef MIP_WINDOW_SCALE_WINDOW
      #ifndef MIP_NO_WINDOW_BUFFERING
        resizeBuffer(AWidth,AHeight);
      #endif
      if (MWindowPainter) {
        MWindowPainter->resize(AWidth,AHeight);
      }
      MRect.w = AWidth;
      MRect.h = AHeight;
      alignWidgets();

      //float sx = MRect.w / EDITOR_WIDTH;
      //float sy = MRect.h / EDITOR_HEIGHT;
      //scaleWidgets(sx);

    #endif

  }

  //----------

  void fillWindowBackground(MIP_FRect ARect) {
    #ifdef MIP_NO_WINDOW_BUFFERING
      MWindowPainter->fillRectangle(ARect,MWindowBackgroundColor);
      //MWindowPainter->flush();
    #else
      MBufferPainter->fillRectangle(ARect,MWindowBackgroundColor);
    #endif
  }

  //----------

  void paintWindow(MIP_FRect ARect) {
    //MWindowPainter->setScale(2,2);
    paintWidgets(MWindowPainter,ARect);
    //MWindowPainter->resetScale();
    MWindowPainter->flush();
  }

//------------------------------
public: // buffer
//------------------------------

  #ifndef MIP_NO_WINDOW_BUFFERING

  bool createBuffer(uint32_t AWidth, uint32_t AHeight) {
    //MIP_Print("%i,%i\n",AWidth,AHeight);
    #ifdef MIP_WINDOW_BUFFER_BITMAP
      MBufferBitmap = new MIP_Bitmap(AWidth,AHeight);
      MIP_Assert(MBufferBitmap);
      MBufferPainter = new MIP_Painter(MBufferBitmap);
    #else
      MBufferSurface = new MIP_Surface(this,AWidth,AHeight);
      MIP_Assert(MBufferSurface);
      MBufferPainter = new MIP_Painter(MBufferSurface);
    #endif
    MIP_Assert(MBufferPainter);
    MBufferWidth = AWidth;
    MBufferHeight = AHeight;
    return true;
  }

  //----------

  void deleteBuffer() {
    //MIP_PRINT;
    delete MBufferPainter;
    MBufferPainter = nullptr;
    #ifdef MIP_WINDOW_BUFFER_BITMAP
      delete MBufferBitmap;
      MBufferBitmap = nullptr;
    #else
      delete MBufferSurface;
      MBufferSurface = nullptr;
    #endif
  }

  //----------

  bool resizeBuffer(uint32_t AWidth, uint32_t AHeight) {
    //MIP_Print("%i,%i\n",AWidth,AHeight);
    uint32_t w = MIP_NextPowerOfTwo(AWidth);
    uint32_t h = MIP_NextPowerOfTwo(AHeight);
    if ((w != MBufferWidth) || (h != MBufferHeight)) {
      deleteBuffer();
      createBuffer(w,h);
    }
    return true;
  }

  //----------

  void paintBuffer(MIP_FRect ARect) {
    //MIP_Print("%.0f,%.0f,%.0f,%.0f\n",ARect.x,ARect.y,ARect.w,ARect.h);
    //MBufferPainter->setScale(2,2);
    paintWidgets(MBufferPainter,ARect);
    //MBufferPainter->resetScale();
    MBufferPainter->flush();
    #ifdef MIP_WINDOW_BUFFER_BITMAP
      MWindowPainter->drawBitmap(ARect.x,ARect.y,MBufferBitmap);
    #else
      #ifdef MIP_WINDOW_SCALE_WINDOW
        MWindowPainter->drawImage(ARect,MBufferSurface,MRect);
      #else
        MWindowPainter->drawImage(ARect.x,ARect.y,MBufferSurface,ARect);
        //blit(ARect.x,ARect.y,MBufferSurface,ARect.x,ARect.y,ARect.w,ARect.h);
      #endif
    #endif

    //MWindowPainter->flush();


  }

  #endif // MIP_NO_WINDOW_BUFFERING

//------------------------------
public:
//------------------------------

  void open() override {
    setOwnerWindow(this);
    //setSkin();
    //setScale(1.5,true); // clipping isn't scfaled...
    alignWidgets();
    MIP_ImplementedWindow::open();
  }

//------------------------------
public: // MIP_BaseWindow
//------------------------------

  //void on_window_open() override {
  //  MIP_Print("\n");
  //}

  //----------

  //void on_window_close() override {
  //  MIP_Print("\n");
  //}

  //----------

  //void on_window_move(int32_t AXpos, int32_t AYpos) override {
  //}

  //----------

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    //MIP_Print("%i,%i scale %.2f,%.2f\n",AWidth,AHeight,MWindowXScale,MWindowYScale);
    //#ifndef MIP_WINDOW_SCALE_EVENTS
      resizeWindow(AWidth,AHeight);
    //#endif
  }

  //----------

  //TODO: focus

  void on_window_keyPress(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) override {
    if (MKeyInputWidget) MKeyInputWidget->on_widget_keyPress(AKey,0,AState);
  }

  //----------

  //TODO: focus

  void on_window_keyRelease(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) override {
    if (MKeyInputWidget) MKeyInputWidget->on_widget_keyRelease(AKey,0,AState);
  }

  //----------

  /*
    * scale
  */

  void on_window_mouseClick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    //#ifdef MIP_WINDOW_SCALE_EVENTS
    //  AXpos *= MWindowXScale;
    //  AYpos *= MWindowXScale;
    //#endif
    //
    //bool double_click = false;
    //if ((ATimeStamp - MPrevClickTime) < MIP_GUI_DBLCLICK_MS) {
    //  double_click = true;
    //}
    //MPrevClickTime  = ATimeStamp;
    //
    MResizingWidth  = MRect.w;
    MResizingHeight = MRect.h;
    MMouseClickedX  = AXpos;
    MMouseClickedY  = AYpos;
    MMousePrevX     = AXpos;
    MMousePrevY     = AYpos;
    MMouseDragX     = AXpos;
    MMouseDragY     = AYpos;
    if (MMouseHoverWidget) {
      if (MMouseHoverWidget->state.active) {
        grabMouseCursor();
        MMouseClickedWidget = MMouseHoverWidget;
        MMouseClickedWidget->on_widget_mouseClick(AXpos,AYpos,AButton,AState,ATimeStamp);
      }
    }
  }

  //----------

  /*
    * scale
  */

  void on_window_mouseRelease(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    //#ifdef MIP_WINDOW_SCALE_EVENTS
    //  AXpos *= MWindowXScale;
    //  AYpos *= MWindowXScale;
    //#endif
    if (MMouseClickedWidget) {
      MMouseClickedWidget->on_widget_mouseRelease(AXpos,AYpos,AButton,AState,ATimeStamp);
      releaseMouseCursor();
      releaseHoverWidget(MMouseClickedWidget,AXpos,AYpos);
      MMouseClickedWidget = nullptr; // must be after releaseHoverWidget
    }
  }

  //----------

  /*
    * scale
  */

  void on_window_mouseMove(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATimeStamp) override {
    //#ifdef MIP_WINDOW_SCALE_EVENTS
    //  AXpos *= MWindowXScale;
    //  AYpos *= MWindowXScale;
    //#endif
    MMouseX = AXpos;
    MMouseY = AYpos;
    if (MMouseClickedWidget) {
      if (MMouseLockedWidget) {
        if ((AXpos == MMouseClickedX) && (AYpos == MMouseClickedY)) {
          MMousePrevX = AXpos;
          MMousePrevY = AYpos;
          return;
        }
        int32_t deltax = AXpos - MMouseClickedX;
        int32_t deltay = AYpos - MMouseClickedY;
        MMouseDragX += deltax;
        MMouseDragY += deltay;
        MMouseClickedWidget->on_widget_mouseMove(MMouseDragX,MMouseDragY,AState,ATimeStamp);
        setMouseCursorPos(MMouseClickedX,MMouseClickedY);
      }
      else { // no captured widgets
        MMouseClickedWidget->on_widget_mouseMove(AXpos,AYpos,AState,ATimeStamp);
      }
    }
    else { // no widget clicked
      updateHoverWidget(AXpos,AYpos,ATimeStamp);
      if (MMouseHoverWidget) MMouseHoverWidget->on_widget_mouseMove(AXpos,AYpos,AState,ATimeStamp);
    }
    MMousePrevX = AXpos;
    MMousePrevY = AYpos;
  }

  //----------

  /*
    * scale
  */

  void on_window_mouseEnter(int32_t AXpos, int32_t AYpos, uint32_t ATimeStamp) override {
    //#ifdef MIP_WINDOW_SCALE_EVENTS
    //  AXpos *= MWindowXScale;
    //  AYpos *= MWindowXScale;
    //#endif
    if (!MMouseClickedWidget) {
      MMouseHoverWidget = nullptr;
      updateHoverWidget(AXpos,AYpos,ATimeStamp);
    }
  }

  //----------

  //TODO: if not dragging?

  /*
    * scale
  */

  void on_window_mouseLeave(int32_t AXpos, int32_t AYpos, uint32_t ATimeStamp) override {
    //#ifdef MIP_WINDOW_SCALE_EVENTS
    //  AXpos *= MWindowXScale;
    //  AYpos *= MWindowXScale;
    //#endif
    if (!MMouseClickedWidget) {
      //MMouseHoverWidget = MIP_NULL;
      updateHoverWidget(AXpos,AYpos,ATimeStamp);
      //on_widget_leave(AXpos,AYpos,MIP_NULL);
    }
  }

  //----------

  //void on_window_timer() override {
  //}

  //----------

  //void on_window_idle() override {
  //}

  //----------

  //void on_window_clientMessage(uint32_t AData, void* APtr) override {
  //}

  //----------

  /*
    * scale
  */

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    //#ifdef MIP_WINDOW_SCALE_EVENTS
    //  AXpos *= MWindowXScale;
    //  AYpos *= MWindowXScale;
    //  AWidth *= MWindowYScale;
    //  AHeight *= MWindowYScale;
    //#endif
    MIP_FRect rect = MIP_FRect(AXpos,AYpos,AWidth,AHeight);
    if (MFillWindowBackground) fillWindowBackground(rect);
    #ifdef MIP_NO_WINDOW_BUFFERING
      paintWindow(rect);
    #else
      paintBuffer(rect);
    #endif
  }

//------------------------------
public: // MIP_Widget
//------------------------------

  // called when a widget updates its value
  // MListener = MIP_Editor

  void do_widget_update(MIP_Widget* AWidget, uint32_t AMode=0) override {
    if (MListener) MListener->on_window_listener_updateWidget(AWidget);
  }

  //----------

  /*
    here we lose all information about what widget to redraw :-/
    later, when the window gets the expose event, we just know the rect

    should we notify the listener (editor), and let it handle it?
  */

  void do_widget_redraw(MIP_Widget* AWidget, MIP_FRect ARect, uint32_t AMode=0) override {
    invalidate(ARect.x,ARect.y,ARect.w,ARect.h);
  }

  //----------

  void do_widget_realign(MIP_Widget* AWidget, bool ARecursive=true) override {
  }

  //----------

  /*
    widget has been resized
  */

  void do_widget_resized(MIP_Widget* ASender, float ADeltaX=0.0f, float ADeltaY=0.0f, uint32_t AMode=0) override {
    MResizingWidth += ADeltaX;
    MResizingHeight += ADeltaY;
    if (MListener) MListener->on_window_listener_resize(MResizingWidth,MResizingHeight);
  }

  //----------

  void do_widget_setMouseCursor(MIP_Widget* AWidget, int32_t ACursor) override {
    switch(ACursor) {
      case MIP_CURSOR_GRAB:
        grabMouseCursor();
        MMouseLockedWidget = AWidget;
        break;
      case MIP_CURSOR_RELEASE:
        releaseMouseCursor();
        MMouseLockedWidget = nullptr;
        break;
      case MIP_CURSOR_SHOW:
        showMouseCursor();
        break;
      case MIP_CURSOR_HIDE:
        hideMouseCursor();
        break;
      default:
        setMouseCursor(ACursor);
        break;
    }
  }

  //----------

  void do_widget_setMouseCursorPos(MIP_Widget* ASender, float AXpos, float AYpos) override {
    setMouseCursorPos(AXpos,AYpos);
  }

  //----------

  void do_widget_setHint(MIP_Widget* AWidget, const char* AHint, uint32_t AType) override {
  }

  //----------

  void do_widget_notify(MIP_Widget* AWidget, uint32_t AValue=0) override {
  }

  //----------

  void do_widget_setModal(MIP_Widget* AWidget) override {
    MModalWidget = AWidget;
    updateHoverWidget(MMouseX,MMouseY);
  }

  //----------

  void do_widget_captureMouse(MIP_Widget* AWidget) override {
    MMouseLockedWidget = AWidget;
  }

  //----------

  void do_widget_captureKeyboard(MIP_Widget* AWidget) override {
    MKeyInputWidget = AWidget;
  }

};

//----------------------------------------------------------------------
#endif
