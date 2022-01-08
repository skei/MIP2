#ifndef mip_window_included
#define mip_window_included
//----------------------------------------------------------------------

#define MIP_NO_WINDOW_BUFFERING

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
  MIP_Widget*         MModalWidget      = nullptr;
  MIP_Widget*         MClickedWidget    = nullptr;
  MIP_Widget*         MCapturedWidget   = nullptr;

  MIP_Painter*        MWindowPainter    = nullptr;
  #ifndef MIP_NO_WINDOW_BUFFERING
  uint32_t            MBufferWidth      = 0;
  uint32_t            MBufferHeight     = 0;
  MIP_Painter*        MBufferPainter    = nullptr;
  MIP_Surface*        MBufferSurface    = nullptr;
  #endif

  bool                MFillWindowBackground   = true;
  MIP_Color           MWindowBackgroundColor  = MIP_Color(0.5);

  //KODE_Widget*  MMouseHoverWidget       = KODE_NULL;
  //KODE_Widget*  MMouseModalWidget       = KODE_NULL;
  //KODE_Widget*  MMouseClickedWidget     = KODE_NULL;
  //KODE_Widget*  MMouseLockedWidget      = KODE_NULL;
  //KODE_Widget*  MKeyInputWidget         = KODE_NULL;
  int32_t       MMouseX                 = 0;
  int32_t       MMouseY                 = 0;
  int32_t       MMousePrevX             = 0;
  int32_t       MMousePrevY             = 0;
  int32_t       MMouseClickedX          = 0;
  int32_t       MMouseClickedY          = 0;
  uint32_t      MMouseClickedB          = 0;
  uint32_t      MMouseClickedS          = 0;
  int32_t       MMouseDragX             = 0;
  int32_t       MMouseDragY             = 0;
  uint32_t      MPrevClickTime          = 0;

//------------------------------
public:
//------------------------------

  MIP_Window(uint32_t AWidth, uint32_t AHeight, const char* ATitle="", MIP_WindowListener* AListener=nullptr, void* AParentPtr=nullptr)
  : MIP_ImplementedWindow(AWidth,AHeight,ATitle,AParentPtr)
  , MIP_Widget(MIP_FRect(AWidth,AHeight)) {
    MName = "MIP_Window";
    MListener = AListener;
    //MOwner = this;
    //setName("KODE_Window");
    //setRect(KODE_FRect(AWidth,AHeight));
    //flags.autoClip = true;
    MWindowPainter = new MIP_Painter(this);
    #ifndef MIP_NO_WINDOW_BUFFERING
    createBuffer(AWidth,AHeight);
    #endif


  }

  //----------

  virtual ~MIP_Window() {
    if (MWindowPainter) delete MWindowPainter;
    #ifndef MIP_NO_WINDOW_BUFFERING
    deleteBuffer();
    #endif
  }

//------------------------------
public:
//------------------------------

  //virtual bool isBuffered() {
  //  #ifdef KODE_NO_WINDOW_BUFFERING
  //  return false;
  //  #else
  //  return true;
  //  #endif
  //}

  //virtual KODE_Painter* getPainter() {
  MIP_Painter* getPainter() {
    #ifdef MIP_NO_WINDOW_BUFFERING
    return MWindowPainter;
    #else
    return MBufferPainter;
    #endif
  }

  MIP_Widget* getHoverWidget()    { return MHoverWidget; }
  MIP_Widget* getModalWidget()    { return MModalWidget; }
  MIP_Widget* getClickedWidget()  { return MClickedWidget; }

  //MIP_Widget* getMouseLockedWidget()  { return MMouseLockedWidget; }
  //MIP_Widget* getKeyInputWidget()      { return MKeyInputWidget; }

//------------------------------
public:
//------------------------------

//  //void setListener(MIP_WindowListener* l) { MListener = l; }
  void setWindowFillBackground(bool s=true)     { MFillWindowBackground = s; }
  void setWindowBackgroundColor(MIP_Color c)    { MWindowBackgroundColor = c; }

  //----------

  void fillWindowBackground(MIP_FRect ARect) {
    #ifdef MIP_NO_WINDOW_BUFFERING
      MWindowPainter->fillRectangle(ARect,MBackgroundColor);
    #else
      MBufferPainter->fillRectangle(ARect,MBackgroundColor);
    #endif
  }

//------------------------------
public:
//------------------------------

//  void paintWidget(KODE_Widget* AWidget, KODE_FRect ARect, uint32_t AMode=0) {
//
//    #ifdef KODE_NO_WINDOW_BUFFERING
//      //if (flags.autoClip) MWindowPainter->pushClip(ARect);
//      AWidget->on_widget_paint(MWindowPainter,ARect,AMode);
//      //if (flags.autoClip) MWindowPainter->popClip();
//    #else
//      //if (flags.autoClip) MBufferPainter->pushClip(ARect);
//      AWidget->on_widget_paint(MBufferPainter,ARect,AMode);
//      //if (flags.autoClip) MBufferPainter->popClip();
//      MWindowPainter->drawBitmap(ARect.x,ARect.y,MBufferSurface,ARect);
//    #endif
//  }

  //----------

//  void resizeWindow(uint32_t AWidth, uint32_t AHeight) {
//    #ifndef KODE_NO_WINDOW_BUFFERING
//      resizeBuffer(AWidth,AHeight);
//    #endif
//    MRect.w = AWidth;
//    MRect.h = AHeight;
//    alignChildren(/*this*/);
//    if (MWindowPainter) MWindowPainter->resize(AWidth,AHeight);
//  }

  //----------

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
    //MIP_Print("x %.2f y %.2f w %.2f h %.2f\n",ARect.x,ARect.y,ARect.w,ARect.h);;
    paintWidgets(MWindowPainter,ARect);
  }

//------------------------------
public: // buffer
//------------------------------

  #ifndef MIP_NO_WINDOW_BUFFERING

  bool createBuffer(uint32_t AWidth, uint32_t AHeight) {
    MBufferSurface = new MIP_Surface(this,AWidth,AHeight);
    MBufferPainter = new MIP_Painter(MBufferSurface);
    MBufferWidth = AWidth;
    MBufferHeight = AHeight;
    return true;
  }

  //----------

  void deleteBuffer() {
    delete MBufferPainter;
    delete MBufferSurface;
    MBufferPainter = nullptr;
    MBufferSurface = nullptr;
  }

  //----------

  bool resizeBuffer(uint32_t AWidth, uint32_t AHeight) {
    //deleteBuffer();
    //createBuffer(AWidth,AHeight);
    //return true;
    uint32_t w = MIP_NextPowerOfTwo(AWidth);
    uint32_t h = MIP_NextPowerOfTwo(AHeight);
    if ((w != MBufferWidth) || (h != MBufferHeight)) {
      deleteBuffer();
      createBuffer(AWidth,AHeight);
    }
    return true;
  }

  //----------

  void paintBuffer(MIP_FRect ARect) {
    //MIP_Print("x %.2f y %.2f w %.2f h %.2f\n",ARect.x,ARect.y,ARect.w,ARect.h);;
    //if (MFillBackground) {
    //  MBufferPainter->fillRectangle(ARect,MBackgroundColor);
    //}
    paintWidgets(MBufferPainter,ARect);
    blit(ARect.x,ARect.y,MBufferSurface,ARect.x,ARect.y,ARect.w,ARect.h);
  }

  #endif

//------------------------------
public: // window
//------------------------------

  void open() override {
    attachWindow(this);
    alignWidgets();
    MIP_ImplementedWindow::open();
    //#ifndef KODE_PLUGIN_EXE
    //  //on_window_paint(0,0,MRect.w,MRect.h);
    //  paintWidget(this,MRect,0);
    //#endif
  }

  //void close() override {
  //  attachWindow(nullptr);
  //  MIP_ImplementedWindow::close();
  //}

  // -> MIP_Widget

//  void paintWidgets(MIP_Painter* APainter, MIP_FRect ARect) {
//    if (MChildren.size() > 0) {
//      for (int32_t i = MChildren.size()-1; i >= 0; i--) {
//        MIP_Widget* child = MChildren[i];
//        if (child->isVisible()) {
//          MIP_FRect rect = child->getRect();
//          if (rect.touches(ARect)) {
//            child->on_widget_paint(APainter,ARect);
//          }
//        }
//      }
//    }
//  }

  //----------

  void paintWidget(MIP_Widget* AWidget) {
    MIP_FRect rect = AWidget->getRect();
    //MIP_Print("x %.2f y %.2f w %.2f h %.2f\n",rect.x,rect.y,rect.w,rect.h);
    #ifdef MIP_NO_WINDOW_BUFFERING
    paintWindow(rect);
    #else
    paintBuffer(rect);
    #endif
  };

  //----------

  void paint(MIP_FRect ARect) {
    //MIP_Print("x %.2f y %.2f w %.2f h %.2f\n",ARect.x,ARect.y,ARect.w,ARect.h);
    #ifdef MIP_NO_WINDOW_BUFFERING
    paintWindow(ARect);
    #else
    paintBuffer(ARect);
    #endif
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
    //resizeWindow(AWidth,AHeight);
    alignWidgets();
  }

  //----------

  //TODO: focus

  void on_window_keyPress(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) override {
    //MIP_Print("k %i s %i ts %i\n",AKey,AState,ATimeStamp);
//    if (MKeyInputWidget) MKeyInputWidget->on_widget_keyPress(AKey,0,AState,ATimeStamp);
  }

  //----------

  //TODO: focus

  void on_window_keyRelease(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) override {
    //MIP_Print("k %i s %i ts %i\n",AKey,AState,ATimeStamp);
//    if (MKeyInputWidget) MKeyInputWidget->on_widget_keyRelease(AKey,0,AState,ATimeStamp);
  }

  //----------

  void on_window_mouseClick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    //MIP_Print("x %i y %i b %i s %i ts %i\n",AXpos,AYpos,AButton,AState,ATimeStamp);
//    //bool double_click = false;
//    //if ((ATimeStamp - MPrevClickTime) < KODE_GUI_DBLCLICK_MS) {
//    //  double_click = true;
//    //}
//    //MPrevClickTime = ATimeStamp;
//    MMouseClickedX  = AXpos;
//    MMouseClickedY  = AYpos;
//    MMousePrevX     = AXpos;
//    MMousePrevY     = AYpos;
//    MMouseDragX     = AXpos;
//    MMouseDragY     = AYpos;
//    if (MMouseHoverWidget) {
//      if (MMouseHoverWidget->flags.active) {
//        grabMouseCursor();
//        MMouseClickedWidget = MMouseHoverWidget;
//        MMouseClickedWidget->on_widget_mouseClick(AXpos,AYpos,AButton,AState,ATimeStamp);
//      }
//    }
    //if (MHoverWidget != this) {
      MClickedWidget = MHoverWidget;
      MCapturedWidget = MHoverWidget;
      MHoverWidget->on_widget_mouseClick(AXpos,AYpos,AButton,AState,ATimeStamp);
    //}
  }

  //----------

  void on_window_mouseRelease(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    //MIP_Print("x %i y %i b %i s %i ts %i\n",AXpos,AYpos,AButton,AState,ATimeStamp);
//    if (MMouseClickedWidget) {
//      MMouseClickedWidget->on_widget_mouseRelease(AXpos,AYpos,AButton,AState,ATimeStamp);
//      releaseMouseCursor();
//      releaseHoverWidget(MMouseClickedWidget,AXpos,AYpos,ATimeStamp);
//      MMouseClickedWidget = KODE_NULL; // must be after releaseHoverWidget
//    }
//    //else {
//    //  on_widget_mouseRelease(AXpos,AYpos,AButton,AState);
//    //}
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

//    MMouseX = AXpos;
//    MMouseY = AYpos;
//    if (MMouseClickedWidget) {
//      if (MMouseLockedWidget) {
//        if ((AXpos == MMouseClickedX) && (AYpos == MMouseClickedY)) {
//          MMousePrevX = AXpos;
//          MMousePrevY = AYpos;
//          return;
//        }
//        int32_t deltax = AXpos - MMouseClickedX;
//        int32_t deltay = AYpos - MMouseClickedY;
//        MMouseDragX += deltax;
//        MMouseDragY += deltay;
//        MMouseClickedWidget->on_widget_mouseMove(MMouseDragX,MMouseDragY,AState,ATimeStamp);
//        setMouseCursorPos(MMouseClickedX,MMouseClickedY);
//      }
//      else { // no captured widgets
//        MMouseClickedWidget->on_widget_mouseMove(AXpos,AYpos,AState,ATimeStamp);
//      }
//    }
//    else { // no widget clicked
//      updateHoverWidget(AXpos,AYpos,ATimeStamp);
//      if (MMouseHoverWidget) MMouseHoverWidget->on_widget_mouseMove(AXpos,AYpos,AState,ATimeStamp);
//    }
//    MMousePrevX = AXpos;
//    MMousePrevY = AYpos;

    updateHoverWidget(AXpos,AYpos);
    if (MCapturedWidget) {
      MCapturedWidget->on_widget_mouseMove(AXpos,AYpos,AState,ATimeStamp);
    }
  }

  //----------

  void on_window_mouseEnter(int32_t AXpos, int32_t AYpos, uint32_t ATimeStamp) override {
    //MIP_Print("x %i y %i ts %i\n",AXpos,AYpos,ATimeStamp);
//    if (!MMouseClickedWidget) {
//      MMouseHoverWidget = KODE_NULL;
//      updateHoverWidget(AXpos,AYpos,ATimeStamp);
//      //on_widget_enter(AXpos,AYpos,KODE_NULL);
//    }
    updateHoverWidget(AXpos,AYpos);
  }

  //----------

  //TODO: if not dragging?

  void on_window_mouseLeave(int32_t AXpos, int32_t AYpos, uint32_t ATimeStamp) override {
    //MIP_Print("x %i y %i ts %i\n",AXpos,AYpos,ATimeStamp);
//    if (!MMouseClickedWidget) {
//      //MMouseHoverWidget = KODE_NULL;
//      updateHoverWidget(AXpos,AYpos,ATimeStamp);
//      //on_widget_leave(AXpos,AYpos,KODE_NULL);
//    }
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

  void do_widget_update(MIP_Widget* AWidget, uint32_t AMode=0) override {
    //MIP_PRINT;
    if (MListener) MListener->do_window_updateWidget(AWidget);
  }

  //----------

  // TODO: queue widget(s), and redraw all later
  // timer, idle..

  void do_widget_redraw(MIP_Widget* AWidget, MIP_FRect ARect, uint32_t AMode=0) override {
    //MIP_PRINT;
//    if (MListener) MListener->do_window_redrawWidget(AWidget);
    invalidate(ARect.x,ARect.y,ARect.w + 1,ARect.h + 1);
  }

  //----------

  void do_widget_realign(MIP_Widget* AWidget, uint32_t AMode=0) override {
    //MIP_PRINT;
  }

  //----------

  /*
    widget has been resized
    notify parent of widget, and realign/redraw
  */

//  void do_widget_resized(KODE_Widget* ASender, float ADeltaX=0.0f, float ADeltaY=0.0f) override {
//    //KODE_Widget* parent = ASender->getParent();
//    //if (parent) {
//    //  parent->alignChildren();
//    //  do_widget_redraw(parent,parent->getRect(),0);
//    //}
//    alignChildren();
//    redraw();
//  }

  //----------


  //----------

  void do_widget_setMouseCursor(MIP_Widget* AWidget, int32_t ACursor) override {
    switch(ACursor) {
      case MIP_CURSOR_GRAB:
        grabMouseCursor();
        //MMouseLockedWidget = AWidget;
        break;
      case MIP_CURSOR_RELEASE:
        releaseMouseCursor();
        //MMouseLockedWidget = nullptr;
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

  void do_widget_setHint(MIP_Widget* AWidget, const char* AHint) override {
    //MIP_Print("%s\n",AHint);
  }

  //----------

  void do_widget_notify(MIP_Widget* AWidget, uint32_t AValue=0) override {
  }

  //----------

//  void do_widget_grabMouseCursor(KODE_Widget* ASender) override {
//    //if (ASender) grabMouseCursor();
//    //else releaseMouseCursor();
//    MMouseLockedWidget = ASender;
//  }

  //----------

//  void do_widget_grabKeyboard(KODE_Widget* AWidget) override {
//    //KODE_Print("%p\n",AWidget);
//    MKeyInputWidget = AWidget;
//  }

  //----------

//  void do_widget_grabModal(KODE_Widget* AWidget) override {
//    //KODE_Print("%p\n",AWidget);
//    MMouseModalWidget = AWidget;
//    //if (AWidget)
//      updateHoverWidget(MMouseX,MMouseY);
//    //else
//    //  releaseHoverWidget(MMouseClickedWidget,MMouseX,MMouseY,0);
//  }

};

//----------------------------------------------------------------------
#endif
