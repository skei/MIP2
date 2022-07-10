#if 0

#ifndef mip_opengl_window_included
#define mip_opengl_window_included
//----------------------------------------------------------------------

//https://www.khronos.org/registry/OpenGL/specs/gl/glx1.4.pdf

#include "mip.h"

#include "gui/opengl/mip_opengl.h"
#include "gui/opengl/mip_opengl_utils.h"
#include "gui/xlib/mip_xlib.h"
#include "gui/xcb/mip_xcb.h"
#include "gui/xcb/mip_xcb_window.h"

//----------------------------------------------------------------------

//typedef GLXContext (*glXCreateContextAttribsARBFUNC)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

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

  MIP_OpenGLWindow(/*MIP_WindowListener* AListener,*/ uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false)
  : MIP_XcbWindow(/*AListener,*/AWidth,AHeight,AEmbedded) {
    initOpenGL();
  }

  //----------

  virtual ~MIP_OpenGLWindow() {
    cleanupOpenGL();
  }

//------------------------------
public:
//------------------------------

  void beginPaint() override  {
    MIP_XcbWindow::beginPaint();
    makeCurrent();
  }

  //----------

  void endPaint() override  {
    swapBuffers();
    MIP_XcbWindow::endPaint();
  }

//------------------------------
protected:
//------------------------------

  GLXContext getGlxContext() {
    return MGlxContext;
  }

  //----------

  bool initOpenGL() {
    MIP_PRINT;

    //----- glx version -----

    // FBConfigs were added in GLX version 1.3.
    int glx_major, glx_minor;
    glXQueryVersion(MDisplay,&glx_major,&glx_minor);
    MIP_Print("glx version: %i.%i\n",glx_major,glx_minor);


    int num_fbc;
    GLXFBConfig* fbc = glXChooseFBConfig(MDisplay,DefaultScreen(MDisplay),visualAtt,&num_fbc);
    MIP_Assert(fbc);
    MIP_Print("found %i fb configs\n",num_fbc);
    //MIP_printFBConfigs(MDisplay,fbc,num_fbc);

    //----- find best fbc -----

    /*
    int selected_samples  = -1;
    for (int i=0; i<num_fbc; i++) {
      //XVisualInfo* vi = glXGetVisualFromFBConfig(MDisplay,fbc[i]);
      //XFree(vi);
      int samples;
      glXGetFBConfigAttrib( MDisplay, fbc[i], GLX_SAMPLES, &samples);
      if (samples > selected_samples) {
        selected_fbc = fbc[i];
        selected_samples = samples;
      }
    }
    */

    GLXFBConfig selected_fbc = fbc[0]; // fbc[num_fbc - 1];
    MIP_Print("selected_fbc %i\n",selected_fbc);

    //XVisualInfo* vi = glXGetVisualFromFBConfig(MDisplay,selected_fbc);


    //----- get create context function -----

    /*
      // NOTE: It is not necessary to create or make current to a context before
      // calling glXGetProcAddressARB

      // Get the default screen's GLX extension list
      const char *glxExts = glXQueryExtensionsString( display, DefaultScreen( display ) );
      glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
      glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );
      GLXContext ctx = 0;
    */

    glXCreateContextAttribsARBFUNC glXCreateContextAttribsARB = (glXCreateContextAttribsARBFUNC)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
    MIP_Assert(glXCreateContextAttribsARB);

    //----- create context -----

    static int contextAttribs[] = {
      GLX_CONTEXT_MAJOR_VERSION_ARB,  MIP_OPENGL_MAJOR,
      GLX_CONTEXT_MINOR_VERSION_ARB,  MIP_OPENGL_MINOR,
      GLX_CONTEXT_PROFILE_MASK_ARB,   GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
      None
    };

    //MGlxContext = glXCreateContextAttribsARB(MDisplay,*fbc,nullptr,True,contextAttribs);
    MGlxContext = glXCreateContextAttribsARB(MDisplay,selected_fbc,nullptr,True,contextAttribs);
    MIP_Assert(MGlxContext);

    XFree(fbc);

    //----- make context current -----

    glXMakeCurrent(MDisplay,MWindow,MGlxContext);

    //----- load opengl functions -----

    // should this be done per window/context, or once per program/library?
    if (!sogl_loadOpenGL()) {
      MIP_Print("sogl_loadOpenGL failed!\n");
      const char** failures = sogl_getFailures();
      while (*failures) {
        MIP_Print("%s\n",*failures);
        failures++;
      }
    }

    //-----

    return true;
  }

  //----------

  void cleanupOpenGL() {
    extern void sogl_cleanup();
  }

//------------------------------
public:
//------------------------------

  void makeCurrent() {
    glXMakeCurrent(MDisplay,MWindow,MGlxContext);
    MIP_OPENGL_ERROR_CHECK;
  }

  //----------

  void swapBuffers() {
    glXSwapBuffers(MDisplay,MWindow);
    MIP_OPENGL_ERROR_CHECK;
  }


};

//----------------------------------------------------------------------
#endif


#endif // 0


