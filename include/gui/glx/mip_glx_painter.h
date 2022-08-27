#ifndef mip_glx_painter_included
#define mip_glx_painter_included
//----------------------------------------------------------------------

// aka: opengl context wrapper (for pixmap/window)

/*
  https://stackoverflow.com/questions/68620893/glfw-causes-memory-leak
  https://github.com/godotengine/godot/issues/27265
  how can i suppress asan warnings?
*/

//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_drawable.h"
#include "gui/base/mip_base_painter.h"
#include "gui/glx/mip_glx.h"
#include "gui/glx/mip_glx_utils.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define MIP_OPENGL_MAJOR 3
#define MIP_OPENGL_MINOR 2

//----------

GLint MIP_GlxPixmapAttribs[] = {
  GLX_X_RENDERABLE,   True,
  GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
  GLX_DRAWABLE_TYPE,  GLX_PIXMAP_BIT,
  GLX_RENDER_TYPE,    GLX_RGBA_BIT,
  GLX_BUFFER_SIZE,    24,
  GLX_DOUBLEBUFFER,   False, // true = error (pixmap has no double buffer?)
  GLX_RED_SIZE,       8,
  GLX_GREEN_SIZE,     8,
  GLX_BLUE_SIZE,      8,
  GLX_ALPHA_SIZE,     8,  // window can't have alpha
  GLX_STENCIL_SIZE,   8,  // nanovg needs stencil?
  //GLX_DEPTH_SIZE,     24,
  //GLX_SAMPLE_BUFFERS, True,
  //GLX_SAMPLES,        2,
  //GLX_Y_INVERTED_EXT, True,
  None
};

//----------

GLint MIP_GlxWindowAttribs[] = {
  GLX_X_RENDERABLE,   True,
  GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
  GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
  GLX_RENDER_TYPE,    GLX_RGBA_BIT,
  GLX_BUFFER_SIZE,    24,
  GLX_DOUBLEBUFFER,   True,
  GLX_RED_SIZE,       8,
  GLX_GREEN_SIZE,     8,
  GLX_BLUE_SIZE,      8,
  GLX_ALPHA_SIZE,     0,  // 0 window can't have alpha
  GLX_STENCIL_SIZE,   8,  // nanovg needs stencil?
  //GLX_DEPTH_SIZE,     24,
  //GLX_SAMPLE_BUFFERS, True,
  //GLX_SAMPLES,        2,
  //GLX_Y_INVERTED_EXT, True,
  None
};

//----------

//static int MIP_GlxContextAttribs[] = {
//  GLX_CONTEXT_MAJOR_VERSION_ARB,  MIP_OPENGL_MAJOR,
//  GLX_CONTEXT_MINOR_VERSION_ARB,  MIP_OPENGL_MINOR,
//  GLX_CONTEXT_PROFILE_MASK_ARB,   GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
//  None
//};


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_GlxPainter
: public MIP_BasePainter {

//------------------------------
private:
//------------------------------

  Display*    MDisplay          = None;

  GLXContext  MContext          = nullptr;
  GLXContext  MPrevContext      = nullptr;

  GLXFBConfig MFBConfig         = nullptr;
  uint32_t    MWidth            = 0;
  uint32_t    MHeight           = 0;
  GLXDrawable MDrawable         = GLX_NONE;
  bool        MDrawableIsWindow = false;
  bool        MIsPainting       = false;

//------------------------------
public:
//------------------------------

  // ASurface = pixmap
  // ATarget  = window

  MIP_GlxPainter(MIP_Drawable* ASurface, MIP_Drawable* ATarget)
  : MIP_BasePainter(ASurface,ATarget) {

    old_x_error_handler = XSetErrorHandler(x_error_handler);

    MWidth = ATarget->drawable_getWidth();
    MHeight = ATarget->drawable_getHeight();
    MDisplay = ASurface->drawable_getXlibDisplay();
    if (ATarget->drawable_isWindow()) {
      MIP_Print("window\n");
      MFBConfig = findFBConfig(MIP_GlxWindowAttribs);
      MContext = createContext(MFBConfig);
      xcb_window_t window = ATarget->drawable_getXcbWindow();
      MDrawable = glXCreateWindow(MDisplay,MFBConfig,window,nullptr);
      MDrawableIsWindow = true;
    }
    else { // todo
      MIP_Print("pixmap\n");
      MFBConfig = findFBConfig(MIP_GlxPixmapAttribs); // crash..
      MIP_Print("1\n");
      MContext = createContext(MFBConfig);
      MIP_Print("2\n");
      xcb_pixmap_t pixmap = ATarget->drawable_getXcbPixmap();
      MIP_Print("3\n");
      MDrawable = glXCreatePixmap(MDisplay,MFBConfig,pixmap,nullptr);
      MIP_Print("4\n");
      MDrawableIsWindow = false;
      MIP_Print("5\n");
    }
    MIP_GLXDisableVSync(MDisplay,MDrawable);
    //resetCurrent();
  }

  //----------

  virtual ~MIP_GlxPainter() {
    //MIP_PRINT;
    if (MDrawableIsWindow) {
      glXDestroyWindow(MDisplay,MDrawable);
    }
    else {
      glXDestroyPixmap(MDisplay,MDrawable);
    }
    destroyContext();
    XSetErrorHandler(old_x_error_handler);
  }

//------------------------------
public:
//------------------------------

//  void select() override {
//    makeCurrent();
//  }
//
//  void deselect() override {
//    resetCurrent();
//  }

  void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    makeCurrent();
    glViewport(0,0,AWidth,AHeight);
  }

  void endPaint() override {
    swapBuffers();
    resetCurrent();
  }

//------------------------------
protected:
//------------------------------

  // ADrawable: GLXPixmap, GLXWindow

  //TOSO: save prev context?

  /*
    glXMakeContextCurrent binds ctx to the current rendering thread and to the
    draw and read GLX drawables. draw and read may be the same.
    draw is used for all OpenGL operations except:
    Any pixel data that are read based on the value of GLX_READ_BUFFER.
    Note that accumulation operations use the value of GLX_READ_BUFFER,
    but are not allowed unless draw is identical to read.
    Any depth values that are retrieved by glReadPixels or glCopyPixels.
    Any stencil values that are retrieved by glReadPixels or glCopyPixels.
    Frame buffer values are taken from draw.
    If the current rendering thread has a current rendering context,
    that context is flushed and replaced by ctx.
    The first time that ctx is made current, the viewport and scissor
    dimensions are set to the size of the draw drawable. The viewport and
    scissor are not modified when ctx is subsequently made current.
    To release the current context without assigning a new one, call
    glXMakeContextCurrent with draw and read set to None and ctx set to NULL.
    glXMakeContextCurrent returns True if it is successful, False otherwise.
    If False is returned, the previously current rendering context and drawable
    (if any) remain unchanged.
  */

  bool makeCurrent() {
    //MIP_PRINT;
    //MPrevContext = glXGetCurrentContext();
    bool res = glXMakeContextCurrent(MDisplay,MDrawable,MDrawable,MContext);
    if (!res) {
      MIP_Print("Error: makeCurrent returned false\n");
      return false;
    }
    return true;
  }

  //----------

  /*
    you have to make it UN-current in one thread
    before you can make it current in another..
  */

  //TOSO: restore prev context?

  bool resetCurrent() {
    //MIP_PRINT;
    bool res = glXMakeContextCurrent(MDisplay,0,0,0);
    //bool res = glXMakeContextCurrent(MDisplay,MDrawable,MDrawable,MPrevContext); // error
    if (!res) {
      MIP_Print("Error: makeCurrent returned false\n");
      return false;
    }
    return true;
  }

  //----------

  void swapBuffers() {
    //MIP_PRINT;
    glXSwapBuffers(MDisplay,MDrawable);
  }

//------------------------------
private:
//------------------------------

  // ADisplay : X11 Display*
  // AAttribs : MIP_GlxPixmapAttribs or MIP_GlxWindowAttribs
  // Use XFree to free the memory returned by glXChooseFBConfig.

  GLXFBConfig findFBConfig(const int* AAttribs) {
    //MIP_PRINT;
    //MDisplay = ADisplay;
    int num_fbc = 0;
    MIP_Print("Display: %p\n",MDisplay);
    MIP_Print("Screen: %i\n",DefaultScreen(MDisplay));
    MIP_Print("AAttribs: %p\n",AAttribs);
    GLXFBConfig* fbconfigs = glXChooseFBConfig(MDisplay,DefaultScreen(MDisplay),AAttribs,&num_fbc);
    MIP_Print("num_fbc: %i\n",num_fbc);
    GLXFBConfig fbconfig = fbconfigs[0];
    XFree(fbconfigs);
    return fbconfig;
  }

  //----------

  //glXCreateContextAttribsARBFUNC glXCreateContextAttribsARB = (glXCreateContextAttribsARBFUNC)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
  //MIP_Assert(glXCreateContextAttribsARB);
  //MContext = glXCreateContextAttribsARB(MDisplay,MFBConfig,nullptr,True,MIP_GlxContextAttribs);
  //    MContext = glXCreateNewContext(MDisplay,MFBConfig,GLX_RGBA_TYPE,nullptr,True);
  //    loadOpenGL();

  GLXContext createContext(GLXFBConfig fbconfig) {
    //MIP_PRINT;
    GLXContext context = glXCreateNewContext(MDisplay,fbconfig,GLX_RGBA_TYPE,nullptr,True);
    MIP_Assert(context);
    loadOpenGL();
    return context;
  }

  //----------

  void destroyContext() {
    //MIP_PRINT;
    glXDestroyContext(MDisplay,MContext);
    unloadOpenGL();
  }

  //----------

  // make context current before calling this
  // should this be done per window/context, or once per program/library?

  bool loadOpenGL() {
    //MIP_Print("calling sogl_loadOpenGL\n");
    if (!sogl_loadOpenGL()) {
      MIP_Print("Error: sogl_loadOpenGL:\n");
      const char** failures = sogl_getFailures();
      while (*failures) {
        MIP_DPrint("  %s\n",*failures);
        failures++;
      }
      return false;
    }
    return true;
  }

  //----------

  void unloadOpenGL() {
    sogl_cleanup();
  }

  //----------

  bool getGlxVersion(int* AMajor, int* AMinor) {
    //MIP_PRINT;
    bool res = glXQueryVersion(MDisplay,AMajor,AMinor);
    if (!res) {
      MIP_Print("Error: getGlxVersion returned false\n");
      return false;
    }
    return true;
  }

  //----------

  // FBConfigs were added in GLX version 1.3.

  /*
  bool checkVersion(Display* display, int major, int minor) {
    int glx_major, glx_minor;
    glXQueryVersion(display,&glx_major,&glx_minor);
    if (glx_major < major) return false;
    if (glx_minor < minor) return false;
    return true;
  }
  */

  //----------

  Display* getCurrentDisplay() {
    //MIP_PRINT;
    return glXGetCurrentDisplay();
  }

  //----------

  // If there is no current context, NULL is returned.

  GLXContext getCurrentContext() {
    //MIP_PRINT;
    return glXGetCurrentContext();
  }

  // get the XID of the current drawable used for rendering, call
  // If there is no current draw drawable, None is returned.

  GLXDrawable getCurrentDrawable() {
    //MIP_PRINT;
    return glXGetCurrentDrawable();
  }

  //----------

  GLXDrawable getCurrentReadDrawable() {
    //MIP_PRINT;
    return glXGetCurrentReadDrawable();
  }

//------------------------------
private:
//------------------------------

  //static
  int (*old_x_error_handler)(Display*,XErrorEvent*);

  //----------

  static
  int x_error_handler(Display* d, XErrorEvent* e) {
    char error_text[512] = {0};
    XGetErrorText(d,e->error_code,error_text,511);
    MIP_DPrint("X Error %i: %s\n",e->error_code,error_text);
    return 0;
  }

  //----------

  //static void glx_error_message(const char *format, ...) {
  //  va_list args;
  //  va_start(args, format);
  //  fprintf(stderr, "glx error: ");
  //  vfprintf(stderr, format, args);
  //  va_end(args);
  //}

};

//----------------------------------------------------------------------
#endif

/*

  GLX functions should not be called between glBegin and glEnd operations. If a
  GLX function is called within a glBegin/glEnd pair, then the result is undefined;
  however, no error is reported.

  https://stackoverflow.com/questions/47918078/creating-opengl-structures-in-a-multithreaded-program
  The requirement for OpenGL is that the context created for rendering should
  be owned by single thread at any given point and the thread that owns context
  should make it current and then call any gl related function. If you do that
  without owning and making context current then you get segmentation faults.
  By default the context will be current for the main thread.

  .. other option is to make one context per thread and make it current when
  thread starts.
*/

//----------

/*
  The glXMakeCurrent subroutine does two things: (1) it makes the specified
  Context parameter the current GLX rendering context of the calling thread,
  replacing the previously current context if one exists, and (2) it attaches
  Context to a GLX drawable (either a window or GLX pixmap). As a result of
  these two actions, subsequent OpenGL rendering calls use Context as a
  rendering context to modify the Drawable GLX drawable. Since the
  glXMakeCurrent subroutine always replaces the current rendering context
  with the specified Context, there can be only one current context per
  thread.

  Pending commands to the previous context, if any, are flushed before it is
  released.

  The first time Context is made current to any thread, its viewport is set
  to the full size of Drawable. Subsequent calls by any thread to the
  glXMakeCurrent subroutine using Context have no effect on its viewport.

  To release the current context without assigning a new one, call the
  glXMakeCurrent subroutine with the Drawable and Context parameters set to
  None and Null, respectively.

  The glXMakeCurrent subroutine returns True if it is successful, False
  otherwise. If False is returned, the previously current rendering context
  and drawable (if any) remain unchanged.

  BadMatch is generated if draw and read are not compatible.
  BadAccess is generated if ctx is current to some other thread.
  GLXContextState is generated if there is a current rendering context and its render mode is either GLX_FEEDBACK or GLX_SELECT.
  GLXBadContext is generated if ctx is not a valid GLX rendering context.
  GLXBadDrawable is generated if draw or read is not a valid GLX drawable.
  GLXBadWindow is generated if the underlying X window for either draw or read is no longer valid.
  GLXBadCurrentDrawable is generated if the previous context of the calling thread has unflushed commands and the previous drawable is no longer valid.
  BadAlloc is generated if the X server does not have enough resources to allocate the buffers.
  BadMatch is generated if:
    draw and read cannot fit into frame buffer memory simultaneously.
    draw or read is a GLXPixmap and ctx is a direct-rendering context.
    draw or read is a GLXPixmap and ctx was previously bound to a GLXWindow or GLXPbuffer.
    draw or read is a GLXWindow or GLXPbuffer and ctx was previously bound to a GLXPixmap.
*/

//----------

/*
  glXCreatePixmap creates an off-screen rendering area and returns its XID.
  Any GLX rendering context that was created with respect to config can be
  used to render into this window. Use glXMakeCurrent to associate the
  rendering area with a GLX rendering context.

  BadMatch is generated if pixmap was not created with a visual that corresponds to config.
  BadMatch is generated if config does not support rendering to windows (e.g., GLX_DRAWABLE_TYPE does not contain GLX_WINDOW_BIT).
  BadWindow is generated if pixmap is not a valid window XID. BadAlloc is generated if there is already a GLXFBConfig associated with pixmap.
  BadAlloc is generated if the X server cannot allocate a new GLX window.
  GLXBadFBConfig is generated if config is not a valid GLXFBConfig.
*/

//----------

/*
  glXCreateWindow creates an on-screen rendering area from an existing X
  window that was created with a visual matching config. The XID of the
  GLXWindow is returned. Any GLX rendering context that was created with
  respect to config can be used to render into this window. Use
  glXMakeContextCurrent to associate the rendering area with a GLX rendering
  context.

  BadMatch is generated if win was not created with a visual that corresponds to config.
  BadMatch is generated if config does not support rendering to windows (i.e., GLX_DRAWABLE_TYPE does not contain GLX_WINDOW_BIT).
  BadWindow is generated if win is not a valid pixmap XID.
  BadAlloc is generated if there is already a GLXFBConfig associated with win.
  BadAlloc is generated if the X server cannot allocate a new GLX window.
  GLXBadFBConfig is generated if config is not a valid GLXFBConfig.
*/

//----------




