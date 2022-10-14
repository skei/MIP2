#ifndef mip_wgl_painter_included
#define mip_wgl_painter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_drawable.h"
#include "gui/base/mip_base_painter.h"
#include "gui/wgl/mip_wgl.h"
#include "gui/wgl/mip_wgl_utils.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_WglPainter
: public MIP_BasePainter {

//------------------------------
private:
//------------------------------

  HDC     MDc   = nullptr;
  HGLRC   MGlRc = nullptr;

  MIP_Drawable* MSurface = nullptr;
  MIP_Drawable* MTarget = nullptr;
//------------------------------
public:
//------------------------------

  // ASurface = pixmap
  // ATarget  = window

  MIP_WglPainter(MIP_Drawable* ASurface, MIP_Drawable* ATarget)
  : MIP_BasePainter(ASurface,ATarget) {

    MSurface = ASurface;
    MTarget = ATarget;

    // The GetDC function retrieves a handle to a device context (DC) for the
    // client area of a specified window or for the entire screen
    // If hwnd argument is NULL, GetDC retrieves the DC for the entire screen.

    HDC tempdc = GetDC(0);
    if (!tempdc) MIP_Win32PrintError("GetDC");

    // The CreateCompatibleDC function creates a memory device context (DC)
    // compatible with the specified device.

    MDc = CreateCompatibleDC(tempdc);
    if (!MDc) MIP_Win32PrintError("CreateCompatibleDC");

    // The ReleaseDC function releases a device context (DC), freeing it for
    // use by other applications.
    // The effect of the ReleaseDC function depends on the type of DC. It frees
    // only common and window DCs. It has no effect on class or private DCs.

    ReleaseDC(0,tempdc);

    // The wglCreateContext function creates a new OpenGL rendering context,
    // which is suitable for drawing on the device referenced by hdc. The
    // rendering context has the same pixel format as the device context.

    MIP_Print("Creating opengl context\n");

    //MGlRc = wglCreateContext(MDc);
    //if (!MGlRc) MIP_Win32PrintError("wglCreateContext");

    //makeCurrent();
    //loadOpenGL();

    MGlRc = MIP_WglInitContext(MDc);
    if (!MGlRc) MIP_Win32PrintError("init_gl_ctx");

    //wglDeleteContext(MGlRc);
    //ReleaseDC(0,MDc);
    //resetCurrent();

    MIP_PRINT;

  }

  //----------

  virtual ~MIP_WglPainter() {
    ReleaseDC(0,MDc);
    wglDeleteContext(MGlRc);
//    unloadOpenGL();
    MIP_Print("\n");
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
    MIP_Print("%i,%i - %i,%i\n",AXpos,AYpos,AWidth,AHeight);

    //    MDc = MTarget->drawable_getDC();
    //    MIP_Assert(MDc);
    //    MGlRc = wglCreateContext(MDc);

    makeCurrent();
    //loadOpenGL();
    glViewport(0,0,AWidth,AHeight);
    //MIP_Print("\n");
  }

  //----------

  void endPaint() override {
    //MIP_PRINT;
    swapBuffers();
    resetCurrent();
    //unloadOpenGL();
//    wglDeleteContext(MGlRc);
//    ReleaseDC(0,MDc);
    //MDc = nullptr;
    //MGlRc = nullptr;
    MIP_Print("\n");
  }

  //----------

//  virtual void pushClip(MIP_DRect ARect) {
//  }

  //----------

//  virtual void pushOverlapClip(MIP_DRect ARect) {
//  }

//------------------------------
public:
//------------------------------

  /*
    The wglMakeCurrent function makes a specified OpenGL rendering context the
    calling thread's current rendering context. All subsequent OpenGL calls
    made by the thread are drawn on the device identified by hdc. You can also
    use wglMakeCurrent to change the calling thread's current rendering context
    so it's no longer current.

    If hglrc is NULL, the function makes the calling thread's current rendering
    context no longer current, and releases the device context that is used by
    the rendering context. In this case, hdc is ignored.
  */

  bool makeCurrent() {
    MIP_Print("\n");
    wglMakeCurrent(MDc, MGlRc);
    return true;
  }

  //----------

  bool resetCurrent() {
    MIP_Print("\n");
    wglMakeCurrent(nullptr,nullptr);
    return true;
  }

  //----------

  /*
    The SwapBuffers function exchanges the front and back buffers if the
    current pixel format for the window referenced by the specified device
    context includes a back buffer.
  */

  void swapBuffers() {
    MIP_Print("\n");
    SwapBuffers(MDc);
  }

  //----------

//  bool loadOpenGL() {
//    MIP_Print("calling sogl_loadOpenGL\n");
//    if (!sogl_loadOpenGL()) {
//      MIP_Print("Error: sogl_loadOpenGL returns false\n");
//
//      MIP_Print("couldn't load: ");
//      const char** failures = sogl_getFailures();
//      while (*failures) {
//        MIP_DPrint("%s ",*failures);
//        failures++;
//      }
//      MIP_DPrint("\n");
//
//      exit(1);
//      //return false;
//    }
//    return true;
//  }
//
//  //----------
//
//  void unloadOpenGL() {
//    sogl_cleanup();
//  }

};

//----------------------------------------------------------------------
#endif
