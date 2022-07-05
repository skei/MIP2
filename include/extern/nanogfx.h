/*========================================================================
  Copyright (c) 2011 ar.gb
  This software is provided 'as-is', without any express or implied warranty.
  In no event will the authors be held liable for any damages arising from
  the use of this software.
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it freely.


  nanoGFX module - small gfx layer for x11,win,osx

author: ar.gb (argb@o2.pl)

Quick Start:
  nanoGFX is define driven small util for cross-platform gfx coding
  ( currently x11/win  and sometimes MacOS).
  To be able to create GL surfaces use NG_GL_SUPPORT flag.
  For basic Vulkan support use NG_VULKAN_SUPPORT
  For EGL support use NG_EGL_SUPPORT
  For specific GL context version set:
  NG_GL_VERSION_MAJOR and NG_GL_VERSION_MINOR
  Thread support can be enabled using NG_THREAD_SUPPORT.
  Timer support using: NG_TIMER_SUPPORT
  Clipboard: NG_CLIPBOARD_SUPPORT

  one of source files needs to have NG_IMPLEMENT define before nanogfx.h
    #define NG_IMPLEMENT
    #include <nanogfx.h>

========================================================================*/

#ifndef DEF_NANOGFX_H
#define DEF_NANOGFX_H

#define NG_VERSION "2.x"

/*========================================================================
  PLATFORM SETUP
  ========================================================================*/

#if !defined NG_LINUX &&  !defined NG_WIN32 &&  !defined NG_OSX
# ifdef __linux__
#  define NG_LINUX
# elif defined _WIN32
#  define NG_WIN32
# elif defined __APPLE__
#  define NG_OSX
# else
#  error no platform defined
# endif
#endif

#undef __nG_X11
#undef __nG_WIN32
#undef __nG_OSX


#ifdef NG_LINUX
# define __nG_X11
#endif

#ifdef NG_WIN32
# define __nG_WIN32
#endif

#ifdef NG_OSX
# define __nG_OSX
#endif
/*++++++++++++++++++++++++++++++++++++++++++*/

#ifdef NG_3D_SUPPORT //legacy
#       define NG_GL_SUPPORT
#       define SURFACE_3D SURFACE_GL
#endif

#ifdef __nG_X11
# include <X11/Xlib.h>
# include <X11/XKBlib.h>
# include <X11/Xatom.h>
# include <X11/Xutil.h>
# include <malloc.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>
# ifdef NG_GL_SUPPORT
#  ifndef NG_EGL_SUPPORT
#    define NG_GLX_SUPPORT
#  endif
# endif
# ifdef NG_THREAD_SUPPORT
#   include <pthread.h>
# endif
# ifdef NG_GLX_SUPPORT
#  include <GL/glx.h>
#   define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#   define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
    typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
# endif
# ifdef NG_EGL_SUPPORT
#  include <EGL/egl.h>
# endif
# ifdef NG_TIMER_SUPPORT
#   include <signal.h>
#   include <time.h>
# endif
#endif
#ifdef __nG_WIN32
# include <windows.h>
# include <windowsx.h>
# ifdef NG_GL_SUPPORT
    typedef HGLRC (APIENTRY *wglCreateContextAttribsARBProc)(HDC hDC, HGLRC hShareContext, const int *attribList);
# endif
#endif

#ifdef NG_VULKAN_SUPPORT
# include <vulkan/vulkan.h>
# ifdef __nG_WIN32
#   include <vulkan/vulkan_win32.h>
# elif defined __nG_X11
#  include <vulkan/vulkan_xlib.h>
# endif
#endif //NG_VULKAN_SUPPORT

#ifdef __nG_OSX
# include <Cocoa/Cocoa.h>
# include <sys/time.h>
# ifdef NG_THREAD_SUPPORT
#   include <pthread.h>
# endif
# ifdef NG_METAL_SUPPORT
#   include <MetalKit/MetalKit.h>
#   include <Metal/Metal.h>
# endif
#endif


#if defined NG_GL_SUPPORT || defined NG_EGL_SUPPORT
# ifdef __nG_OSX
#  include <OpenGL/gl.h>
# else
#  include <GL/gl.h>
# endif
# ifndef NG_GL_VERSION_MAJOR
#   define NG_GL_VERSION_MAJOR 1
# endif
# ifndef NG_GL_VERSION_MINOR
#   define NG_GL_VERSION_MINOR 1
# endif
#endif

#ifdef NG_CLIPBOARD_SUPPORT
# include <string>
#endif

#define __NGDEBUG
# include <cstdio>
#ifdef __NGDEBUG
# include <cstdio>
# ifndef ng_dbg
#    define ng_dbg(...) { fprintf( stderr,"=>[%s][%d][%s]",__FILE__,__LINE__,__FUNCTION__); fprintf( stderr,__VA_ARGS__); fprintf( stderr,"\n");}
# endif

# ifndef ng_err
#  define ng_err(...) { fprintf( stderr,"[ERROR][%s][%d][%s]",__FILE__,__LINE__,__FUNCTION__); fprintf( stderr,__VA_ARGS__); fprintf( stderr,"\n");}
# endif
#else
# define ng_dbg(...)
# define ng_err(...)
#endif /* __NGDEBUG */



namespace nanoGFX {
struct nGEvent;
class nGSurface;
}

#ifdef __nG_OSX

#ifdef NG_GL_SUPPORT
@interface nGOpenGLView : NSOpenGLView {
    CVDisplayLinkRef displayLink;
    bool isFullscreen;
    bool do_init;
}
@property (nonatomic,assign) nanoGFX::nGSurface* ng_ptr;
@end
#endif /* NG_GL_SUPPORT*/

#ifdef NG_METAL_SUPPORT
@interface nGMetalView : MTKView {
    id<MTLDevice> device;
    CVDisplayLinkRef displayLink;
    bool isFullscreen;
    bool do_init;
}
- (id) getDevice;
@property (nonatomic,assign) nanoGFX::nGSurface* ng_ptr;
@end
#endif /* NG_METAL_SUPPORT*/



#ifdef NG_2D_SUPPORT
@interface nGImageView : NSImageView {
}
@property (nonatomic,assign) bool do_init;
@property (nonatomic,assign) nanoGFX::nGSurface* ng_ptr;
@end
#endif /*NG_2D_SUPPORT*/

@interface nGWindow : NSWindow<NSWindowDelegate> {
    bool commandKeyState, altKeyState, controlKeyState, shiftKeyState, capsKeyState;
}
@property (nonatomic,assign) nanoGFX::nGSurface* ng_ptr;


@end

@interface nGApplication : NSApplication {
}
@end
#endif

namespace nanoGFX {


typedef unsigned int (*nGThreadTask_f)(void* user_data);
typedef void (*nGEventHandler_f)(const nGSurface& surface, const nGEvent& ev, void* user_data);

struct nGEventListener {
    virtual void onEvent(const nGSurface& surface, const nGEvent& ev) = 0;
};

/*========================================================================
  nGFlags are passed to surface init function

  nGResize - allow the window to be resized
  nGMaximize - allow the window to be maximized
  nGMinimize - allow the window to be minimized
  nGNoTitleBar - disable title bar
  nGDropShadow - window drops shadow
  ========================================================================*/
enum nGFlags{
    nGResize =1,
    nGMaximize =2,
    nGMinimize =4,
    nGNoTitleBar = 8,
    nGDropShadow = 16,
    nGStayOnTop = 32,
    nGFullscreen = 64,
    nGMaximized = 128,
    nGHideInTaskBar = 256,
    nGKeepOnBottom = 512
};

/*========================================================================
  nGResult - not much here, either ok or fails
  ========================================================================*/
enum nGResult {
    RES_OK = 0,
    RES_FAILED = -1
};

/*========================================================================
  nGSurfaceType - type of created surface: 2D/3D
  ========================================================================*/
enum nGSurfaceType {
    SURFACE_2D = 0,
    SURFACE_GL = 1,
    SURFACE_NONE
};

/*========================================================================
  nGButton - mouse buttons definitions
  ========================================================================*/
enum nGButton {
    BUTTON_LEFT = 1,
    BUTTON_MIDDLE = 2,
    BUTTON_RIGHT = 3,
    BUTTON_SCROLL_UP = 4,
    BUTTON_SCROLL_DOWN = 5
};

/*========================================================================
  nGKeySym -  keycodes definitions
  ========================================================================*/
enum nGKeySym {
    KEYCODE_NONE = 0xDADA,
    KEYCODE_SPACE = 32,
    KEYCODE_PERIOD = 46,
    KEYCODE_0 = 48,
    KEYCODE_1 = 49,
    KEYCODE_2 = 50,
    KEYCODE_3 = 51,
    KEYCODE_4 = 52,
    KEYCODE_5 = 53,
    KEYCODE_6 = 54,
    KEYCODE_7 = 55,
    KEYCODE_8 = 56,
    KEYCODE_9 = 57,
    KEYCODE_COLON = 0x58,
    KEYCODE_SEMICOLON = 0x59,
    KEYCODE_a = 97,
    KEYCODE_b = 98,
    KEYCODE_c = 99,
    KEYCODE_d = 100,
    KEYCODE_e = 101,
    KEYCODE_f = 102,
    KEYCODE_g = 103,
    KEYCODE_h = 104,
    KEYCODE_i = 105,
    KEYCODE_j = 106,
    KEYCODE_k = 107,
    KEYCODE_l = 108,
    KEYCODE_m = 109,
    KEYCODE_n = 110,
    KEYCODE_o = 111,
    KEYCODE_p = 112,
    KEYCODE_q = 113,
    KEYCODE_r = 114,
    KEYCODE_s = 115,
    KEYCODE_t = 116,
    KEYCODE_u = 117,
    KEYCODE_v = 118,
    KEYCODE_w = 119,
    KEYCODE_x = 120,
    KEYCODE_y = 121,
    KEYCODE_z = 122,

    KEYCODE_DELETE      = 127,
    KEYCODE_BACKSPACE   = 8,
    KEYCODE_ESCAPE      = 27,
    KEYCODE_RETURN      = 13,
    KEYCODE_LSHIFT      = 0x100 | 225,
    KEYCODE_RSHIFT      = 0x100 | 226,
    KEYCODE_HOME        = 278,
    KEYCODE_END         = 279,
    KEYCODE_PAGEUP      = 341,
    KEYCODE_PAGEDOWN    = 342,
    KEYCODE_LCTRL       = 483,
    KEYCODE_RCTRL       = 484,
    KEYCODE_LEFT        = 0x100 | 81,
    KEYCODE_UP          = 0x100 | 82,
    KEYCODE_RIGHT       = 0x100 | 83,
    KEYCODE_DOWN        = 0x100 | 84,
    KEYCODE_COMMAND     = 0x200 | 1,
    KEYCODE_LALT        = 0x200 | 2,
    KEYCODE_RALT        = 0x200 | 3

};

/*========================================================================
  nGKeyMod -  keycodes modificators
  ========================================================================*/
enum nGKeyMod {
    KEYMOD_NONE = 0,
    KEYMOD_LSHIFT = 0x0001,
    KEYMOD_RSHIFT = 0x0002,
};


/*========================================================================
  nGEvent struct, returned by getEvent method
type:
- keyboard events:
- KEY_DOWN
- KEY_UP
symbol and state passed in event.key struct
- mouse events:
- MOUSE_MOTION
- MOUSE_BUTTON_DOWN
- MOUSE_BUTTON_UP
state, button, position and position relative to window are passed
in event.mouse struct
- window events:
- WINDOW_EXPOSE
- WINDOW_CREATE
- WINDOW_ENTER
- WINDOW_LEAVE
- WINDOW_RESIZE
- FOCUS_OUT
- FOCUS_IN
- application events:
- SELECTION_REQUEST
========================================================================*/
struct nGEvent{
    enum nGEventType_t {
        NONE = 0,
        KEY_DOWN = 1,
        KEY_UP = 2,
        MOUSE_MOTION = 3,
        MOUSE_BUTTON_DOWN = 4,
        MOUSE_BUTTON_UP = 5,
        WINDOW_EXPOSE = 6,
        WINDOW_CREATE = 7,
        WINDOW_DESTROY = 8,
        WINDOW_ENTER = 9,
        WINDOW_LEAVE = 10,
        WINDOW_RESIZE = 11,
        FOCUS_OUT = 12,
        FOCUS_IN = 13,
        SELECTION_REQUEST = 14,
        TEXT_INPUT = 15,
        WINDOW_REPAINT = 16,
        TIMER_EVENT = 17
    } type;

    union
    {
        struct KeyboardEvent {
            unsigned int symbol;
            unsigned int state;
        } key;
        struct MouseButtonEvent {
            unsigned int state;
            unsigned int button;
            int x,y;
            int root_x, root_y;
            int wheel_delta;
        } mouse;
        struct TimerEvent {
            size_t timerId;
        } timer;
    } data;
    nGEvent() : type(NONE) {};
    nGEvent(nGEventType_t t) : type(t) {};
    void clear() { type = NONE; };
};

/*========================================================================
  nGTime - time utils class
  static get() - returns current time in ms
  static sleep(x) - sleeps x ms
  ========================================================================*/
class nGTime {
    nGTime() {};
public:
    static unsigned long long get()
    {
#if defined __nG_X11 || defined __nG_OSX
        struct timeval tv;
        gettimeofday(&tv,NULL);
        return  tv.tv_sec*1000 + tv.tv_usec/1000;
#endif
#ifdef __nG_WIN32
        return GetTickCount();
#endif
        return 0;
    }
    static void sleep(unsigned int ms)
    {
#if defined __nG_X11 || defined __nG_OSX
        usleep( ms*1000);
#endif
#ifdef __nG_WIN32
        ::Sleep(ms);
#endif

    }
};
#ifdef NG_THREAD_SUPPORT
/*========================================================================
  nGThread - thread utils class
  ========================================================================*/
class nGThread{
#if defined __nG_X11  || defined __nG_OSX
    pthread_t thread;
#endif /*__nG_X11*/
#ifdef __nG_WIN32
    HANDLE thread;
    DWORD thread_id;
#endif /*__nG_WIN32*/
    nGThreadTask_f task;
    void* udata;
public:
    nGThread(nGThreadTask_f _task, void* _udata) :task(_task), udata(_udata) {};
    nGResult start()
    {
        nGResult result = RES_FAILED;
#if defined __nG_X11  || defined __nG_OSX
        pthread_create(&thread, NULL, (void*(*)(void*))task, udata);
        if( thread) result = RES_OK;
#endif /*__nG_X11*/
#ifdef __nG_WIN32
        thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)task, udata,0,&thread_id);
        if( thread) result = RES_OK;
#endif /*__nG_WIN32*/
        return result;
    }
};

/*========================================================================
  nGMutex - mutex utils class
  ========================================================================*/
class nGMutex{
#if defined __nG_X11 || defined __nG_OSX
    pthread_mutex_t mtx;
#endif /*__nG_X11*/
#ifdef __nG_WIN32
    HANDLE mtx;
#endif /*nG_WIN32*/
public:
#if defined __nG_X11 || defined __nG_OSX
    nGMutex()  { pthread_mutex_init(&mtx,NULL); }
    ~nGMutex() {pthread_mutex_destroy(&mtx); }
    void lock() { pthread_mutex_lock(&mtx); }
    void unlock() { pthread_mutex_unlock(&mtx); }
#endif /*__nG_X11*/
#ifdef __nG_WIN32
    nGMutex()  {mtx = CreateMutex(NULL,FALSE,NULL);}
    ~nGMutex(){ CloseHandle(mtx); }
    void lock() { WaitForSingleObject(mtx, INFINITE); }
    void unlock() { ReleaseMutex(mtx); }
#endif /*nG_WIN32*/
};


/*========================================================================
  nGMutexLocker - mutex utils class
  ========================================================================*/
class nGMutexLocker{
    nGMutex& mtx;
public:
    nGMutexLocker(nGMutex& _mtx): mtx(_mtx) { mtx.lock();}
    ~nGMutexLocker() { mtx.unlock();}
};

#endif /*NG_THREAD_SUPPORT*/

#ifdef NG_TIMER_SUPPORT
struct nGTimer {
    nGSurface* surface;
    size_t uid;
    void* handle;
    nGTimer() : surface(NULL), uid(0) {};
};
#endif /*NG_TIMER_SUPPORT*/

/*========================================================================
  nGSurface - surface handling class

  destroy() - destroy surface
  create(width,height,surfaceType,winAttr,parent) - create surface with desired width, height, type(SURFACE_2D/SURFACE_GL), attributes (nGFlags - default 0) and parent window( default NULL)
  toClipboard(const std::string& data) - copy data to clipboard
  activate() - activate surface ( set gl context to valid window )
  update() - draw and process events
  getSurface() -return surface pointer ( for 2D )
  getWidth() - return surface width
  getHeight() - return surface height
  valid() - return true if surface isn;t destroyed
  setWindowTitle(name) - set new window title
  setFocus() - focus surface
  setMouseCapture() - enable/disable mouse capture
  void move(x,y) -  move window to x,y
  setIcon(data,width, height) - set app icon
  ========================================================================*/
class nGSurface {
    unsigned int sWidth,sHeight;
    unsigned int sX, sY;
    unsigned int winAttr;
    unsigned char* surface;
    bool renderEnabled;
    bool doublebuffer;
    nGSurface* parent;

#ifdef __nG_X11
    XImage* screenImage;
    Display *display;
    Window window;
    Visual* visual;
    Atom wmDeleteMessage;
    Pixmap iconPixmap;
    GC gc;
    int depth;
    int screen;
    long handle;
# ifdef NG_GLX_SUPPORT
    GLXContext glctx;
# endif
#endif /*__nG_X11*/

#ifdef __nG_WIN32
    HWND handle;
    HDC hdc;
    HINSTANCE hInstance;
    BITMAPINFO bmpinfo;
# ifdef NG_GL_SUPPORT
    HGLRC hrc;
# endif
#endif /*__nG_WIN32*/

#ifdef __nG_OSX
    nGWindow* handle;
# ifdef NG_GL_SUPPORT
    nGOpenGLView* glView;
# endif /*NG_GL_SUPPORT*/
# ifdef NG_METAL_SUPPORT
    nGMetalView* metalView;
#endif //NG_METAL_SUPPORT

# ifdef NG_2D_SUPPORT
    NSImage* image;
    nGImageView* imageView;
    CGImageRef imageRef;
    CGColorSpaceRef colorSpace;
    CGDataProviderRef dataProvider;
# endif /*NG_2D_SUPPORT*/
#endif /*__nG_OSX*/

    bool valid;
    unsigned int keyMask;
    nGSurfaceType surfaceType;
    nGEventHandler_f nGEventHandler;
    void* nGEventHandlerUData;
    nGEventListener* eventListener;

# ifdef NG_EGL_SUPPORT
    EGLDisplay eglDisplay;
    EGLSurface eglSurface;
    EGLContext eglCtx;
# endif

#ifdef __nG_OSX

    typedef nGApplication* nGApplicationPtr;
    static nGApplicationPtr& getAppHandlePtr();

# ifdef NG_2D_SUPPORT
    static const void* dataProviderSurfacePtr(void* udata);

    static void deleteSurfaceData(void*, const void* udata);
# endif /*NG_2D_SUPPORT*/
#endif /* __nG_OSX */

#ifdef __nG_WIN32
    /*========================================================================
      nGSurface::winCallback :    event handling routine - win version
      ========================================================================*/
    static LRESULT CALLBACK winCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    bool fakeLButton, fakeRButton; //faking continuous mbdown event
    int fakeX, fakeY, fakeRootX, fakeRootY;
#endif /*__nG_WIN32*/

    void init();

    /*========================================================================
      nGSurface::rebuild2DSurface : rebuild 2d surface after resize
      ========================================================================*/
    void rebuild2DSurface();

    /*========================================================================
      nGSurface::adjustSize : adjust  window size to witdh and height of client area
      ========================================================================*/
    void adjustSize(size_t& width, size_t& height);

    /*========================================================================
      nGSurface::translateKeyCode : translate key code to used by nanoGFX
      ========================================================================*/
    unsigned int translateKeyCode( unsigned int keycode);




    /*========================================================================
      nGSurface::processEvents : process Events from OS
      ========================================================================*/
    nGResult processEvents();

#ifdef NG_CLIPBOARD_SUPPORT
    std::string clipboard;
#endif
#ifdef NG_TIMER_SUPPORT
#   ifdef __nG_X11
        static void timerHandlerS(sigval t);
        void timerHandler(nGTimer* ev);
#   endif  //__nG_X11
#endif

public:
#ifdef __nG_WIN32
    static const LPTSTR getWindowClassName();
    static WNDCLASSEX& getWindowClass();
#endif /*__nG_WIN32*/

    static int& getObjCountRef();

    /*========================================================================
      nGSurface::~nGSurface
      ========================================================================*/
    nGSurface();

    /*========================================================================
      nGSurface::~nGSurface
      ========================================================================*/
    ~nGSurface();

    /*========================================================================
      nGSurface::destroy surface, release resources, native window
      ========================================================================*/
    nGResult destroy();

    /*========================================================================
      nGSurface::setEventHandler : register surface event handler function
      ========================================================================*/
    void setEventHandler(nGEventHandler_f func, void* user_data);

    /*========================================================================
      nGSurface::setEventHandler : register surface event listener
      ========================================================================*/
    void setEventHandler(nGEventListener* listener);

    /*========================================================================
      nGSurface::create : create new surface
      ========================================================================*/
    nGResult create(unsigned int width, unsigned int height,nGSurfaceType _surfaceType,unsigned int a_winAttr = 0, nGSurface* parent = NULL);


    /*========================================================================
      nGSurface::sendEvent :   add event to queue
      ========================================================================*/
    void sendEvent(nGEvent& ev);

#ifdef NG_CLIPBOARD_SUPPORT

    /*========================================================================
      nGSurface::toClipboard : copy data to clipboard
      ========================================================================*/
    nGResult toClipboard(const std::string& data);
#endif /*NG_CLIPBOARD_SUPPORT*/

    /*========================================================================
      nGSurface::activate : activate current surface
      ========================================================================*/
    nGResult activate();

    /*========================================================================
      nGSurface::update : draw and get events
      ========================================================================*/
    void update();

    /*========================================================================
      nGSurface::getSurface()  - return rgba surface pointer
      ========================================================================*/
    unsigned char* getSurface();

    /*========================================================================
      nGSurface::getWidth()
      ========================================================================*/
    unsigned int getWidth() const;

    /*========================================================================
      nGSurface::getHeight()
      ========================================================================*/
     unsigned int getHeight() const;

    /*========================================================================
      nGSurface::isValid : is surface properly created and wasn't destroyed
      ========================================================================*/
    bool isValid() const;

    /*========================================================================
      nGSurface::setWindowTitle : set new window title
      ========================================================================*/
    void setWindowTitle( const char* name);

    /*========================================================================
      nGSurface::setFocus : set focus to window containing current nGSurface
      ========================================================================*/
    void setFocus( );

    /*========================================================================
      nGSurface:setMouseCapture : enable/disable mouse capture
    ========================================================================*/
    void setMouseCapture(bool val);

    /*========================================================================
      nGSurface::move : move window containing current surface
      ========================================================================*/
    void move(int x, int y);

    /*========================================================================
      nGSurface::resize : resize window containing current surface
      ========================================================================*/
    void resize(int w, int h);

    /*========================================================================
      nGSurface::resize : resize window containing current surface
      ========================================================================*/
    void resize(int x, int y, int w, int h);

    /*========================================================================
      nGSurface::getPosition : get current window position
      ========================================================================*/
    void getPosition(int& x, int& y);

    /*========================================================================
      nGSurface::setRenderEnabled : enable surface repainting during update step
      ========================================================================*/
    void setRenderEnabled(bool value);

    /*========================================================================
      nGSurface::setIcon : set icon for window
      ========================================================================*/
    void setIcon(unsigned char* data, unsigned int width, unsigned int height);

    /*========================================================================
      nGSurface::getHandle : returns surface handle
      ========================================================================*/
    void* getHandle();

    /*========================================================================
      nGSurface::getDevice : returns surface device
      ========================================================================*/
    void* getDevice();

    /*========================================================================
      nGSurface::getParent : returns surface parent
      ========================================================================*/
    nGSurface* getParent() { return parent;}

    /*========================================================================
      nGSurface::handleResize : handle window resize - internal
      ========================================================================*/
    void handleResize();

    /*========================================================================
      nGSurface::doRepaint : repaints surface (...) - internal
      ========================================================================*/
    void doRepaint();

#ifdef NG_VULKAN_SUPPORT
    nGResult createVulkanSurface(VkInstance instance, VkSurfaceKHR* surface);

#endif

#ifdef NG_EGL_SUPPORT
    nGResult createEGLSurface();
#endif

#ifdef NG_METAL_SUPPORT
    nGResult createMetalSurface();
#endif

#ifdef NG_TIMER_SUPPORT
    nGResult setTimer(size_t uid, size_t interval, nGTimer& timer);
    nGResult removeTimer(nGTimer& timer);
#endif

    /*========================================================================
      nGSurface::run : static, initializes application (macos)
      ========================================================================*/
    static void run();

    /*========================================================================
      nGSurface::shutdown : static, terminates application (macos)
      ========================================================================*/
    static void shutdown();

}; //nGSurface



/*========================================================================
  nGUtils - util class
  ========================================================================*/
struct nGUtils {
    /*========================================================================
      nGUtils::getScreenWidth  returns current screen width
      ========================================================================*/
    static int getScreenWidth();

    /*========================================================================
      nGUtils::getScreenHeight  returns current screen height
      ========================================================================*/
    static int getScreenHeight();
}; //nGUtils

}; //namespace nanoGFX



#ifdef NG_IMPLEMENT

#ifdef __nG_X11
#pragma message "[ NANOGFX BUILD: linux-x11 target ]"
#endif
#ifdef __nG_WIN32
#pragma message ("[ NANOGFX BUILD: win32 target ]")
#endif
#ifdef __nG_OSX
#pragma message "[ NANOGFX BUILD: osx-cocoa target ]"
#endif
#ifdef NG_GL_SUPPORT
#pragma message ("[ NANOGFX BUILD: 3d support enabled ]")
#endif
#ifdef NG_EGL_SUPPORT
#pragma message ("[ NANOGFX BUILD: 3d egl support enabled ]")
#endif
#ifdef NG_2D_SUPPORT
#pragma message ("[ NANOGFX BUILD: 2d surface support enabled ]")
#endif


namespace nanoGFX {

#ifdef __nG_OSX

nGSurface::nGApplicationPtr& nGSurface::getAppHandlePtr()
{
    static nGSurface::nGApplicationPtr ah;
    return ah;
}
# ifdef NG_2D_SUPPORT
const void* nGSurface::dataProviderSurfacePtr(void* udata)
{
    if(udata) {
        nGSurface* surf = (nGSurface*) udata;
        return surf->surface;
    }
    return NULL;
}
void nGSurface::deleteSurfaceData(void*, const void* udata) {
}
# endif /*NG_2D_SUPPORT*/
#endif /* __nG_OSX */

#ifdef __nG_WIN32
/*========================================================================
  nGSurface::winCallback :    event handling routine - win version
  ========================================================================*/
LRESULT CALLBACK nGSurface::winCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    nGSurface* c = (nGSurface*)GetWindowLongPtr( hwnd, GWLP_USERDATA);
    nGEvent event;
    POINT p; //for global cursor position
    if(!c) return DefWindowProc (hwnd, message, wParam, lParam);;
    switch (message)
    {
        case WM_CHAR:
            {
                event.type = nGEvent::TEXT_INPUT;
                event.data.key.state = 1;
                ng_dbg("WM_CHAR: lParam: %Ix wParam: %Ix", lParam, wParam);
                event.data.key.symbol = wParam;//c->translateKeyCode(wParam);
                ng_dbg("WM_CHAR: keycode: %d", event.data.key.symbol);
                switch(wParam)
                {
                    case VK_SPACE:
                    case VK_RETURN:
                    case VK_BACK:
                    case VK_ESCAPE:
                        break;
                    default:
                        c->sendEvent(event);
                }

                break;
            }
        case WM_KEYDOWN:
        case WM_KEYUP:
            {
                event.type = (message == WM_KEYDOWN) ? nGEvent::KEY_DOWN : nGEvent::KEY_UP;
                event.data.key.state = (event.type == nGEvent::KEY_DOWN);
                event.data.key.symbol = KEYCODE_NONE;
                ng_dbg("wParam: %IX", wParam);
                switch(wParam)
                {
                    case VK_ESCAPE:
                        event.data.key.symbol = KEYCODE_ESCAPE; break;
                    case VK_RETURN:
                        event.data.key.symbol = KEYCODE_RETURN; break;
                    case VK_SPACE:
                        event.data.key.symbol = KEYCODE_SPACE; break;
                    case VK_BACK:
                        event.data.key.symbol = KEYCODE_BACKSPACE; break;
                    case VK_DELETE:
                        event.data.key.symbol = KEYCODE_DELETE; break;
                    case VK_LEFT:
                        event.data.key.symbol = KEYCODE_LEFT; break;
                    case VK_UP:
                        event.data.key.symbol = KEYCODE_UP; break;
                    case VK_DOWN:
                        event.data.key.symbol = KEYCODE_DOWN; break;
                    case VK_RIGHT:
                        event.data.key.symbol = KEYCODE_RIGHT; break;
                    case VK_CONTROL:
                        event.data.key.symbol = KEYCODE_RCTRL; break;
                    case VK_RCONTROL:
                        event.data.key.symbol = KEYCODE_RCTRL; break;
                    case VK_LCONTROL:
                        event.data.key.symbol = KEYCODE_LCTRL; break;
                    case VK_SHIFT:
                        event.data.key.symbol = KEYCODE_RSHIFT; break;
                    case VK_LSHIFT:
                        event.data.key.symbol = KEYCODE_LSHIFT; break;
                    case VK_RSHIFT:
                        event.data.key.symbol = KEYCODE_RSHIFT; break;
                    case VK_PRIOR:
                        event.data.key.symbol = KEYCODE_PAGEUP; break;
                    case VK_HOME:
                        event.data.key.symbol = KEYCODE_HOME; break;
                    case VK_END:
                        event.data.key.symbol = KEYCODE_END; break;
                    case VK_NEXT:
                        event.data.key.symbol = KEYCODE_PAGEDOWN; break;
                    default:
                        ng_dbg("wParam: %IX", wParam);
                        break;
                }
                if( (wParam>=0x30 && wParam<=0x39) || (wParam>=0x41 && wParam<=0x5A) )
                    event.data.key.symbol = c->translateKeyCode(wParam);
                if(wParam>=0x41 && wParam<=0x57) event.data.key.symbol+=0x20;
                ng_dbg("keycode: %d", event.data.key.symbol);
                if(event.data.key.symbol != KEYCODE_NONE)
                    c->sendEvent(event);
                break;
            }
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
            event.type = (message == WM_LBUTTONDOWN) ? nGEvent::MOUSE_BUTTON_DOWN:nGEvent::MOUSE_BUTTON_UP;
            event.data.mouse.state = (message == WM_LBUTTONDOWN) ? 1: 0;
            event.data.mouse.button = 1;
            event.data.mouse.x = GET_X_LPARAM(lParam);
            event.data.mouse.y = GET_Y_LPARAM(lParam);
            GetCursorPos(&p);
            event.data.mouse.root_x = p.x;
            event.data.mouse.root_y = p.y;
            c->sendEvent(event);
            c->fakeLButton = (message==WM_LBUTTONDOWN);
            c->fakeX = event.data.mouse.x;
            c->fakeY = event.data.mouse.y;
            c->fakeRootX = p.x;
            c->fakeRootY = p.y;
            break;
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            event.type = (message == WM_MBUTTONDOWN) ? nGEvent::MOUSE_BUTTON_DOWN:nGEvent::MOUSE_BUTTON_UP;
            event.data.mouse.state = (message == WM_MBUTTONDOWN) ? 1: 0;
            event.data.mouse.button = BUTTON_MIDDLE;
            event.data.mouse.x = GET_X_LPARAM(lParam);
            event.data.mouse.y = GET_Y_LPARAM(lParam);
            GetCursorPos(&p);
            event.data.mouse.root_x = p.x;
            event.data.mouse.root_y = p.y;
            c->sendEvent(event);
            break;
        case WM_MOUSEWHEEL:
            event.type = nGEvent::MOUSE_BUTTON_DOWN;
            event.data.mouse.state = 1;
            event.data.mouse.button = GET_WHEEL_DELTA_WPARAM(wParam)>0 ? BUTTON_SCROLL_UP : BUTTON_SCROLL_DOWN;
            GetCursorPos(&p);
            event.data.mouse.root_x = p.x;
            event.data.mouse.root_y = p.y;
            ScreenToClient(hwnd, &p);
            event.data.mouse.x = p.x;
            event.data.mouse.y = p.y;
            event.data.mouse.wheel_delta = GET_WHEEL_DELTA_WPARAM(wParam);
            c->sendEvent(event);
            break;
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            event.type = (message == WM_RBUTTONDOWN) ? nGEvent::MOUSE_BUTTON_DOWN : nGEvent::MOUSE_BUTTON_UP ;
            event.data.mouse.state = (message == WM_RBUTTONDOWN) ? 1: 0;
            event.data.mouse.button = 3;
            event.data.mouse.x =  GET_X_LPARAM(lParam);
            event.data.mouse.y = GET_Y_LPARAM(lParam);
            GetCursorPos(&p);
            event.data.mouse.root_x = p.x;
            event.data.mouse.root_y = p.y;
            c->sendEvent(event);
            c->fakeRButton = (message==WM_RBUTTONDOWN);
            c->fakeX = event.data.mouse.x;
            c->fakeY = event.data.mouse.y;
            c->fakeRootX = p.x;
            c->fakeRootY = p.y;
            break;
        case WM_MOUSEMOVE:

            TRACKMOUSEEVENT tme;
            tme.cbSize= sizeof(TRACKMOUSEEVENT);
            tme.dwFlags=TME_HOVER|TME_LEAVE;
            tme.hwndTrack = hwnd;
            tme.dwHoverTime = 1;
            TrackMouseEvent(&tme);

            event.type = nGEvent::MOUSE_MOTION;
            event.data.mouse.x =  GET_X_LPARAM(lParam);
            event.data.mouse.y = GET_Y_LPARAM(lParam);
            GetCursorPos(&p);
            event.data.mouse.root_x = p.x;
            event.data.mouse.root_y = p.y;

            c->fakeX = event.data.mouse.x;
            c->fakeY = event.data.mouse.y;
            c->fakeRootX = p.x;
            c->fakeRootY = p.y;
            c->sendEvent(event);
            break;
        case WM_MOUSELEAVE:
            event.type = nGEvent::WINDOW_LEAVE;
            c->sendEvent(event);
            break;
        case WM_MOUSEHOVER:
            event.type = nGEvent::WINDOW_ENTER;
            c->sendEvent(event);
            break;
        case WM_SIZE:
            event.type = nGEvent::WINDOW_RESIZE;
            c->sWidth = lParam & 0xffff;
            c->sHeight = (lParam&0xffff0000)>>16;
            c->handleResize();
            c->sendEvent(event);
            c->doRepaint();
            break;
        case WM_CLOSE:
            event.type = nGEvent::WINDOW_DESTROY;
            c->sendEvent(event);
            break;
        case WM_KILLFOCUS:
            event.type = nGEvent::FOCUS_OUT;
            c->sendEvent(event);
            break;
        case WM_SETFOCUS:
            {
                if(c->winAttr & nGKeepOnBottom) {
                    RECT rect;
                    GetWindowRect(hwnd, &rect );
                    SetWindowPos(hwnd , HWND_BOTTOM,  rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW|SWP_NOOWNERZORDER);
                }
                break;
            }
        case WM_PAINT:
            {
                HDC hdc;
                PAINTSTRUCT ps;
                hdc = BeginPaint( hwnd, &ps );
                EndPaint( hwnd, &ps );
                return 0;
            }
        case WM_TIMER:
            {
                event.type = nGEvent::TIMER_EVENT;
                event.data.timer.timerId = wParam - WM_USER;
                c->sendEvent(event);
                break;
            }
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return DefWindowProc (hwnd, message, wParam, lParam);
}
#endif /*__nG_WIN32*/

void nGSurface::init()
{
#ifdef __nG_X11
    screenImage = NULL;
    display = NULL;
    window = 0;
    visual = NULL;
    gc = 0;
    depth = 0;
    screen = 0;
    iconPixmap = 0;
#endif /*__nG_X11*/
#ifdef __nG_WIN32
    hInstance = 0;
    handle = 0;
    hdc = 0;
#endif /*__nG_WIN32*/
#ifdef __nG_OSX
    handle = 0;
# ifdef NG_GL_SUPPORT
    glView = 0;
# endif /*NG_GL_SUPPORT*/
# ifdef NG_METAL_SUPPORT
    metalView = 0;
# endif //NG_METAL_SUPPORT
#endif
}


/*========================================================================
  nGSurface::handleResize : handle window resize
  ========================================================================*/
void nGSurface::handleResize()
{
#ifdef NG_2D_SUPPORT
    rebuild2DSurface();
#endif
}

/*========================================================================
  nGSurface::translateKeyCode : translate key code to used by nanoGFX
  ========================================================================*/
unsigned int nGSurface::translateKeyCode( unsigned int keycode)
{
    static char codes[] ="`1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./";
    static char codes_shift[] ="~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?";
    unsigned int val = 0;
#ifdef __nG_X11
    val = XkbKeycodeToKeysym(display,keycode,0,0);
    switch(val)
    {
        case XK_Delete: return KEYCODE_DELETE;
        default:
                        break;
    }
#endif /*__nG_X11*/


#ifdef __nG_WIN32
    val = keycode;
    //if(keycode>=0x41 && keycode<=0x57) val+=0x20;
#endif /*__nG_WIN32*/

    if( (val &0xff00 )== 0xff00)
    {

        val&=0xff; //TODO proper translation
        if(val>=0x20)
            val|=0x100;
    }
    else
    {
        val&=0xff; //TODO proper translation
        if( (keyMask & KEYMOD_LSHIFT) || (keyMask & KEYMOD_RSHIFT))
        {
            char* pos = strchr(codes,(char)(val));
            if( pos) val = codes_shift[pos-codes];
        }
    }
    return val;
}
#ifdef __nG_X11
#  ifdef NG_TIMER_SUPPORT
void nGSurface::timerHandlerS(sigval t)
{
    nGTimer* ev = static_cast<nGTimer*>(t.sival_ptr);
    if(ev && ev->surface) ev->surface->timerHandler(ev);
}

void nGSurface::timerHandler(nGTimer* ev)
{
    nGEvent event;
    event.type = nGEvent::TIMER_EVENT;
    event.data.timer.timerId = ev->uid;
    sendEvent(event);
}
#  endif //NG_TIMER_SUPPORT
#endif //__nG_X11

/*========================================================================
  nGSurface::processEvents : process Events from OS
  ========================================================================*/
nGResult nGSurface::processEvents()
{
    nGResult result = RES_FAILED;
    if(!valid) return RES_FAILED;
    nGEvent event;
#ifdef __nG_X11
    XEvent e;
    while(XPending(display))
    {
        event.clear();
        XNextEvent(display, &e);
        if (e.type == KeyPress)
        {

            event.type = nGEvent::KEY_DOWN;
            event.data.key.state = 1;
            event.data.key.symbol = translateKeyCode(e.xkey.keycode);

            if( event.data.key.symbol == KEYCODE_LSHIFT) keyMask |= KEYMOD_LSHIFT;
            if( event.data.key.symbol == KEYCODE_RSHIFT) keyMask |= KEYMOD_RSHIFT;
            ng_dbg("\033[34;1mkeydown  ev.type: %d, %d", event.type, translateKeyCode(e.xkey.keycode)   );
        }

        if (e.type == KeyRelease)
        {
            bool skip = false;
            if (XEventsQueued(display, QueuedAfterReading))
            {
                XEvent next_e;
                XPeekEvent(display, &next_e);
                if (next_e.type == KeyPress && next_e.xkey.time == e.xkey.time &&
                    next_e.xkey.keycode == e.xkey.keycode)
                {
                    XNextEvent (display, &e);
                    skip = true;
                }
            }
            if( !skip)
            {
                event.type = nGEvent::KEY_UP;
                event.data.key.state = 0;
                event.data.key.symbol = translateKeyCode(e.xkey.keycode);
                if( event.data.key.symbol == KEYCODE_LSHIFT) keyMask &= ~KEYMOD_LSHIFT;
                if( event.data.key.symbol == KEYCODE_RSHIFT) keyMask &= ~KEYMOD_RSHIFT;
            }
        }

        if (e.type == ButtonPress)
        {
            event.type = nGEvent::MOUSE_BUTTON_DOWN;
            event.data.mouse.state = 1;
            event.data.mouse.button = e.xbutton.button;
            event.data.mouse.x = e.xbutton.x;
            event.data.mouse.y = e.xbutton.y;
            event.data.mouse.root_x = e.xbutton.x_root;
            event.data.mouse.root_y = e.xbutton.y_root;
        }

        if (e.type == ButtonRelease)
        {
            event.type = nGEvent::MOUSE_BUTTON_UP;
            event.data.mouse.state = 0;
            event.data.mouse.button = e.xbutton.button;
            event.data.mouse.x = e.xbutton.x;
            event.data.mouse.y = e.xbutton.y;
            event.data.mouse.root_x = e.xbutton.x_root;
            event.data.mouse.root_y = e.xbutton.y_root;
        }

        if (e.type == MotionNotify)
        {
            event.type = nGEvent::MOUSE_MOTION;
            event.data.mouse.x = e.xbutton.x;
            event.data.mouse.y = e.xbutton.y;
            event.data.mouse.root_x = e.xbutton.x_root;
            event.data.mouse.root_y = e.xbutton.y_root;
        }
        if( e.type == DestroyNotify)
        {
            event.type = nGEvent::WINDOW_DESTROY;
        }
        if( e.type == CreateNotify)
        {
            event.type = nGEvent::WINDOW_CREATE;
        }
        if( e.type == EnterNotify)
        {
            event.type = nGEvent::WINDOW_ENTER;
        }

        if( e.type == LeaveNotify)
        {
            event.type = nGEvent::WINDOW_LEAVE;
        }
        if( e.type == FocusOut)
        {
            event.type = nGEvent::FOCUS_OUT;
        }

        if( e.type == FocusIn)
        {
            event.type = nGEvent::FOCUS_IN;
        }
        if (e.type == ClientMessage)
        {
            if ((Atom)e.xclient.data.l[0] == wmDeleteMessage) {
                event.type = nGEvent::WINDOW_DESTROY;
            }
        }
        if ( e.type == ConfigureNotify )
        {
            event.type = nGEvent::WINDOW_RESIZE;
            sWidth = e.xconfigure.width;
            sHeight = e.xconfigure.height;
            handleResize();
        }
        if( e.type ==SelectionRequest)
        {
            XEvent respond;
            event.type = nGEvent::SELECTION_REQUEST;
            XSelectionRequestEvent *req;
            req=&(e.xselectionrequest);
            Atom XA_TARGETS = XInternAtom(display, "TARGETS", True);
            if( req->target == XA_TARGETS )
            {
                Atom targets[] = { XA_TARGETS, XA_STRING };
                XChangeProperty(display, req->requestor, req->property, XA_ATOM, 32, PropModeReplace, reinterpret_cast<const unsigned char*>(&targets), sizeof(targets)/sizeof(targets[0]));
            }
            if(req->target == XA_STRING)
            {
#ifdef NG_CLIPBOARD_SUPPORT
                XChangeProperty (display, req->requestor, req->property, XA_STRING, 8,PropModeReplace,
                                 (unsigned char*)clipboard.c_str(), clipboard.size());
                respond.xselection.property=req->property;
#endif /*NG_CLIPBOARD_SUPPORT*/
            }
            else
            {
                respond.xselection.property= None;
            }
            respond.xselection.type= SelectionNotify;
            respond.xselection.display= req->display;
            respond.xselection.requestor= req->requestor;
            respond.xselection.selection=req->selection;
            respond.xselection.target= req->target;
            respond.xselection.time = req->time;
            XSendEvent (display, req->requestor,0,0,&respond);
            XFlush (display);
        }
        sendEvent(event);
        if( display == NULL) break;
    }
#endif /*__nG_X11*/

#ifdef __nG_WIN32
    result = RES_OK;
    MSG Msg;
    while (PeekMessage(&Msg, handle, 0, 0, PM_NOREMOVE) == TRUE)
    {
        if (GetMessage(&Msg, handle, 0, 0))
        {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
    }
    /*
       if(fakeLButton || fakeRButton)
       {
       nGEvent event;
       event.type = nGEvent::MOUSE_BUTTON_DOWN;
       event.data.mouse.state = 1;
       event.data.mouse.x = fakeX;
       event.data.mouse.y = fakeY;
       if(fakeLButton) {event.data.mouse.button = 1; sendEvent(event); ng_dbg("fake L");}
       if(fakeRButton) {event.data.mouse.button = 3; sendEvent(event); ng_dbg("fake R"); }
       } */
#endif /*__nG_WIN32*/

#ifdef __nG_OSX
    result = RES_OK;
#endif /*__nG_OSX*/
    return result;
}



#ifdef __nG_WIN32
const LPTSTR nGSurface::getWindowClassName() {
    static const LPTSTR wcName = TEXT("ngWindowClass");
    return wcName;
}

WNDCLASSEX& nGSurface::getWindowClass() {
    static WNDCLASSEX wc;
    return wc;
}
#endif /*__nG_WIN32*/

int& nGSurface::getObjCountRef() {
    static int objcount = 0;
    return objcount;
}

nGSurface::nGSurface() :  sWidth(0),sHeight(0), surface(NULL), renderEnabled(true), parent(NULL), valid(false), keyMask(KEYMOD_NONE), surfaceType(SURFACE_NONE), nGEventHandler(NULL), nGEventHandlerUData(NULL), eventListener(NULL)
#ifdef NG_GLX_SUPPORT
    ,glctx(NULL)
#endif
#ifdef NG_EGL_SUPPORT
    ,eglDisplay(NULL), eglSurface(NULL), eglCtx(NULL)
#endif
{
    init();
};
#if 0
nGSurface(unsigned int width, unsigned int height) : sWidth(width),sHeight(height), nG_INIT_COMMON
{
    init();
    create(width,height, SURFACE_2D);
}
#endif

/*========================================================================
  nGSurface::~nGSurface : destructor
  ========================================================================*/
nGSurface::~nGSurface()
{
    destroy();
}


/*========================================================================
  nGSurface::destroy : destroy surface
  ========================================================================*/
nGResult nGSurface::destroy()
{
    if(valid)
    {
        valid = false;
        --getObjCountRef();
        sWidth = 0;
        sHeight = 0;
#ifdef __nG_X11


#ifdef NG_2D_SUPPORT
        if( surfaceType == SURFACE_2D )
        {
            if( screenImage) {
                XDestroyImage(screenImage);
            }
            screenImage = NULL;
            surface  = NULL;
            ng_dbg("!");
            XDestroyWindow(display,window);
            XFreeGC(display,gc);
        }
#endif /* NG_2D_SUPPORT*/

        if( surfaceType == SURFACE_GL)
        {
#ifdef NG_GLX_SUPPORT
            glXMakeCurrent(display, window, glctx); //activate current win
            glXMakeCurrent(display, None, NULL); //release
            glXDestroyContext(display, glctx);
            glctx = NULL;
#endif /*NG_GLX_SUPPORT */
#ifdef NG_EGL_SUPPORT
          if(eglDisplay && eglSurface && eglCtx) {
            eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglCtx);
            eglDestroyContext(eglDisplay, eglCtx);
            eglDestroySurface(eglDisplay, eglSurface);
            eglTerminate(eglDisplay);
            ng_dbg("egldisp: %p", eglDisplay);
            eglCtx = NULL;
            eglSurface = NULL;
            eglDisplay = NULL;
          }
          XDestroyWindow(display,window);
#endif
        }
        if( iconPixmap) XFreePixmap(display, iconPixmap);
        if( surfaceType == SURFACE_NONE) {
            XDestroyWindow(display,window);
        }
        ng_dbg("disp: %p", display);
        XCloseDisplay(display);
        display = NULL;
        gc= 0;
        window = 0;
#endif /*__nG_X11*/

#ifdef __nG_WIN32

        delete[] surface;
        surface = NULL;
        if(handle)
        {
            ReleaseDC(handle, hdc );
            DestroyWindow(handle);
            handle = NULL;
        }
        if(getObjCountRef() == 0)
        {
            UnregisterClass(getWindowClassName(),getWindowClass().hInstance);
        }
#endif /*__nG_WIN32*/

#ifdef __nG_OSX

# ifdef NG_GL_SUPPORT
        if(glView) [glView release];
        glView = nil;
# endif /*NG_GL_SUPPORT*/

# ifdef NG_METAL_SUPPORT
        if(metalView) [metalView release];
        metalView = nil;
# endif /*NG_GL_SUPPORT*/
        if(handle) [handle release];

# ifdef NG_2D_SUPPORT
        if(image) [image release];
        if(imageView) [imageView release];
        if(imageRef != nil) CGImageRelease(imageRef);
        CGDataProviderRelease(dataProvider);
        CGColorSpaceRelease(colorSpace);
# endif /*NG_2D_SUPPORT*/
#endif /*__nG_OSX*/

    }
    return RES_OK;
}

/*========================================================================
  nGSurface::setEventHandler : register surface event handler
  ========================================================================*/
void nGSurface::setEventHandler(nGEventHandler_f func, void* user_data)
{
    nGEventHandlerUData = user_data;
    nGEventHandler = func;
}

/*========================================================================
  nGSurface::setEventHandler : register surface event handler
  ========================================================================*/
void nGSurface::setEventHandler(nGEventListener* listener)
{
    nGEventHandlerUData = NULL;
    eventListener = listener;
}

/*========================================================================
  nGSurface::create : create new surface
  ========================================================================*/
nGResult nGSurface::create(unsigned int width, unsigned int height,nGSurfaceType _surfaceType,unsigned int a_winAttr, nGSurface* a_parent)
{
    // nGMutexLocker m(ngMtx);
    winAttr = a_winAttr;
    sWidth = width;
    sHeight = height;
    surfaceType = _surfaceType;
    parent = a_parent;
    nGResult result = RES_FAILED;

#ifdef __nG_X11

# ifdef NG_GLX_SUPPORT

     static int attrDB[] = {
      GLX_X_RENDERABLE, True, GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
      GLX_RENDER_TYPE     , GLX_RGBA_BIT,GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
      GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8,GLX_ALPHA_SIZE, 8,
      GLX_DEPTH_SIZE, 24, GLX_STENCIL_SIZE, 8,GLX_DOUBLEBUFFER, True,
      None
    };
    static int attrSB[] = {GLX_RGBA, GLX_RED_SIZE, 4,  GLX_GREEN_SIZE, 4,  GLX_BLUE_SIZE, 4,  GLX_DEPTH_SIZE, 16,  None};
# endif /*NG_GLX_SUPPORT */
    display = XOpenDisplay(NULL);
    if(display == NULL) {
        ng_err("couldn't open display");
        return RES_FAILED;
    }
    screen = DefaultScreen(display);
    Window parent_window = parent ? parent->window : RootWindow(display,screen);
#ifdef NG_GLX_SUPPORT
    XVisualInfo*  vinfo = NULL;
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    GLXFBConfig bestFbc = NULL;
#endif
    if( surfaceType == SURFACE_GL ) {
# ifdef NG_GLX_SUPPORT
      int glx_major = 0, glx_minor = 0;
      if ( glXQueryVersion( display, &glx_major, &glx_minor) == 0 || ( (glx_major == 1) && (glx_minor < 3) ) || (glx_major < 1) ) {
        doublebuffer = true;
        vinfo = glXChooseVisual( display, screen, attrDB);
        if( !vinfo) {
            vinfo = glXChooseVisual( display, screen, attrSB );
            ng_dbg("no doublebuffering");
            doublebuffer=false;
        }
        int depth = 0, stencil = 0, multisamp = 0, samps = 0;
        glXGetConfig(display, vinfo, GLX_DEPTH_SIZE, &depth);
        glXGetConfig(display, vinfo, GLX_STENCIL_SIZE, &stencil);
        glXGetConfig(display, vinfo, GLX_SAMPLE_BUFFERS_ARB, &multisamp);
        glXGetConfig(display, vinfo, GLX_SAMPLES_ARB, &samps);
        ng_dbg("d: %d , s: %d ms: %d, sps: %d", depth, stencil, multisamp, samps);

        glctx = glXCreateContext( display, vinfo, 0, GL_TRUE);
        visual = vinfo->visual;
      }
      else {  //https://www.khronos.org/opengl/wiki/Tutorial:_OpenGL_3.0_Context_Creation_(GLX)
        doublebuffer = true;
        int fbcount = 0;
        GLXFBConfig* fbc = glXChooseFBConfig(display, screen, attrDB, &fbcount);
        if(fbc) {
          int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;
          int samp_buf = 0, samples = 0;
          for(int i = 0; i < fbcount; ++i) {
            XVisualInfo *vi= glXGetVisualFromFBConfig( display, fbc[i] );
            if(vi) {
              glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
              glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLES       , &samples  );
            }
            if ( best_fbc < 0 || (samp_buf && (samples > best_num_samp)) ) {
              best_fbc = i, best_num_samp = samples;
            }
            if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp ) {
              worst_fbc = i, worst_num_samp = samples;
            }
            XFree(vi);
          }
          bestFbc = fbc[ best_fbc ];
          XFree(fbc);
          vinfo = glXGetVisualFromFBConfig( display, bestFbc );
          visual = vinfo->visual;
          glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );
        }
      }
# else
# ifndef NG_EGL_SUPPORT
        ng_err("3d support not compiled!!!!");
#else
        visual = DefaultVisual(display,screen);
# endif //NG_EGL_SUPPORT
# endif /*NG_GLX_SUPPORT */
    }
    else if( surfaceType == SURFACE_2D) {
        visual = DefaultVisual(display,screen);
    } else if(surfaceType == SURFACE_NONE) {
        visual = CopyFromParent;
    }
    if(visual == NULL && surfaceType != SURFACE_NONE) { //CopyFromParent equals 0L
        if(surfaceType == SURFACE_GL) {
# ifndef NG_GL_SUPPORT
            ng_err("no visual. 3D support not compiled!!!!");
# endif
        }
        else {
            ng_err("no visual!!!!");
        }
        return RES_FAILED;
    }
    unsigned int eventMask = CWBorderPixel | CWEventMask | CWColormap;
    XSetWindowAttributes attr;
    attr.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask |ButtonReleaseMask| StructureNotifyMask | ButtonMotionMask | PointerMotionMask | FocusChangeMask| EnterWindowMask|LeaveWindowMask;
    attr.border_pixel = CopyFromParent;
    attr.colormap = XCreateColormap(display, parent_window, visual, AllocNone);
    window = XCreateWindow(display, parent_window, 0, 0, width, height, CopyFromParent, CopyFromParent, InputOutput, visual, eventMask , &attr);


    if (! (winAttr & nGResize) ) {
        XSizeHints hints;
        memset(&hints, 0, sizeof(XSizeHints));
        hints.min_width = hints.max_width = width;
        hints.min_height = hints.max_height = height;
        hints.flags = PMaxSize | PMinSize;
        XSetWMNormalHints(display, window, &hints);
    }
    if( winAttr & nGStayOnTop ) {
        Atom wmStateAbove = XInternAtom( display, "_NET_WM_STATE_ABOVE", 1 );
        Atom wmNetWmState = XInternAtom( display, "_NET_WM_STATE", 1 );
        if( wmStateAbove != None ) {
            XClientMessageEvent xclient;
            memset( &xclient, 0, sizeof (xclient) );
            xclient.type = ClientMessage;
            xclient.window = window;
            xclient.message_type = wmNetWmState;
            xclient.format = 32;
            xclient.data.l[0] = 1;//_NET_WM_STATE_ADD;
            xclient.data.l[1] = wmStateAbove;
            xclient.data.l[2] = 0;
            xclient.data.l[3] = 0;
            xclient.data.l[4] = 0;
            XSendEvent( display, DefaultRootWindow( display ), False, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent *)&xclient );
        }
    }
    if( winAttr & nGNoTitleBar) {
# define MWM_HINTS_DECORATIONS   (1L << 1)
        struct {
            unsigned long flags;
            unsigned long functions;
            unsigned long decorations;
            long input_mode;
            unsigned long status;
        } mwmhints = { MWM_HINTS_DECORATIONS, 0,
            0/*MWM_DECOR_NONE*/, 0, 0 };
        Atom prop;
        prop = XInternAtom(display, "_MOTIF_WM_HINTS", False);
        XChangeProperty(display, window, prop, prop, 32, PropModeReplace, (unsigned char *) &mwmhints, sizeof(mwmhints)/4);
    }

    if(winAttr & nGHideInTaskBar) {
        Atom net_wm_state_skip_taskbar=XInternAtom (display,  "_NET_WM_STATE_SKIP_TASKBAR", False);
        Atom net_wm_state = XInternAtom (display,   "_NET_WM_STATE", False);
        XChangeProperty (display, window, net_wm_state, XA_ATOM, 32, PropModeAppend, (unsigned char *)&net_wm_state_skip_taskbar, 1);
    }
    wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(display, window, &wmDeleteMessage, 1);
    XMapRaised(display, window);
# ifdef NG_GLX_SUPPORT
    if ( surfaceType == SURFACE_GL ) {
       if(glXCreateContextAttribsARB != NULL && NG_GL_VERSION_MAJOR >= 3) {
          int context_attribs[] = { GLX_CONTEXT_MAJOR_VERSION_ARB, NG_GL_VERSION_MAJOR, GLX_CONTEXT_MINOR_VERSION_ARB, NG_GL_VERSION_MINOR,
                        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                        None };
          glctx = glXCreateContextAttribsARB( display, bestFbc, 0, True, context_attribs );
        }
      else {
          glctx = glXCreateContext( display, vinfo, 0, GL_TRUE);
      }
      if(glctx) {
        XSync( display, False );
        glXMakeCurrent( display, window, glctx );
      }
    }
    if(vinfo) {
      XFree(vinfo);
      vinfo = NULL;
    }
#endif
# ifdef NG_2D_SUPPORT
    if( surfaceType == SURFACE_2D ) {
        gc = XCreateGC(display,window,0,0);
        depth = DefaultDepth(display,screen);
        surface = (unsigned char*)malloc(sWidth*sHeight*4);
        screenImage = XCreateImage( display, visual, depth, ZPixmap, 0, (char*)surface, sWidth, sHeight, 32, sWidth*4);
    }
# endif /*NG_2D_SUPPORT*/

    if( surfaceType == SURFACE_GL) {
# ifdef NG_GL_SUPPORT
      activate();
#  ifdef NG_GLX_SUPPORT
      if (!glXIsDirect(display, glctx)) { ng_dbg("no direct rendering");}
#   endif //NG_GLX_SUPPORT
# endif /*NG_GL_SUPPORT*/
    }
    ng_dbg("!");

    result = RES_OK;
# ifndef NG_EGL_SUPPORT
    nanoGFX::nGEvent ev(nanoGFX::nGEvent::WINDOW_CREATE);
    sendEvent(ev);
# endif
#endif /*__nG_X11*/

#ifdef __nG_WIN32

    hInstance= GetModuleHandle(0);
    ng_dbg("this: %p", this);
    if(getObjCountRef() == 0) {
        memset(&getWindowClass(),0, sizeof(WNDCLASSEX));
        getWindowClass().cbSize = sizeof(WNDCLASSEX);
        getWindowClass().lpfnWndProc = winCallback;
        getWindowClass().hInstance     = hInstance;
        getWindowClass().hCursor       = LoadCursor(NULL, IDC_ARROW);
        getWindowClass().hbrBackground = (HBRUSH) NULL;//(HBRUSH)(COLOR_BTNSHADOW+1);
        getWindowClass().lpszClassName = getWindowClassName();
        getWindowClass().style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        if(!RegisterClassEx(&getWindowClass()))  return RES_FAILED;
    }

    unsigned int x = CW_USEDEFAULT;
    unsigned int y = CW_USEDEFAULT;
    unsigned int winflags= 0;
    if(winAttr & nGFullscreen) {
        winflags = WS_POPUP;
        x = 0;
        y = 0;
    } else if(winAttr & nGMaximized) {
        winflags = WS_POPUP| WS_EX_TOPMOST;
        sWidth = GetSystemMetrics(SM_CXSCREEN);
        sHeight = GetSystemMetrics(SM_CYSCREEN)-1;
        x = 0;
        y = 0;
    }
    else {
        winflags = WS_VISIBLE;
        if (winAttr & nGNoTitleBar) winflags |= WS_POPUP;
        else winflags=WS_OVERLAPPED | WS_SYSMENU;
        if (winAttr & nGResize) winflags |= WS_THICKFRAME;
        if (winAttr & nGMaximize) winflags |= WS_MAXIMIZEBOX;
        if (winAttr & nGMinimize) winflags |= WS_MINIMIZEBOX;
    }
    winflags |=WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    const LPTSTR winName = TEXT(" ");
    handle = CreateWindowEx(WS_EX_APPWINDOW,getWindowClassName(), winName, winflags , x,y,sWidth, sHeight, NULL, NULL, hInstance, NULL);
    if( handle) {
        if(parent!= NULL) { SetParent(handle, parent->handle);}
        SetWindowLongPtr(handle, GWLP_WNDPROC, GetWindowLongPtr(handle, GWLP_WNDPROC));
        SetWindowLongPtr( handle, GWLP_USERDATA , (LONG_PTR)this);

        if(!(winAttr & nGFullscreen)) {
            size_t width = sWidth, height = sHeight;
            adjustSize(width, height);
            BOOL res =MoveWindow(handle,sX, sY, width, height, TRUE);
            if(winAttr & nGDropShadow) {
                SetClassLongPtr(handle,GCL_STYLE, CS_DROPSHADOW);
            }

        }
        else {
            DEVMODE dmScreenSettings;                   // Device Mode
            memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
            dmScreenSettings.dmSize = sizeof(dmScreenSettings);
            dmScreenSettings.dmPelsWidth    = sWidth;
            dmScreenSettings.dmPelsHeight   = sHeight;
            dmScreenSettings.dmBitsPerPel   = GetDeviceCaps(GetDC(handle),BITSPIXEL);
            dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
            ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
        }

        if(winAttr & nGStayOnTop) {
            RECT rect;
            GetWindowRect(handle, &rect );
            SetWindowPos(handle , HWND_TOPMOST,  rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
        }

        if(winAttr & nGKeepOnBottom) {
            RECT rect;
            GetWindowRect(handle, &rect );
            SetWindowPos(handle , HWND_BOTTOM,  rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW|SWP_NOOWNERZORDER);
        }


        ShowWindow(handle,SW_SHOWDEFAULT);
        UpdateWindow(handle);

        memset(&bmpinfo, 0, sizeof(BITMAPINFO));
        bmpinfo.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
        bmpinfo.bmiHeader.biPlanes = 1;
        bmpinfo.bmiHeader.biBitCount = 32;
        bmpinfo.bmiHeader.biCompression = BI_RGB;
        bmpinfo.bmiHeader.biWidth = sWidth;
        bmpinfo.bmiHeader.biHeight = -(LONG)sHeight;
        hdc = GetDC(handle);
        TRACKMOUSEEVENT tme;
        tme.cbSize= sizeof(TRACKMOUSEEVENT);
        tme.dwFlags=TME_HOVER|TME_LEAVE;
        tme.hwndTrack = handle;
        tme.dwHoverTime = 1;
        TrackMouseEvent(&tme);

# ifdef NG_GL_SUPPORT
        if( surfaceType == SURFACE_GL) {
            unsigned int pfdFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL;
            if(!(winAttr & nGResize)) {
                pfdFlags |= PFD_DOUBLEBUFFER;
                doublebuffer = true;
            }
            else {
                doublebuffer = false;
                //pfdFlags |= PFD_SUPPORT_GDI; //problem on amd
            }
            const static PIXELFORMATDESCRIPTOR pfd={sizeof(PIXELFORMATDESCRIPTOR),1,pfdFlags, PFD_TYPE_RGBA,
                24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0,PFD_MAIN_PLANE, 0, 0, 0, 0};
            SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);
            int iPixelFormat = GetPixelFormat(hdc);
            PIXELFORMATDESCRIPTOR pfd2;
            memset(&pfd2, 0, sizeof(pfd2) );
            DescribePixelFormat(hdc, iPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd2);
            hrc = wglCreateContext(hdc);
            wglMakeCurrent(hdc,hrc );
            if(NG_GL_VERSION_MAJOR > 1) {
                wglCreateContextAttribsARBProc wglCreateContextAttribsARB = (wglCreateContextAttribsARBProc)wglGetProcAddress("wglCreateContextAttribsARB");
                if(wglCreateContextAttribsARB) {
#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_FLAGS_ARB                   0x2094
#define WGL_CONTEXT_DEBUG_BIT_ARB               0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB  0x0002
                  const int contextAttributes[] = {
                      WGL_CONTEXT_MAJOR_VERSION_ARB, NG_GL_VERSION_MAJOR,
                      WGL_CONTEXT_MINOR_VERSION_ARB, NG_GL_VERSION_MINOR,
                  //    WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                      0
                  };
                  HGLRC hrcTemp = wglCreateContextAttribsARB( hdc, hrc, contextAttributes);
                  if(hrcTemp) {
                      wglMakeCurrent( hdc, hrcTemp );
                      wglDeleteContext(hrc);
                      hrc = hrcTemp;
                  }
                }
            }
        }
# endif /*NG_GL_SUPPORT*/

# ifdef NG_2D_SUPPORT
        if( surfaceType == SURFACE_2D) {
            surface = new unsigned char[sWidth*sHeight*4];
        }
# endif /*NG_2D_SUPPORT*/
        result = RES_OK;
    }
    fakeLButton = false;
    fakeRButton = false;
    nanoGFX::nGEvent ev(nanoGFX::nGEvent::WINDOW_CREATE);
    sendEvent(ev);
#endif /*__nG_WIN32*/


#ifdef __nG_OSX
    bool firstRun = false;
    if(getObjCountRef() == 0)
    {
        [NSAutoreleasePool new];
        getAppHandlePtr() = (nGApplication*)[nGApplication sharedApplication];
        [getAppHandlePtr() setActivationPolicy:NSApplicationActivationPolicyRegular];
        firstRun = true;
    }

    NSRect frame = NSMakeRect(0,0,sWidth, sHeight);
    unsigned int mask = NSWindowStyleMaskTitled|NSWindowStyleMaskClosable;
    if(winAttr & nGNoTitleBar) {
        mask = NSWindowStyleMaskBorderless;
    }
    if(winAttr & nGFullscreen) {
        mask = NSWindowStyleMaskBorderless;
        frame = [[NSScreen mainScreen] frame];
    }
    if(winAttr & nGResize) {
        mask  |= NSWindowStyleMaskResizable;
    }
    handle = [[nGWindow alloc] initWithContentRect:frame styleMask:mask backing:NSBackingStoreBuffered defer:NO];
    if(parent) {
        [(nGWindow*)parent->getHandle() addChildWindow: handle ordered: NSWindowAbove];
    }
    if(winAttr & nGFullscreen) {
        [handle setLevel:NSMainMenuWindowLevel+1];
        [handle setHidesOnDeactivate:YES];
    }
    if(winAttr & nGStayOnTop) {
        [handle setLevel:NSMainMenuWindowLevel+1];
    }
    if(winAttr & nGKeepOnBottom) {
        [handle setLevel:kCGDesktopIconWindowLevel+1];
    }
    [handle setOpaque:YES];
    if(surfaceType == SURFACE_GL) {
#ifdef NG_GL_SUPPORT
        glView = [[nGOpenGLView alloc] initWithFrame:frame];
        glView.ng_ptr = this;
        [handle setContentView:glView];

        [glView addTrackingRect:frame owner:glView userData:NULL assumeInside:NO];
#endif /*NG_GL_SUPPORT*/
    }
    if(surfaceType == SURFACE_2D)
    {
# ifdef NG_2D_SUPPORT

        surface = new unsigned char[sWidth*sHeight*4];
        CGDataProviderDirectCallbacks providerCallbacks = {0,dataProviderSurfacePtr, deleteSurfaceData,0,0};
        colorSpace = CGColorSpaceCreateDeviceRGB();
        dataProvider = CGDataProviderCreateDirect(this, sWidth*sHeight*4,	&providerCallbacks);
        imageRef = CGImageCreate(sWidth, sHeight, 8, 32, sWidth * 4, colorSpace, kCGImageAlphaLast | kCGBitmapByteOrder32Big, dataProvider, 0, false, kCGRenderingIntentDefault);
        image = [[NSImage alloc] initWithCGImage: imageRef size:frame.size];
        imageView = [[nGImageView alloc] init];
        imageView.ng_ptr = this;
        imageView.do_init = true;
        [imageView setImage:image];
 //       [image drawInRect:frame];

        [imageView addTrackingRect:frame owner:imageView userData:NULL assumeInside:NO];
        [handle setContentView:imageView];
# endif /*NG_2D_SUPPORT*/
    }
    handle.ng_ptr = this;
    [handle setDelegate:handle];
    [handle makeKeyAndOrderFront: handle];
    [handle makeFirstResponder:handle];
    [handle setAcceptsMouseMovedEvents:YES];
    [NSApp activateIgnoringOtherApps:YES];
    result = RES_OK;
#endif /*__nG_OSX*/

    if( result == RES_OK)
    {
        ++getObjCountRef();
        valid = true;

    }
    return result;
}

/*========================================================================
  nGSurface::sendEvent :   add event to queue
  ========================================================================*/
void nGSurface::sendEvent(nGEvent& ev) {
    if(nGEventHandler != NULL) nGEventHandler(*this, ev, nGEventHandlerUData);
    if(eventListener != NULL) eventListener->onEvent(*this, ev);
    if (ev.type != nGEvent::WINDOW_REPAINT && ev.type != nGEvent::MOUSE_MOTION) { ng_dbg("send event, type: %d", ev.type); }
}

#ifdef NG_CLIPBOARD_SUPPORT

/*========================================================================
  nGSurface::toClipboard : copy data to clipboard
  ========================================================================*/
nGResult nGSurface::toClipboard(const std::string& data)
{
#ifdef __nG_X11
    XSetSelectionOwner(display, XInternAtom(display,"CLIPBOARD",True), RootWindow(display, DefaultScreen(display)), CurrentTime );
#endif /*__nG_X11*/

#ifdef __nG_WIN32
    char far *buffer;
    int bytes;
    HGLOBAL clipbuffer;
    bytes = data.size();

    OpenClipboard(NULL);
    EmptyClipboard();
    clipbuffer = GlobalAlloc(GMEM_DDESHARE,bytes+1);
    buffer = (char far*)GlobalLock(clipbuffer);
    if (buffer == NULL)
        return RES_FAILED;
    strcpy(buffer,data.c_str() );
    GlobalUnlock(clipbuffer);
    SetClipboardData(CF_TEXT,clipbuffer);
    CloseClipboard();
#endif
    clipboard = data;
    return RES_OK;
}
#endif


/*========================================================================
  nGSurface::activate : activate current surface
  ========================================================================*/
nGResult nGSurface::activate()
{

    if( !valid) return RES_FAILED;
    if(surfaceType != SURFACE_GL) return RES_OK;
#ifdef NG_GL_SUPPORT
#  ifdef NG_GLX_SUPPORT
    glXMakeCurrent(display, window, glctx);
#  endif /*NG_GLX_SUPPORT*/
#  ifdef __nG_WIN32
    wglMakeCurrent(hdc,hrc );
#  endif
#endif /*NG_GL_SUPPORT */
#ifdef NG_EGL_SUPPORT
    if(eglDisplay && eglSurface && eglCtx) {
      if(!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglCtx)) {
        ng_err("eglMakeCurrent failed!");
      }
    }
#endif
    return RES_OK;
}

/*========================================================================
  nGSurface::update : draw and get events
  ========================================================================*/
void nGSurface::update()
{
    static nGEvent repaintEvent(nGEvent::WINDOW_REPAINT);
    if( !valid) return;
    if(renderEnabled) {
#ifndef __nG_OSX
        //activate();
        //sendEvent(repaintEvent);

        doRepaint();
#endif
#ifdef __nG_X11
        if( surfaceType == SURFACE_GL) {
#  ifdef NG_GLX_SUPPORT
            glXMakeCurrent(display, window, glctx);
            if(doublebuffer) {
                glXSwapBuffers(display, window);
            }
#  endif
#  ifdef NG_EGL_SUPPORT
            if(eglDisplay && eglSurface) {
              if(!eglSwapBuffers(eglDisplay, eglSurface)) {
                ng_err("eglSwapBuffers failed!");
              }
            }
#  endif
        }
        if( surfaceType == SURFACE_2D) {
            XPutImage(display, window,gc,  screenImage, 0,0,0,0,sWidth, sHeight);
            XSync(display,0);
            XFlush(display);
        }
#endif /*__nG_X11*/



#ifdef __nG_OSX
#  ifdef NG_GL_SUPPORT
        if( surfaceType == SURFACE_GL) {
            [glView setNeedsDisplay:YES];
        }
#  endif //NG_GL_SUPPORT
# ifdef NG_METAL_SUPPORT
        if(metalView) {
            [metalView setNeedsDisplay:YES];
        }
# endif //NG_METAL_SUPPORT
        if( surfaceType == SURFACE_2D )
        {
# ifdef NG_2D_SUPPORT
            [imageView setNeedsDisplay:YES];
# endif /* NG_2D_SUPPORT*/
        }
#endif /*__nG_OSX*/
    }
    processEvents();
}

unsigned char* nGSurface::getSurface() { return surface; }
unsigned int nGSurface::getWidth() const{
#ifdef __nG_OSX
    return [handle contentRectForFrameRect: [handle frame]].size.width;
#endif

    return sWidth;
};
unsigned int nGSurface::getHeight() const{
#ifdef __nG_OSX
    return [handle contentRectForFrameRect: [handle frame]].size.height;
#endif
    return sHeight;
};
bool nGSurface::isValid() const {  return valid; }

/*========================================================================
  nGSurface::setWindowTitle : set new window title
  ========================================================================*/
void nGSurface::setWindowTitle( const char* name)
{
#ifdef __nG_X11
    XStoreName(display, window, name);
#endif /*__nG_X11*/

#ifdef __nG_WIN32
# ifdef _UNICODE
    int size = MultiByteToWideChar(CP_UTF8, 0, name, strlen(name), NULL, 0);
    std::wstring s( size, 0 );
    MultiByteToWideChar(CP_UTF8, 0, name, strlen(name), &s[0], size);
    SetWindowTextW(handle, s.c_str());
# else
    SetWindowTextA(handle, name);
# endif /*_UNICODE*/
#endif /*__nG_WIN32*/
#ifdef __nG_OSX
    [handle setTitle: [NSString stringWithUTF8String: name]];
#endif
}

/*========================================================================
  nGSurface::setFocus : set focus to window containing current nGSurface
  ========================================================================*/
void nGSurface::setFocus( )
{
#ifdef __nG_X11
    XSetInputFocus(display, window, RevertToNone, CurrentTime);
    XRaiseWindow(display,window);
#endif
#ifdef __nG_WIN32
    ::SetFocus(handle);
    SwitchToThisWindow(handle, true);
#endif /*__nG_WIN32*/
#ifdef __nG_OSX
    [[NSApp mainWindow] makeKeyAndOrderFront:handle];
#endif //__nG_OSX
}


/*========================================================================
nGSurface:setMouseCapture : enable/disable mouse capture
========================================================================*/
void nGSurface::setMouseCapture(bool val)
{
#ifdef __nG_WIN32
    if(val)
        SetCapture(handle);
    else ReleaseCapture();
#elif defined __nG_X11
    if(val)
        XGrabPointer(display, window, False, ButtonPressMask | ButtonReleaseMask | PointerMotionMask ,
                     GrabModeAsync, GrabModeAsync, RootWindow(display, DefaultScreen(display)), None,  CurrentTime);
    else
        XUngrabPointer(display, CurrentTime);
#endif /*__nG_WIN32*/
}

/*========================================================================
  nGSurface::move : move window containing current surface
  ========================================================================*/
void nGSurface::move(int x, int y)
{
    sX = x;
    sY = y;
#ifdef __nG_X11
    XMoveWindow(display,window,x,y);
#endif /*__nG_X11*/

#ifdef __nG_WIN32
    size_t width = sWidth, height = sHeight;
    adjustSize(width, height);
    BOOL res =MoveWindow(handle,sX, sY, width, height, FALSE);
#endif /*__nG_WIN32*/

#ifdef __nG_OSX
    NSRect frame = [handle frame];
    if(parent != NULL) {
        NSRect parentRect = [(nGWindow*)parent->getHandle() frame];
        frame.origin.x = parentRect.origin.x + x;
        frame.origin.y = parentRect.origin.y + [(nGWindow*)parent->getHandle() contentRectForFrameRect: parentRect].size.height - frame.size.height - y;
    }
    else {
        NSRect screen = [[NSScreen mainScreen] frame];
        frame.origin.x = x;
        frame.origin.y = screen.size.height - sHeight - y;
    }
    [handle setFrame: frame display : NO];
#endif
}

/*========================================================================
  nGSurface::rebuild2DSurface : resize backbuffer for 2d surface
  ========================================================================*/
void nGSurface::rebuild2DSurface()
{
# ifdef NG_2D_SUPPORT
    if( surfaceType == SURFACE_2D ) {
#ifdef __nG_X11
      if(screenImage) {
        XDestroyImage(screenImage);
        surface = (unsigned char*)malloc(sWidth * sHeight * 4);
        screenImage = XCreateImage( display, visual, depth, ZPixmap, 0, (char*)surface, sWidth, sHeight, 32, sWidth*4);
      }
#endif //__nG_X11
#ifdef __nG_WIN32
      delete [] surface;
      surface = new unsigned char[sWidth * sHeight * 4];
      bmpinfo.bmiHeader.biWidth = sWidth;
      bmpinfo.bmiHeader.biHeight = -(LONG)sHeight;
#endif //__nG_WIN32
#ifdef __nG_OSX
      NSRect contentRect = [handle contentRectForFrameRect: [handle frame] ];
      sWidth = contentRect.size.width;
      sHeight = contentRect.size.height;
      delete [] surface;
      surface = new unsigned char[sWidth * sHeight * 4];
      if(image) [image release];
      if(imageView) [imageView release];
      if(imageRef != nil) CGImageRelease(imageRef);
      CGDataProviderRelease(dataProvider);
      CGDataProviderDirectCallbacks providerCallbacks = {0,dataProviderSurfacePtr, deleteSurfaceData,0,0};
      dataProvider = CGDataProviderCreateDirect(this, sWidth*sHeight*4,	&providerCallbacks);
      imageRef = CGImageCreate(sWidth, sHeight, 8, 32, sWidth * 4, colorSpace, kCGImageAlphaLast | kCGBitmapByteOrder32Big, dataProvider, 0, false, kCGRenderingIntentDefault);
      image = [[NSImage alloc] initWithCGImage: imageRef size:contentRect.size];
      imageView = [[nGImageView alloc] init];
      imageView.ng_ptr = this;
      [imageView setImage:image];
      [imageView addTrackingRect:contentRect owner:imageView userData:NULL assumeInside:NO];
      [handle setContentView:imageView];
#endif //__nG_OSX
    }
#endif //NG_2D_SUPPORT
}

/*========================================================================
  nGSurface::adjustSize : adjust window client size to match provided width/height
  ========================================================================*/
void nGSurface::adjustSize(size_t& width, size_t& height)
{
# ifdef __nG_WIN32
    RECT rcWindow, rcClient;
    GetWindowRect(handle, &rcWindow);
    GetClientRect(handle, &rcClient);
    width += (rcWindow.right - rcWindow.left) - rcClient.right;
    height += (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
# endif //__nG_WIN32
}

/*========================================================================
  nGSurface::resize : resize window containing current surface
  ========================================================================*/
void nGSurface::resize(int w, int h)
{
    sWidth = w;
    sHeight = h;
#ifdef __nG_X11
    XResizeWindow(display,window, w, h);
#endif /*__nG_X11*/

#ifdef __nG_WIN32
    RECT r;
    GetWindowRect(handle, &r);
    sX = r.left;
    sY = r.top;
    size_t width = sWidth, height = sHeight;
    adjustSize(width, height);
    BOOL res =MoveWindow(handle,sX, sY, width, height, TRUE);
#endif /*__nG_WIN32*/

#ifdef __nG_OSX
    NSRect frame = [handle frame];
    frame.size = NSMakeSize(w, h);
    [handle setFrame: frame display : NO];
#endif /*__nG_OSX*/
    handleResize();
}

/*========================================================================
  nGSurface::resize : resize window containing current surface
  ========================================================================*/
void nGSurface::resize(int x, int y, int w, int h)
{
    sX = x;
    sY = y;
    sWidth = w;
    sHeight = h;
#ifdef __nG_X11
    XMoveResizeWindow(display,window,x,y, w, h);
#endif /*__nG_X11*/

#ifdef __nG_WIN32
    size_t width = sWidth, height = sHeight;
    adjustSize(width, height);
    BOOL res =MoveWindow(handle,sX, sY, width, height, TRUE);
#endif /*__nG_WIN32*/

#ifdef __nG_OSX
    NSRect frame = [handle frame];
    frame.size.width = w;
    frame.size.height = h;
    if(parent != NULL) {
        NSRect parentRect = [(nGWindow*)parent->getHandle() frame];
        frame.origin.x = parentRect.origin.x + x;
        frame.origin.y = parentRect.origin.y + [(nGWindow*)parent->getHandle() contentRectForFrameRect: parentRect].size.height - frame.size.height - y;
    }
    else {
        NSRect screen = [[NSScreen mainScreen] frame];
        frame.origin.x = x;
        frame.origin.y = screen.size.height - sHeight - y;
    }
    [handle setFrame: frame display : NO];
#endif /*__nG_OSX*/
    handleResize();
}

/*========================================================================
  nGSurface::getPosition : get current window position
  ========================================================================*/
void nGSurface::getPosition(int& x, int& y)
{
    x = sX;
    y = sY;
#ifdef __nG_WIN32
    //RECT r;
    //GetWindowRect(handle,&r);
    //x = r.left;
    //y = r.top;
#endif /*__nG_WIN32*/
#ifdef __linux__
  XWindowAttributes xwa;
  XGetWindowAttributes(display, window, &xwa);
  x = xwa.x;
  y = xwa.y;
#endif //__linux__
}
void nGSurface::setRenderEnabled(bool value) { renderEnabled = value; }


/*========================================================================
  nGSurface::setIcon : set icon for window
  ========================================================================*/
void nGSurface::setIcon(unsigned char* data, unsigned int width, unsigned int height)
{

#ifdef __nG_X11
    Atom net_wm_icon = XInternAtom(display, "_NET_WM_ICON", False);
    Atom cardinal = XInternAtom(display, "CARDINAL", False);
    typedef unsigned long icoitem_t;
    icoitem_t* data_rev = new icoitem_t[width * height + 2];
    icoitem_t* ptr = data_rev;
    ng_dbg("size: %lu", sizeof(icoitem_t));
    ptr[0] = width;
    ptr[1] = height;
    ptr+=2;
    for(unsigned int i = 0; i < width * height * 4; i += 4) {
        icoitem_t val = ((((icoitem_t)data[i+3]) << 24) | (((icoitem_t)data[i]) << 16) | (((icoitem_t)data[i+1]) << 8) | ((icoitem_t)data[i+2]))  & 0xffffffff;
        *ptr = val;
        ++ptr;
    }
    XChangeProperty(display, window, net_wm_icon, cardinal, 32, PropModeReplace, (const unsigned char*) data_rev, width * height + 2);
    delete [] data_rev;
#endif /*__nG_X11*/


#ifdef __nG_WIN32

    unsigned char* data_rev = new unsigned char[width * height * 4];
    for(unsigned int i = 0; i < width * height * 4; i += 4) {
        data_rev[i] = data[i + 2];
        data_rev[i + 1] = data[i + 1];
        data_rev[i + 2] = data[i];
        data_rev[i + 3] = data[i + 3];
    }
    HICON icon = CreateIcon(GetModuleHandle(NULL), width, height, 1, 32, NULL, data_rev);
    if (icon)
    {
        SendMessage(handle, WM_SETICON, ICON_BIG,   (LPARAM)icon);
        SendMessage(handle, WM_SETICON, ICON_SMALL, (LPARAM)icon);
    }
    else
    {
        ng_dbg("couldn't create icon");
    }
    delete[] data_rev;


#endif /*__nG_WIN32*/

#ifdef __nG_OSX
    CGColorSpaceRef space = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrder32Big | kCGImageAlphaLast;
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, data, width * height * 4, nil);
    CGImageRef cgref = CGImageCreate(width, height, 8, 32, width*4, space,bitmapInfo, provider, NULL, false , renderingIntent);
    CFRelease(space);
    NSImage *icon = [[NSImage alloc] autorelease];
    [icon initWithCGImage: cgref size:NSMakeSize(width,height)];
    [NSApp setApplicationIconImage:icon];
    CFRelease(provider);
    CFRelease(cgref);
#endif /* __nG_OSX*/
}

/*========================================================================
  nGSurface::getHandle : returns surface handle
  ========================================================================*/
void* nGSurface::getHandle()
{
    return (void*)handle;
}

void* nGSurface::getDevice()
{
#ifdef NG_METAL_SUPPORT
    return [metalView getDevice];
#endif
    return NULL;
}


void nGSurface::doRepaint()
{
    static nGEvent repaintEvent(nanoGFX::nGEvent::WINDOW_REPAINT);
    activate();
    if(!doublebuffer) {
#ifdef __nG_OSX
# ifdef NG_GL_SUPPORT
    if(surfaceType == SURFACE_GL) {
        glDrawBuffer(GL_FRONT);
    }
# endif /*NG_GL_SUPPORT*/
#endif
    }
    sendEvent(repaintEvent);
#ifdef __nG_OSX
# ifdef NG_GL_SUPPORT
    if(surfaceType == SURFACE_GL) {
        [[glView openGLContext] flushBuffer];
    }
# endif	/*NG_GL_SUPPORT*/
# ifdef NG_2D_SUPPORT
    if(surfaceType == SURFACE_2D) {
        imageRef = CGImageCreate(sWidth, sHeight, 8, 32, sWidth * 4, colorSpace, kCGImageAlphaLast | kCGBitmapByteOrder32Big, dataProvider, 0, false, kCGRenderingIntentDefault);
        NSRect frame = NSMakeRect(0,0,sWidth,sHeight);
        if([NSGraphicsContext currentContext] != nullptr) {
           CGContextDrawImage((CGContextRef)[[NSGraphicsContext currentContext] graphicsPort], frame, imageRef);
        }
        else {
            ng_err("context is null!")
        }
        CGImageRelease(imageRef);
        imageRef = nil;
    }
# endif /*NG_2D_SUPPORT*/
#endif //__nG_OSX
#ifdef __nG_WIN32
    if( surfaceType == SURFACE_GL) {
#  ifdef NG_GL_SUPPORT
        wglMakeCurrent(hdc,hrc );
        if(doublebuffer) {
            SwapBuffers(hdc);
        }
#  endif //NG_GL_SUPPORT
#  ifdef NG_EGL_SUPPORT
        eglSwapBuffers(eglDisplay, eglSurface);
#  endif
    }
#  ifdef NG_2D_SUPPORT
    if( surfaceType == SURFACE_2D ) {
        StretchDIBits( hdc, 0, 0, sWidth, sHeight, 0, 0, sWidth, sHeight, surface, &bmpinfo, DIB_RGB_COLORS, SRCCOPY );
    }
#  endif //NG_2D_SUPPORT
#endif /*__nG_WIN32*/
}



#ifdef NG_VULKAN_SUPPORT
nGResult nGSurface::createVulkanSurface(VkInstance instance, VkSurfaceKHR* surface) {
    nGResult result = RES_FAILED;
#ifdef __nG_X11
    VkXlibSurfaceCreateInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    info.pNext = NULL;
    info.flags = 0;
    info.dpy = display;
    info.window = window;
    VkResult res = vkCreateXlibSurfaceKHR(instance, &info, NULL, surface);
    if(res == VK_SUCCESS) { result = RES_OK;}
#elif defined __nG_WIN32
    VkWin32SurfaceCreateInfoKHR info =  {};
    info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    info.pNext = NULL;
    info.flags = 0;
    info.hinstance = GetModuleHandle(NULL);//hInstance;
    info.hwnd = handle;
    VkResult res = vkCreateWin32SurfaceKHR(instance, &info, NULL, surface);
    ng_dbg("res: %d", res);
    if(res == VK_SUCCESS) { result = RES_OK;}
#endif
    return result;
}
#endif

#ifdef NG_EGL_SUPPORT
nGResult nGSurface::createEGLSurface() {
    nGResult result = RES_FAILED;
  static const EGLint configAttribs[] = {
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_DEPTH_SIZE, 8,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_NONE
  };
#ifdef __nG_X11
  eglDisplay = eglGetDisplay((EGLNativeDisplayType)display);
  ng_dbg("egldisp: %p, disp: %p", eglDisplay, display);
#endif
  if(eglDisplay) {
    EGLint maj = 0, min = 0;
    if(eglInitialize(eglDisplay, &maj, &min)) {
      ng_dbg("maj: %d, min:%d\n", maj, min);
      EGLConfig config = NULL;
      EGLint num_config = 0;
      if(eglChooseConfig(eglDisplay, configAttribs, &config, 1, &num_config)) {
        EGLNativeWindowType win;
#ifdef __nG_X11
        win = (EGLNativeWindowType)window;
#elif defined __nG_WIN32
        win = (EGLNativeWindowType)handle;
#endif
         eglSurface = eglCreateWindowSurface(eglDisplay, config, (EGLNativeWindowType)win, NULL);
         if(eglSurface) {
           eglBindAPI(EGL_OPENGL_API);
            const EGLint ctxAttr[] = {
              EGL_CONTEXT_MAJOR_VERSION, NG_GL_VERSION_MAJOR,
              EGL_CONTEXT_MINOR_VERSION, NG_GL_VERSION_MINOR,
              EGL_NONE
            };
            eglCtx = eglCreateContext(eglDisplay, config, EGL_NO_CONTEXT, ctxAttr);

 //           eglCtx = eglCreateContext(eglDisplay, config, EGL_NO_CONTEXT, NULL);
            if(eglCtx) {
              eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglCtx);
              nanoGFX::nGEvent ev(nanoGFX::nGEvent::WINDOW_CREATE);
              sendEvent(ev);
            }
         }
      }
    }
    else {
      ng_err("can't initialize egl!");
    }
  }
  return result;
}
#endif

#ifdef NG_METAL_SUPPORT
nGResult nGSurface::createMetalSurface() {
    nGResult result = RES_FAILED;
    metalView = [[nGMetalView alloc] initWithFrame: [handle frame]];
    if(metalView) {
        metalView.ng_ptr = this;
        [handle setContentView: metalView];
        result = RES_OK;
    }
    return result;
}
#endif /* NG_METAL_SUPPORT */

#ifdef NG_TIMER_SUPPORT
nGResult nGSurface::setTimer(size_t uid, size_t a_interval, nGTimer& timer) {
    long int interval = (long int)a_interval;
    nGResult result = RES_FAILED;
    timer.surface = this;
    timer.uid = uid;
#ifdef __nG_WIN32
    if(SetTimer(handle, WM_USER+uid, interval, NULL)) result = RES_OK;
#endif
#ifdef __nG_X11
    timer_t tid;
    sigevent sig;
    sig.sigev_notify = SIGEV_THREAD;
    sig.sigev_notify_function = timerHandlerS;
    sig.sigev_notify_attributes = NULL;
    sig.sigev_value.sival_ptr = &timer;
    timer_create(CLOCK_REALTIME, &sig, &tid);
    timer.handle = static_cast<void*>(tid);
    itimerspec t  = {{interval/1000, interval*1000}, {interval/1000, interval*1000}};
    timer_settime(tid, 0, &t, 0);
#endif
    return result;
}

nGResult nGSurface::removeTimer(nGTimer& timer) {
    nGResult result = RES_FAILED;
#ifdef __nG_WIN32
    if(KillTimer(handle, WM_USER+timer.uid) ) result = RES_OK;
#endif
#ifdef __nG_X11
    timer_delete(static_cast<timer_t>(timer.handle));
#endif
    return result;
}
#endif //NG_TIMER_SUPPORT

void nGSurface::run() {
#ifdef __nG_OSX
    [getAppHandlePtr() run];
#endif
}


void nGSurface::shutdown() {
#ifdef __nG_OSX
    [getAppHandlePtr() stop:nil];
#endif
}


int nGUtils::getScreenWidth()
{

#ifdef __nG_WIN32
    return GetSystemMetrics(SM_CXSCREEN);
#endif
#ifdef __nG_OSX
    return (int)[[NSScreen mainScreen] frame].size.width;
#endif
    return 0;
}


/*========================================================================*/
int nGUtils::getScreenHeight()
{
#ifdef __nG_WIN32
    return GetSystemMetrics(SM_CYSCREEN);
#endif
#ifdef __nG_OSX
    return (int)[[NSScreen mainScreen] frame].size.height;
#endif
    return 0;
}


}; //namespace nanoGFX



#ifdef __nG_OSX
/*========================================================================
  nGImageView osx 2d surface implementation
  ========================================================================*/

# ifdef NG_2D_SUPPORT
@implementation nGImageView
@synthesize ng_ptr;
@synthesize do_init;
/*
   - (id) initWithFrame: (NSRect)frame {
   do_init =true;
   return self = [super initWithFrame:frame];
   }
 */
-(void) drawRect:(NSRect)dirtyRect {
    if(ng_ptr) {
        if(do_init) {
            nanoGFX::nGEvent ev(nanoGFX::nGEvent::WINDOW_CREATE);
            do_init = false;
            ng_ptr->sendEvent(ev);
        }
        ng_ptr->doRepaint();
    }
}

@end
# endif /*NG_2D_SUPPORT*/

/*========================================================================
  nGIOpenGLView osx 3d surface implementation
  ========================================================================*/
#ifdef NG_GL_SUPPORT
@implementation nGOpenGLView
@synthesize ng_ptr;


static CVReturn displayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    static nanoGFX::nGEvent ev(nanoGFX::nGEvent::WINDOW_REPAINT);
    nGOpenGLView* glView= (nGOpenGLView*)displayLinkContext;
    if(glView) {
        [[glView openGLContext] makeCurrentContext];
        if(glView->ng_ptr) {
            glView->ng_ptr->sendEvent(ev);
        }
        [[glView openGLContext] flushBuffer];
    }
    return kCVReturnSuccess;
}

- (id) initWithFrame: (NSRect)frame
{
    GLuint attribs[] =
    {
        NSOpenGLPFANoRecovery,
        NSOpenGLPFAAccelerated,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAStencilSize, 8,
        NSOpenGLPFAAccumSize, 0,
        0
    } ;
    do_init = true;
    NSOpenGLPixelFormat* fmt = [[NSOpenGLPixelFormat alloc] initWithAttributes: (NSOpenGLPixelFormatAttribute*) attribs];
    if (!fmt) ng_err("No suitable OpenGL pixel format");
    return self = [super initWithFrame:frame pixelFormat: [fmt autorelease]];
}

-(void) dealloc
{
    if(displayLink  && CVDisplayLinkIsRunning(displayLink)) {
        CVDisplayLinkStop(displayLink);
    }
    CVDisplayLinkRelease(displayLink);
    [super dealloc];
}

- (void)prepareOpenGL {
    [super prepareOpenGL];
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
#if 1
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    CVDisplayLinkSetOutputCallback(displayLink, &displayLinkCallback, self);
    CGLContextObj cglContext = [[self openGLContext] CGLContextObj];
    CGLPixelFormatObj cglPixelFormat = [[self pixelFormat] CGLPixelFormatObj];
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
    CVDisplayLinkStart(displayLink);
#endif
    ng_dbg("START");
}

-(void)drawRect:(NSRect)dirtyRect {
    if(!displayLink  || !CVDisplayLinkIsRunning(displayLink)) {
        if(ng_ptr) {
            ng_ptr->doRepaint();
        }
    }
}


-(void)reshape {
    [[self openGLContext] makeCurrentContext];
    [[self openGLContext] update];
    if(ng_ptr) {
        nanoGFX::nGEvent ev(do_init ? nanoGFX::nGEvent::WINDOW_CREATE :nanoGFX::nGEvent::WINDOW_RESIZE);
        do_init = false;
        if(!do_init) {
          ng_ptr->handleResize();
        }
        ng_ptr->sendEvent(ev);
    }
    ng_dbg("reshape %.02f %.02f", [self frame].size.width, [self frame].size.height);
}

-(BOOL)acceptsFirstResponder {
    return YES;
}


@end

#endif /*NG_GL_SUPPORT*/

/*========================================================================
  nGMetalView osx 3d metal surface implementation
  ========================================================================*/
#ifdef NG_METAL_SUPPORT
@implementation nGMetalView
@synthesize ng_ptr;


- (id) initWithFrame: (NSRect)frame
{
    do_init = true;
    device = MTLCreateSystemDefaultDevice();

    return self = [super initWithFrame:frame device: device];
}

- (id) getDevice {
    return device;
}

- (void) dealloc
{
    [device release];
    device = nil;
    if(displayLink  && CVDisplayLinkIsRunning(displayLink)) {
        CVDisplayLinkStop(displayLink);
    }
    CVDisplayLinkRelease(displayLink);
    [super dealloc];
}

-(void)drawRect:(NSRect)dirtyRect {
    if(!displayLink  || !CVDisplayLinkIsRunning(displayLink)) {
        if(ng_ptr) {
            ng_ptr->doRepaint();
        }
    }
}

-(BOOL)acceptsFirstResponder {
    return YES;
}

@end

#endif /*NG_GL_SUPPORT*/

@implementation nGWindow
@synthesize ng_ptr;

- (void)windowDidResignMain:(NSNotification*) notification {
    if(ng_ptr) {
        nanoGFX::nGEvent ev(nanoGFX::nGEvent::FOCUS_OUT);
        ng_ptr->sendEvent(ev);
    }
}

- (void)windowDidResize:(NSNotification*) notification {
    if(ng_ptr) {
        nanoGFX::nGEvent ev(nanoGFX::nGEvent::WINDOW_RESIZE);
        ng_ptr->handleResize();
        ng_ptr->sendEvent(ev);
    }
}

-(void)windowDidBecomeMain:(NSNotification*)notification {
    if(ng_ptr) {
        nanoGFX::nGEvent ev(nanoGFX::nGEvent::FOCUS_IN);
        ng_ptr->sendEvent(ev);
    }
    commandKeyState = false; controlKeyState = false; altKeyState = false; shiftKeyState = false; capsKeyState = false;
}

-(void)windowWillClose:(NSNotification*)notification {
    if(ng_ptr) {
        nanoGFX::nGEvent ev(nanoGFX::nGEvent::WINDOW_DESTROY);
        ng_dbg("TERMINATE!");
        ng_ptr->sendEvent(ev);
    }
}

-(void)keyDown:(NSEvent*)a_Event {
    if(ng_ptr) {

        unichar c = [[a_Event charactersIgnoringModifiers] characterAtIndex:0];
        ng_dbg("keyDown event! : %#X", [a_Event keyCode]);
        if([self isText:c]) {
            nanoGFX::nGEvent ev(nanoGFX::nGEvent::TEXT_INPUT);
            ev.data.key.state = 1;
            ev.data.key.symbol = [self osxKeyTranslate:c];
            ng_ptr->sendEvent(ev);
        }
        nanoGFX::nGEvent ev(nanoGFX::nGEvent::KEY_DOWN);
        ev.data.key.state = 1;
        ev.data.key.symbol = [self osxKeyCodeTranslate:[a_Event keyCode]];
        ng_ptr->sendEvent(ev);
    }
}

-(void)keyUp:(NSEvent*)a_Event {
    if(ng_ptr) {
        ng_dbg("keyUp event! : %#X", [a_Event keyCode]);
        nanoGFX::nGEvent ev(nanoGFX::nGEvent::KEY_UP);
        ev.data.key.state = 0;
        ev.data.key.symbol = [self osxKeyCodeTranslate:[a_Event keyCode]];
        ng_ptr->sendEvent(ev);
    }
}


-(void)sendModifier:(unsigned long)flags in_mask:(NSUInteger)mask in_state:(bool&)state in_symbol:(unsigned int)symbol {
    if(flags & mask) {
        state = true;
        nanoGFX::nGEvent ev(nanoGFX::nGEvent::KEY_DOWN);
        ev.data.key.state = 1;
        ev.data.key.symbol = symbol;
        ng_ptr->sendEvent(ev);
    }
    else if(state) {
        state = false;
        nanoGFX::nGEvent ev(nanoGFX::nGEvent::KEY_UP);
        ev.data.key.state = 0;
        ev.data.key.symbol = symbol;
        ng_ptr->sendEvent(ev);
    }

}

- (void)flagsChanged:(NSEvent *)a_Event {
    if(ng_ptr) {
        unsigned long flags = [a_Event modifierFlags];
        [self sendModifier:flags in_mask:NSEventModifierFlagCommand in_state:commandKeyState in_symbol:nanoGFX::KEYCODE_COMMAND];
        [self sendModifier:flags in_mask:NSEventModifierFlagControl in_state:controlKeyState in_symbol:nanoGFX::KEYCODE_RCTRL];
        [self sendModifier:flags in_mask:NSEventModifierFlagOption in_state:altKeyState in_symbol:nanoGFX::KEYCODE_LALT];
        [self sendModifier:flags in_mask:NSEventModifierFlagShift in_state:shiftKeyState in_symbol:nanoGFX::KEYCODE_LSHIFT];
    }
}

-(void)sendMousePosition:(NSEvent*)a_Event event:(nanoGFX::nGEvent&) ev {
    if(ng_ptr) {
        NSPoint curPoint = [a_Event locationInWindow];
        NSRect screen = [[NSScreen mainScreen] frame];
        //ev.data.mouse.button = [a_Event buttonNumber];
        ev.data.mouse.x = curPoint.x > 0 ? curPoint.x : 0;
        ev.data.mouse.y = curPoint.y > 0 ? ng_ptr->getHeight() - curPoint.y: ng_ptr->getHeight();
#if 0
        NSRect contentRect = [self contentRectForFrameRect: self.frame];
        int dx = contentRect.origin.x - self.frame.origin.x;
        int dy = contentRect.origin.y - self.frame.origin.y + self.frame.size.height - contentRect.size.height;
        ev.data.mouse.x -= dx;
        ev.data.mouse.y -= dy;
#endif

        ev.data.mouse.root_x = [NSEvent mouseLocation].x;
        ev.data.mouse.root_y = screen.size.height - [NSEvent mouseLocation].y;
        ng_ptr->sendEvent(ev);
    }
}


- (void)mouseDown:(NSEvent *)a_Event {
    nanoGFX::nGEvent ev(nanoGFX::nGEvent::MOUSE_BUTTON_DOWN);
    ev.data.mouse.state = 1;
    if ([a_Event type] == NSEventTypeLeftMouseDown) ev.data.mouse.button = nanoGFX::BUTTON_LEFT;
    if ([a_Event type] == NSEventTypeRightMouseDown) ev.data.mouse.button = nanoGFX::BUTTON_RIGHT;
    [self sendMousePosition:a_Event event:ev];
}

- (void)mouseUp:(NSEvent *)a_Event {
    nanoGFX::nGEvent ev(nanoGFX::nGEvent::MOUSE_BUTTON_UP);
    ev.data.mouse.state = 0;
    if ([a_Event type] == NSEventTypeLeftMouseUp) ev.data.mouse.button = nanoGFX::BUTTON_LEFT;
    if ([a_Event type] == NSEventTypeRightMouseUp) ev.data.mouse.button = nanoGFX::BUTTON_RIGHT;
    [self sendMousePosition:a_Event event:ev];
}

- (void)mouseMoved:(NSEvent *)a_Event {
    nanoGFX::nGEvent ev(nanoGFX::nGEvent::MOUSE_MOTION);
    [self sendMousePosition:a_Event event:ev];
}

- (void)mouseDragged:(NSEvent *)a_Event {
    nanoGFX::nGEvent ev(nanoGFX::nGEvent::MOUSE_MOTION);
    [self sendMousePosition:a_Event event:ev];
}

-(void)mouseEntered:(NSEvent*)a_Event {
    nanoGFX::nGEvent ev(nanoGFX::nGEvent::WINDOW_ENTER);
    if(ng_ptr) ng_ptr->sendEvent(ev);
}

-(void)mouseExited:(NSEvent*)a_Event {
    nanoGFX::nGEvent ev(nanoGFX::nGEvent::WINDOW_LEAVE);
    if(ng_ptr) ng_ptr->sendEvent(ev);
}

- (BOOL)canBecomeKeyWindow
{
    return YES;
}

- (BOOL)canBecomeMainWindow
{
    return YES;
}

-(bool) isText:(unsigned int)key {
    switch(key) {
        case NSUpArrowFunctionKey:
        case NSDownArrowFunctionKey:
        case NSLeftArrowFunctionKey:
        case NSRightArrowFunctionKey:
        case NSHomeFunctionKey:
        case NSEndFunctionKey:
        case NSPageUpFunctionKey:
        case NSPageDownFunctionKey:
        case NSDeleteFunctionKey:
            return false;
        default:
            return true;
    }
}

-(unsigned int) osxKeyTranslate:(unsigned int)key {
    switch(key) {
        case NSUpArrowFunctionKey: return nanoGFX::KEYCODE_UP;
        case NSDownArrowFunctionKey: return nanoGFX::KEYCODE_DOWN;
        case NSLeftArrowFunctionKey: return nanoGFX::KEYCODE_LEFT;
        case NSRightArrowFunctionKey: return nanoGFX::KEYCODE_RIGHT;
        case NSHomeFunctionKey: return nanoGFX::KEYCODE_HOME;
        case NSEndFunctionKey: return nanoGFX::KEYCODE_END;
        case NSPageUpFunctionKey: return nanoGFX::KEYCODE_PAGEUP;
        case NSPageDownFunctionKey: return nanoGFX::KEYCODE_PAGEDOWN;
        case NSDeleteFunctionKey: return nanoGFX::KEYCODE_BACKSPACE;
        default:
                                  return key;
    }
    return key;
}

-(unsigned int) osxKeyCodeTranslate:(unsigned int)key {
    const unsigned int alphaNum[] = {0,11,8,2,14,3,5,4,34,38,40,37,46,45,31,35,12,15,1,17,32,9,13,7,16,6};
    if(key >= 18 && key <= 28) return nanoGFX::KEYCODE_1 + key -  18;
    if(key == 29) return nanoGFX::KEYCODE_0;
    if(key<=46) {
        for(unsigned int i = 0 ; i < sizeof(alphaNum); ++i) { if(alphaNum[i] == key) {return nanoGFX::KEYCODE_a + i;}};
    }
    switch(key) {
        case /*kVK_Return */ 0x24: return 0xa;
        case /*kVK_Tab */ 0x30: return 0x9;
        case /*kVK_Space */0x31 : return nanoGFX::KEYCODE_SPACE;
        case /*kVK_Delete  */ 0x33: return nanoGFX::KEYCODE_BACKSPACE;
        case /*kVK_Escape  */ 0x35: return nanoGFX::KEYCODE_ESCAPE;
        case /*kVK_Command*/0x37: return nanoGFX::KEYCODE_COMMAND;
        case /*kVK_Shift   */ 0x38: return nanoGFX::KEYCODE_RSHIFT;
                                    //kVK_CapsLock                  = 0x39,
        case /*kVK_Option  */ 0x3A: return nanoGFX::KEYCODE_LALT;
        case /*kVK_Control  */ 0x3B: return nanoGFX::KEYCODE_LCTRL;
        case /*kVK_RightShift  */ 0x3C: return nanoGFX::KEYCODE_RSHIFT;
        case /*kVK_RightOption   */ 0x3D: return nanoGFX::KEYCODE_RALT;
        case /*kVK_RightControl   */ 0x3E: return nanoGFX::KEYCODE_RCTRL;
                                           /*
                                              kVK_Function                  = 0x3F,
                                              kVK_F17                       = 0x40,
                                              kVK_VolumeUp                  = 0x48,
                                              kVK_VolumeDown                = 0x49,
                                              kVK_Mute                      = 0x4A,
                                              kVK_F18                       = 0x4F,
                                              kVK_F19                       = 0x50,
                                              kVK_F20                       = 0x5A,
                                              kVK_F5                        = 0x60,
                                              kVK_F6                        = 0x61,
                                              kVK_F7                        = 0x62,
                                              kVK_F3                        = 0x63,
                                              kVK_F8                        = 0x64,
                                              kVK_F9                        = 0x65,
                                              kVK_F11                       = 0x67,
                                              kVK_F13                       = 0x69,
                                              kVK_F16                       = 0x6A,
                                              kVK_F14                       = 0x6B,
                                              kVK_F10                       = 0x6D,
                                              kVK_F12                       = 0x6F,
                                              kVK_F15                       = 0x71,
                                              kVK_Help                      = 0x72,
                                            */
        case /*kVK_Home  */ 0x73: return nanoGFX::KEYCODE_HOME;
        case /*kVK_PageUp  */ 0x74: return nanoGFX::KEYCODE_PAGEUP;
        case /*kVK_ForwardDelete */ 0x75: return nanoGFX::KEYCODE_DELETE;
                                          //kVK_F4                        = 0x76,
        case /*kVK_End */ 0x77: return nanoGFX::KEYCODE_END;
                                //kVK_F2                        = 0x78,
        case /*kVK_PageDown   */ 0x79: return nanoGFX::KEYCODE_PAGEDOWN;
                                       //kVK_F1                        = 0x7A:
        case /*kVK_LeftArrow   */ 0x7B: return nanoGFX::KEYCODE_LEFT;
        case /*kVK_RightArrow  */ 0x7C: return nanoGFX::KEYCODE_RIGHT;
        case /*kVK_DownArrow  */0x7D: return nanoGFX::KEYCODE_DOWN;
        case /*kVK_UpArrow  */0x7E: return nanoGFX::KEYCODE_UP;
        default: break;
    }
    return key;
}


@end

@implementation nGApplication

-(void)sendEvent:(NSEvent*)a_Event {
    bool result = false;
    if (([a_Event type] == NSEventTypeKeyUp ||[a_Event type] == NSEventTypeKeyDown) && [a_Event window]) {
        [[a_Event window] sendEvent:a_Event];
        result = true;
    }
    if(result == false)
        [super sendEvent:a_Event];
}

@end

#endif //__nG_OSX

#endif /* NG_IMPLEMENT */

#endif /*DEF_NANOGFX_H*/
