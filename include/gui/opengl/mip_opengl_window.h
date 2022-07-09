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

    /*
      GLX FBCONFIG ID             XID       XID of GLXFBConfig
      GLX BUFFER SIZE             integer   depth of the color buffer
      GLX LEVEL                   integer   frame buffer level
      GLX DOUBLEBUFFER            boolean   True if color buffers have front/back pairs
      GLX STEREO                  boolean   True if color buffers have left/right pairs
      GLX AUX BUFFERS             integer   no. of auxiliary color buffers
      GLX RED SIZE                integer   no. of bits of Red in the color buffer
      GLX GREEN SIZE              integer   no. of bits of Green in the color buffer
      GLX BLUE SIZE               integer   no. of bits of Blue in the color buffer
      GLX ALPHA SIZE              integer   no. of bits of Alpha in the color buffer
      GLX DEPTH SIZE              integer   no. of bits in the depth buffer
      GLX STENCIL SIZE            integer   no. of bits in the stencil buffer
      GLX ACCUM RED SIZE          integer   no. Red bits in the accum. buffer
      GLX ACCUM GREEN SIZE        integer   no. Green bits in the accum. buffer
      GLX ACCUM BLUE SIZE         integer   no. Blue bits in the accum. buffer
      GLX ACCUM ALPHA SIZE        integer   no. of Alpha bits in the accum. buffer
      GLX SAMPLE BUFFERS          integer   number of multisample buffers
      GLX SAMPLES                 integer   number of samples per pixel
      GLX RENDER TYPE             bitmask   which rendering modes are supported.
      GLX DRAWABLE TYPE           bitmask   which GLX drawables are supported.
      GLX X RENDERABLE            boolean   True if X can render to drawable
      GLX X VISUAL TYPE           integer   X visual type of the associated visual
      GLX CONFIG CAVEAT           enum      any caveats for the configuration
      GLX TRANSPARENT TYPE        enum      type of transparency supported
      GLX TRANSPARENT INDEX VALUE integer   transparent index value
      GLX TRANSPARENT RED VALUE   integer   transparent red value
      GLX TRANSPARENT GREEN VALUE integer   transparent green value
      GLX TRANSPARENT BLUE VALUE  integer   transparent blue value
      GLX TRANSPARENT ALPHA VALUE integer   transparent alpha value
      GLX MAX PBUFFER WIDTH       integer   maximum width of GLXPbuffer
      GLX MAX PBUFFER HEIGHT      integer   maximum height of GLXPbuffer
      GLX MAX PBUFFER PIXELS      integer   maximum size of GLXPbuffer
      GLX VISUAL ID               integer   XID of corresponding Visual
    */

    /*
      GLX_X_VISUAL_TYPE
        GLX_TRUE_COLOR    TrueColor             0x8002
        GLX_DIRECT_COLOR  DirectColor           0x8003
        GLX_PSEUDO_COLOR  PseudoColor           0x8004
        GLX_STATIC_COLOR  StaticColor           0x8005
        GLX_GRAY_SCALE    GrayScale             0x8006
        GLX_STATIC_GRAY   StaticGray            0x8007
        GLX_X_VISUAL_TYPE No Associated Visual  0x0022
      */

    /*
      GLX_DRAWABLE_TYPE
        GLX_WINDOW_BIT, GLX Windows are supported.    0x01
        GLX_PIXMAP_BIT  GLX Pixmaps are supported.    0x02
        GLX_PBUFFER_BIT GLX Pbuffers are supported.   0x04
    */

    /*
      GLX_RENDER_TYPE
        GLX_RGBA_BIT        RGBA rendering supported.         0x01
        GLX_COLOR_INDEX_BIT Color index rendering supported.  0x02
    */

    GLint visualAtt[] = {
      //GLX_X_RENDERABLE,   True,
      GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
      GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
      GLX_RENDER_TYPE,    GLX_RGBA_BIT,
      //GLX_BUFFER_SIZE,    24,
      GLX_DOUBLEBUFFER,   True,
      GLX_RED_SIZE,       8,
      GLX_GREEN_SIZE,     8,
      GLX_BLUE_SIZE,      8,
      //GLX_ALPHA_SIZE,     0,  // window can't have alpha
      GLX_STENCIL_SIZE,   1,  // nanovg needs stencil?
      //GLX_DEPTH_SIZE,     24,
      //GLX_SAMPLE_BUFFERS, True,
      //GLX_SAMPLES,        2,
      None
    };

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


