#ifndef mip_opengl_painter_included
#define mip_opengl_painter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/opengl/mip_opengl.h"
#include "gui/opengl/mip_opengl_utils.h"
#include "gui/mip_paint_target.h"

//----------------------------------------------------------------------

typedef GLXContext (*glXCreateContextAttribsARBFUNC)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_OpenGLPainter {

//------------------------------
private:
//------------------------------

  MIP_PaintTarget*  MTarget     = nullptr;
  GLXContext        MContext    = nullptr;

  Display*          MDisplay    = nullptr;
  GLXFBConfig       MFBConfig   = nullptr;
  XVisualInfo*      MVisualInfo = nullptr;

  GLXPixmap         MPixmap     = GLX_NONE;
  //GLXWindow         MWindow     = GLX_NONE;

//------------------------------
public:
//------------------------------

  MIP_OpenGLPainter() {
  }

  //----------


  MIP_OpenGLPainter(MIP_PaintTarget* ATarget, MIP_PaintTarget* ASource) {
    MTarget = ATarget;
    MDisplay = ASource->getXlibDisplay();
    MIP_Print("Display: %p\n",MDisplay);
    if (MTarget->isWindow()) {
      //TODO
      //initWindow();
    }
    else if (MTarget->isSurface()) {
      initSurface();
    }
    else {
      //TODO:
      MIP_Assert(false); // target not handled..
    }
  }

  //----------

  virtual ~MIP_OpenGLPainter() {
    cleanupSurface();
    //cleanupWindow();
  }

//------------------------------
public:
//------------------------------

  //----------

  void makeCurrent() {
    glXMakeCurrent(MDisplay,MPixmap,MContext);
    MIP_OPENGL_ERROR_CHECK;
  }

  //----------

  void swapBuffers() {
    glXSwapBuffers(MDisplay,MPixmap);
    MIP_OPENGL_ERROR_CHECK;
  }

  //----------



//------------------------------
private:
//------------------------------

  /*
    The glXCreatePixmap subroutine creates an off-screen rendering area and
    returns its XID. Any GLX rendering context that was created with respect to
    the config parameter can be used to render into this off-screen area. Use
    the glXMakeContextCurrent subroutine to associate the rendering area with a
    GLX rendering context.
  */

  bool initSurface() {
    MIP_PRINT;
    GLint attribs[] = {
      GLX_X_RENDERABLE,   True,
      GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
      GLX_DRAWABLE_TYPE,  GLX_PIXMAP_BIT,
      GLX_RENDER_TYPE,    GLX_RGBA_BIT,
      GLX_BUFFER_SIZE,    24,
      //GLX_DOUBLEBUFFER,   True, // not for pixmap!
      GLX_RED_SIZE,       8,
      GLX_GREEN_SIZE,     8,
      GLX_BLUE_SIZE,      8,
      //GLX_ALPHA_SIZE,     0,  // window can't have alpha
      //GLX_STENCIL_SIZE,   8,  // nanovg needs stencil?
      //GLX_DEPTH_SIZE,     24,
      //GLX_SAMPLE_BUFFERS, True,
      //GLX_SAMPLES,        2,
      None
    };
    int num_fbc = 0;
    GLXFBConfig* fbc = glXChooseFBConfig(MDisplay,DefaultScreen(MDisplay),attribs,&num_fbc);
    //MIP_OPENGL_ERROR_CHECK;
    MIP_Print("%i fb configs\n",num_fbc);
    //for (int i=0; i<num_fbc; i++) {
    //  int attrib;
    //  glXGetFBConfigAttrib( MDisplay, fbc[i], GLX_BUFFER_SIZE, &attrib);
    //  MIP_Print("%i = %i\n",i,attrib);
    //}
    MFBConfig = fbc[0]; // fbc[num_fbc - 1];
    MIP_printFBConfigs(MDisplay,&MFBConfig,1);
    XFree(fbc);
    createOpenGLContext();
    //MPixmap = glXCreatePixmap(MDisplay,MFBConfig,MTarget->getXcbPixmap(),nullptr);
    MVisualInfo = glXGetVisualFromFBConfig(MDisplay,MFBConfig);
    MPixmap = glXCreateGLXPixmap(MDisplay,MVisualInfo,MTarget->getXcbPixmap());
    glXMakeCurrent(MDisplay,MPixmap,MContext);
    //makeCurrent();

    MIP_loadOpenGL();
    MIP_PRINT;
    return true;
  }

  //----------

  void cleanupSurface() {
    MIP_PRINT;
    if (MPixmap != GLX_NONE) {
      //glXDestroyPixmap(MDisplay,MPixmap);
      glXDestroyGLXPixmap(MDisplay,MPixmap);
      MIP_OPENGL_ERROR_CHECK;
      MPixmap = GLX_NONE;
    }
    MIP_PRINT;
  }

  //----------

  //bool initWindow() {
  //  return false;
  //}

  //----------

  //void cleanupWindow() {
  //  if (MWindow != GLX_NONE) {
  //    glXDestroyWindow(MDisplay,MWindow);
  //    MIP_OPENGL_ERROR_CHECK;
  //    MWindow = GLX_NONE;
  //  }
  //}

//------------------------------
private:
//------------------------------

  // FBConfigs were added in GLX version 1.3.

  bool checkVersion(Display* display, int major, int minor) {
    int glx_major, glx_minor;
    glXQueryVersion(display,&glx_major,&glx_minor);
    if (glx_major < major) return false;
    if (glx_minor < minor) return false;
    return true;
  }

  //----------

   bool selectFBConfig() {
    return false;
  }

  //----------

  /*
    // NOTE: It is not necessary to create or make current to a context before
    // calling glXGetProcAddressARB
    // Get the default screen's GLX extension list
    const char *glxExts = glXQueryExtensionsString( display, DefaultScreen( display ) );
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );
    GLXContext ctx = 0;
  */

  bool createOpenGLContext() {
    MIP_PRINT;
    glXCreateContextAttribsARBFUNC glXCreateContextAttribsARB = (glXCreateContextAttribsARBFUNC)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
    MIP_Assert(glXCreateContextAttribsARB);
    static int contextAttribs[] = {
      GLX_CONTEXT_MAJOR_VERSION_ARB,  MIP_OPENGL_MAJOR,
      GLX_CONTEXT_MINOR_VERSION_ARB,  MIP_OPENGL_MINOR,
      GLX_CONTEXT_PROFILE_MASK_ARB,   GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
      None
    };
    MContext = glXCreateContextAttribsARB(MDisplay,MFBConfig,nullptr,True,contextAttribs);
    //MIP_OPENGL_ERROR_CHECK;
    MIP_Assert(MContext);
    //XFree(fbc);
    MIP_PRINT;
    return true;
  }

};

//----------------------------------------------------------------------
#endif

