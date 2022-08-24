#ifndef s3_window_win32_included
#define s3_window_win32_included
//----------------------------------------------------------------------

/*
  - the HINSTANCE is saved when DllMain is called..
    after global variable initialization, but before main()
  - window class is registered the first time a window is created
    see bottom of this file for the singleton-ish class and global/static
    variable used for this..
  - if window is created with null listener, self is used..
  - buffer-events (on_xxxBuffer) are only called if S3_NO_WINDOW_BACKBUFFER is not defined
*/

//----------------------------------------------------------------------

#include "common/s3_rect.h"
#include "common/s3_utils.h"
#include "gui/base/s3_window_base.h"
#include "gui/s3_widget.h"
#include "gui/s3_window_listener.h"

//----------------------------------------------------------------------

//class S3_Window_Win32;
//typedef S3_Window_Win32 S3_Window_Implementation;

//----------

char* S3_Window_ClassName(void);
LRESULT CALLBACK s3_eventproc_win32(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//----------

//static
const char* s3_win32_cursors[] = {
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

class S3_Window_Win32
: public S3_Window_Base
, public S3_WindowListener
, public S3_Widget {

  friend LRESULT CALLBACK s3_eventproc_win32(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

  protected:
    S3_WindowListener*  MListener;

  private:
    const char*         MTitle;
    void*               MParent;
    bool                MEmbedded;
    int32               MMouseXpos;
    int32               MMouseYpos;
    int32               MAdjustedWidth;
    int32               MAdjustedHeight;
    int32               MCurrentCursor;
  private:
    HCURSOR             MDefaultCursor;
    HWND                MWinHandle;
    HCURSOR             MWinCursor;
    PAINTSTRUCT         MWinPaintStruct;
    HDC                 MWinPaintDC;
    HCURSOR             MUserCursors[128];

  //------------------------------
  //
  //------------------------------

  public:

    S3_Window_Win32(S3_WindowListener* AListener, S3_Rect ARect, void* AParent=S3_NULL)
    : S3_Window_Base(AListener,ARect,AParent)
    , S3_Widget(ARect,s3_wa_fillClient) {
      MName = "S3_Window_Win32";
      if (AListener) MListener = AListener;
      else MListener = this;
      MName = "S3_Window";
      MTitle = "S3_Window";
      MParent = AParent;
      //
      MMouseXpos  = -1;
      MMouseYpos  = -1;
      S3_Memset(MUserCursors,0,sizeof(MUserCursors));
      MCurrentCursor = -1;
      setCursor(s3_mc_default);
      //
      RECT rc = { 0,0,ARect.w+1,ARect.h+1};
      if (AParent) {
        //MEmbedded = true;
        MEmbedded = true;
        AdjustWindowRectEx(&rc,WS_POPUP,FALSE,WS_EX_TOOLWINDOW);
        int32 x = rc.left;
        int32 y = rc.top;
        int32 w = rc.right - rc.left + 1;
        int32 h = rc.bottom - rc.top + 1;
        MAdjustedWidth = w - ARect.w;
        MAdjustedHeight = h - ARect.h;
        MWinHandle = CreateWindowEx(
          0, //WS_EX_TOOLWINDOW,
          //kode_global_WinClassName,
          S3_Window_ClassName(),
          0,
          WS_CHILD + WS_VISIBLE, //WS_POPUP,
          x, //rc.left,
          y, //rc.top,
          w, //rc.right-rc.left+1, // +2 ??
          h, //rc.bottom-rc.top+1, // +2 ??
          HWND(AParent), // Hint: Conversion between ordinals and pointers is not portable,
          0,
          s3_global_WinInstance,
          0
        );
        //reparent(AParent);

      }
      else {
        MEmbedded = false;
        AdjustWindowRectEx(&rc,WS_OVERLAPPEDWINDOW,FALSE,WS_EX_OVERLAPPEDWINDOW);
        int32 wx = ((GetSystemMetrics(SM_CXSCREEN)-MRect.w) >> 1) + rc.left;
        int32 wy = ((GetSystemMetrics(SM_CYSCREEN)-MRect.h) >> 1) + rc.top;
        int32 x = wx;
        int32 y = wy;
        int32 w = rc.right-rc.left+1;
        int32 h = rc.bottom-rc.top+1;
        MAdjustedWidth = w - MRect.w;
        MAdjustedHeight = h - MRect.h;
        MWinHandle = CreateWindowEx(
          WS_EX_OVERLAPPEDWINDOW,   // dwExStyle
          S3_Window_ClassName(),     // lpClassName
          MTitle,                   // lpWindowName
          WS_OVERLAPPEDWINDOW,      // dwStyle
          x,                        // center x
          y,                        // center y
          w,                        // wWidth
          h,                        // wHeight
          0,                        // hWndParent
          0,                        // hMenu
          s3_global_WinInstance,    // hInstance
          0                         // lpParam
        );
        SetFocus(MWinHandle);
      }
      SetWindowLongPtr(MWinHandle,GWLP_USERDATA,(LONG_PTR)this);
    }

    //----------

    virtual ~S3_Window_Win32() {
      destroyUserCursors();
      DestroyWindow(MWinHandle);
    }

  //------------------------------
  //
  //------------------------------

  public:

    HWND  hwnd(void)  { return MWinHandle; }
    HDC   hdc(void)   { return MWinPaintDC; }

  //------------------------------
  // set/get
  //------------------------------

  public:

    virtual // S3_Window_Base
    void setPos(int32 AXpos, int32 AYpos) {
      SetWindowPos(MWinHandle,0,AXpos,AYpos,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
    }

    //----------

    virtual // S3_Window_Base
    void setSize(int32 AWidth, int32 AHeight) {
      int w = AWidth + MAdjustedWidth + 0;
      int h = AHeight + MAdjustedHeight + 0;
      SetWindowPos(MWinHandle,HWND_TOP,0,0,w,h, SWP_NOMOVE);
      MRect.w = w;
      MRect.h = h;
    }

    //----------

    virtual // S3_Window_Base
    void setTitle(const char* ATitle) {
      SetWindowText(MWinHandle, ATitle);
    }

  //------------------------------
  //
  //------------------------------

  public:

    virtual // S3_Window_Base
    void open(void) {
      ShowWindow(MWinHandle,SW_SHOW);
      #ifdef S3_WIN32_IDLE_TIMER
      SetTimer(MWinHandle,s3_ts_idle,S3_WIN32_IDLE_MS,S3_NULL);
      #endif
    }

    //----------

    virtual // S3_Window_Base
    void close(void) {
      #ifdef S3_WIN32_IDLE_TIMER
      KillTimer(MWinHandle,s3_ts_idle);
      #endif
      ShowWindow(MWinHandle,SW_HIDE);
    }

    //----------

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
    void reparent(void* AParent) {
      SetWindowLongPtr( MWinHandle, GWL_STYLE, ( GetWindowLongPtr(MWinHandle,GWL_STYLE) & ~WS_POPUP ) | WS_CHILD );
      SetParent(MWinHandle,(HWND)AParent);
    }

  //------------------------------
  // event loop
  //------------------------------

  public:

    virtual // S3_Window_Base
    void eventLoop(void) {
      MSG msg;
      while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
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

  //------------------------------
  // painting
  //------------------------------

  public:

    virtual // S3_Window_Base
    void invalidate(int32 AXpos, int32 AYpos, int32 AWidth, int32 AHeight) {
      RECT R;
      R.left   = AXpos;
      R.top    = AYpos;
      R.right  = AXpos + AWidth;  // - 1;
      R.bottom = AYpos + AHeight; // - 1;
      InvalidateRect(MWinHandle,&R,false);
    }

    //----------

    virtual // S3_Window_Base
    void beginPaint(void) {
      MWinPaintDC = BeginPaint(MWinHandle,&MWinPaintStruct);
    }

    //----------

    virtual // S3_Window_Base
    void endPaint(void) {
      EndPaint(MWinHandle,&MWinPaintStruct);
    }

  //------------------------------
  // mouse
  //------------------------------

  public:

    virtual // S3_Window_Base
    void setCursor(int32 ACursor) {
      if (ACursor!=MCurrentCursor) {
        MCurrentCursor = ACursor;
        if (ACursor>=128) MWinCursor = MUserCursors[ACursor-128];
        else MWinCursor = LoadCursor(NULL,s3_win32_cursors[ACursor]);
        SetCursor(MWinCursor);
      }
    }

    //----------

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

    //----------

    // Moves the cursor to the specified screen coordinates
    // will fire a WM_MOUSEMOVE event..

    virtual // S3_Window_Base
    void setCursorPos(int32 AXpos, int32 AYpos) {
      POINT pos;
      pos.x = AXpos;
      pos.y = AYpos;
      ClientToScreen(MWinHandle,&pos);
      int32 x = pos.x;
      int32 y = pos.y;
      SetCursorPos(x,y);
    }

    //----------

    //virtual // S3_Window_Base
    //void getCursorPos(int32* AXpos, int32* AYpos) {
    //  *AXpos = 0;
    //  *AYpos = 0;
    //}

    //----------

    virtual // S3_Window_Base
    void showCursor(void) {
      ShowCursor(true);
    }

    //----------

    virtual // S3_Window_Base
    void hideCursor(void) {
      ShowCursor(false);
    }

    //----------

    virtual // S3_Window_Base
    void grabCursor(void) {
      SetCapture(MWinHandle);
    }

    //----------

    virtual // S3_Window_Base
    void releaseCursor(void) {
      ReleaseCapture();
    }

    //----------

    //void resetCursor(void) {
    //}

    //----------

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

    //----------

  //------------------------------
  //
  //------------------------------

  public:

    // only valid dugring begin/endPaint...
    // needs MWinPaintDC

    virtual // S3_Window_Base
    void blit(int32 dst_x, int32 dst_y, S3_Painter* APainter, int32 src_x, int32 src_y, int32 src_w, int32 src_h) {
      BitBlt(MWinPaintDC,dst_x,dst_y,src_w,src_h,APainter->hdc(),src_x,src_y,SRCCOPY);
    }

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
  // event handler
  //------------------------------

  private:

    //----------

    int32 remapMouseButton(int32 AButton) {
      int32 result = s3_mb_none;
      switch(AButton) {
        case MK_LBUTTON:  result = s3_mb_left;      break;
        case MK_MBUTTON:  result = s3_mb_middle;    break;
        case MK_RBUTTON:  result = s3_mb_right;     break;
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

    int32 remapMouseKey(int32 AState) {
      int32 result = s3_mb_none;
      if ((AState & MK_SHIFT  ) != 0) result |= s3_mk_shift;
      //if ((AState & ) != 0) result |= smk_caps;
      if ((AState & MK_CONTROL  ) != 0) result |= s3_mk_ctrl;
      //if ((AState & ) != 0) result |= smk_alt;
      //if ((AState & 128) != 0) result |= smk_altgr;
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

    int32 remapKeyCode(WPARAM wParam, LPARAM lParam) {
      switch(wParam) {
       case VK_BACK: return s3_kc_backspace;
      }
      return 0;
    }


    //------------------------------
    //
    //------------------------------

    LRESULT eventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
      LRESULT result = 0;
      S3_Rect   rc;
      int     x,y,b,w,h,d;//,k;
      switch (message) {

        case WM_DESTROY:
          if (!MEmbedded) PostQuitMessage(0);
          break;

        case WM_GETDLGCODE:
          // we want keypresses
          result = DLGC_WANTALLKEYS;
          break;

        case WM_ERASEBKGND:
          // tell window not to erase background
          result = 1;
          break;

        case WM_KEYDOWN:
          //if (MListener) MListener->on_keyDown(this,wParam,lParam);
          //k = remapKeyCode(wParam,lParam);
          on_widgetKeyDown(this,wParam,lParam);
          break;

        case WM_KEYUP:
          //if (MListener) MListener->on_keyUp(this,wParam,lParam);
          //k = remapKeyCode(wParam,lParam);
          on_widgetKeyUp(this,wParam,lParam);
          break;

        //case WM_CHAR:
        //  break;

        case WM_XBUTTONDOWN:
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
          switch (message) {
            case WM_LBUTTONDOWN:  b = s3_mb_left;   break;
            case WM_MBUTTONDOWN:  b = s3_mb_middle; break;
            case WM_RBUTTONDOWN:  b = s3_mb_right;  break;
            case WM_XBUTTONDOWN:  b = s3_mb_side;   break;
            default:              b = s3_mb_none;   //break;
          }
          if (b==s3_mb_side) {
            if ((wParam & MK_XBUTTON1) != 0) b = s3_mb_backward;
            if ((wParam & MK_XBUTTON2) != 0) b = s3_mb_forward;
          }
          x = short(LOWORD(lParam));
          y = short(HIWORD(lParam));
          //if (MListener) MListener->on_mouseDown(this,x,y,b,remapKey(wParam));
          on_widgetMouseDown(this,x,y,b,remapMouseKey(wParam));
          if (MFlags&s3_wf_capture) grabCursor();
          break;

        case WM_MOUSEMOVE:
          x = short(LOWORD(lParam));
          y = short(HIWORD(lParam));
          //if (MListener) MListener->on_mouseMove(this,x,y,remapKey(wParam));
          on_widgetMouseMove(this,x,y,remapMouseKey(wParam));
          MMouseXpos = x;
          MMouseYpos = y;
          break;

        case WM_XBUTTONUP:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
          switch (message) {
            case WM_LBUTTONUP:  b = s3_mb_left;   break;
            case WM_MBUTTONUP:  b = s3_mb_middle; break;
            case WM_RBUTTONUP:  b = s3_mb_right;  break;
            case WM_XBUTTONUP:  b = s3_mb_side;   break;
            default:            b = s3_mb_none;   //break;
          }

          x = short(LOWORD(lParam));
          y = short(HIWORD(lParam));
          //if (MListener) MListener->on_mouseUp(this,x,y,b,remapKey(wParam));
          on_widgetMouseUp(this,x,y,b,remapMouseKey(wParam));
          if (MFlags&s3_wf_capture) releaseCursor();
          break;

        case WM_MOUSEWHEEL:
          d = GET_WHEEL_DELTA_WPARAM(wParam);
          //if (d>0) { if (MListener) MListener->on_mouseDown(this,MMouseXpos,MMouseYpos,smb_wheelUp,  smb_none); }
          //if (d<0) { if (MListener) MListener->on_mouseDown(this,MMouseXpos,MMouseYpos,smb_wheelDown,smb_none); }
          if (d>0) { on_widgetMouseDown(this,MMouseXpos,MMouseYpos,s3_mb_wheelUp,  s3_mb_none); }
          if (d<0) { on_widgetMouseDown(this,MMouseXpos,MMouseYpos,s3_mb_wheelDown,s3_mb_none); }
          break;

        /*
          Only windows that have the CS_DBLCLKS style can receive
          WM_LBUTTONDBLCLK messages, which the system generates whenever the
          user presses, releases, and again presses the left mouse button
          within the system's double-click time limit. Double-clicking the left
          mouse button actually generates a sequence of four messages:
          WM_LBUTTONDOWN, WM_LBUTTONUP, WM_LBUTTONDBLCLK, and WM_LBUTTONUP.
        */

        #ifdef S3_MOUSE_DOUBLECLICK
        case WM_LBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_XBUTTONDBLCLK:
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
        #endif // S3_MOUSE_DOUBLECLICK

        /*
          is it possible to receive a WM_PAINT when some other thread is
          drawing to the backbuffer?
        */

        case WM_PAINT:
          beginPaint();
          rc = S3_Rect( MWinPaintStruct.rcPaint.left,
                        MWinPaintStruct.rcPaint.top,
                        MWinPaintStruct.rcPaint.right -  MWinPaintStruct.rcPaint.left + 1,
                        MWinPaintStruct.rcPaint.bottom - MWinPaintStruct.rcPaint.top + 1);
          if (MListener) {
            #ifndef S3_NO_WINDOW_BACKBUFFER
            MListener->on_bufferBlit(this,rc);
            #else
            MListener->on_windowPaint(this,rc);
            #endif
          }
          endPaint();
          break;

        case WM_SETCURSOR:

          // called every mouse move
          if (LOWORD(lParam)==HTCLIENT) {
            SetCursor(MWinCursor);
            //setCursor(MCurrentCursor);
            result = 1;
          }
          else result = DefWindowProc(hWnd,message,wParam,lParam);
          break;

        case WM_SIZE:
          w = short(LOWORD(lParam));
          h = short(HIWORD(lParam));
          if ( (w!=MRect.w) || (h!=MRect.h) ) {
            if (MListener) {
              #ifndef S3_NO_WINDOW_BACKBUFFER
              MListener->on_bufferResize(this,w,h);
              #else
              MListener->on_windowResize(this,w,h);
              #endif
            }
            on_widgetSetSize(this,w,h);
            if (MFlags & s3_wf_autoalign) on_widgetAlign(this);
            #ifndef S3_NO_WINDOW_BACKBUFFER
            if (MListener) MListener->on_bufferPaint(this,S3_NULL,s3_pm_normal); //redraw;
            #endif
          }
          break;

        case WM_TIMER:
          if (MListener) {
            if (wParam==s3_ts_timer) MListener->on_windowTimer(this);
            if (wParam==s3_ts_idle) MListener->on_windowIdle(this);
          }
          break;

        default:
          result = DefWindowProc(hWnd,message,wParam,lParam);
          break;
      }
      return result;
    }

    //----------

};

//----------------------------------------------------------------------
// event proc
//----------------------------------------------------------------------

LRESULT CALLBACK s3_eventproc_win32(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  S3_Window_Win32* wnd = (S3_Window_Win32*)GetWindowLongPtr(hWnd,GWLP_USERDATA);
	if (!wnd) return DefWindowProc(hWnd,message,wParam,lParam);
  return wnd->eventHandler(hWnd, message, wParam, lParam);
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

class S3_WindowClass_Win32 {

  private:
    bool     MRegistered;
    char     MName[S3_MAX_STRING_SIZE];
    ATOM     MAtom;
    WNDCLASS MClass;

  public:

    S3_WindowClass_Win32() {
      MRegistered = false;
    }

    ~S3_WindowClass_Win32() {
      if (MRegistered) {
        UnregisterClass(MName,s3_global_WinInstance);
      }
    }

    char* registerClass(void) {
      if (!MRegistered) {
        S3_CreateUniqueName(MName,(char*)"s3_window_",&s3_global_WinInstance);
        S3_Memset(&MClass,0,sizeof(MClass));
        MClass.style          = CS_HREDRAW | CS_VREDRAW;// | CS_DBLCLKS;
        #ifdef S3_MOUSE_DOUBLECLICK
        MClass.style |= CS_DBLCLKS;
        #endif
        MClass.lpfnWndProc    = &s3_eventproc_win32;
        MClass.hInstance      = s3_global_WinInstance;
        MClass.lpszClassName  = MName;
        MAtom                 = RegisterClass(&MClass);
        MRegistered           = true;
      }
      return MName;
    }

};

//------------------------------

/*
  destructor automatically called when dll/exe unloaded

  it should have been hidden from user, but since we're using
  only .h files, that's a bit problematic..
*/

S3_WindowClass_Win32 s3_global_windowclass;

//----------

// singleton-ish..

char* S3_Window_ClassName(void) {
  return s3_global_windowclass.registerClass();
}


//----------------------------------------------------------------------
#endif
