#ifndef mip_xcb_window_included
#define mip_xcb_window_included
//----------------------------------------------------------------------

//#include "gui/mip_window_listener.h"
#include "gui/mip_drawable.h"
#include "gui/base/mip_base_window.h"
#include "gui/xcb/mip_xcb.h"
#include "gui/xcb/mip_xcb_utils.h"

#include "gui/xlib/mip_xlib.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#define MIP_XCB_WAIT_FOR_MAPNOTIFY
#define MIP_XCB_WINDOW_RESIZE_SPEED 500

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//class MIP_BaseWindow
//: public MIP_WindowListener {
//public:
//  virtual void beginPaint() {}
//  virtual void endPaint() {}
//};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_XcbWindow
: public MIP_BaseWindow
, public MIP_Drawable
/*, public MIP_TimerListener*/ {

  friend class MIP_OpenGLWindow;

//------------------------------
private:
//------------------------------

  xcb_window_t                MWindow                       = XCB_NONE;
  int32_t                     MWindowXpos                   = 0;
  int32_t                     MWindowYpos                   = 0;
  int32_t                     MWindowWidth                  = 0;
  int32_t                     MWindowHeight                 = 0;
  bool                        MWindowMapped                 = false;
  bool                        MWindowExposed                = false;

//  double                      MWindowInitialWidth           = 0.0;
//  double                      MWindowInitialHeight          = 0.0;
//  double                      MWindowWidthScale             = 1.0;
//  double                      MWindowHeightScale            = 1.0;
//  double                      MWindowScale                  = 1.0;

  bool                        MFillBackground               = false;
  uint32_t                    MBackgroundColor              = 0x808080;

  // connection
  Display*                    MDisplay                      = nullptr;
  xcb_connection_t*           MConnection                   = nullptr;
  // screen
  int                         MDefaultScreen                = 0;
  xcb_screen_t*               MScreen                       = nullptr;
  uint32_t                    MScreenWidth                  = 0;
  uint32_t                    MScreenHeight                 = 0;
  uint32_t                    MScreenDepth                  = 0;
  xcb_window_t                MScreenWindow                 = XCB_NONE;
  xcb_colormap_t              MScreenColormap               = XCB_NONE;
  xcb_visualid_t              MScreenVisual                 = XCB_NONE;
  xcb_drawable_t              MScreenDrawable               = XCB_NONE;
  xcb_gcontext_t              MScreenGC                     = XCB_NONE;
  // mouse
  xcb_pixmap_t                MEmptyPixmap                  = XCB_NONE;
  xcb_cursor_t                MHiddenCursor                 = XCB_NONE;
  xcb_cursor_t                MWindowCursor                 = XCB_NONE;
  bool                        MCursorHidden                 = false;
  // keyboard
  xcb_key_symbols_t*          MKeySyms                      = nullptr;
  // events
  char*                       MExposeEventBuffer[32]        = {0};
  char*                       MClientMessageEventBuffer[32] = {0};
  xcb_expose_event_t*         MExposeEvent                  = (xcb_expose_event_t*)MExposeEventBuffer;
  xcb_client_message_event_t* MClientMessageEvent           = (xcb_client_message_event_t*)MClientMessageEventBuffer;
  // wantQuitEvemts
  xcb_atom_t                  MWMProtocolsAtom              = XCB_NONE;
  xcb_atom_t                  MWMDeleteWindowAtom           = XCB_NONE;
  // event thread
  pthread_t                   MEventThread                  = 0;
  bool                        MEventThreadActive            = false;

  void (*MEventThreadStartCallback)(void* AUser) = nullptr;
  void (*MEventThreadStopCallback)(void* AUser) = nullptr;

  int32_t MWindowInitialWidth = 0.0;
  int32_t MWindowInitialHeight = 0.0;
  double MWindowWidthScale = 1.0;
  double MWindowHeightScale = 1.0;

  //bool        MResizing     = false;
  //MIP_Timer*  MResizeTimer  = nullptr;

//------------------------------
public:
//------------------------------

  MIP_XcbWindow(uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false) {

    //MWindowListener = this;

    MWindowInitialWidth = AWidth;
    MWindowInitialHeight = AHeight;

    initConnection(nullptr);
    initScreen();
    initScreenGC();
    initWindow(AWidth,AHeight);
    initMouseCursor();
    initKeyboard();
    if (AEmbedded) { removeDecorations(); }
    else { wantQuitEvents(); }
    //MResizeTimer = new MIP_Timer(this);
  }

  //----------

  virtual ~MIP_XcbWindow() {
    cleanupKeyboard();
    cleanupMouseCursor();
    cleanupWindow();
    cleanupScreenGC();
    cleanupScreen();
    cleanupConnection();
    //delete MResizeTimer;
  }

//------------------------------
public:
//------------------------------

  virtual bool    isWindowExposed() { return MWindowExposed; }
  virtual bool    isWindowMapped()  { return MWindowMapped; }
  virtual int32_t getWindowWidth()  { return MWindowWidth; }
  virtual int32_t getWindowHeight() { return MWindowHeight; }

  virtual double  getWindowWidthScale()  { return MWindowWidthScale; }
  virtual double  getWindowHeightScale() { return MWindowHeightScale; }

  virtual void    setWindowFillBackground(bool AFill=true)          { MFillBackground = AFill; }
  virtual void    setWindowBackgroundColor(uint32_t AColor)         { MBackgroundColor = AColor; }

  //virtual void paint() {
  //  if (MWindowListener) MWindowListener->on_window_paint(0,0,MWindowWidth,MWindowHeight);
  //}

//------------------------------
public: // drawable
//------------------------------

  bool                drawable_isWindow()          final { return true; }
  bool                drawable_isDrawable()        final { return true; }

  uint32_t            drawable_getWidth()          final { return MWindowWidth; }
  uint32_t            drawable_getHeight()         final { return MWindowHeight; }
  uint32_t            drawable_getDepth()          final { return MScreenDepth; }

  xcb_connection_t*   drawable_getXcbConnection()  final { return MConnection; }
  xcb_visualid_t      drawable_getXcbVisual()      final { return MScreenVisual; }
  xcb_drawable_t      drawable_getXcbDrawable()    final { return MWindow; }
  xcb_window_t        drawable_getXcbWindow()      final { return MWindow; }

  Display*            drawable_getXlibDisplay()    final { return MDisplay; }

  //#ifdef MIP_USE_CAIRO
  //bool                drawable_isCairo()           final { return true; }
  //cairo_surface_t*    drawable_getCairoSurface()   final { return MCairoSurface; }
  //#endif

//------------------------------
public: // window
//------------------------------

  void setWindowPos(uint32_t AXpos, uint32_t AYpos) override {
    uint32_t values[] = { AXpos, AYpos };
    xcb_configure_window(MConnection,MWindow,XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,values);
    xcb_flush(MConnection);
  }

  //----------

  void setWindowSize(uint32_t AWidth, uint32_t AHeight) override {
    MWindowWidth = AWidth;
    MWindowHeight = AHeight;
    uint32_t values[] = { AWidth, AHeight };
    xcb_configure_window(MConnection,MWindow,XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,values);
    xcb_flush(MConnection);
  }

  //----------

  void setWindowTitle(const char* ATitle) override {
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

  void openWindow() override {
    xcb_map_window(MConnection,MWindow);
    xcb_flush(MConnection);
    #ifdef MIP_XCB_WAIT_FOR_MAPNOTIFY
      waitForMapNotify();
    #endif
  }

  //----------

  void closeWindow() override {
    xcb_unmap_window(MConnection,MWindow);
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
  //----------

  void eventLoop() override {
    xcb_generic_event_t* event = getEvent(true);
    while (event) {
      bool quit = !processEvent(event);
      if (quit) break;
      event = getEvent(true);
      //MIP_PRINT;
    }
  }

  //----------

  void startEventThread() override {
    //MIP_PRINT;
    MEventThreadActive = true;
    pthread_create(&MEventThread,nullptr,xcb_event_thread_proc,this);
  }

  //----------

  void stopEventThread() override {
    //MIP_PRINT;
    void* ret;
    MEventThreadActive = false;
    sendClientMessage(MIP_THREAD_ID_KILL,0);
    pthread_join(MEventThread,&ret);
  }

  //----------

  /*
    https://www.x.org/releases/X11R7.7/doc/man/man3/xcb_send_event.3.xhtml#heading8
    https://stackoverflow.com/questions/40533318/xcb-custom-message-to-event-loop
  */

  void sendClientMessage(uint32_t AData, uint32_t AType) override {
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

  void invalidateRegion(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
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

  void redrawRegion(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    invalidateRegion(AXpos,AYpos,AWidth,AHeight);
  }

  //----------

  void reparentWindow(uint32_t AParent) override {
    xcb_reparent_window(MConnection,MWindow,AParent,0,0);
    xcb_flush(MConnection);
  }

  //----------

  void beginPaint() override {
  }

  //----------

  void endPaint() override {
    xcb_flush(MConnection);
  }

  //----------

  void setEventThreadCallbacks( void (*AStart)(void* AUser), void (*AStop)(void* AUser) ) override {
    MEventThreadStartCallback = AStart;
    MEventThreadStopCallback = AStop;
  }


//------------------------------
public: // mouse
//------------------------------

  void setMouseCursor(uint32_t ACursor) override {
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

//------------------------------
public: // paint
//------------------------------

  //uint32_t xcb_color(MIP_Color AColor) {
  //  uint8_t r = AColor.r * 255.0f;
  //  uint8_t g = AColor.g * 255.0f;
  //  uint8_t b = AColor.b * 255.0f;
  //  uint8_t a = AColor.a * 255.0f;
  //  uint32_t color = (a << 24) + (r << 16) + (g << 8) + b;
  //  return color;
  //}

  //----------

  void fillColor(uint32_t AColor) override {
    fillColor(0,0,MWindowWidth,MWindowHeight,AColor);
  }

  //----------

  void fillColor(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor) override {
    // set color
    uint32_t mask = XCB_GC_FOREGROUND;
    uint32_t values[1];
    values[0] = AColor;
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

  // MIP_Bitmap->getWidth()
  // MIP_Bitmap->getHeight()
  // MIP_Bitmap->getData()
  // MIP_Bitmap->getStride()

  void blitBuffer(int32_t ADstX, int32_t ADstY, void* AData, uint32_t AStride, int32_t ASrcW, int32_t ASrcH) override {
      mip_xcb_put_image(
        MConnection,
        MWindow,
        MScreenGC,
        ASrcW,
        ASrcH,
        ADstX,
        ADstY,
        MScreenDepth,//ASource->getDepth(),
        AStride,
        AData
      );
      xcb_flush(MConnection);
  }

  //----------

//  void blitImage(int32_t ADstX, int32_t ADstY, /*xcb_image_t*/void* AImage) override {
//    xcb_image_put(
//      MConnection,  // xcb_connection_t *  conn,
//      MWindow,      // xcb_drawable_t      draw,
//      MScreenGC,    // xcb_gcontext_t      gc,
//      (xcb_image_t*)AImage,       // xcb_image_t *       image,
//      ADstX,        // int16_t             x,
//      ADstY,        // int16_t             y,
//      0             // uint8_t             left_pad
//    );
//    xcb_flush(MConnection);
//  }

  //----------

  void blitDrawable(int32_t ADstX, int32_t ADstY, MIP_Drawable* ADrawable, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) override {
    xcb_copy_area(
      MConnection,  // Pointer to the xcb_connection_t structure
      ADrawable->drawable_getXcbDrawable(),    // The Drawable we want to paste
      MWindow,      // The Drawable on which we copy the previous Drawable
      MScreenGC,    // A Graphic Context
      ASrcX,        // Top left x coordinate of the region we want to copy
      ASrcY,        // Top left y coordinate of the region we want to copy
      ADstX,        // Top left x coordinate of the region where we want to copy
      ADstY,        // Top left y coordinate of the region where we want to copy
      ASrcW,        // Width                 of the region we want to copy
      ASrcH         // Height of the region we want to copy
    );
    xcb_flush(MConnection);
  }

//------------------------------
private: // connection
//------------------------------

  /*
    glx needs a Display*, so we need to use xlib to open a connection,
    then do some 'trickery' and hand it over to xcb
  */

  bool initConnection(const char* ADisplayName=nullptr) {
    //MConnection = xcb_connect(ADisplayName,&MDefaultScreen);
    XInitThreads(); // is this needed ???
    MDisplay = XOpenDisplay(ADisplayName);
    MConnection = XGetXCBConnection(MDisplay);
    XSetEventQueueOwner(MDisplay,XCBOwnsEventQueue);
    return true;
  }

  //----------

  void cleanupConnection() {
    //xcb_disconnect(MConnection);
    XSetEventQueueOwner(MDisplay,XlibOwnsEventQueue);
    XCloseDisplay(MDisplay);
    MConnection = nullptr;
  }

//------------------------------
private: // screen
//------------------------------

  bool initScreen() {
    uint32_t screen_num = MDefaultScreen;
    xcb_screen_iterator_t iter;
    iter = xcb_setup_roots_iterator(xcb_get_setup(MConnection));
    for (; iter.rem; --screen_num, xcb_screen_next(&iter)) {
      if (screen_num == 0) {
        MScreen = iter.data;
        break;
      }
    }
    MScreenWidth    = MScreen->width_in_pixels;
    MScreenHeight   = MScreen->height_in_pixels;
    MScreenDepth    = MScreen->root_depth;
    MScreenWindow   = MScreen->root;
    MScreenColormap = MScreen->default_colormap;
    MScreenVisual   = MScreen->root_visual;
    MScreenDrawable = MScreen->root;
    return true;
  }

  //----------

  void cleanupScreen() {
  }

//------------------------------
private: // gc
//------------------------------

  bool initScreenGC() {
    MScreenGC = xcb_generate_id(MConnection);
    xcb_drawable_t draw = MScreen->root;
    uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
    uint32_t values[2];
    values[0] = MScreen->black_pixel;
    values[1] = MScreen->white_pixel;
    xcb_create_gc(MConnection, MScreenGC, draw, mask, values);
    return true;
  }

  //----------

  void cleanupScreenGC() {
    xcb_free_gc(MConnection,MScreenGC);
  }

//------------------------------
private: // window
//------------------------------

  bool initWindow(uint32_t AWidth, uint32_t AHeight) {
    MWindowWidth = AWidth;
    MWindowHeight = AWidth;
    uint32_t event_mask =
      XCB_EVENT_MASK_KEY_PRESS          |
      XCB_EVENT_MASK_KEY_RELEASE        |
      XCB_EVENT_MASK_BUTTON_PRESS       |
      XCB_EVENT_MASK_BUTTON_RELEASE     |
      XCB_EVENT_MASK_ENTER_WINDOW       |
      XCB_EVENT_MASK_LEAVE_WINDOW       |
      XCB_EVENT_MASK_POINTER_MOTION     |
      XCB_EVENT_MASK_EXPOSURE           |
      //XCB_EVENT_MASK_RESIZE_REDIRECT  |
      XCB_EVENT_MASK_STRUCTURE_NOTIFY;
    uint32_t window_mask =
      XCB_CW_BACK_PIXMAP    |
      XCB_CW_BORDER_PIXEL   |
      XCB_CW_EVENT_MASK     |
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
      MScreen->root,                  //MWindowParent,  // parent window
      0, 0,                           // x, y
      AWidth, AHeight,                // width, height
      0,                              // border_width
      XCB_WINDOW_CLASS_INPUT_OUTPUT,  // class
      MScreen->root_visual,           // visual
      window_mask,
      window_mask_values
    );
    return true;
  }

  //----------

  void cleanupWindow() {
    xcb_destroy_window(MConnection,MWindow);
  }

  //----------

  void wantQuitEvents() {
    xcb_intern_atom_cookie_t  protocol_cookie = xcb_intern_atom_unchecked(MConnection, 1, 12, "WM_PROTOCOLS");
    xcb_intern_atom_cookie_t  close_cookie    = xcb_intern_atom_unchecked(MConnection, 0, 16, "WM_DELETE_WINDOW");
    xcb_intern_atom_reply_t*  protocol_reply  = xcb_intern_atom_reply(MConnection, protocol_cookie, 0);
    xcb_intern_atom_reply_t*  close_reply     = xcb_intern_atom_reply(MConnection, close_cookie, 0);
    MWMProtocolsAtom    = protocol_reply->atom;
    MWMDeleteWindowAtom = close_reply->atom;
    xcb_change_property(MConnection, XCB_PROP_MODE_REPLACE, MWindow, MWMProtocolsAtom, 4, 32, 1, &MWMDeleteWindowAtom);
    //xcb_flush(MConnection);
    free(protocol_reply); // note malloc'ed ??
    free(close_reply);
  }

  //----------

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

//------------------------------
private: // xcb hierarchy
//------------------------------

  xcb_window_t getXcbParent() {
    xcb_query_tree_cookie_t cookie = xcb_query_tree(MConnection,MWindow);
    xcb_query_tree_reply_t* reply = xcb_query_tree_reply(MConnection,cookie,nullptr);
    xcb_window_t parent = reply->parent;
    free(reply);
    return parent;
  }

  //----------

  uint32_t getXcbChildCount() {
    xcb_query_tree_cookie_t cookie = xcb_query_tree(MConnection,MWindow);
    xcb_query_tree_reply_t* reply;
    if ((reply = xcb_query_tree_reply(MConnection,cookie,nullptr))) {
      int num_children = xcb_query_tree_children_length(reply);
      free(reply);
      return num_children;
    }
    return 0;
  }

  //----------

  uint32_t getXcbChild(uint32_t AIndex) {
    xcb_query_tree_cookie_t cookie = xcb_query_tree(MConnection,MWindow);
    xcb_query_tree_reply_t* reply;
    if ((reply = xcb_query_tree_reply(MConnection,cookie,nullptr))) {
      xcb_window_t *children = xcb_query_tree_children(reply);
      xcb_window_t child = children[AIndex];
      free(reply);
      return child;
    }
    return 0;
  }

//------------------------------
private: // mouse
//------------------------------

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

  void freeXcbCursor(void) {
    if (MWindowCursor != XCB_NONE) {
      xcb_free_cursor(MConnection,MWindowCursor);
      MWindowCursor = XCB_NONE;
      //xcb_flush(MConnection);
    }
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

  //----------

  uint32_t remapButton(uint32_t AButton, uint32_t AState) {
    uint32_t b = AButton;
    return b;
  }

//------------------------------
private: // keyboard
//------------------------------

  void initKeyboard() {
    MKeySyms = xcb_key_symbols_alloc(MConnection);
  }

  //----------

  void cleanupKeyboard() {
    xcb_key_symbols_free(MKeySyms);
  }

  //----------

  // https://github.com/etale-cohomology/xcb/blob/master/loop.c

  uint32_t remapKey(uint32_t AKey, uint32_t AState) {
    int col = 0;
    xcb_keysym_t keysym = xcb_key_symbols_get_keysym(MKeySyms,AKey,col);
    //xcb_keycode_t* keycode = xcb_key_symbols_get_keycode(MKeySyms,keysym);
    //MIP_Print("AKey %i AState %i keysym %i keycode %i\n",AKey,AState,keysym,keycode[0]);
    //free(keycode);
    return keysym;
  }

  //----------

  uint32_t remapState(uint32_t AState) {
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

//------------------------------
private: // events
//------------------------------

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

  // returns null if no events
  // caller must free(event) !!!!

  xcb_generic_event_t* getEvent(bool ABlock=false) {
    //xcb_flush(MConnection);
    xcb_generic_event_t* event = nullptr;
    if (ABlock) event = xcb_wait_for_event(MConnection);
    else event = xcb_poll_for_event(MConnection);
    //if (event) {
    //  eventHandler(event);
    //  free(event); // not MALLOC'ed
    //}
    return event;
  }

  //----------

  // returns false if we received MWMDeleteWindowAtom
  // -> we are done (quit)
  // frees AEvent

  bool processEvent(xcb_generic_event_t* AEvent) {
    switch (AEvent->response_type & ~0x80) {

      case XCB_MAP_NOTIFY: {
        MWindowMapped = true;
        on_window_open();
        break;
      }

      case XCB_UNMAP_NOTIFY: {
        MWindowMapped = false;
        on_window_close();
        break;
      }

      /*
        Well... personally, I never needed these fancy functions.
        The most complicated that I needed was "only handle the last
        MotionNotify event in the queue and ignore earlier ones". For that,
        I had a variable xcb_generic_event_t *motion = NULL; before my loop.
        Each new motion notify event was not handled, but saved in this
        function. Other events were handled immediately. After the loop,
        if there was a motion notify event, it was handled.
      */

      case XCB_CONFIGURE_NOTIFY: {
        xcb_configure_notify_event_t* configure_notify = (xcb_configure_notify_event_t*)AEvent;

        // https://cairographics.org/cookbook/xcbsurface.c/
        // Avoid extra redraws by checking if this is the last expose event in the sequence
        /*
          while(configure_notify->count != 0) {
            xcb_generic_event_t* e2 = xcb_wait_for_event(MConnection);
            xcb_expose_event_t* ex2 = (xcb_expose_event_t *)e2;
            RECT.combine( KODE_Rect( ex2->x, ex2->y, ex2->width, ex2->height ) );
          }
        */

        /*
          while (TRUE) {
            if (XPending(display) || !pendingRedraws) {
              // if an event is pending, fetch it and process it
              // otherwise, we have neither events nor pending redraws, so we can
              // safely block on the event queue
              XNextEvent (display, &ev);
              if (isExposeEvent(&ev)) {
                pendingRedraws = TRUE;
            }
            else {
              processEvent(&ev);
            }
          }
          else {
            // we must have a pending redraw
            redraw();
            pendingRedraws = FALSE;
          }
        }
        */

        int16_t x = configure_notify->x;
        int16_t y = configure_notify->y;
        int16_t w = configure_notify->width;
        int16_t h = configure_notify->height;
        //MIP_Print("w,h: %i,%i MWindowWidth/Height: %i,%i  \n",w,h,MWindowWidth,MWindowHeight);
        //if ((x != MWindowXpos) || (y != MWindowYpos)) {
          MWindowXpos = x;
          MWindowYpos = y;
          on_window_move(x,y);
        //}

        //if ((w != MWindowWidth) || (h != MWindowHeight)) {
//          MIP_PRINT;
//          if ((w > 0) && (h > 0)) {
//            MWindowWidthScale = (double)w / (double)MWindowInitialWidth;
//            MWindowHeightScale = (double)h / (double)MWindowInitialHeight;
//            MIP_Print("scale: %.3f,%.3f\n",MWindowWidthScale,MWindowHeightScale);
//          }

//          if (MResizing) {
//            //MIP_Print("We're already resizing!\n");
//          }
//          else {
//            MResizing = true;
//            MIP_Print("starting resize timer\n");
//            MResizeTimer->start(MIP_XCB_WINDOW_RESIZE_SPEED,true);
//          }

          MWindowWidth  = w;
          MWindowHeight = h;

          on_window_resize(w,h);
        //}

        break;
      }

      case XCB_EXPOSE: {
        MWindowExposed = true;
        xcb_expose_event_t* expose = (xcb_expose_event_t *)AEvent;

        // https://cairographics.org/cookbook/xcbsurface.c/
        // Avoid extra redraws by checking if this is the last expose event in the sequence
        //while(expose->count != 0) {
        //  xcb_generic_event_t* e2 = xcb_wait_for_event(MConnection);
        //  xcb_expose_event_t* ex2 = (xcb_expose_event_t *)e2;
        //  RECT.combine( KODE_Rect( ex2->x, ex2->y, ex2->width, ex2->height ) );
        //}

        int16_t x = expose->x;
        int16_t y = expose->y;
        int16_t w = expose->width;
        int16_t h = expose->height;
        beginPaint();
        if (MFillBackground) fillColor(x,y,w,h,MBackgroundColor);
        on_window_paint(x,y,w,h);
        //xcb_flush(MConnection);
        endPaint();
        break;
      }

      case XCB_KEY_PRESS: {
        //if (!MWindowMapped) break;
        xcb_key_press_event_t* key_press = (xcb_key_press_event_t*)AEvent;
        uint8_t  k  = key_press->detail;
        uint16_t s  = key_press->state;
        uint32_t ts = key_press->time;
        k = remapKey(k,s);
        s = remapState(s);
        /*if (MWindowListener) MWindowListener->*/on_window_key_press(k,s,ts);
        break;
      }

      case XCB_KEY_RELEASE: {
        //if (!MWindowMapped) break;
        xcb_key_release_event_t* key_release = (xcb_key_release_event_t*)AEvent;
        uint8_t  k  = key_release->detail;
        uint16_t s  = key_release->state;
        uint32_t ts = key_release->time;
        k = remapKey(k,s);
        s = remapState(s);
        /*if (MWindowListener) MWindowListener->*/on_window_key_release(k,s,ts);
        break;
      }

      case XCB_BUTTON_PRESS: {
        //if (!MWindowMapped) break;
        xcb_button_press_event_t* button_press = (xcb_button_press_event_t*)AEvent;
        uint8_t  b  = button_press->detail;
        uint32_t s  = button_press->state;
        int32_t  x  = button_press->event_x;
        int32_t  y  = button_press->event_y;
        uint32_t ts = button_press->time;
        b = remapButton(b,s);
        s = remapState(s);
        /*if (MWindowListener) MWindowListener->*/on_window_mouse_click(b,s,x,y,ts);
        break;
      }

      case XCB_BUTTON_RELEASE: {
        //if (!MWindowMapped) break;
        xcb_button_release_event_t* button_release = (xcb_button_release_event_t*)AEvent;
        uint32_t b  = button_release->detail;
        uint32_t s  = button_release->state;
        int32_t  x  = button_release->event_x;
        int32_t  y  = button_release->event_y;
        uint32_t ts = button_release->time;
        b = remapButton(b,s);
        s = remapState(s);
        /*if (MWindowListener) MWindowListener->*/on_window_mouse_release(b,s,x,y,ts);
        break;
      }

      case XCB_MOTION_NOTIFY: {
        //if (!MWindowMapped) break;
        xcb_motion_notify_event_t* motion_notify = (xcb_motion_notify_event_t*)AEvent;
        //uint32_t  b = motion_notify->detail;
        uint32_t  s = motion_notify->state;
        int32_t   x = motion_notify->event_x;
        int32_t   y = motion_notify->event_y;
        uint32_t ts = motion_notify->time;
        s = remapState(s);
        /*if (MWindowListener) MWindowListener->*/on_window_mouse_move(s,x,y,ts);
        break;
      }

      case XCB_ENTER_NOTIFY: {
        if (!MWindowMapped) break;
        xcb_enter_notify_event_t* enter_notify = (xcb_enter_notify_event_t*)AEvent;
        //uint32_t  m = enter_notify->mode;
        //uint32_t  s = enter_notify->state;
        int32_t   x = enter_notify->event_x;
        int32_t   y = enter_notify->event_y;
        uint32_t ts = enter_notify->time;
        /*if (MWindowListener) MWindowListener->*/on_window_enter(x,y,ts);
        break;
      }

      case XCB_LEAVE_NOTIFY: {
        if (!MWindowMapped) break;
        xcb_leave_notify_event_t* leave_notify = (xcb_leave_notify_event_t*)AEvent;
        //uint32_t  m = leave_notify->mode;
        //uint32_t  s = leave_notify->state;
        int32_t   x = leave_notify->event_x;
        int32_t   y = leave_notify->event_y;
        uint32_t ts = leave_notify->time;
        /*if (MWindowListener) MWindowListener->*/on_window_leave(x,y,ts);
        break;
      }

      case XCB_CLIENT_MESSAGE: {
        xcb_client_message_event_t* client_message = (xcb_client_message_event_t*)AEvent;
        xcb_atom_t type = client_message->type;
        uint32_t data = client_message->data.data32[0];
        /*if (MWindowListener) MWindowListener->*/on_window_client_message(data);
        switch(data) {
          /*
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
          */
          case MIP_THREAD_ID_KILL:
            free(AEvent); // not malloc'ed
            return false; // we re finished
          default:
            break;
        }
        if (type == MWMProtocolsAtom) {
          if (data == MWMDeleteWindowAtom) {
            free(AEvent); // not malloc'ed
            return false; // we re finished
          }
        }

        break;
      } // switch

    }
    free(AEvent);
    return true; // we are still alive
  }

//------------------------------
//private: // resize timer
//------------------------------

//  void on_timerCallback(MIP_Timer* ATimer) {
//    if (ATimer == MResizeTimer) {
//      MIP_Print("resize timeout\n");
//      MResizeTimer->stop();
//      MResizing = false;
//    }
//  };

//------------------------------
//private: // event thread
//------------------------------

  static
  void* xcb_event_thread_proc(void* AWindow) {
    //mip_xcb_event_thread_pid = getpid();
    MIP_XcbWindow* window = (MIP_XcbWindow*)AWindow;
    if (window) {
      window->xcb_event_thread_start_callback(window);
      xcb_connection_t* connection = window->MConnection;
      xcb_flush(connection);
      while (window->MEventThreadActive) {
        xcb_generic_event_t* event = xcb_wait_for_event(connection);
        if (event) {
          //MIP_Print("event!\n");
          // double-check (in case we have closed the window before processing
          // all events in queue
          //MIP_PRINT;
          if (window->MEventThreadActive) {
            //MIP_PRINT;
            if (!window->processEvent(event)) {
              window->xcb_event_thread_stop_callback(window);
              return nullptr;
            }
          } // active
        } // event
      } // while
      window->xcb_event_thread_stop_callback(window);
      return nullptr;
    } // window
    return nullptr;
  }

//------------------------------
//
//------------------------------

  void xcb_event_thread_start_callback(void* AUser) {
    if (MEventThreadStartCallback) MEventThreadStartCallback(AUser);
  }

  void xcb_event_thread_stop_callback(void* AUser) {
    if (MEventThreadStopCallback) MEventThreadStopCallback(AUser);
  }

};

//----------------------------------------------------------------------
#endif
