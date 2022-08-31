#ifndef mip_win32_window_included
#define mip_win32_window_included
//----------------------------------------------------------------------

/*
  - the HINSTANCE is saved when DllMain is called..
    after global variable initialization, but before main()
  - window class is registered the first time a window is created
    see bottom of this file for the singleton-ish class and global/static
    variable used for this..
  - if window is created with null listener, self is used..
  - buffer-events (on_xxxBuffer) are only called if MIP_NO_WINDOW_BACKBUFFER is not defined
*/

//----------------------------------------------------------------------

#include "mip.h"
#include "gui/base/mip_base_window.h"
#include "gui/win32/mip_win32.h"

//----------------------------------------------------------------------

char* MIP_Win32ClassName(void);
LRESULT CALLBACK mip_eventproc_win32(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

const char* mip_win32_cursors[] = {
  IDC_ARROW,        //  0  smc_default
  IDC_ARROW,        //  1  smc_arrow
  IDC_UPARROW,      //  2  smc_arrowUp
  IDC_ARROW,        //  3  smc_arrowDown        // missing
  IDC_ARROW,        //  4  smc_arrowLeft        // missing
  IDC_ARROW,        //  5  smc_arrowRight       // missing
  IDC_SIZENS,       //  6  smc_arrowUpDown
  IDC_SIZEWE,       //  7  smc_arrowLeftRight
  IDC_SIZENWSE,     //  8  smc_arrowDiagLeft
  IDC_SIZENESW,     //  9  smc_arrowDiagRight
  IDC_SIZEALL,      // 10  smc_move
  IDC_WAIT,         // 11  smc_wait
  IDC_APPSTARTING,  // 12  smc_arrowWait
  IDC_HAND,         // 13  smc_hand             // missing
  IDC_HAND,         // 14  smc_finger
  IDC_CROSS,        // 15  smc_cross
  IDC_ARROW,        // 16  smc_pencil           // missing
  IDC_ARROW,        // 17  smc_plus             // missing
  IDC_HELP,         // 18  smc_question
  IDC_IBEAM         // 19  smc_ibeam
  //IDC_NO
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Win32Window
: public MIP_BaseWindow
, public MIP_Drawable {

  friend LRESULT CALLBACK mip_win32_eventproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//------------------------------
private:
//------------------------------

  HCURSOR     MDefaultCursor;
  HWND        MWinHandle;
  HCURSOR     MWinCursor;
  PAINTSTRUCT MWinPaintStruct;
  HDC         MWinPaintDC;
  HCURSOR     MUserCursors[128];

  const char* MWindowTitle    = "";
  void*       MWindowParent   = nullptr;
  bool        MEmbedded       = false;
  int32_t     MMouseXpos      = 0;
  int32_t     MMouseYpos      = 0;
  int32_t     MAdjustedWidth  = 0;
  int32_t     MAdjustedHeight = 0;
  int32_t     MCurrentCursor  = 0;

//------------------------------
private:
//------------------------------

  int32_t     MWindowWidth        = 0;
  int32_t     MWindowHeight       = 0;
  uint32_t    MScreenDepth        = 0;
  double      MWindowWidthScale   = 1.0;
  double      MWindowHeightScale  = 1.0;

  bool        MFillBackground     = false;
  uint32_t    MBackgroundColor    = 0x808080;

//------------------------------
public:
//------------------------------

  //MIP_Win32Window(uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false)
  MIP_Win32Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent)
  : MIP_BaseWindow() {

    MWindowTitle = "MIP_Win32Window";
    //
    MMouseXpos  = -1;
    MMouseYpos  = -1;
    memset(MUserCursors,0,sizeof(MUserCursors));
    MCurrentCursor = -1;
    setMouseCursor(MIP_CURSOR_DEFAULT);

    RECT rc = { 0,0,(long int)AWidth + 1,(long int)AHeight + 1};

//    //if (AEmbedded) {
//    if (AParent != 0) {

      MEmbedded = true;
      AdjustWindowRectEx(&rc,WS_POPUP,FALSE,WS_EX_TOOLWINDOW);
      int32_t x = rc.left;
      int32_t y = rc.top;
      int32_t w = rc.right - rc.left + 1;
      int32_t h = rc.bottom - rc.top + 1;
      MAdjustedWidth = w - AWidth;
      MAdjustedHeight = h - AHeight;

      MWinHandle = CreateWindowEx(
        0, //WS_EX_TOOLWINDOW,
        //kode_global_WinClassName,
        MIP_Win32ClassName(),
        0,
        //WS_CHILD + WS_VISIBLE, //WS_POPUP,
        WS_POPUP + WS_VISIBLE,
        x,              //rc.left,
        y,              //rc.top,
        w,              //rc.right-rc.left+1, // +2 ??
        h,              //rc.bottom-rc.top+1, // +2 ??
        HWND(AParent),  // Hint: Conversion between ordinals and pointers is not portable,
        0,
        MIP_GLOBAL_WIN32_INSTANCE,
        0
      );
      //reparent(AParent);

//    }
//
//    else {
//
//      MEmbedded = false;
//
//      AdjustWindowRectEx(&rc,WS_OVERLAPPEDWINDOW,FALSE,WS_EX_OVERLAPPEDWINDOW);
//      int32_t wx = ((GetSystemMetrics(SM_CXSCREEN)-AWidth) >> 1) + rc.left;
//      int32_t wy = ((GetSystemMetrics(SM_CYSCREEN)-AHeight) >> 1) + rc.top;
//      int32_t x = wx;
//      int32_t y = wy;
//      int32_t w = rc.right-rc.left+1;
//      int32_t h = rc.bottom-rc.top+1;
//      MAdjustedWidth = w - AWidth;
//      MAdjustedHeight = h - AHeight;
//
//      MWinHandle = CreateWindowEx(
//        WS_EX_OVERLAPPEDWINDOW,     // dwExStyle
//        MIP_Win32ClassName(),     // lpClassName
//        MWindowTitle,                     // lpWindowName
//        WS_OVERLAPPEDWINDOW,        // dwStyle
//        x,                          // center x
//        y,                          // center y
//        w,                          // wWidth
//        h,                          // wHeight
//        0,                          // hWndParent
//        0,                          // hMenu
//        MIP_GLOBAL_WIN32_INSTANCE,  // hInstance
//        0                           // lpParam
//      );
//      SetFocus(MWinHandle);
//
//    } // embedded

    SetWindowLongPtr(MWinHandle,GWLP_USERDATA,(LONG_PTR)this);
  }

  //----------

  virtual ~MIP_Win32Window() {
    //destroyUserCursors();
    DestroyWindow(MWinHandle);
  }

//------------------------------
public:
//------------------------------

  HWND  hwnd(void)  { return MWinHandle; }
  HDC   hdc(void)   { return MWinPaintDC; }

//------------------------------
public:
//------------------------------

  virtual int32_t getWindowWidth()  { return MWindowWidth; }
  virtual int32_t getWindowHeight() { return MWindowHeight; }
  virtual double  getWindowWidthScale()  { return MWindowWidthScale; }
  virtual double  getWindowHeightScale() { return MWindowHeightScale; }

  virtual void    setWindowFillBackground(bool AFill=true)          { MFillBackground = AFill; }
  virtual void    setWindowBackgroundColor(uint32_t AColor)         { MBackgroundColor = AColor; }

  //virtual bool    isWindowExposed() { return MWindowExposed; }
  //virtual bool    isWindowMapped()  { return MWindowMapped; }

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

  //xcb_connection_t*   drawable_getXcbConnection()  final { return MConnection; }
  //xcb_visualid_t      drawable_getXcbVisual()      final { return MScreenVisual; }
  //xcb_drawable_t      drawable_getXcbDrawable()    final { return MWindow; }
  //xcb_window_t        drawable_getXcbWindow()      final { return MWindow; }

  //Display*            drawable_getXlibDisplay()    final { return MDisplay; }

  //#ifdef MIP_USE_CAIRO
  //bool                drawable_isCairo()           final { return true; }
  //cairo_surface_t*    drawable_getCairoSurface()   final { return MCairoSurface; }
  //#endif

//------------------------------
public:
//------------------------------

  void setWindowPos(uint32_t AXpos, uint32_t AYpos) override {
    SetWindowPos(MWinHandle,0,AXpos,AYpos,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
  }

  //----------

  void setWindowSize(uint32_t AWidth, uint32_t AHeight) override {
    int w = AWidth + MAdjustedWidth + 0;
    int h = AHeight + MAdjustedHeight + 0;
    SetWindowPos(MWinHandle,HWND_TOP,0,0,w,h, SWP_NOMOVE);
    //MRect.w = w;
    //MRect.h = h;
    MWindowWidth = w;
    MWindowHeight = h;
  }

  //----------

  void setWindowTitle(const char* ATitle) override {
    SetWindowText(MWinHandle, ATitle);
  }

  //----------

  void openWindow() override {
    ShowWindow(MWinHandle,SW_SHOW);
    #ifdef MIP_WIN32_IDLE_TIMER
    SetTimer(MWinHandle,s3_ts_idle,S3_WIN32_IDLE_MS,S3_NULL);
    #endif
  }

  //----------

  void closeWindow() override {
    #ifdef MIP_WIN32_IDLE_TIMER
    KillTimer(MWinHandle,s3_ts_idle);
    #endif
    ShowWindow(MWinHandle,SW_HIDE);
  }

  //----------

  void flush(void) override {
  }

  //----------

  void sync(void) override {
  }

  //----------

  void eventLoop() override {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  //----------

  void startEventThread() override {
  }

  //----------

  void stopEventThread() override {
  }

  //----------

  void sendClientMessage(uint32_t AData, uint32_t AType) override {
  }

  //----------

  void invalidateRegion(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    RECT R;
    R.left   = AXpos;
    R.top    = AYpos;
    R.right  = AXpos + AWidth;  // - 1;
    R.bottom = AYpos + AHeight; // - 1;
    InvalidateRect(MWinHandle,&R,false);
  }

  //----------

  void redrawRegion(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
  }

  //----------

  /*
    AParent: A handle to the new parent window. If this parameter is NULL, the
    desktop window becomes the new parent window. If this parameter is
    HWND_MESSAGE, the child window becomes a message-only window.

    Remarks
    An application can use the SetParent function to set the parent window of a
    pop-up, overlapped, or child window.
    If the window identified by the hWndChild parameter is visible, the system
    performs the appropriate redrawing and repainting.
    For compatibility reasons, SetParent does not modify the WS_CHILD or
    WS_POPUP window styles of the window whose parent is being changed.
    Therefore, if hWndNewParent is NULL, you should also clear the WS_CHILD bit
    and set the WS_POPUP style after calling SetParent. Conversely, if
    hWndNewParent is not NULL and the window was previously a child of the
    desktop, you should clear the WS_POPUP style and set the WS_CHILD style
    before calling SetParent.
  */

  void reparentWindow(intptr_t AParent) override {
    MIP_PRINT;
    SetWindowLongPtr( MWinHandle, GWL_STYLE, ( GetWindowLongPtr(MWinHandle,GWL_STYLE) & ~WS_POPUP ) | WS_CHILD );
    SetParent(MWinHandle,(HWND)AParent);
    //MEmbedded = true;
  }

  //----------

  void beginPaint()  override {
    MWinPaintDC = BeginPaint(MWinHandle,&MWinPaintStruct);
  }

  //----------

  void endPaint()  override {
    EndPaint(MWinHandle,&MWinPaintStruct);
  }

  //----------

  void setEventThreadCallbacks( void (*AStart)(void* AUser), void (*AStop)(void* AUser) ) override {
  }

//------------------------------
public: // mouse
//------------------------------

  void setMouseCursor(int32_t ACursor) override {
    if (ACursor != MCurrentCursor) {
      MCurrentCursor = ACursor;
      if (ACursor>=128) MWinCursor = MUserCursors[ACursor-128];
      else MWinCursor = LoadCursor(NULL,mip_win32_cursors[ACursor]);
      SetCursor(MWinCursor);
    }
  }

  //----------

  // Moves the cursor to the specified screen coordinates
  // will fire a WM_MOUSEMOVE event..

  void setMouseCursorPos(int32_t AXpos, int32_t AYpos) override {
    POINT pos;
    pos.x = AXpos;
    pos.y = AYpos;
    ClientToScreen(MWinHandle,&pos);
    int32_t x = pos.x;
    int32_t y = pos.y;
    SetCursorPos(x,y);
  }

  //----------

  void hideMouseCursor(void) override {
    ShowCursor(false);
  }

  //----------

  void showMouseCursor(void) override {
    ShowCursor(true);
  }

  //----------

  void grabMouseCursor(void) override {
    SetCapture(MWinHandle);
  }

  //----------

  void releaseMouseCursor(void) override {
    ReleaseCapture();
  }

//------------------------------
public: // paint
//------------------------------

  void fillColor(uint32_t AColor) override {
  }

  void fillColor(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor) override {
  }

  void blitBuffer(int32_t ADstX, int32_t ADstY, void* AData, uint32_t AStride, int32_t ASrcW, int32_t ASrcH) override {
  }

  //void blitImage(int32_t ADstX, int32_t ADstY, /*xcb_image_t*/void* AImage) {}
  //void blitDrawable(int32_t ADstX, int32_t ADstY, /*xcb_drawable_t*/intptr_t ADrawable, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) {}

  void blitDrawable(int32_t ADstX, int32_t ADstY, MIP_Drawable* ADrawable, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) override {
  }

  //----------

  // only valid dugring begin/endPaint...
  // needs MWinPaintDC

  //  void blit(int32 dst_x, int32 dst_y, S3_Painter* APainter, int32 src_x, int32 src_y, int32 src_w, int32 src_h) {
  //    BitBlt(MWinPaintDC,dst_x,dst_y,src_w,src_h,APainter->hdc(),src_x,src_y,SRCCOPY);
  //  }

  /*
    a bitmap can't be attached to more than one dc at a time..
    and it's currently (probably) attached to the painter
    so, we would need to detach it from there first if we want to
    use this method..
  */

  //void blit(int32 ADstX, int32 ADstY, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {
  //  HDC tempdc = CreateCompatibleDC(MWinPaintDC);
  //  HGDIOBJ oldbitmap = SelectObject(tempdc,ASource->hbitmap());
  //  uint32 rop = s3_ro_src;//MSrcRasterOp;//SRCCOPY;
  //  BitBlt(MWinPaintDC,ADstX,ADstY,ASrcW,ASrcH,tempdc,ASrcX,ASrcY,rop);
  //  SelectObject(tempdc,oldbitmap);
  //  DeleteDC(tempdc);
  //}

//------------------------------
private: // event handler
//------------------------------

  int32_t remapMouseButton(int32_t AButton) {
    int32_t result = MIP_BUTTON_NONE;
    switch(AButton) {
      case MK_LBUTTON:  result = MIP_BUTTON_LEFT;      break;
      case MK_MBUTTON:  result = MIP_BUTTON_MIDDLE;    break;
      case MK_RBUTTON:  result = MIP_BUTTON_RIGHT;     break;
      //case MK_XBUTTON1: S3_Trace("xbutton1\n"); /*result = smb_x1;*/        break;
      //case MK_XBUTTON2: S3_Trace("xbutton2\n"); /*result = smb_x2;*/        break;
      //case WM_LBUTTONDOWN: result = smb_left;      break;
      //case WM_MBUTTONDOWN: result = smb_middle;    break;
      //case WM_RBUTTONDOWN: result = smb_right;     break;
      //case 4: result = smb_wheelUp;   break;
      //case 5: result = smb_wheelDown; break;
      //case 8: result = smb_backward;  break;
      //case 9: result = smb_forward;   break;
    }
    return result;
  }

  //----------

  int32_t remapMouseKey(int32_t AState) {
    int32_t result = MIP_KEY_NONE;
    if ((AState & MK_SHIFT  ) != 0) result |= MIP_KEY_SHIFT;
    //if ((AState & ) != 0) result |= MIP_KEY_CAPS;
    if ((AState & MK_CONTROL  ) != 0) result |= MIP_KEY_CTRL;
    //if ((AState & ) != 0) result |= MIP_KEY_ALT;
    //if ((AState & 128) != 0) result |= MIP_KEY_ALTGR;
    return result;
  }

  //----------

  /*
  WM_KEYDOWN

  wParam  The virtual-key code of the nonsystem key. See Virtual-Key Codes.
  lParam  The repeat count, scan code, extended-key flag, context code,
  previous key-state flag, and transition-state flag, as shown following.

  Bits	Meaning
  0-15  The repeat count for the current message. The value is the number of
        times the keystroke is autorepeated as a result of the user holding
        down the key. If the keystroke is held long enough, multiple messages
        are sent. However, the repeat count is not cumulative.
  16-23 The scan code. The value depends on the OEM.
  24    Indicates whether the key is an extended key, such as the right-hand
        ALT and CTRL keys that appear on an enhanced 101- or 102-key keyboard.
        The value is 1 if it is an extended key; otherwise, it is 0.
  25-28 Reserved; do not use.
  29    The context code. The value is always 0 for a WM_KEYDOWN message.
  30    The previous key state. The value is 1 if the key is down before the
        message is sent, or it is zero if the key is up.
  31    The transition state. The value is always 0 for a WM_KEYDOWN message.
  */

  /*
  )int WINAPI ToAscii(
    _In_           UINT   uVirtKey,
    _In_           UINT   uScanCode,
    _In_opt_ const BYTE   *lpKeyState,
    _Out_          LPWORD lpChar,
    _In_           UINT   uFlags
  );
  */

  int32_t remapKeyCode(WPARAM wParam, LPARAM lParam) {
    switch(wParam) {
     case VK_BACK: return MIP_KEY_BACKSPACE;
    }
    return 0;
  }

  //------------------------------
  //
  //------------------------------

  LRESULT eventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    LRESULT result = 0;
    MIP_DRect rc;
    int32_t x,y,b,w,h,d;//,k;
    switch (message) {

      case WM_DESTROY: {
        if (!MEmbedded) PostQuitMessage(0);
        break;
      }

      case WM_GETDLGCODE: {
        // we want keypresses
        result = DLGC_WANTALLKEYS;
        break;
      }

      case WM_ERASEBKGND: {
        // tell window not to erase background
        result = 1;
        break;
      }

      case WM_KEYDOWN: {
        //if (MListener) MListener->on_keyDown(this,wParam,lParam);
        //k = remapKeyCode(wParam,lParam);
        on_window_key_press(wParam,lParam,0);
        break;
      }

      case WM_KEYUP: {
        //if (MListener) MListener->on_keyUp(this,wParam,lParam);
        //k = remapKeyCode(wParam,lParam);
        on_window_key_release(wParam,lParam,0);
        break;
      }

      //case WM_CHAR:
      //  break;

      case WM_XBUTTONDOWN:
      case WM_LBUTTONDOWN:
      case WM_RBUTTONDOWN:
      case WM_MBUTTONDOWN: {
        switch (message) {
          case WM_LBUTTONDOWN:  b = MIP_BUTTON_LEFT;   break;
          case WM_MBUTTONDOWN:  b = MIP_BUTTON_MIDDLE; break;
          case WM_RBUTTONDOWN:  b = MIP_BUTTON_RIGHT;  break;
          case WM_XBUTTONDOWN:
            if ((wParam & MK_XBUTTON1) != 0) b = MIP_BUTTON_BACKWARD;
            if ((wParam & MK_XBUTTON2) != 0) b = MIP_BUTTON_FORWARD;
            break;
          default:              b = MIP_BUTTON_NONE;   break;
        }
        x = short(LOWORD(lParam));
        y = short(HIWORD(lParam));
        //if (MListener) MListener->on_mouseDown(this,x,y,b,remapKey(wParam));
        on_window_mouse_click(b,remapMouseKey(wParam),x,y,0);
//        if (MFlags & s3_wf_capture) grabCursor();
        break;
      }

      case WM_MOUSEMOVE: {
        x = short(LOWORD(lParam));
        y = short(HIWORD(lParam));
        //if (MListener) MListener->on_mouseMove(this,x,y,remapKey(wParam));
        on_window_mouse_move(remapMouseKey(wParam),x,y,0);
        MMouseXpos = x;
        MMouseYpos = y;
        break;
      }

      case WM_XBUTTONUP:
      case WM_LBUTTONUP:
      case WM_RBUTTONUP:
      case WM_MBUTTONUP: {
        switch (message) {
          case WM_LBUTTONUP:  b = MIP_BUTTON_LEFT;   break;
          case WM_MBUTTONUP:  b = MIP_BUTTON_MIDDLE; break;
          case WM_RBUTTONUP:  b = MIP_BUTTON_RIGHT;  break;
          //case WM_XBUTTONUP:  b = MIP_BUTTON_s3_mb_side;   break;
          default:            b = MIP_BUTTON_NONE;   break;
        }
        x = short(LOWORD(lParam));
        y = short(HIWORD(lParam));
        //if (MListener) MListener->on_mouseUp(this,x,y,b,remapKey(wParam));
        on_window_mouse_release(b,remapMouseKey(wParam),x,y,0);
//        if (MFlags&s3_wf_capture) releaseCursor();
        break;
      }

      case WM_MOUSEWHEEL: {
        d = GET_WHEEL_DELTA_WPARAM(wParam);
        //if (d>0) { if (MListener) MListener->on_mouseDown(this,MMouseXpos,MMouseYpos,smb_wheelUp,  smb_none); }
        //if (d<0) { if (MListener) MListener->on_mouseDown(this,MMouseXpos,MMouseYpos,smb_wheelDown,smb_none); }
        if (d > 0) { on_window_mouse_click(MIP_BUTTON_SCROLL_UP,   MIP_KEY_NONE, MMouseXpos,MMouseYpos,0); }
        if (d < 0) { on_window_mouse_click(MIP_BUTTON_SCROLL_DOWN, MIP_KEY_NONE, MMouseXpos,MMouseYpos,0); }
        break;
      }

      /*
        Only windows that have the CS_DBLCLKS style can receive
        WM_LBUTTONDBLCLK messages, which the system generates whenever the
        user presses, releases, and again presses the left mouse button
        within the system's double-click time limit. Double-clicking the left
        mouse button actually generates a sequence of four messages:
        WM_LBUTTONDOWN, WM_LBUTTONUP, WM_LBUTTONDBLCLK, and WM_LBUTTONUP.
      */

      #ifdef MIP_MOUSE_DOUBLECLICK
      /*
      case WM_LBUTTONDBLCLK:
      case WM_MBUTTONDBLCLK:
      case WM_RBUTTONDBLCLK:
      case WM_XBUTTONDBLCLK: {
        switch (message) {
          case WM_LBUTTONDBLCLK:  b = s3_mb_left;   break;
          case WM_MBUTTONDBLCLK:  b = s3_mb_middle; break;
          case WM_RBUTTONDBLCLK:  b = s3_mb_right;  break;
          case WM_XBUTTONDBLCLK:  b = s3_mb_side;   break;
          default:                b = s3_mb_none;   //break;
        }
        if (b==s3_mb_side) {
          if ((wParam & MK_XBUTTON1) != 0) b = s3_mb_backward;
          if ((wParam & MK_XBUTTON2) != 0) b = s3_mb_forward;
        }
        //S3_Trace("dblclick: %i\n",b);
        x = short(LOWORD(lParam));
        y = short(HIWORD(lParam));
        //x = GET_X_LPARAM(lParam);
        //y = GET_Y_LPARAM(lParam);
        //if (MListener) MListener->on_mouseUp(this,x,y,b,remapKey(wParam));
        on_widgetMouseDoubleClick(this,x,y,b,remapMouseKey(wParam));
        break;
      }
      */
      #endif // MIP_MOUSE_DOUBLECLICK

      /*
        is it possible to receive a WM_PAINT when some other thread is
        drawing to the backbuffer?
      */

      case WM_PAINT: {
        beginPaint();

        int32_t x = MWinPaintStruct.rcPaint.left;
        int32_t y = MWinPaintStruct.rcPaint.top;
        int32_t w = MWinPaintStruct.rcPaint.right  - MWinPaintStruct.rcPaint.left + 1;
        int32_t h = MWinPaintStruct.rcPaint.bottom - MWinPaintStruct.rcPaint.top + 1;
        //if (MFillBackground) fillColor(x,y,w,h,MBackgroundColor);
        //rc = MIP_DRect(x,y,w,h);
        //MIP_Print("WM_PAINT: x %i y %i w %i h %i\n",x,y,w,h);

        on_window_paint(x,y,w,h);
        endPaint();
        break;
      }

      case WM_SETCURSOR: {
        // called every mouse move
        if (LOWORD(lParam)==HTCLIENT) {
          SetCursor(MWinCursor);
          //setCursor(MCurrentCursor);
          result = 1;
        }
        else result = DefWindowProc(hWnd,message,wParam,lParam);
        break;
      }

      case WM_SIZE: {
        w = short(LOWORD(lParam));
        h = short(HIWORD(lParam));
        //if ( (w!=MRect.w) || (h!=MRect.h) ) {
        if ( (w != MWindowWidth) || (h != MWindowHeight) ) {
          //if (MListener) {
          //  #ifndef S3_NO_WINDOW_BACKBUFFER
          //  MListener->on_bufferResize(this,w,h);
          //  #else
          //  MListener->on_windowResize(this,w,h);
          //  #endif
          //}
          on_window_resize(w,h);
          //if (MFlags & s3_wf_autoalign) on_widgetAlign(this);
          //#ifndef S3_NO_WINDOW_BACKBUFFER
          //if (MListener) MListener->on_bufferPaint(this,S3_NULL,s3_pm_normal); //redraw;
          //#endif
        }
        break;
      }

      case WM_TIMER: {
        //if (MListener) {
        //  if (wParam==s3_ts_timer) MListener->on_windowTimer(this);
        //  if (wParam==s3_ts_idle) MListener->on_windowIdle(this);
        //}
        break;
      }

      default: {
        result = DefWindowProc(hWnd,message,wParam,lParam);
        break;
      }
    }
    return result;
  }

//------------------------------
private:
//------------------------------

  /*

  virtual // S3_Window_Base
  void startTimer(int32 ms) {
    SetTimer(MWinHandle,s3_ts_timer,ms,S3_NULL);
  }

  //----------

  // The KillTimer function does not remove WM_TIMER messages already posted
  // to the message queue.

  virtual // S3_Window_Base
  void stopTimer(void) {
    KillTimer(MWinHandle,s3_ts_timer);
  }

  //----------

    virtual // S3_Window_Base
    void renderLoop(void) {
      MSG msg;
      while (true) {
        while (PeekMessage(&msg,S3_NULL,0,0,PM_REMOVE)) {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
        if (msg.message == WM_QUIT) break;
        if (MListener) MListener->on_windowRenderFrame(this);
      }
    }

    virtual // S3_Window_Base
    void createUserCursor(uint32 AIndex, int32 AWidth, int32 AHeight, const uint8* AAndMask, const uint8* AXorMask, int32 AXhotspot, int32 AYhotspot) {
      if ((AIndex>=128) && (AIndex<=255)) {
        HCURSOR cursor = CreateCursor(s3_global_WinInstance,AXhotspot,AYhotspot,AWidth,AHeight,AAndMask,AXorMask);
        MUserCursors[AIndex-128] = cursor;
      }
    }

    virtual // S3_Window_Base
    void destroyUserCursors(void) {
      for (uint32 i=0; i<128; i++) {
        HCURSOR cursor = MUserCursors[i];
        if (cursor) DestroyCursor(cursor);
      }
    }

    //virtual // S3_Window_Base
    //void getCursorPos(int32* AXpos, int32* AYpos) {
    //  *AXpos = 0;
    //  *AYpos = 0;
    //}

    virtual // S3_Window_Base
    void warpCursor(int32 AXpos, int32 AYpos, int32 AWidth, int32 AHeight) {
      RECT rc;
      POINT pt = { AXpos, AYpos };
      POINT pt2 = { AXpos+AWidth-1, AYpos+AHeight-1 };
      ClientToScreen(MWinHandle, &pt);
      ClientToScreen(MWinHandle, &pt2);
      SetRect(&rc, pt.x, pt.y, pt2.x, pt2.y);
      ClipCursor(&rc);
    }

    //----------

    virtual // S3_Window_Base
    void unwarpCursor(void) {
      ClipCursor(NULL);
    }

  */

};

//----------------------------------------------------------------------
//
// event proc
//
//----------------------------------------------------------------------

LRESULT CALLBACK mip_win32_eventproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  MIP_Win32Window* window = (MIP_Win32Window*)GetWindowLongPtr(hWnd,GWLP_USERDATA);
	if (!window) return DefWindowProc(hWnd,message,wParam,lParam);
  return window->eventHandler(hWnd, message, wParam, lParam);
}

//----------------------------------------------------------------------
// window class
//----------------------------------------------------------------------

/*
  https://msdn.microsoft.com/en-us/library/windows/desktop/ms633586%28v=vs.85%29.aspx

  All window classes that an application registers are unregistered when it
  terminates.
  No window classes registered by a DLL are unregistered when the DLL is
  unloaded. A DLL must explicitly unregister its classes when it is unloaded.
*/

//----------

/*
  this one is a bit messy..
  we need:
  - HINSTANCE (from DLL)
  - callback function (which calls the window class)
  it should be registered only once, even for multiple instances on a plugin,
  so the plugin can't destroy the window class when it dies, since another
  instance might still use the window class for its editor..
  so, we do something singleton-ish..
  a static class with a constructor that is called before DllMain and any
  other function/method/class..
  and a destructor that is called when the global variable goes out of scope,
  when the dll is unloaded..
*/

class MIP_Win32WindowClass {

//------------------------------
private:
//------------------------------

  bool     MRegistered                  = false;
  char     MName[MIP_MAX_STRING_LENGTH] = "mip_window_0";
  ATOM     MAtom                        = 0;
  WNDCLASS MClass                       = {};

//------------------------------
public:
//------------------------------

  MIP_Win32WindowClass() {
    MRegistered = false;
  }

  //----------

  ~MIP_Win32WindowClass() {
    if (MRegistered) {
      UnregisterClass(MName,MIP_GLOBAL_WIN32_INSTANCE);
    }
  }

  //----------

  char* registerClass(void) {
    if (!MRegistered) {
      MIP_CreateUniqueString(MName,(char*)"mip_window_",&MIP_GLOBAL_WIN32_INSTANCE);
      memset(&MClass,0,sizeof(MClass));
      MClass.style = CS_HREDRAW | CS_VREDRAW;// | CS_DBLCLKS;
      //#ifdef MIP_MOUSE_DOUBLECLICK
      MClass.style |= CS_DBLCLKS;
      //#endif
      MClass.lpfnWndProc    = &mip_win32_eventproc;
      MClass.hInstance      = MIP_GLOBAL_WIN32_INSTANCE;
      MClass.lpszClassName  = MName;
      MAtom                 = RegisterClass(&MClass);
      MRegistered           = true;
    }
    return MName;
  }

};

//----------

/*
  destructor automatically called when dll/exe unloaded
  it should have been hidden from user, but since we're using
  only .h files, that's a bit problematic..
*/

MIP_Win32WindowClass MIP_GLOBAL_WIN32_WINDOW_CLASS;

char* MIP_Win32ClassName(void) {
  return MIP_GLOBAL_WIN32_WINDOW_CLASS.registerClass();
}



//----------------------------------------------------------------------
#endif
