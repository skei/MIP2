#ifndef mip_opengl_window_included
#define mip_opengl_window_included
//----------------------------------------------------------------------

// https://www.khronos.org/opengl/wiki/Tutorial:_OpenGL_3.0_Context_Creation_(GLX)

#include "mip.h"

#include "gui/opengl/mip_opengl.h"
#include "gui/xlib/mip_xlib.h"
#include "gui/xcb/mip_xcb.h"
#include "gui/xcb/mip_xcb_window.h"

//----------------------------------------------------------------------

#include <GL/gl.h>
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

  MIP_OpenGLWindow(/*MIP_WindowListener* AListener,*/ uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false)
  : MIP_XcbWindow(/*AListener,*/AWidth,AHeight,AEmbedded) {
    initOpenGL();
  }

  //----------

  virtual ~MIP_OpenGLWindow() {
    cleanupOpenGL();
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

    int glx_major, glx_minor;
    // FBConfigs were added in GLX version 1.3.
    glXQueryVersion(MDisplay,&glx_major,&glx_minor);
    MIP_Print("glx version: %i.%i\n",glx_major,glx_minor);

    // ----- choose fb config -----

    /*
      The glXChooseFBConfig subroutine returns a pointer to a list of
      GLX FBConfig structures that match a specified list of attributes. The
      GLX attributes of the returned GLX FBConfigs match or exceed the the
      specified values, based on the table, below. To free the data returned by
      this function, use the XFree subroutine.

      If an attribute is not specified in AttributeList then the default value
      will be used instead. If the default value is GLX_DONT_CARE and the
      attribute is not in AttributeList then the attribute will not be checked.
      GLX_DONT_CARE may be specified for all attributes except GLX_LEVEL. If
      GLX_DONT_CARE is specified for an attribute then the attribute will not
      be checked. If AttributeList is NULL or empty (that is, the first
      attribute is None (or 0)), then the selection and sorting of the
      GLXFBConfigs is done according to the default values.

      To retrieve a GLX FBConfig given its XID, use the GLX_FBCONFIG_ID
      attribute. When GLX_FBCONFIG_ID is specified, all other attributes are
      ignored and only the GLX FBConfig with the given XID is returned (NULL
      (or 0) is returned if it does not exist).
    */

    // https://www.ibm.com/docs/en/aix/7.1?topic=environment-glxgetfbconfigattrib-subroutine#glxgetfbconfigattrib

    GLint visualAtt[] = {
      //GLX_X_RENDERABLE,   True,
      //GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
      GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT, // WINDOW, PIXMAP, PBUFFER
      GLX_RENDER_TYPE,    GLX_RGBA_BIT,
      GLX_DOUBLEBUFFER,   True,
      GLX_RED_SIZE,       8,
      GLX_GREEN_SIZE,     8,
      GLX_BLUE_SIZE,      8,
      //GLX_ALPHA_SIZE,     8, // window can't have alpha
      //GLX_STENCIL_SIZE,   1,    // nanovg needs stencil
      //GLX_DEPTH_SIZE,   24,
      //GLX_SAMPLE_BUFFERS, 1,
      //GLX_SAMPLES,        4,
      None
    };

    int numFBC;
    GLXFBConfig* fbc = glXChooseFBConfig(MDisplay,DefaultScreen(MDisplay),visualAtt,&numFBC);
    MIP_Assert(fbc);

    //----- find best fbc -----

    //GLXFBConfig selected_fbc = fbc[0];

    /*
    for (int i=0; i<numFBC; i++) {
      //XVisualInfo* vi = glXGetVisualFromFBConfig(MDisplay,fbc[i]);
      //if (vi) {
        int samp_buf;
        int samples;
        glXGetFBConfigAttrib( MDisplay, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
        glXGetFBConfigAttrib( MDisplay, fbc[i], GLX_SAMPLES       , &samples  );
        MIP_Print("%i. samp_buf %i samples %i\n",i,samp_buf,samples);
        if ((samp_buf > 0) && (samples == 8)) {
          best_fbc = fbc[i];
          //break;
        }
      //}
      //XFree(vi);
    }
    */

    //----- get create context function -----

    /*
    // Get the default screen's GLX extension list
    const char *glxExts = glXQueryExtensionsString( display, DefaultScreen( display ) );
    // NOTE: It is not necessary to create or make current to a context before
    // calling glXGetProcAddressARB
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );
    GLXContext ctx = 0;
    */

    glXCreateContextAttribsARBFUNC glXCreateContextAttribsARB = (glXCreateContextAttribsARBFUNC)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");

    //----- create context -----

    static int contextAttribs[] = {
      GLX_CONTEXT_MAJOR_VERSION_ARB,  MIP_OPENGL_MAJOR, //4,
      GLX_CONTEXT_MINOR_VERSION_ARB,  MIP_OPENGL_MINOR, //5,
      GLX_CONTEXT_PROFILE_MASK_ARB,   GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
      None
    };

    MGlxContext = glXCreateContextAttribsARB(MDisplay,*fbc,nullptr,True,contextAttribs);
    //MGlxContext = glXCreateContextAttribsARB(MDisplay,selected_fbc,nullptr,True,contextAttribs);

    //

    // we don't need this anymore
    XFree(fbc);

    //----- make context current -----

    glXMakeCurrent(MDisplay,MWindow,MGlxContext);

    //----- load opengl functions -----

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
