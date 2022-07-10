#ifndef mip_editor_included
#define mip_editor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_surface.h"
#include "plugin/clap/mip_clap.h"

#include "gui/nanovg/mip_nanovg.h"
#include "gui/mip_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

GLfloat MPositionData[] = {
  0,    0.9,
  0.9,  0,
 -0.9, -0.9,
};

//----------

GLubyte MColorData[] = {
  255, 0, 0,
  0, 255, 0,
  0, 0, 255
};

//----------

const char* MVertexShaderSource =
  " #version 450                      \n"
  "  layout(location=0) in vec4 pos;  \n"
  " layout(location=1)  in vec3 col;  \n"
  " out vec3 vColor;                  \n"
  " void main() {                     \n"
  "   vColor = col;                   \n"
  "   gl_Position = pos;              \n"
  "}                                  \n";

//----------

const char* MFragmentShaderSource =
  " #version 450                      \n"
  " in vec3 vColor;                   \n"
  " out vec4 fragColor;               \n"
  " void main() {                     \n"
  "   fragColor = vec4(vColor, 1.0);  \n"
  " }                                 \n";

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


class MIP_EditorListener {
public:
  virtual void on_editor_listener_update_parameter(uint32_t AIndex, double AValue) = 0;
  virtual void on_editor_listener_resize_window(uint32_t AWidth, uint32_t AHeight) = 0;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Editor
: public MIP_WindowListener {

//------------------------------
private:
//------------------------------

  MIP_EditorListener* MListener       = nullptr;
  MIP_Window*         MWindow         = nullptr;
  uint32_t            MWidth          = 256;
  uint32_t            MHeight         = 256;
  double              MScale          = 1.0;
  bool                MIsWindowOpen   = false;

  MIP_Surface*        MSurface        = nullptr;
  NVGcontext*         MNvgContext     = nullptr;
  MIP_Painter*        MPixmapPainter  = nullptr;
  //MIP_Painter*        MWindowPainter  = nullptr;

  // test

  GLuint              MVertexArray    = 0;
  GLuint              MPosBuffer      = 0;
  GLuint              MColBuffer      = 0;
  GLuint              MVertexShader   = 0;
  GLuint              MFragmentShader = 0;
  GLuint              MProgram        = 0;

//------------------------------
public:
//------------------------------

  MIP_Editor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight) {
    MListener = AListener;
    MWidth = AWidth;
    MHeight = AHeight;
    MWindow = new MIP_Window(/*this,*/MWidth,MHeight,true);
    MWindow->setListener(this);

    MWindow->setFillBackground();
    MSurface = new MIP_Surface(MWindow,256,256);

    MPixmapPainter = new MIP_Painter(MSurface,MWindow);
    MPixmapPainter->makeCurrent();
    setupTriangle();
    glClearColor(0.5,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    renderTriangle();
    MPixmapPainter->swapBuffers();
    //glXWaitGL();

    MNvgContext = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    MIP_Assert(MNvgContext);
    nvgCreateFont(MNvgContext,"font1","/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf");

    //MWindowPainter = new MIP_Painter(MWindow,MWindow);

  }

  //----------

  virtual ~MIP_Editor() {
    //delete MWindowPainter;
    nvgDeleteGL3(MNvgContext);
    delete MPixmapPainter;
    delete MSurface;
    if (MIsWindowOpen) hide();
    delete MWindow;
  }

//------------------------------
public:
//------------------------------

  void updateParameter(uint32_t AIndex, double AValue, bool ARedraw=true) {
    // if index -> connected.. set value, redraw
  }

//------------------------------
public: // window listener
//------------------------------

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    MIP_PRINT;
    int32_t w = MWindow->getWidth();
    int32_t h = MWindow->getHeight();
    MPixmapPainter->makeCurrent();

    //glViewport(0,0,w,h);
    //glViewport(0,256-h,w,h);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //gluOrtho2D(-320, 319,-240, 239);
    //glDepthMask (GL_TRUE);
    //glColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glClearColor(1,0,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    nvgBeginFrame(MNvgContext,w,h,1.0);
      nvgBeginPath(MNvgContext);
      nvgCircle(MNvgContext, 128,128,127);
      nvgFillColor(MNvgContext, nvgRGBA(128,64,64,255));
      nvgFill(MNvgContext);
      nvgFontFace(MNvgContext,"font1");
      nvgFontSize(MNvgContext,32);
      nvgFillColor(MNvgContext, nvgRGBA(255,255,255,128));
      nvgText(MNvgContext,30,40,"Hello world!",0);
      nvgFillColor(MNvgContext, nvgRGBA(0,0,0,255));
      nvgFontSize(MNvgContext,8);
      nvgText(MNvgContext,30,100,"Tiny, tiny 123 æøå",0);
      nvgFontSize(MNvgContext,9);
      nvgText(MNvgContext,30,110,"Tiny, tiny 123 æøå",0);
      nvgFontSize(MNvgContext,10);
      nvgText(MNvgContext,30,120,"Tiny, tiny 123 æøå",0);
    nvgEndFrame(MNvgContext);

    MPixmapPainter->swapBuffers();
    glXWaitGL();

    //MIP_Painter* window_painter = new MIP_Painter(MSurface,MWindow);
    //window_painter->makeCurrent();
    //window_painter->swapBuffers();
    //delete window_painter;

    MWindow->blitDrawable(32,32,MSurface->getXcbDrawable(),0,0,256,256);

  }

//------------------------------
public:
//------------------------------

  virtual bool set_scale(double scale) {
    MScale = scale;
    return true;
  }

  virtual bool get_size(uint32_t *width, uint32_t *height) {
    *width = MWidth;
    *height = MHeight;
    return true;
  }

  virtual bool can_resize() {
    return true;
  }

  virtual bool get_resize_hints(clap_gui_resize_hints_t *hints) {
    hints->can_resize_horizontally  = true;
    hints->can_resize_vertically    = true;
    hints->aspect_ratio_width       = 16;
    hints->aspect_ratio_height      = 9;
    hints->preserve_aspect_ratio    = false;
    return true;
  }

  virtual bool adjust_size(uint32_t *width, uint32_t *height) {
    //*width = MWidth;
    //*height = MHeight;
    //return true;
    return false;
  }

  virtual bool set_size(uint32_t width, uint32_t height) {
    MWidth = width;
    MHeight = height;
    MWindow->setSize(MWidth,MHeight);
    return true;
  }

  virtual bool set_parent(const clap_window_t *window) {
    MWindow->reparent(window->x11);
    return true;
  }

  virtual bool set_transient(const clap_window_t *window) {
    return true;
  }

  virtual void suggest_title(const char *title) {
    MWindow->setTitle(title);
  }

  virtual bool show() {
    MWindow->open();
    MWindow->startEventThread();
    MIsWindowOpen = true;
    return true;
  }

  virtual bool hide() {
    MIsWindowOpen = false;
    MWindow->stopEventThread();
    MWindow->close();
    return true;
  }






//------------------------------
private:
//------------------------------

  void setupTriangle() {

    // vertex array
    glGenVertexArrays(1,&MVertexArray);
    glBindVertexArray(MVertexArray);
    // positions
    glGenBuffers(1,&MPosBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,MPosBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(GLfloat), MPositionData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    // colors
    glGenBuffers(1,&MColBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, MColBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLubyte), MColorData, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, NULL);
    glEnableVertexAttribArray(1);

    // vs
    MVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(MVertexShader,1,&MVertexShaderSource,NULL);
    glCompileShader(MVertexShader);
    // fs
    MFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(MFragmentShader,1,&MFragmentShaderSource, NULL);
    glCompileShader(MFragmentShader);
    // program
    MProgram = glCreateProgram();
    glAttachShader(MProgram,MVertexShader);
    glAttachShader(MProgram,MFragmentShader);
    glLinkProgram(MProgram);

    int params = -1;
    glGetProgramiv(MProgram,GL_LINK_STATUS,&params);
    if (params != GL_TRUE) {
      MIP_Print("Program did not link!\n");
    }
  }

  void renderTriangle() {
    glClearColor(0.3, 0.1, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(MProgram);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

};

//----------------------------------------------------------------------
#endif
