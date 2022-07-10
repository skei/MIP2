#ifndef mip_opengl_painter_included
#define mip_opengl_painter_included
//----------------------------------------------------------------------


    //glBindFramebuffer(GL_FRAMEBUFFER,MPixmap);



#include "mip.h"
#include "gui/opengl/mip_opengl.h"
#include "gui/opengl/mip_opengl_utils.h"
#include "gui/mip_paint_target.h"

//----------------------------------------------------------------------

typedef GLXContext (*glXCreateContextAttribsARBFUNC)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

GLint MIP_GlxPixmapAttribs[] = {
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
  //GLX_Y_INVERTED_EXT, True,
  None
};

GLint MIP_GlxWindowAttribs[] = {
  GLX_X_RENDERABLE,   True,
  GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
  GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
  GLX_RENDER_TYPE,    GLX_RGBA_BIT,
  //GLX_BUFFER_SIZE,    24,
  //GLX_DOUBLEBUFFER,   True, // not for pixmap!
  GLX_RED_SIZE,       8,
  GLX_GREEN_SIZE,     8,
  GLX_BLUE_SIZE,      8,
  //GLX_ALPHA_SIZE,     0,  // window can't have alpha
  //GLX_STENCIL_SIZE,   8,  // nanovg needs stencil?
  //GLX_DEPTH_SIZE,     24,
  //GLX_SAMPLE_BUFFERS, True,
  //GLX_SAMPLES,        2,
  //GLX_Y_INVERTED_EXT, True,
  None
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_OpenGLPainter {

//------------------------------
private:
//------------------------------

  GLXContext        MContext      = nullptr;
  MIP_PaintTarget*  MTarget       = nullptr;
  MIP_PaintSource*  MSource       = nullptr;

  Display*          MDisplay      = nullptr;
  XVisualInfo*      MVisualInfo   = nullptr;
  GLXFBConfig*      MFBConfigList = nullptr;
  GLXFBConfig       MFBConfig     = nullptr;

  GLXPixmap         MPixmap       = GLX_NONE;
  GLXWindow         MWindow       = GLX_NONE;

//------------------------------
public:
//------------------------------

  // target = what we're painting to
  // source = window (xcb connection, visual, etc)

  MIP_OpenGLPainter(MIP_PaintTarget* ATarget, MIP_PaintSource* ASource) {
    MTarget = ATarget;
    MDisplay = ASource->getXlibDisplay();
    if (MTarget->isSurface()) createPixmapContext();
    else if (MTarget->isWindow()) createWindowContext();
    else MIP_Assert(false); // target not handled..
    makeCurrent();
    loadOpenGL();
  }

  //----------

  virtual ~MIP_OpenGLPainter() {
    if (MPixmap != GLX_NONE) {
      glXDestroyPixmap(MDisplay,MPixmap);
      MPixmap = GLX_NONE;
    }
    if (MWindow != GLX_NONE) {
      glXDestroyWindow(MDisplay,MWindow);
      MWindow = GLX_NONE;
    }
    if (MFBConfigList) {
      XFree(MFBConfigList);
      MFBConfigList = nullptr;
    }
  }

//------------------------------
public:
//------------------------------

  //----------

  void makeCurrent() {
    if (MPixmap != GLX_NONE) glXMakeCurrent(MDisplay,MPixmap,MContext);
    if (MWindow != GLX_NONE) glXMakeCurrent(MDisplay,MWindow,MContext);
  }

  //----------

  void swapBuffers() {
    if (MPixmap != GLX_NONE) glXSwapBuffers(MDisplay,MPixmap);
    if (MWindow != GLX_NONE) glXSwapBuffers(MDisplay,MWindow);
  }

  //----------

  // make context current before calling this
  // should this be done per window/context, or once per program/library?

  bool loadOpenGL() {
    if (!sogl_loadOpenGL()) {
      MIP_Print("sogl_loadOpenGL failed!\n");
      const char** failures = sogl_getFailures();
      while (*failures) {
        MIP_Print("%s\n",*failures);
        failures++;
      }
      return false;
    }
    return true;
  }

  //----------

  float getGlxVersion(Display* display) {
    int major, minor;
    glXQueryVersion(display,&major,&minor);
    char buf[16] = {0};
    sprintf(buf,"%i.%i",major,minor);
    return atof(buf);
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

//------------------------------
private: // context creation
//------------------------------

  /*
    The glXCreatePixmap subroutine creates an off-screen rendering area and
    returns its XID. Any GLX rendering context that was created with respect to
    the config parameter can be used to render into this off-screen area. Use
    the glXMakeContextCurrent subroutine to associate the rendering area with a
    GLX rendering context.
  */

  //for (int i=0; i<num_fbc; i++) {
  //  int attrib;
  //  glXGetFBConfigAttrib( MDisplay, fbc[i], GLX_BUFFER_SIZE, &attrib);
  //  MIP_Print("%i = %i\n",i,attrib);
  //}

  bool createPixmapContext() {
    int num_fbc = 0;
    MFBConfigList = glXChooseFBConfig(MDisplay,DefaultScreen(MDisplay),MIP_GlxPixmapAttribs,&num_fbc);
    //MIP_Print("%i fb configs\n",num_fbc);
    MFBConfig = MFBConfigList[0];
    //MIP_printFBConfigs(MDisplay,&MFBConfig,1);
    createContext();
    MPixmap = glXCreatePixmap(MDisplay,MFBConfig,MTarget->getXcbPixmap(),nullptr);
    return true;
  }

  //----------

  bool createWindowContext() {
    int num_fbc = 0;
    MFBConfigList = glXChooseFBConfig(MDisplay,DefaultScreen(MDisplay),MIP_GlxWindowAttribs,&num_fbc);
    //MIP_Print("%i fb configs\n",num_fbc);
    MFBConfig = MFBConfigList[0];
    //MIP_printFBConfigs(MDisplay,&MFBConfig,1);
    createContext();
    //MVisualInfo = glXGetVisualFromFBConfig(MDisplay,MFBConfig);
    MWindow = glXCreateWindow(MDisplay,MFBConfig,MTarget->getXcbWindow(),nullptr);
    return true;
  }

  //----------

  bool createContext() {
    glXCreateContextAttribsARBFUNC glXCreateContextAttribsARB = (glXCreateContextAttribsARBFUNC)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
    MIP_Assert(glXCreateContextAttribsARB);
    static int contextAttribs[] = {
      GLX_CONTEXT_MAJOR_VERSION_ARB,  MIP_OPENGL_MAJOR,
      GLX_CONTEXT_MINOR_VERSION_ARB,  MIP_OPENGL_MINOR,
      GLX_CONTEXT_PROFILE_MASK_ARB,   GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
      None
    };
    MContext = glXCreateContextAttribsARB(MDisplay,MFBConfig,nullptr,True,contextAttribs);
    MIP_Assert(MContext);
    return true;
  }

  //----------


};

//----------------------------------------------------------------------
#endif


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



