#ifndef mip_opengl_window_included
#define mip_opengl_window_included
//----------------------------------------------------------------------

#include "mip.h"

#include "gui/opengl/mip_opengl.h"
#include "gui/xlib/mip_xlib.h"
#include "gui/xcb/mip_xcb.h"
#include "gui/xcb/mip_xcb_window.h"

//----------------------------------------------------------------------

#include <GL/glx.h>

typedef GLXContext (*glXCreateContextAttribsARBFUNC)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

//----------------------------------------------------------------------

class MIP_OpenGLWindow
: public MIP_XcbWindow {

//------------------------------
protected:
//------------------------------

  GLXContext  MGlxContext = nullptr;

//------------------------------
public:
//------------------------------

  MIP_OpenGLWindow(uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false)
  : MIP_XcbWindow(AWidth,AHeight,AEmbedded) {
    initOpenGL();
  }

  //----------

  virtual ~MIP_OpenGLWindow() {
    cleanupOpenGL();
  }

//------------------------------
protected:
//------------------------------

  bool initOpenGL() {
    GLint visualAtt[] = {
      GLX_RENDER_TYPE,    GLX_RGBA_BIT,
      GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
      GLX_DOUBLEBUFFER,   True,
      GLX_RED_SIZE,       8,  // 1,
      GLX_GREEN_SIZE,     8,  // 1,
      GLX_BLUE_SIZE,      8,  // 1,
      GLX_DEPTH_SIZE,     24, // 1,
      GLX_STENCIL_SIZE,   8,  // 1,
      None
    };
    int numFBC = 0;
    GLXFBConfig* fbc = glXChooseFBConfig(MDisplay,DefaultScreen(MDisplay),visualAtt,&numFBC);
    if (!fbc) {
      MIP_Print("Unable to get framebuffer\n");
      return false;
    }
    glXCreateContextAttribsARBFUNC glXCreateContextAttribsARB = (glXCreateContextAttribsARBFUNC)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
    if (!glXCreateContextAttribsARB) {
      MIP_Print("Unable to get proc glXCreateContextAttribsARB\n");
      XFree(fbc);
      return false;
    }
    static int contextAttribs[] = {
      GLX_CONTEXT_MAJOR_VERSION_ARB,  MIP_OPENGL_MAJOR, //4,
      GLX_CONTEXT_MINOR_VERSION_ARB,  MIP_OPENGL_MINOR, //5,
      GLX_CONTEXT_PROFILE_MASK_ARB,   GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
      None
    };
    MGlxContext = glXCreateContextAttribsARB(MDisplay,*fbc,nullptr,True,contextAttribs);
    XFree(fbc);
    if (!MGlxContext) {
      MIP_Print("Unable to create OpenGL context\n");
      return false;
    }
    glXMakeCurrent(MDisplay,MWindow,MGlxContext);
    if (!sogl_loadOpenGL()) {
      const char** failures = sogl_getFailures();
      while (*failures) {
        MIP_Print("sogl_loadOpenGL failed: %s\n", *failures);
        failures++;
      }
    }
    return true;
  }

  //----------

  void cleanupOpenGL() {
  }

//------------------------------
public:
//------------------------------

  void makeCurrent() {
    glXMakeCurrent(MDisplay,MWindow,MGlxContext);
  }

  //----------

  void swapBuffers() {
    glXSwapBuffers(MDisplay,MWindow);
  }


};

//----------------------------------------------------------------------
#endif
