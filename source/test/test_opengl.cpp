
#include "mip.h"
#include "base/system/mip_time.h"
#include "base/system/mip_timer.h"

#include "gui/gl/mip_gl.h"
#include "gui/gl/mip_gl_window.h"

#define NANOVG_GL3_IMPLEMENTATION	// Use GL2 implementation.
#include "extern/nanovg/nanovg.h"
#include "extern/nanovg/nanovg_gl.h"
  // get rid of "warning: ‘visibility’ attribute ignored [-Wattributes]"
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include "extern/nanovg/nanovg.c"
  #pragma GCC diagnostic pop

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
: public MIP_GlWindow
, public MIP_WindowListener
, public MIP_TimerListener {

//------------------------------
private:
//------------------------------

  MIP_Timer*  MTimer          = nullptr;

  GLuint      MVertexArray    = 0;
  GLuint      MPosBuffer      = 0;
  GLuint      MColBuffer      = 0;
  GLuint      MVertexShader   = 0;
  GLuint      MFragmentShader = 0;
  GLuint      MProgram        = 0;

  struct NVGcontext* vg = nullptr;

//------------------------------
public:
//------------------------------

  myWindow(uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false)
  : MIP_GlWindow(AWidth,AHeight,AEmbedded) {
    setListener(this);
    setTitle("OpenGL + NanoVG");
    setup_gl();
    vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    nvgCreateFont(vg,"Liberation","/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf");
  }

  //----------

  virtual ~myWindow() {
  }

//------------------------------
public: // window listener
//------------------------------

  //void on_window_open() final { MIP_Print("\n"); }
  //void on_window_close() final { MIP_Print("\n"); }
  //void on_window_move(int32_t AXpos, int32_t AYpos) final { MIP_Print("x:%i y:%i\n",AXpos,AYpos); }
  //void on_window_resize(int32_t AWidth, int32_t AHeight) final { MIP_Print("w:%i h:%i\n",AWidth,AHeight); }

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) final {
    //MIP_Print("x:%i y:%i w:%i h:%i\n",AXpos,AYpos,AWidth,AHeight);
    //makeCurrent();
    //render_gl();

    glViewport(0,0,MWindowWidth,MWindowHeight);

    glClearColor(0.3, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    {
      nvgBeginFrame(vg,MWindowWidth,MWindowHeight,1.0);
      nvgBeginPath(vg);
      nvgCircle(vg, 200,200,150);
      nvgFillColor(vg, nvgRGB(128,96,0));
      nvgFill(vg);
      nvgFontFace(vg,"Liberation");
      nvgFontSize(vg,32);
      nvgFillColor(vg, nvgRGB(255,255,255));
      nvgText(vg,30,130,"Hello world!",0);
      nvgEndFrame(vg);
    }

    swapBuffers();

  }

  //void on_window_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) final { MIP_Print("k: %i s:%i ts:%i\n",AKey,AState,ATime); }
  //void on_window_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) final { MIP_Print("k: %i s:%i ts:%i\n",AKey,AState,ATime); }
  //void on_window_mouse_press(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) final { MIP_Print("b: %i s:%i x:%i y:%i ts:%i\n",AButton,AState,AXpos,AYpos,ATime); }
  //void on_window_mouse_release(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) final { MIP_Print("b: %i s:%i x:%i y:%i ts:%i\n",AButton,AState,AXpos,AYpos,ATime); }
  //void on_window_mouse_move(uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) final { MIP_Print("s:%i x:%i y:%i ts:%i\n",AState,AXpos,AYpos,ATime); }
  //void on_window_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime) final { MIP_Print("x:%i y:%i time:%i\n",AXpos,AYpos,ATime); }
  //void on_window_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime) final { MIP_Print("x:%i y:%i time:%i\n",AXpos,AYpos,ATime); }
  //void on_window_client_message(uint32_t AData) final { MIP_Print("data:%i\n",AData); }

//------------------------------
public:
//------------------------------

  void start_timer() {
    MTimer = new MIP_Timer(this);
    MTimer->start(100,false);
  }

  void stop_timer() {
    MTimer->stop();
  }

//------------------------------
public:
//------------------------------

  void setup_gl() {
    glClearColor(0.3, 0.3, 0.3, 1.0);

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

    glUseProgram(MProgram);

  }

  //----------

  void render_gl() {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

int main() {

  myWindow* window = new myWindow(640,480);
  window->setFillBackground(false);
  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}



