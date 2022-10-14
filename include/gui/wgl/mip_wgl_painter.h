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

#define MIP_OPENGL_MAJOR 3
#define MIP_OPENGL_MINOR 2

//----------

//PIXELFORMATDESCRIPTOR MIP_WglSurfacePFD = {
//};

//----------

PIXELFORMATDESCRIPTOR MIP_WglWindowPFD = {
  sizeof(MIP_WglWindowPFD),
  1,
  PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
  PFD_TYPE_RGBA,
  32,               // bits
  0, 0, 0, 0, 0, 0,
  8,//0,            // alpha
  0,                // shift
  0,                // accumulation
  0, 0, 0, 0,
  24,               // depth
  8,//0,            // stencil
  0,                // aux
  PFD_MAIN_PLANE,
  0, 0, 0, 0
};

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
  HGLRC   MGlrc = nullptr;

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

    // pixel format

    if (MTarget->drawable_isWindow()) {
      HWND hwnd = MTarget->drawable_getWin32Hwnd();
      MDc = GetDC(hwnd);
      if (!MDc) MIP_Win32PrintError("GetDC");
      int pf = ChoosePixelFormat(MDc, &MIP_WglWindowPFD);
      SetPixelFormat(MDc, pf, &MIP_WglWindowPFD);
    }
    else {
      HDC tempdc = GetDC(0);
      MDc = CreateCompatibleDC(tempdc);
      ReleaseDC(0,tempdc);
      //int pf = ChoosePixelFormat(hdc, &MIP_WglSurfacePFD);
      //SetPixelFormat(hdc, pf, &MIP_WglSurfacePFD);
    }

    // temp context

    HGLRC temp_ctx = wglCreateContext(MDc);
    if (temp_ctx) {
      MIP_Print("created temp context\n");
      wglMakeCurrent(MDc,temp_ctx);
    }
    else {
      MIP_Print("error! couldn't create temp conext\n");
      exit(1);
    }

    // extension string

    /*
    const char* get_extensions();
    PFNWGLGETEXTENSIONSSTRINGEXTPROC wgl_GetExtensionsStringEXT;
    wgl_GetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
    if (!wgl_GetExtensionsStringEXT) {
      MIP_Print("error! wglGetExtensionsStringEXT not found\n");
      exit(1);
    }
    else {
      MIP_Print("wglGetExtensionsStringEXT found\n");
      const char* extensions = wgl_GetExtensionsStringEXT();
      if (strstr(extensions, "WGL_ARB_create_context")) {
        //ext->create_context_attribs = 1;
      }
      if (strstr(extensions, "WGL_EXT_swap_control")) {
        //ext->swap_control = 1;
        if (strstr(extensions, "WGL_EXT_swap_control_tear")) {
          //ext->swap_control_tear = 1;
        }
      }
      if (strstr(extensions, "NV")) {
        //ext->appears_to_be_nvidia = 1;
      }
    }
    //MIP_Assert( wgl_GetExtensionsStringEXT );
    */

    // load opengl functions

    if (!sogl_loadOpenGL()) {
      MIP_Print("error! sogl_loadOpenGL failures: \n");
      const char** failures = sogl_getFailures();
      while (*failures) {
        MIP_DPrint("%s ",*failures);
        failures++;
      }
      MIP_DPrint("\n");
      wglMakeCurrent(MDc, NULL);
      wglDeleteContext(temp_ctx);
      exit(1);
    }
    else {
      MIP_Print("loaded opengl functions (sogl_loadOpenGL)\n");
    }

    // check version

    int maj, min;
    glGetIntegerv(GL_MAJOR_VERSION, &maj);
    glGetIntegerv(GL_MINOR_VERSION, &min);
    MIP_Print("GL_MAJOR_VERSION: %i GL_MINOR_VERSION: %i\n",maj,min);   // GL_MAJOR_VERSION: 3 GL_MINOR_VERSION: 1

    MIP_Print("GL_VERSION: %s\n",     (char*)glGetString(GL_VERSION));    // 3.1 Mesa 21.2.6
    MIP_Print("GL_VENDOR: %s\n",      (char*)glGetString(GL_VENDOR));     // Mesa/X.org
    MIP_Print("GL_RENDERER: %s\n",    (char*)glGetString(GL_RENDERER));   // llvmpipe (LLVM 12.0.0, 256 bits)
    //MIP_Print("GL_EXTENSIONS: %s\n",  (char*)glGetString(GL_EXTENSIONS)); // crashes!

    // create context ARB

    PFNWGLCREATECONTEXTATTRIBSARBPROC wgl_CreateContextAttribsARB;
    wgl_CreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if (!wgl_CreateContextAttribsARB) {
      MIP_Print("error! wglCreateContextAttribsARB not found\n");
      exit(1);
    }
    else {
      MIP_Print("wglCreateContextAttribsARBfound\n");
    }

    const int ctx_attribs[] = {
      WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
      WGL_CONTEXT_MINOR_VERSION_ARB, 2,
      WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
      0
    };

    MGlrc = wgl_CreateContextAttribsARB(MDc, NULL, ctx_attribs);
    if (!MGlrc) {
      MIP_Print("error creating context\n");
      wglMakeCurrent(MDc, NULL);
      wglDeleteContext(temp_ctx);
      exit(1);
    }
    else {
      MIP_Print("context created\n");
    }

    //----------

    wglMakeCurrent(MDc,NULL);
    wglDeleteContext(temp_ctx);
    wglMakeCurrent(MDc, MGlrc);

  }

  //----------

  virtual ~MIP_WglPainter() {
    ReleaseDC(0,MDc);
    wglDeleteContext(MGlrc);
    //unloadOpenGL();
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
    wglMakeCurrent(MDc, MGlrc);
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

//------------------------------
private:
//------------------------------

};

//----------------------------------------------------------------------
#endif



    //HMODULE gl_module = LoadLibrary(TEXT("opengl32.dll"));
    //wgl_GetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)GetProcAddress(gl_module,"wglGetExtensionsStringEXT");
    //FreeLibrary(gl_module);

