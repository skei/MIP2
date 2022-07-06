#ifndef mip_gl_window_included
#define mip_gl_window_included
//----------------------------------------------------------------------

#include "mip.h"

#include "gui/gl/mip_gl.h"
#include "gui/xlib/mip_xlib.h"
#include "gui/xcb/mip_xcb.h"
#include "gui/xcb/mip_xcb_window.h"

//----------------------------------------------------------------------

#include <GL/glx.h>

typedef GLXContext (*glXCreateContextAttribsARBFUNC)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

//----------------------------------------------------------------------

class MIP_GlWindow
: public MIP_XcbWindow {

//------------------------------
public:
//------------------------------

  GLXContext  MGlxContext = nullptr;

//------------------------------
public:
//------------------------------

  MIP_GlWindow(uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false)
  : MIP_XcbWindow(AWidth,AHeight,AEmbedded) {
    initGl();
  }

  //----------

  virtual ~MIP_GlWindow() {
    cleanupGl();
  }

//------------------------------
public:
//------------------------------

  // after map window?

  bool initGl() {

    int numFBC = 0;
    GLint visualAtt[] = {
      GLX_RENDER_TYPE, GLX_RGBA_BIT,
      GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
      GLX_DOUBLEBUFFER, True,
      GLX_RED_SIZE, 1,
      GLX_GREEN_SIZE, 1,
      GLX_BLUE_SIZE, 1,
      GLX_DEPTH_SIZE, 1,
      GLX_STENCIL_SIZE, 1,
      None
    };
    GLXFBConfig *fbc = glXChooseFBConfig(MDisplay, DefaultScreen(MDisplay), visualAtt, &numFBC);
    if (!fbc) {
      MIP_Print("Unable to get framebuffer\n");
      return false;
    }

    glXCreateContextAttribsARBFUNC glXCreateContextAttribsARB = (glXCreateContextAttribsARBFUNC) glXGetProcAddress((const GLubyte *) "glXCreateContextAttribsARB");
    if (!glXCreateContextAttribsARB) {
      MIP_Print("Unable to get proc glXCreateContextAttribsARB\n");
      XFree(fbc);
      return false;
    }

    static int contextAttribs[] = {
      GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
      GLX_CONTEXT_MINOR_VERSION_ARB, 5,
      GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
      None
    };
    MGlxContext = glXCreateContextAttribsARB(MDisplay, *fbc, NULL, True, contextAttribs);

    XFree(fbc);

    if (!MGlxContext) {
      MIP_Print("Unable to create OpenGL context\n");
      return false;
    }

    glXMakeCurrent(MDisplay,MWindow,MGlxContext);

    if (!sogl_loadOpenGL()) {
      const char **failures = sogl_getFailures();
      while (*failures) {
        MIP_Print("sogl_loadOpenGL failed: %s\n", *failures);
        failures++;
      }
    }

    //---

//    glXMakeCurrent(MDisplay,MWindow,MGlxContext);
//
//    glClearColor(0.0, 0.0, 0.0, 1.0);
//
//    GLuint vertexArray = 0;
//    glGenVertexArrays(1,&vertexArray);
//    glBindVertexArray(vertexArray);
//
//    GLfloat positionData[] = {
//      -0.5, -0.5,
//       0.5, -0.5,
//       0.0,  0.5
//    };
//    GLuint positions = 0;
//    glGenBuffers(1,&positions);
//    glBindBuffer(GL_ARRAY_BUFFER,positions);
//    glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(GLfloat), positionData, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
//    glEnableVertexAttribArray(0);
//
//    GLubyte colorData[] = {
//      255, 0, 0,
//      0, 255, 0,
//      0, 0, 255
//    };
//    GLuint colors = 0;
//    glGenBuffers(1, &colors);
//    glBindBuffer(GL_ARRAY_BUFFER, colors);
//    glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLubyte), colorData, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, NULL);
//    glEnableVertexAttribArray(1);
//
//    const char* vsSource =
//      "#version 450\n"
//      "layout(location=0) in vec4 position;\n"
//      "layout(location=1) in vec3 color;\n"
//      "out vec3 vColor;\n"
//      "void main() {\n"
//      "    vColor = color;\n"
//      "    gl_Position = position;\n"
//      "}\n";
//    const char* fsSource =
//      "#version 450\n"
//      "in vec3 vColor;\n"
//      "out vec4 fragColor;\n"
//      "void main() {\n"
//      "    fragColor = vec4(vColor, 1.0);\n"
//      "}\n";
//
//    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vs, 1, &vsSource, NULL);
//    glCompileShader(vs);
//
//    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fs, 1, &fsSource, NULL);
//    glCompileShader(fs);
//
//    GLuint program = glCreateProgram();
//    glAttachShader(program, vs);
//    glAttachShader(program, fs);
//    glLinkProgram(program);
//
//    int params = -1;
//    glGetProgramiv(program, GL_LINK_STATUS, &params);
//    if (params != GL_TRUE) {
//      fprintf(stderr, "Program did not link!\n");
//    }
//    glUseProgram(program);

    //---

    return true;
  }

  //----------

  void cleanupGl() {
  }

  //----------

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











#if 0









  Atom wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(display, window, &wmDeleteMessage, 1);

  // Animation loop
  while (1) {
    if (XCheckTypedWindowEvent(display, window, Expose, &event) == True) {

      XGetWindowAttributes(display, window, &xWinAtt);
      glViewport(0, 0, xWinAtt.width, xWinAtt.height);
    }
    if (XCheckTypedWindowEvent(display, window, ClientMessage, &event) == True) {
      if (event.xclient.data.l[0] == wmDeleteMessage) {
        break;
      }
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glXSwapBuffers(display, window);
  };
}






#endif // 0



