#ifndef mip_xcb_window_included
#define mip_xcb_window_included
//----------------------------------------------------------------------

/*
  TODO:
    invalidate:
    send user/client message, with rect and widget, so we can optimize
    redrawing.. draw only from widget and downwards, so we don't have to
    draw everything from the window/background and up..
*/

//----------------------------------------------------------------------

#include "mip.h"
#include "base/system/mip_time.h"
#include "base/types/mip_color.h"
#include "gui/mip_drawable.h"
#include "gui/mip_widget.h"
#include "gui/xcb/mip_xcb.h"
#include "gui/xcb/mip_xcb_utils.h"

#ifdef MIP_USE_CAIRO
  #include "gui/cairo/mip_cairo.h"
#endif


#include "gui/mip_drawable.h"
#include "gui/base/mip_base_window.h"
//#include "gui/xcb/mip_xcb.h"
//#include "gui/xcb/mip_xcb_utils.h"

//  pid_t _mip_xcb_event_thread_pid = 0;
//  pid_t _mip_xcb_event_thread_tid = 0;

//----------------------------------------------------------------------

class MIP_XcbWindow
: public MIP_BaseWindow
, public MIP_Drawable {

//------------------------------
private:
//------------------------------

  xcb_connection_t*           MConnection                   = nullptr;

  xcb_screen_t*               MScreen                       = nullptr;
  uint32_t                    MScreenNum                    = 0;
  uint32_t                    MScreenWidth                  = 0;
  uint32_t                    MScreenHeight                 = 0;
  uint32_t                    MScreenDepth                  = 0;
  xcb_window_t                MScreenWindow                 = XCB_NONE;
  xcb_drawable_t              MScreenDrawable               = XCB_NONE;
  xcb_gcontext_t              MScreenGC                     = XCB_NONE;
  xcb_visualid_t              MScreenVisual                 = XCB_NONE;
  xcb_colormap_t              MScreenColormap               = XCB_NONE;

  xcb_window_t                MWindow                       = XCB_NONE;
  xcb_window_t                MWindowParent                 = XCB_NONE;
  int32_t                     MWindowXpos                   = 0;
  int32_t                     MWindowYpos                   = 0;
  int32_t                     MWindowWidth                  = 0;
  int32_t                     MWindowHeight                 = 0;
  bool                        MWindowMapped                 = false;
  bool                        MWindowExposed                = false;

  xcb_atom_t                  MWMProtocolsAtom              = XCB_NONE; // KWantQuitEvents
  xcb_atom_t                  MWMDeleteWindowAtom           = XCB_NONE; // KWantQuitEvents

  char*                       MExposeEventBuffer[32]        = {0};
  char*                       MClientMessageEventBuffer[32] = {0};
  xcb_expose_event_t*         MExposeEvent                  = (xcb_expose_event_t*)MExposeEventBuffer;
  xcb_client_message_event_t* MClientMessageEvent           = (xcb_client_message_event_t*)MClientMessageEventBuffer;

  xcb_pixmap_t                MEmptyPixmap                  = XCB_NONE;
  xcb_cursor_t                MHiddenCursor                 = XCB_NONE;
  xcb_cursor_t                MWindowCursor                 = XCB_NONE;
  bool                        MCursorHidden                 = false;

  xcb_key_symbols_t*          MKeySyms                      = nullptr;

  pthread_t                   MTimerThread                  = 0;
  bool                        MTimerThreadActive            = false;
  int32_t                     MTimerSleep                   = 20; // 20 ms = 50 hz
  pthread_t                   MEventThread                  = 0;
  bool                        MEventThreadActive            = false;
  bool                        MQuitEventLoop                = false;
  bool                        MUseEventThread               = false;

//------------------------------
private:
//------------------------------

  const char* MIP_XCB_WM_CURSORS[20] = {
    "left_ptr",           // 0 MIP_CURSOR_DEFAULT
    "left_ptr",           // MIP_CURSOR_ARROW
    "sb_up_arrow",        // MIP_CURSOR_ARROWUP
    "sb_down_arrow",      // MIP_CURSOR_ARROWDOWN
    "sb_left_arrow",      // MIP_CURSOR_ARROWLEFT
    "sb_right_arrow",     // MIP_CURSOR_ARROWRIGHT
    "sb_v_double_arrow",  // MIP_CURSOR_ARROWUPDOWN
    "sb_h_double_arrow",  // MIP_CURSOR_ARROWLEFTRIGHT
    "top_left_corner",    // MIP_CURSOR_ARROWDIAGLEFT
    "top_right_corner",   // MIP_CURSOR_ARROWDIAGRIGHT
    "fleur",              // MIP_CURSOR_MOVE
    "watch",              // MIP_CURSOR_WAIT
    "clock",              // MIP_CURSOR_ARROWWAIT
    "hand2",              // MIP_CURSOR_HAND
    "hand1",              // MIP_CURSOR_FINGER
    "crosshair",          // MIP_CURSOR_CROSS
    "pencil",             // MIP_CURSOR_PENCIL
    "plus",               // MIP_CURSOR_PLUS
    "question_arrow",     // MIP_CURSOR_QUESTION
    "xterm"               // MIP_CURSOR_IBEAM
  };

//------------------------------
public:
//------------------------------

  MIP_XcbWindow(uint32_t AWidth, uint32_t AHeight, const char* ATitle="", void* AParent=nullptr)
  : MIP_BaseWindow(AWidth,AHeight,ATitle,AParent) {
    MName = "MIP_XcbWindow";
    MUseEventThread = AParent ? true : false;
    connect();
    initGC();
    createWindow(AWidth,AHeight,ATitle,AParent);
    initMouseCursor();
    initKeyboard();
    initThreads();
    xcb_flush(MConnection);
  }

  //----------

  virtual ~MIP_XcbWindow() {
    cleanupThreads();
    cleanupKeyboard();
    cleanupMouseCursor();
    destroyWindow();
    cleanupGC();
    disconnect();
  }

//------------------------------
public: // drawable
//------------------------------

  bool                isWindow()          final { return true; }
  bool                isDrawable()        final { return true; }

  uint32_t            getWidth()          final { return MWindowWidth; }
  uint32_t            getHeight()         final { return MWindowHeight; }
  uint32_t            getDepth()          final { return MScreenDepth; }

  xcb_window_t        getXcbWindow()      final { return MWindow; }
  xcb_drawable_t      getXcbDrawable()    final { return MWindow; }
  xcb_connection_t*   getXcbConnection()  final { return MConnection; }
  xcb_visualid_t      getXcbVisual()      final { return MScreenVisual; }

  #ifdef MIP_USE_CAIRO

  bool isCairo() final {
    return true;
  }

  cairo_surface_t* createCairoSurface() final {
    cairo_surface_t* surface = cairo_xcb_surface_create(
      MConnection,
      MWindow,
      mip_xcb_find_visual(MConnection,MScreenVisual),
      MWindowWidth,
      MWindowHeight
    );
    return surface;
  }

  #endif

//------------------------------
private:
//------------------------------

  void connect() {
    int screen_num;
    MConnection = xcb_connect(nullptr,&screen_num);
    xcb_screen_iterator_t iter;
    iter = xcb_setup_roots_iterator(xcb_get_setup(MConnection));
    for (; iter.rem; --screen_num, xcb_screen_next(&iter)) {
      if (screen_num == 0) {
        MScreen = iter.data;
        break;
      }
    }
    MScreenNum      = screen_num;
    MScreenWidth    = MScreen->width_in_pixels;
    MScreenHeight   = MScreen->height_in_pixels;
    MScreenDepth    = MScreen->root_depth;
    MScreenWindow   = MScreen->root;
    MScreenColormap = MScreen->default_colormap;
    MScreenVisual   = MScreen->root_visual;
    MScreenGC       = xcb_generate_id(MConnection);
    MScreenDrawable = MScreen->root;
  }

  //----------

  void disconnect() {
    xcb_free_gc(MConnection,MScreenGC);
    xcb_disconnect(MConnection);
  }

  //----------

  void initGC() {
    MScreenGC = xcb_generate_id(MConnection);
    xcb_drawable_t draw = MScreen->root;
    uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
    uint32_t values[2];
    values[0] = MScreen->black_pixel;
    values[1] = MScreen->white_pixel;
    xcb_create_gc(MConnection, MScreenGC, draw, mask, values);
  }

  //----------

  void cleanupGC() {
    xcb_free_gc(MConnection,MScreenGC);
  }

  //----------

  void createWindow(uint32_t AWidth, uint32_t AHeight, const char* ATitle, void* AParent=nullptr) {
    if (AParent) MWindowParent = (xcb_window_t)(intptr_t)AParent;
    else MWindowParent = MScreen->root;
    uint32_t event_mask =
      XCB_EVENT_MASK_KEY_PRESS      |
      XCB_EVENT_MASK_KEY_RELEASE    |
      XCB_EVENT_MASK_BUTTON_PRESS   |
      XCB_EVENT_MASK_BUTTON_RELEASE |
      XCB_EVENT_MASK_ENTER_WINDOW   |
      XCB_EVENT_MASK_LEAVE_WINDOW   |
      XCB_EVENT_MASK_POINTER_MOTION |
      XCB_EVENT_MASK_EXPOSURE       |
      XCB_EVENT_MASK_STRUCTURE_NOTIFY;
    uint32_t window_mask =
      XCB_CW_BACK_PIXMAP  |
      XCB_CW_BORDER_PIXEL |
      XCB_CW_EVENT_MASK   |
      XCB_CW_COLORMAP;
    uint32_t window_mask_values[4] = {
      XCB_NONE,
      0,
      event_mask,
      MScreen->default_colormap
    };
    MWindow = xcb_generate_id(MConnection);
    xcb_create_window(
      MConnection,                    // connection
      XCB_COPY_FROM_PARENT,           // depth (same as root)
      MWindow,                        // window Id
      MWindowParent,                  // parent window
      0, 0,                           // x, y
      AWidth, AHeight,                // width, height
      0,                              // border_width
      XCB_WINDOW_CLASS_INPUT_OUTPUT,  // class
      MScreen->root_visual,           // visual
      window_mask,
      window_mask_values
    );
    if (AParent) {
      //reparent(AParent);
      removeDecorations();
    }
    else {
      setTitle(ATitle);
      wantDeleteWindowEvent();
    }
    MWindowXpos   = 0;
    MWindowYpos   = 0;
    MWindowWidth  = AWidth;
    MWindowHeight = AHeight;
  }

  //----------

  void destroyWindow() {
    xcb_destroy_window(MConnection,MWindow);
  }

  //----------

  //----------

  void initMouseCursor() {
    MEmptyPixmap = xcb_generate_id(MConnection);
    MHiddenCursor = xcb_generate_id(MConnection);
    xcb_create_pixmap(
      MConnection,
      1,
      MEmptyPixmap,
      MWindow,
      1,
      1
    );
    xcb_create_cursor(
      MConnection,
      MHiddenCursor,
      MEmptyPixmap,
      MEmptyPixmap,
      0, 0, 0,  // fore color
      0, 0, 0,  // back color
      0, 0      // x,y
    );
  }

  //----------

  void cleanupMouseCursor() {
    xcb_free_pixmap(MConnection, MEmptyPixmap);
    xcb_free_cursor(MConnection,MHiddenCursor);
    if (MWindowCursor != XCB_NONE) xcb_free_cursor(MConnection,MWindowCursor);
  }

  //----------

  void initKeyboard() {
    MKeySyms = xcb_key_symbols_alloc(MConnection);
  }

  //----------

  void cleanupKeyboard() {
    xcb_key_symbols_free(MKeySyms);
  }

  //----------

  void initThreads() {
    MTimerThread        = 0;
    MTimerThreadActive  = false;
    MTimerSleep         = 30;
    //#ifndef MIP_XCB_NO_EVENT_THREAD
    MEventThread        = 0;
    MEventThreadActive  = false;
    //#endif
  }

  //----------

  void cleanupThreads() {
    if (MTimerThreadActive) stopTimer();
    //#ifndef MIP_XCB_NO_EVENT_THREAD
    if (MEventThreadActive) stopEventThread();
    //#endif
  }

  static const unsigned long MWM_HINTS_FUNCTIONS   = 1 << 0;
  static const unsigned long MWM_HINTS_DECORATIONS = 1 << 1;
  //static const unsigned long MWM_DECOR_ALL         = 1 << 0;
  static const unsigned long MWM_DECOR_BORDER      = 1 << 1;
  static const unsigned long MWM_DECOR_RESIZEH     = 1 << 2;
  static const unsigned long MWM_DECOR_TITLE       = 1 << 3;
  static const unsigned long MWM_DECOR_MENU        = 1 << 4;
  static const unsigned long MWM_DECOR_MINIMIZE    = 1 << 5;
  static const unsigned long MWM_DECOR_MAXIMIZE    = 1 << 6;
  //static const unsigned long MWM_FUNC_ALL          = 1 << 0;
  static const unsigned long MWM_FUNC_RESIZE       = 1 << 1;
  static const unsigned long MWM_FUNC_MOVE         = 1 << 2;
  static const unsigned long MWM_FUNC_MINIMIZE     = 1 << 3;
  static const unsigned long MWM_FUNC_MAXIMIZE     = 1 << 4;
  static const unsigned long MWM_FUNC_CLOSE        = 1 << 5;

  struct WMHints {
    uint32_t flags;
    uint32_t functions;
    uint32_t decorations;
    int32_t  inputMode;
    uint32_t state;
  };

  //----------

  void removeDecorations() {
    xcb_atom_t prop = mip_xcb_get_intern_atom(MConnection,"_MOTIF_WM_HINTS");
    if (prop) {
      WMHints hints;
      hints.flags = MWM_HINTS_DECORATIONS;
      hints.decorations = 0;
      const unsigned char* ptr = (const unsigned char*)(&hints);
      xcb_change_property(
        MConnection,
        XCB_PROP_MODE_REPLACE,
        MWindow,
        prop,     // hintsAtomReply->atom,
        prop,     // XCB_ATOM_WM_HINTS,
        32,
        5,        // PROP_MOTIF_WM_HINTS_ELEMENTS
        ptr
      );
    }
  }

  //----------

  void wantDeleteWindowEvent() {
    xcb_intern_atom_cookie_t  protocol_cookie = xcb_intern_atom_unchecked(MConnection, 1, 12, "WM_PROTOCOLS");
    xcb_intern_atom_cookie_t  close_cookie    = xcb_intern_atom_unchecked(MConnection, 0, 16, "WM_DELETE_WINDOW");
    xcb_intern_atom_reply_t*  protocol_reply  = xcb_intern_atom_reply(MConnection, protocol_cookie, 0);
    xcb_intern_atom_reply_t*  close_reply     = xcb_intern_atom_reply(MConnection, close_cookie, 0);
    MWMProtocolsAtom    = protocol_reply->atom;
    MWMDeleteWindowAtom = close_reply->atom;
    //xcb_change_property(MConnection, XCB_PROP_MODE_REPLACE, MWindow, protocol_reply->atom, 4, 32, 1, &MDeleteWindowAtom);
    xcb_change_property(MConnection, XCB_PROP_MODE_REPLACE, MWindow, MWMProtocolsAtom, 4, 32, 1, &MWMDeleteWindowAtom);
    //xcb_flush(MConnection);
    free(protocol_reply); // note malloc'ed ??
    free(close_reply);
  }

  //----------

  /*
    xcb_wait_for_event() doesn't flush
    have we flushed already?
    when we create the window?
    when we open it?
  */

  void waitForMapNotify() {
    xcb_flush(MConnection);
    while (1) {
      xcb_generic_event_t* event;
      event = xcb_wait_for_event(MConnection);
      uint8_t e = event->response_type & ~0x80;
      free(event); // not malloc'ed
      if (e == XCB_MAP_NOTIFY) break;
    }
  }

  //----------

  void startEventThread() {
    MEventThreadActive = true;
    pthread_create(&MEventThread,nullptr,xcb_event_thread_proc,this);
  }

  //----------

  void stopEventThread() {
    void* ret;
    MEventThreadActive = false;
    sendEvent(MIP_THREAD_ID_KILL,0);
    pthread_join(MEventThread,&ret);
  }

  //----------

  void quitEventLoop() {
    MQuitEventLoop = true;
  }

  //----------

  uint32_t remapButton(uint32_t AButton, uint32_t AState) {
    //XCB_Print("xcb: remapButton AButton %i AState %i\n",AButton,AState);
    //MIP_Print("remapButton AButton %i AState %i\n",AButton,AState);
    uint32_t b = AButton;
    return b;
  }

  //----------

  // https://github.com/etale-cohomology/xcb/blob/master/loop.c

  uint32_t remapKey(uint32_t AKey, uint32_t AState) {
    //XCB_Print("xcb: remapKey AKey %i AState %i\n",AKey,AState);
    //uint32_t k = AKey;
    int col = 0;
    xcb_keysym_t keysym = xcb_key_symbols_get_keysym(MKeySyms,AKey,col);
    //xcb_keycode_t* keycode = xcb_key_symbols_get_keycode(MKeySyms,keysym);
    //MIP_Print("AKey %i AState %i keysym %i keycode %i\n",AKey,AState,keysym,keycode[0]);
    //free(keycode);
    return keysym;
  }

  //----------

  uint32_t remapState(uint32_t AState) {
    //XCB_Print("xcb: remapState AState %i\n",AState);
    uint32_t s = MIP_KEY_NONE;
    if (AState & XCB_MOD_MASK_SHIFT)    s += MIP_KEY_SHIFT;
    if (AState & XCB_MOD_MASK_LOCK)     s += MIP_KEY_CAPS;
    if (AState & XCB_MOD_MASK_CONTROL)  s += MIP_KEY_CTRL;
    if (AState & XCB_MOD_MASK_1)        s += MIP_KEY_ALT;
    if (AState & XCB_MOD_MASK_5)        s += MIP_KEY_ALTGR;
    //if (AState & XCB_MOD_MASK_1) MIP_Print("1\n");
    //if (AState & XCB_MOD_MASK_2) MIP_Print("2\n");
    //if (AState & XCB_MOD_MASK_3) MIP_Print("3\n");
    //if (AState & XCB_MOD_MASK_4) MIP_Print("4\n");
    //if (AState & XCB_MOD_MASK_5) MIP_Print("5\n");
    return s;
  }

  //----------

  void freeXcbCursor(void) {
    if (MWindowCursor != XCB_NONE) {
      xcb_free_cursor(MConnection,MWindowCursor);
      MWindowCursor = XCB_NONE;
      //xcb_flush(MConnection);
    }
  }

  //----------

  void setXcbCursor(xcb_cursor_t ACursor) {
    uint32_t mask;
    uint32_t values;
    mask   = XCB_CW_CURSOR;
    values = ACursor;
    xcb_change_window_attributes(MConnection,MWindow,mask,&values);
    // without xcb_flush, the mouse cursor wasn't updating properly (standalone)..
    xcb_flush(MConnection);
  }

  //----------

  void setWMCursor(uint32_t ACursor) {
    xcb_cursor_context_t *ctx;
    if (xcb_cursor_context_new(MConnection, MScreen, &ctx) >= 0) {
      const char* name = MIP_XCB_WM_CURSORS[ACursor];
      xcb_cursor_t cursor = xcb_cursor_load_cursor(ctx, name);
      if (cursor != XCB_CURSOR_NONE) {
        xcb_change_window_attributes(MConnection, MWindow, XCB_CW_CURSOR, &cursor);
      }
    }
    xcb_cursor_context_free(ctx);
    xcb_flush(MConnection);
  }

//------------------------------
private:
//------------------------------

  void eventHandler(xcb_generic_event_t* AEvent) {

    switch (AEvent->response_type & ~0x80) {

      //----------

      case XCB_MAP_NOTIFY: {
        //xcb_map_notify_event_t* map_notify = (xcb_map_notify_event_t*)AEvent;
        MWindowMapped = true;
        //on_window_open();
        break;
      }

      //----------

      case XCB_UNMAP_NOTIFY: {
        //xcb_unmap_notify_event_t* unmap_notify = (xcb_unmap_notify_event_t*)AEvent;
        MWindowMapped = false;
        //on_window_close();
        break;
      }

      //----------

      case XCB_CONFIGURE_NOTIFY: {
        xcb_configure_notify_event_t* configure_notify = (xcb_configure_notify_event_t*)AEvent;
        int32_t x = configure_notify->x;
        int32_t y = configure_notify->y;
        int32_t w = configure_notify->width;
        int32_t h = configure_notify->height;
        if ((x != MWindowXpos) || (y != MWindowYpos)) {
          MWindowXpos = x;
          MWindowYpos = y;
          on_window_move(x,y);
        }
        if ((w != MWindowWidth) || (h != MWindowHeight)) {
          MWindowWidth = w;
          MWindowHeight = h;
          on_window_resize(w,h);
        }
        break;
      }

      //----------


      case XCB_EXPOSE: {
        MWindowExposed = true;
        //MIP_Rect RECT;
        xcb_expose_event_t* expose = (xcb_expose_event_t *)AEvent;
        int32_t x = expose->x;
        int32_t y = expose->y;
        int32_t w = expose->width;
        int32_t h = expose->height;
        //RECT = MIP_Rect(x,y,w,h);

        // https://cairographics.org/cookbook/xcbsurface.c/
        // Avoid extra redraws by checking if this is the last expose event in the sequence
        //if (expose->count != 0) break;

        //while(expose->count != 0) {
        //  xcb_generic_event_t* e2 = xcb_wait_for_event(MConnection);
        //  xcb_expose_event_t* ex2 = (xcb_expose_event_t *)e2;
        //  RECT.combine( MIP_Rect( ex2->x, ex2->y, ex2->width, ex2->height ) );
        //}

      if ((w < 1) || (h < 1)) {
        }
        else {
          beginPaint();
          paint(x,y,w,h);
          endPaint();
        }
        break;
      }

      //----------

      //s = AEvent->xkey.state;
      //XLookupString(&AEvent->xkey, c, 1, &keysym, NULL);
      //k = remapKey(keysym);

      /*
      // wrapper to get xcb keysymbol from keycode
      static xcb_keysym_t xcb_get_keysym(xcb_keycode_t keycode) {
        xcb_key_symbols_t *keysyms;
        if (!(keysyms = xcb_key_symbols_alloc(conn))) return 0;
        xcb_keysym_t keysym = xcb_key_symbols_get_keysym(keysyms, keycode, 0);
        xcb_key_symbols_free(keysyms);
        return keysym;
      }
      */

      case XCB_KEY_PRESS: {
        xcb_key_press_event_t* key_press = (xcb_key_press_event_t*)AEvent;
        uint32_t  k = remapKey(   key_press->detail, key_press->state );
        uint32_t  s = remapState( key_press->state );
        uint32_t ts = key_press->time;
        //int32_t   x = key_press->event_x;
        //int32_t   y = key_press->event_y;
        on_window_keyPress(k,s,ts);
        break;
      }

      //----------

      case XCB_KEY_RELEASE: {
        xcb_key_release_event_t* key_release = (xcb_key_release_event_t*)AEvent;
        uint32_t  k = remapKey( key_release->detail, key_release->state );
        uint32_t  s = remapState( key_release->state );
        uint32_t ts = key_release->time;
        //int32_t   x = key_release->event_x;
        //int32_t   y = key_release->event_y;
        on_window_keyRelease(k,s,ts);
        break;
      }

      //----------

      case XCB_BUTTON_PRESS: {
        xcb_button_press_event_t* button_press = (xcb_button_press_event_t*)AEvent;
        uint32_t  b = remapButton( button_press->detail, button_press->state );
        uint32_t  s = remapState( button_press->state );
        int32_t   x = button_press->event_x;
        int32_t   y = button_press->event_y;
        uint32_t ts = button_press->time;
        on_window_mouseClick(x,y,b,s,ts);
        break;
      }

      //----------

      case XCB_BUTTON_RELEASE: {
        xcb_button_release_event_t* button_release = (xcb_button_release_event_t*)AEvent;
        uint32_t  b = remapButton( button_release->detail, button_release->state );
        uint32_t  s = remapState( button_release->state );
        int32_t   x = button_release->event_x;
        int32_t   y = button_release->event_y;
        uint32_t ts = button_release->time;
        on_window_mouseRelease(x,y,b,s,ts);
        break;
      }

      //----------

      case XCB_MOTION_NOTIFY: {
        xcb_motion_notify_event_t* motion_notify = (xcb_motion_notify_event_t*)AEvent;
        //uint32_t  b = motion_notify->detail;
        uint32_t  s = remapState( motion_notify->state );
        int32_t   x = motion_notify->event_x;
        int32_t   y = motion_notify->event_y;
        uint32_t ts = motion_notify->time;
      on_window_mouseMove(x,y,s,ts);
        break;
      }

      //----------

      case XCB_ENTER_NOTIFY: {
        //#ifdef MIP_DEBUG_XCB
        xcb_enter_notify_event_t* enter_notify = (xcb_enter_notify_event_t*)AEvent;
        //uint32_t  t = enter_notify->time;
        //uint32_t  m = enter_notify->mode;
        //uint32_t  s = enter_notify->state;
        int32_t   x = enter_notify->event_x;
        int32_t   y = enter_notify->event_y;
        uint32_t ts = enter_notify->time;
      //#endif
        on_window_mouseEnter(x,y,ts);
        break;
      }

      case XCB_LEAVE_NOTIFY: {
        //#ifdef MIP_DEBUG_XCB
        xcb_leave_notify_event_t* leave_notify = (xcb_leave_notify_event_t*)AEvent;
        //uint32_t  t = leave_notify->time;
        //uint32_t  m = leave_notify->mode;
        //uint32_t  s = leave_notify->state;
        int32_t   x = leave_notify->event_x;
        int32_t   y = leave_notify->event_y;
        uint32_t ts = leave_notify->time;
        //#endif
        on_window_mouseLeave(x,y,ts);
        break;
      }

      //----------

      case XCB_CLIENT_MESSAGE: {
        xcb_client_message_event_t* client_message = (xcb_client_message_event_t*)AEvent;
        //xcb_atom_t type = client_message->type;
        uint32_t data = client_message->data.data32[0];
        switch(data) {
          case MIP_THREAD_ID_TIMER:
            on_window_timer();
            break;
          case MIP_THREAD_ID_IDLE:
            if (MWindowMapped && MWindowExposed) {
              on_window_idle();
            }
            break;
          default:
            on_window_clientMessage(data,nullptr);
            break;
        }
        break;
      }

      //----------

      //case XCB_PROPERTY_NOTIFY: {
      //  xcb_property_notify_event_t* property_notify = (xcb_property_notify_event_t*)AEvent;
      //  xcb_atom_t      a = property_notify->atom;
      ////xcb_timestamp_t t = property_notify->time;
      ////uint32_t          s = property_notify->state;
      //  char  buffer1[256];
      //  char* name = buffer1;
      //  bool  ok1  = getAtomName(a,&name);
      //  char buffer2[256];
      //  char* property = buffer2;
      //  xcb_atom_t type = XCB_ATOM;
      //  switch (a) {
      //    case XCB_ATOM_WM_NAME:
      //      type = XCB_ATOM_STRING;
      //      break;
      //  }
      //  bool ok2 = getAtomProperty(a,type,&property);
      //  XCB_Print("XCB_PROPERTY_NOTIFY (%i) %s %s\n",a,ok1 ? name : "---", ok2 ? property : "---" );
      //  break;
      //}

      //----------

      //case XCB_FOCUS_IN: {
      //  xcb_focus_in_event_t* focus_in = (xcb_focus_in_event_t*)AEvent;
      //  XCB_Print("XCB_FOCUS_IN\n");
      //  //on_window_focus(true);
      //  break;
      //}

      //case XCB_FOCUS_OUT: {
      //  xcb_focus_out_event_t* focus_out = (xcb_focus_out_event_t*)AEvent;
      //  XCB_Print("XCB_FOCUS_OUT\n");
      //  //on_window_focus(false);
      //  break;
      //}

      //case XCB_KEYMAP_NOTIFY: {
      //  xcb_keymap_notify_event_t* keymap_notify = (xcb_keymap_notify_event_t*)AEvent;
      //  XCB_Print("XCB_KEYMAP_NOTIFY\n");
      //  break;
      //}

      //case XCB_GRAPHICS_EXPOSURE: {
      //  xcb_graphics_exposure_event_t *graphics_exposure = (xcb_graphics_exposure_event_t *)AEvent;
      //  XCB_Print("XCB_GRAPHICS_EXPOSURE\n");
      //  break;
      //}

      //case XCB_NO_EXPOSURE: {
      //  xcb_no_exposure_event_t *no_exposure = (xcb_no_exposure_event_t *)AEvent;
      //  XCB_Print("XCB_NO_EXPOSURE\n");
      //  break;
      //}

      //case XCB_VISIBILITY_NOTIFY: {
      //  xcb_visibility_notify_event_t *visibility_notify = (xcb_visibility_notify_event_t *)AEvent;
      //  XCB_Print("XCB_VISIBILITY_NOTIFY\n");
      //  break;
      //}

      //case XCB_CREATE_NOTIFY: {
      //  xcb_create_notify_event_t* create_notify = (xcb_create_notify_event_t*)AEvent;
      //  XCB_Print("XCB_CREATE_NOTIFY\n");
      //  break;
      //}

      //case XCB_DESTROY_NOTIFY: {
      //  xcb_destroy_notify_event_t* destroy_notify = (xcb_destroy_notify_event_t*)AEvent;
      //  XCB_Print("XCB_DESTROY_NOTIFY\n");
      //  break;
      //}

      //case XCB_MAP_REQUEST: {
      //  xcb_map_request_event_t* map_request = (xcb_map_request_event_t*)AEvent;
      //  XCB_Print("XCB_MAP_REQUEST\n");
      //  break;
      //}

      //case XCB_REPARENT_NOTIFY: {
      //  //xcb_reparent_notify_event_t* reparent_notify = (xcb_reparent_notify_event_t*)AEvent;
      //  XCB_Print("XCB_REPARENT_NOTIFY\n");
      //  break;
      //}

      //case XCB_CONFIGURE_REQUEST: {
      //  xcb_configure_request_event_t* configure_request = (xcb_configure_request_event_t*)AEvent;
      //  XCB_Print("XCB_CONFIGURE_REQUEST\n");
      //  break;
      //}

      //case XCB_GRAVITY_NOTIFY: {
      //  xcb_gravity_notify_event_t* gravity_notify = (xcb_gravity_notify_event_t*)AEvent;
      //  XCB_Print("XCB_GRAVITY_NOTIFY\n");
      //  break;
      //}

      //case XCB_RESIZE_REQUEST: {
      //  xcb_resize_request_event_t* resize_request = (xcb_resize_request_event_t*)AEvent;
      //  XCB_Print("XCB_RESIZE_REQUEST\n");
      //  break;
      //}

      //case XCB_CIRCULATE_NOTIFY: {
      //  xcb_circulate_notify_event_t* circulate_notify = (xcb_circulate_notify_event_t*)AEvent;
      //  XCB_Print("XCB_CIRCULATE_NOTIFY\n");
      //  break;
      //}

      //case XCB_CIRCULATE_REQUEST: {
      //  xcb_circulate_request_event_t* circulate_request = (xcb_circulate_request_event_t*)AEvent;
      //  XCB_Print("XCB_CIRCULATE_REQUEST\n");
      //  break;
      //}

      //case XCB_SELECTION_CLEAR: {
      //  xcb_selection_clear_event_t* selection_clear = (xcb_selection_clear_event_t*)AEvent;
      //  XCB_Print("XCB_SELECTION_CLEAR\n");
      //  break;
      //}

      //case XCB_SELECTION_REQUEST: {
      //  xcb_selection_request_event_t* selection_request = (xcb_selection_request_event_t*)AEvent;
      //  XCB_Print("XCB_SELECTION_REQUEST\n");
      //  break;
      //}

      //case XCB_SELECTION_NOTIFY: {
      //  xcb_selection_notify_event_t* selection_notify = (xcb_selection_notify_event_t*)AEvent;
      //  XCB_Print("XCB_SELECTION_NOTIFY\n");
      //  break;
      //}

      //case XCB_COLORMAP_NOTIFY: {
      //  xcb_colormap_notify_event_t* colormap_notify = (xcb_colormap_notify_event_t*)AEvent;
      //  XCB_Print("XCB_COLORMAP_NOTIFY\n");
      //  break;
      //}

      //case XCB_MAPPING_NOTIFY: {
      //  xcb_mapping_notify_event_t* mapping_notify = (xcb_mapping_notify_event_t*)AEvent;
      //  XCB_Print("XCB_MAPPING_NOTIFY\n");
      //  break;
      //}

      //default: {
      //  XCB_Print("unhandled xcb event: %i\n",event->response_type);
      //  break;
      //}

    } // switch event type

  }

//------------------------------
private:
//------------------------------

  static
  void* xcb_event_thread_proc(void* AWindow) {
  //_mip_xcb_event_thread_pid = getpid();
  //_mip_xcb_event_thread_tid = getpid();
  MIP_XcbWindow* window = (MIP_XcbWindow*)AWindow;
    if (window) {
      xcb_connection_t* connection = window->MConnection;
      xcb_flush(connection);
      while (window->MEventThreadActive) {
        xcb_generic_event_t* event = xcb_wait_for_event(connection);
        if (event) {
          //uint32_t e = event->response_type & ~0x80;
          //if (e == XCB_CLIENT_MESSAGE) {
          //  xcb_client_message_event_t* client_message = (xcb_client_message_event_t*)event;
          //  xcb_atom_t  type = client_message->type;
          //  uint32_t      data = client_message->data.data32[0];
          //  KPrint("XCB_CLIENT_MESSAGE: type %i data %i\n",type,data);
          //}
          window->eventHandler(event);
          free(event); // not MALLOC'ed
        }
      }
    }
    return nullptr;
  }

  //----------

  static
  void* xcb_timer_thread_proc(void* AWindow) {
    MIP_XcbWindow* window = (MIP_XcbWindow*)AWindow;
    if (window) {
      xcb_connection_t* connection = window->MConnection;
      xcb_flush(connection); // so we're sure all messages have been sent before entering loop
      while (window->MTimerThreadActive) {
        //window->on_window_timer();
        window->sendEvent(MIP_THREAD_ID_TIMER,0);
        MIP_Sleep(window->MTimerSleep); // ???
      }
    }
    return nullptr;
  }

//------------------------------
public:
//------------------------------

  void setPos(uint32_t AXpos, uint32_t AYpos) override {
    MWindowXpos = AXpos;
    MWindowYpos = AYpos;
    static uint32_t values[] = {
      (uint32_t)AXpos,
      (uint32_t)AYpos
    };
    xcb_configure_window(MConnection,MWindow,XCB_CONFIG_WINDOW_X|XCB_CONFIG_WINDOW_Y,values);
    xcb_flush(MConnection);
  }

  //----------

  void setSize(uint32_t AWidth, uint32_t AHeight) override {
    MWindowWidth = AWidth;
    MWindowHeight = AHeight;
    static uint32_t values[] = {
      (uint32_t)AWidth,
      (uint32_t)AHeight
    };
    xcb_configure_window(MConnection,MWindow,XCB_CONFIG_WINDOW_WIDTH|XCB_CONFIG_WINDOW_HEIGHT,values);
    xcb_flush(MConnection);
    //cairo_xcb_surface_set_size:
  }

  //----------

  void setTitle(const char* ATitle) override {
    xcb_change_property(
      MConnection,
      XCB_PROP_MODE_REPLACE,
      MWindow,
      XCB_ATOM_WM_NAME,
      XCB_ATOM_STRING,
      8,
      strlen(ATitle),
      ATitle
    );
    xcb_flush(MConnection);
  }

  //----------

  //----------

  void open() override {
    //MIP_PRINT;
    xcb_map_window(MConnection,MWindow);
    #ifdef MIP_XCB_WAIT_FOR_MAPNOTIFY
    waitForMapNotify();
    #endif
    if (MUseEventThread) startEventThread();
    //paint?
    //xcb_flush(MConnection);
    setMouseCursor(MIP_CURSOR_DEFAULT);
    xcb_flush(MConnection);

    //on_window_open(MWindowWidth,MWindowHeight);

  }

  //----------

  void close() override {

    //on_window_close();

    if (MUseEventThread) stopEventThread();
    xcb_unmap_window(MConnection,MWindow);
    xcb_flush(MConnection);
  }

  //----------

  void eventLoop() override {
    MQuitEventLoop = false;
    xcb_flush(MConnection);
    xcb_generic_event_t* event;// = xcb_wait_for_event(MConnection);
    //while (event) {
    while ((event = xcb_wait_for_event(MConnection))) {
      uint32_t e = event->response_type & ~0x80;
      if (e == XCB_CLIENT_MESSAGE) {
        xcb_client_message_event_t* client_message = (xcb_client_message_event_t*)event;
        xcb_atom_t  type = client_message->type;
        uint32_t      data = client_message->data.data32[0];
        if (type == MWMProtocolsAtom) {
          if (data == MWMDeleteWindowAtom) {
            free(event); // not malloc'ed
            //MQuitEventLoop = true;
            break;
          }
        }
      }
      eventHandler(event);
      free(event); // not malloc'ed
      if (MQuitEventLoop) break;
      //event = xcb_wait_for_event(MConnection);
    }
  }

  //----------

  void reparent(void* AParent) override {
    MWindowParent = (intptr_t)AParent;
    xcb_reparent_window(MConnection,MWindow,MWindowParent,0,0);
    xcb_flush(MConnection);
  }

  //----------

  void startTimer(uint32_t ms) override {
    MTimerSleep = ms;
    MTimerThreadActive = true;
    pthread_create(&MTimerThread,nullptr,xcb_timer_thread_proc,this);
  }

  //----------

  void stopTimer(void) override {
    void* ret;
    MTimerThreadActive = false;
    pthread_join(MTimerThread,&ret);
  }

  //----------

  //MWindowCursor = XCreateFontCursor(MDisplay, mip_xlib_cursors[ACursor]);
  //XDefineCursor(MDisplay,MWindow,MWindowCursor);

  void setMouseCursor(uint32_t ACursor) override {
    //if (!MCursorHidden) {
    //  freeXcbCursor();
    //  MWindowCursor = mip_xcb_create_font_cursor(MConnection, xcb_cursors[ACursor] );
    //  setXcbCursor(MWindowCursor);
    //}
    setWMCursor(ACursor);
  }

  //----------

  void setMouseCursorPos(int32_t AXpos, int32_t AYpos) override {
    xcb_warp_pointer(MConnection,XCB_NONE,MWindow,0,0,0,0,AXpos,AYpos);
    xcb_flush(MConnection);
  }

  //----------

  void hideMouseCursor(void) override {
    if (!MCursorHidden) {
      setXcbCursor(MHiddenCursor);
      MCursorHidden = true;
    }
  }

  //----------

  void showMouseCursor(void) override {
    if (MCursorHidden) {
      setXcbCursor(MWindowCursor);
      MCursorHidden = false;
    }
  }

  //----------

  /*
    xcb_grab_pointer:
    Actively grabs control of the pointer. Further pointer events are reported
    only to the grabbing client. Overrides any active pointer grab by this
    client.
  */

  void grabMouseCursor(void) override {
    int32_t event_mask =
      XCB_EVENT_MASK_BUTTON_PRESS   |
      XCB_EVENT_MASK_BUTTON_RELEASE |
      XCB_EVENT_MASK_POINTER_MOTION |
      XCB_EVENT_MASK_FOCUS_CHANGE   |
      XCB_EVENT_MASK_ENTER_WINDOW   |
      XCB_EVENT_MASK_LEAVE_WINDOW;
    xcb_grab_pointer(
      MConnection,
      0,                                      // owner_events
      MWindow, /*rootWindow,*/                // grab_window
      event_mask,                             // event_mask
      XCB_GRAB_MODE_ASYNC,                    // pointer_mode
      XCB_GRAB_MODE_ASYNC,                    // keyboard_mode
      XCB_NONE, /*rootWindow,*/               // confine_to
      XCB_NONE,                               // cursor
      XCB_CURRENT_TIME                        // timestamp
    );
    xcb_flush(MConnection);
  }

  //----------

  void releaseMouseCursor(void) override {
    xcb_ungrab_pointer(MConnection,XCB_CURRENT_TIME);
    xcb_flush(MConnection);
  }

  //----------

  void beginPaint() override {
  }

  //----------

  void paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    //MIP_Print("x %i y %i w %i h %i\n",AXpos,AYpos,AWidth,AHeight);
    //on_paint(MWindowPainter,ARect);

    if (MFillBackground) {
      fill(MBackgroundColor);
    }

    on_window_paint(AXpos,AYpos,AWidth,AHeight);
  }

  //----------

  void endPaint() override {
    xcb_flush(MConnection);
  }

  //----------

  void invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    //MIP_Print("xcb: invalidate %i %i %i %i\n",AXpos,AYpos,AWidth,AHeight);
    memset(MExposeEventBuffer,0,sizeof(MExposeEventBuffer));
    MExposeEvent->window        = MWindow;
    MExposeEvent->response_type = XCB_EXPOSE;
    MExposeEvent->x             = AXpos;
    MExposeEvent->y             = AYpos;
    MExposeEvent->width         = AWidth;
    MExposeEvent->height        = AHeight;
    xcb_send_event(
      MConnection,
      false,
      MWindow,
      XCB_EVENT_MASK_EXPOSURE,
      (char*)MExposeEvent
    );
    xcb_flush(MConnection);
  }

  //----------

  /*
    https://www.x.org/releases/X11R7.7/doc/man/man3/xcb_send_event.3.xhtml#heading8
    https://stackoverflow.com/questions/40533318/xcb-custom-message-to-event-loop
  */

  void sendEvent(uint32_t AData, uint32_t AType) override {
    memset(MClientMessageEventBuffer,0,sizeof(MClientMessageEventBuffer));
    MClientMessageEvent->window         = MWindow;
    MClientMessageEvent->response_type  = XCB_CLIENT_MESSAGE;
    MClientMessageEvent->format         = 32; // didn't work without this
    MClientMessageEvent->type           = AType;
    MClientMessageEvent->data.data32[0] = AData;
    xcb_send_event(
      MConnection,
      false,
      MWindow,
      XCB_EVENT_MASK_NO_EVENT,
      (char*)MClientMessageEvent
    );
    xcb_flush(MConnection);
  }

  //----------

  void flush(void) override {
    xcb_flush(MConnection);
  }

  //----------

  void sync(void) override {
    xcb_aux_sync(MConnection);
  }

//------------------------------
public:
//------------------------------

  uint32_t xcb_color(MIP_Color AColor) {
    uint8_t r = AColor.r * 255.0f;
    uint8_t g = AColor.g * 255.0f;
    uint8_t b = AColor.b * 255.0f;
    uint8_t a = AColor.a * 255.0f;
    uint32_t color = (a << 24) + (r << 16) + (g << 8) + b;
    return color;

  }

//------------------------------
public:
//------------------------------

  void fill(MIP_Color AColor) override {
    fill(0,0,MWindowWidth,MWindowHeight,AColor);
  }

  //----------

  void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, MIP_Color AColor) override {

    // set color
    uint32_t mask = XCB_GC_FOREGROUND;
    uint32_t values[1];
    values[0] = xcb_color(AColor);
    xcb_change_gc(MConnection,MScreenGC,mask,values);

    // fill rectangle
    xcb_rectangle_t rectangles[] = {{
      (int16_t)AXpos,     //0,
      (int16_t)AYpos,     //0,
      (uint16_t)AWidth,   //MWindowWidth,
      (uint16_t)AHeight,  //MWindowHeight
    }};
    xcb_poly_fill_rectangle(MConnection,MWindow,MScreenGC,1,rectangles);
    xcb_flush(MConnection);
  }

  //----------

  void blit(int32_t ADstX, int32_t ADstY, MIP_Drawable* ASource) override {
    if (ASource->isImage()) {
      xcb_image_put(
        MConnection,            // xcb_connection_t *  conn,
        MWindow,                // xcb_drawable_t      draw,
        MScreenGC,              // xcb_gcontext_t      gc,
        ASource->getXcbImage(), // xcb_image_t *       image,
        ADstX,                  // int16_t             x,
        ADstY,                  // int16_t             y,
        0                       // uint8_t             left_pad
      );
      xcb_flush(MConnection);
    }
    else if (ASource->isSurface()) {
      xcb_copy_area(
        MConnection,                // Pointer to the xcb_connection_t structure
        ASource->getXcbDrawable(),  // The Drawable we want to paste
        MWindow,                    // The Drawable on which we copy the previous Drawable
        MScreenGC,                  // A Graphic Context
        0,                          // Top left x coordinate of the region we want to copy
        0,                          // Top left y coordinate of the region we want to copy
        ADstX,                      // Top left x coordinate of the region where we want to copy
        ADstY,                      // Top left y coordinate of the region where we want to copy
        ASource->getWidth(),        // Width                 of the region we want to copy
        ASource->getHeight()        // Height of the region we want to copy
      );
      xcb_flush(MConnection);
    }
    //else {
    //  MIP_Trace("unknown ADrawable for blit()\n");
    //}
  }


  void blit(int32_t ADstX, int32_t ADstY, MIP_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) override {
    if (ASource->isImage()) {
      mip_xcb_put_image(
        MConnection,
        MWindow,
        MScreenGC,
        ASrcW,
        ASrcH,
        ADstX,
        ADstY,
        MScreenDepth,//ASource->getDepth(),
        ASource->getStride(),
        ASource->getBitmap()->getPixelPtr(ASrcX,ASrcY)//getBuffer()
      );
      xcb_flush(MConnection);
    }
    else if (ASource->isSurface()) {
      xcb_copy_area(
        MConnection,                // Pointer to the xcb_connection_t structure
        ASource->getXcbDrawable(),  // The Drawable we want to paste
        MWindow,                    // The Drawable on which we copy the previous Drawable
        MScreenGC,                  // A Graphic Context
        ASrcX,                      // Top left x coordinate of the region we want to copy
        ASrcY,                      // Top left y coordinate of the region we want to copy
        ADstX,                      // Top left x coordinate of the region where we want to copy
        ADstY,                      // Top left y coordinate of the region where we want to copy
        ASrcW,                      // Width                 of the region we want to copy
        ASrcH                       // Height of the region we want to copy
      );
      xcb_flush(MConnection);
    }
    //else {
    //  MIP_Trace("unknown ADrawable for blit()\n");
    //}
  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif
