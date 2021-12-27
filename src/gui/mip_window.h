#ifndef mip_window_included
#define mip_window_included
//----------------------------------------------------------------------

//#define MIP_NO_WINDOW_BUFFERING

//

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

typedef MIP_ImplementedWindow MIP_BasicWindow;;

//----------------------------------------------------------------------

class MIP_WindowListener {
public:
  virtual void do_window_updateWidget(MIP_Widget* AWidget) {}
  virtual void do_window_redrawWidget(MIP_Widget* AWidget) {}
};

//----------------------------------------------------------------------
//
// window
//
//----------------------------------------------------------------------

class MIP_Window
: public MIP_ImplementedWindow
, public MIP_Widget {

//------------------------------
private:
//------------------------------

  MIP_WindowListener* MListener         = nullptr;
  MIP_Widget*         MHoverWidget      = nullptr;
  MIP_Widget*         MClickedWidget    = nullptr;
  MIP_Widget*         MCapturedWidget   = nullptr;
  MIP_Painter*        MWindowPainter    = nullptr;
  bool                MFillBackground   = true;
  MIP_Color           MBackgroundColor  = MIP_Color(0.5);
  #ifndef MIP_NO_WINDOW_BUFFERING
  MIP_Painter*        MBufferPainter    = nullptr;
  MIP_Surface*        MBufferSurface    = nullptr;
  #endif

//------------------------------
public:
//------------------------------

  MIP_Window(int32_t AWidth, int32_t AHeight, const char* ATitle="", void* AParentPtr=nullptr)
  : MIP_ImplementedWindow(AWidth,AHeight,ATitle,AParentPtr)
  , MIP_Widget(MIP_FRect(AWidth,AHeight)) {
    MName = "MIP_Window";
    MWindowPainter = new MIP_Painter(this);
    #ifndef MIP_NO_WINDOW_BUFFERING
    createBuffer(AWidth,AHeight);
    #endif
  }

  //----------

  virtual ~MIP_Window() {
    delete MWindowPainter;
    #ifndef MIP_NO_WINDOW_BUFFERING
    deleteBuffer();
    #endif
  }

//------------------------------
public:
//------------------------------

  void setListener(MIP_WindowListener* l) { MListener = l; }
  void setFillBackground(bool s=true)     { MFillBackground = s; }
  void setBackgroundColor(MIP_Color c)    { MBackgroundColor = c; }
//------------------------------
public:
//------------------------------

  void updateHoverWidget(int32_t AXpos, int32_t AYpos) {
    bool is_dragging = (MClickedWidget != nullptr);
    MIP_Widget* hover = findWidget(AXpos,AYpos);
    if (hover != MHoverWidget) {
      if (is_dragging) {
        if (MHoverWidget) MHoverWidget->on_widget_mouseDragLeave(AXpos,AYpos,hover);
        hover->on_widget_mouseDragEnter(AXpos,AYpos,MHoverWidget);
        MHoverWidget = hover;
      }
      else {
        if (MHoverWidget) MHoverWidget->on_widget_mouseLeave(AXpos,AYpos,hover);
        hover->on_widget_mouseEnter(AXpos,AYpos,MHoverWidget);
        MHoverWidget = hover;
      }
    }
  }

  //----------

  void releaseHoverWidget(int32_t AXpos, int32_t AYpos) {
    MIP_Widget* hover = findWidget(AXpos,AYpos);
    if (hover != MClickedWidget) {
      if (MClickedWidget) MClickedWidget->on_widget_mouseLeave(AXpos,AYpos,hover);
      hover->on_widget_mouseEnter(AXpos,AYpos,MHoverWidget);
    }
    MHoverWidget = hover;
  }

//------------------------------
public: // window
//------------------------------

  void paintWindow(MIP_FRect ARect) {
    paintWidgets(MWindowPainter,ARect);
  }

//------------------------------
public: // buffer
//------------------------------

  #ifndef MIP_NO_WINDOW_BUFFERING

  bool createBuffer(uint32_t AWidth, uint32_t AHeight) {
    MBufferSurface = new MIP_Surface(this,AWidth,AHeight);
    MBufferPainter = new MIP_Painter(MBufferSurface);
    return true;
  }

  //----------

  void deleteBuffer() {
    delete MBufferPainter;
    delete MBufferSurface;
  }

  //----------

  bool resizeBuffer(uint32_t AWidth, uint32_t AHeight) {
    deleteBuffer();
    createBuffer(AWidth,AHeight);
    return true;
  }

  //----------

  void paintBuffer(MIP_FRect ARect) {
    if (MFillBackground) {
      MBufferPainter->fillRectangle(ARect,MBackgroundColor);
    }
    paintWidgets(MBufferPainter,ARect);
    blit(ARect.x,ARect.y,MBufferSurface,ARect.x,ARect.y,ARect.w,ARect.h);
  }

  #endif

//------------------------------
public: // MIP_BaseWindow
//------------------------------

  //----------

  void paintWidgets(MIP_Painter* APainter, MIP_FRect ARect) {
    if (MChildren.size() > 0) {
      for (int32_t i = MChildren.size()-1; i >= 0; i--) {
        MIP_Widget* child = MChildren[i];
        if (child->isVisible()) {
          MIP_FRect rect = child->getRect();
          if (rect.touches(ARect)) {
            child->on_widget_paint(APainter,ARect);
          }
        }
      }
    }
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

  void on_window_move(int32_t AXpos, int32_t AYpos) override {
    //MIP_Print("x %i y %i\n",AXpos,AYpos);
  }

  //----------

  // TOD: realign

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    //MIP_Print("w %i h %i\n",AWidth,AHeight);
  }

  //----------

  //TODO: focus

  void on_window_keyPress(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) override {
    //MIP_Print("k %i s %i ts %i\n",AKey,AState,ATimeStamp);
  }

  //----------

  //TODO: focus

  void on_window_keyRelease(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) override {
    //MIP_Print("k %i s %i ts %i\n",AKey,AState,ATimeStamp);
  }

  //----------

  void on_window_mouseClick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    //MIP_Print("x %i y %i b %i s %i ts %i\n",AXpos,AYpos,AButton,AState,ATimeStamp);
    //if (MHoverWidget != this) {
      MClickedWidget = MHoverWidget;
      MCapturedWidget = MHoverWidget;
      MHoverWidget->on_widget_mouseClick(AXpos,AYpos,AButton,AState,ATimeStamp);
    //}
  }

  //----------

  void on_window_mouseRelease(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    //MIP_Print("x %i y %i b %i s %i ts %i\n",AXpos,AYpos,AButton,AState,ATimeStamp);
    if (MCapturedWidget) {
      MCapturedWidget->on_widget_mouseRelease(AXpos,AYpos,AButton,AState,ATimeStamp);
      MHoverWidget = MClickedWidget;
      releaseHoverWidget(AXpos,AYpos);
      MCapturedWidget = nullptr;
      MClickedWidget = nullptr;
    }
  }

  //----------

  void on_window_mouseMove(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATimeStamp) override {
    //MIP_Print("x %i y %i s %i ts %i\n",AXpos,AYpos,AState,ATimeStamp);
    updateHoverWidget(AXpos,AYpos);
    if (MCapturedWidget) {
      MCapturedWidget->on_widget_mouseMove(AXpos,AYpos,AState,ATimeStamp);
    }
  }

  //----------

  void on_window_mouseEnter(int32_t AXpos, int32_t AYpos, uint32_t ATimeStamp) override {
    //MIP_Print("x %i y %i ts %i\n",AXpos,AYpos,ATimeStamp);
    updateHoverWidget(AXpos,AYpos);
  }

  //----------

  void on_window_mouseLeave(int32_t AXpos, int32_t AYpos, uint32_t ATimeStamp) override {
    //MIP_Print("x %i y %i ts %i\n",AXpos,AYpos,ATimeStamp);
    if (MHoverWidget) {
      MHoverWidget->on_widget_mouseLeave(AXpos,AYpos, nullptr);
    }
    MHoverWidget = nullptr;
  }

  //----------

  void on_window_timer() override {
    //MIP_Print("\n");
  }

  //----------

  void on_window_idle() override {
    //MIP_Print("\n");
  }

  //----------

  void on_window_clientMessage(uint32_t AData, void* APtr) override {
    //MIP_Print("data %i ptr %p\n",AData,APtr);
  }

  //----------

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    //MIP_Print("x %i y %i w %i h %i\n",AXpos,AYpos,AWidth,AHeight);
    MIP_FRect rect = MIP_FRect(AXpos,AYpos,AWidth,AHeight);
    #ifdef MIP_NO_WINDOW_BUFFERING
    paintWindow(rect);
    #else
    paintBuffer(rect);
    #endif
  }

//------------------------------
public: // MIP_Widget
//------------------------------

  void do_widget_update(MIP_Widget* AWidget) override {
    MIP_PRINT;
    if (MListener) MListener->do_window_updateWidget(AWidget);
  }

  //----------

  // TODO: queue widget(s), and redraw all later
  // timer, idle..

  void do_widget_redraw(MIP_Widget* AWidget) override {
    MIP_PRINT;
    if (MListener) MListener->do_window_redrawWidget(AWidget);
  }

  //----------

  //void do_widget_realign(MIP_Widget* AWidget) override {
  //}

  //----------

  void do_widget_setMouseCursor(MIP_Widget* AWidget, int32_t ACursor) override {
    switch(ACursor) {
      case MIP_CURSOR_GRAB:
        grabMouseCursor();
        break;
      case MIP_CURSOR_RELEASE:
        releaseMouseCursor();
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

  void do_widget_setHint(MIP_Widget* AWidget, const char* AHint) override {
    MIP_Print("%s\n",AHint);
  }

};

//----------------------------------------------------------------------
#endif
