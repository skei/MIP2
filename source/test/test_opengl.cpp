
#include "mip.h"
#include "base/system/mip_time.h"
#include "base/system/mip_timer.h"
#include "gui/opengl/mip_opengl.h"
#include "gui/nanovg/mip_nanovg_window.h"

#include "plugin/mip_editor.h"
#include "gui/opengl/mip_opengl.h"

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

class myWindow
: public MIP_NanoVGWindow
, public MIP_TimerListener {

//------------------------------
private:
//------------------------------

  GLuint              MVertexArray    = 0;
  GLuint              MPosBuffer      = 0;
  GLuint              MColBuffer      = 0;
  GLuint              MVertexShader   = 0;
  GLuint              MFragmentShader = 0;
  GLuint              MProgram        = 0;

  MIP_Timer*          MTimer          = nullptr;
  NVGLUframebuffer*   MBuffer         = nullptr;

//------------------------------
public:
//------------------------------

  myWindow(/*MIP_WindowListener* AListener,*/ uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false)
  : MIP_NanoVGWindow(/*AListener,*/AWidth,AHeight,AEmbedded) {
    MIP_PRINT;
    setTitle("MIP_NanoVGWindow");
    setupTriangle();
    MBuffer = createFrameBuffer(AWidth,AHeight);
    bindFrameBuffer(MBuffer);
    renderTriangle();
    unbindFrameBuffer();
  }

  //----------

  virtual ~myWindow() {
    deleteFrameBuffer(MBuffer);
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

//------------------------------
private:
//------------------------------

  // render to buffer

  void renderTriangle() {
    glClearColor(0.3, 0.1, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(MProgram);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

  //----------

  // render using buffer

  void renderBuffer() {
    NVGpaint img = nvgImagePattern(MNvgContext, 0,0, 640,480, 0, MBuffer->image, 1);
    //nvgSave(MNvgContext);
    nvgBeginPath(MNvgContext);
    nvgRect(MNvgContext,0,0,640,480);
    nvgFillPaint(MNvgContext, img);
    nvgFill(MNvgContext);
    //nvgRestore(MNvgContext);
  }

  //----------

  // circle and text

  void renderNanoVG() {

    nvgBeginPath(MNvgContext);
    nvgCircle(MNvgContext, 200,200,150);
    nvgFillColor(MNvgContext, nvgRGBA(128,96,0,128));
    nvgFill(MNvgContext);

    nvgFontFace(MNvgContext,"font1");
    nvgFontSize(MNvgContext,64);
    nvgFillColor(MNvgContext, nvgRGBA(255,255,255,128));
    nvgText(MNvgContext,30,130,"Hello world!",0);

    nvgFillColor(MNvgContext, nvgRGBA(0,0,0,255));
    nvgFontSize(MNvgContext,8);
    nvgText(MNvgContext,280,160,"Tiny, tiny 123 æøå",0);

    nvgFontSize(MNvgContext,9);
    nvgText(MNvgContext,280,170,"Tiny, tiny 123 æøå",0);

    nvgFontSize(MNvgContext,10);
    nvgText(MNvgContext,280,182,"Tiny, tiny 123 æøå",0);

  }

//------------------------------
public: // window listener
//------------------------------

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) final {
    MIP_Print("x:%i y:%i w:%i h:%i\n",AXpos,AYpos,AWidth,AHeight);

    makeCurrent();

    glViewport(0,0,getWidth(),getHeight());
    MIP_OPENGL_ERROR_CHECK;

    glClearColor(0.3, 0.3, 0.3, 1.0);
    MIP_OPENGL_ERROR_CHECK;

    //glStencilMask(~0);
    //glDisable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    MIP_OPENGL_ERROR_CHECK;

    nvgBeginFrame(MNvgContext,getWidth(),getHeight(),1.0);
    renderBuffer();
    renderNanoVG();
    nvgEndFrame(MNvgContext);
    swapBuffers();
    MIP_OPENGL_ERROR_CHECK;
  }

//------------------------------
public: // timer
//------------------------------

  virtual void on_timerCallback(void) {
    MIP_Print("\n");
    //on_window_paint(0,0,640,480);
  }

  //----------

  void startTimer(float ms) {
    MTimer = new MIP_Timer(this);
    MTimer->start(ms,false);
  }

  //----------

  void stopTimer() {
    MTimer->stop();
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

int main() {
  myWindow* window = new myWindow(640,480);
  //MIP_NanoVGWindow* window = new MIP_NanoVGWindow(640,480);
  window->open();

  window->eventLoop();

  //window->startEventThread();
  //window->paint();
  //MIP_Sleep(5 * 1000);
  //window->stopEventThread();

  window->close();
  delete window;
  return 0;
}


