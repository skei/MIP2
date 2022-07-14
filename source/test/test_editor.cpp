
//#define MIP_NO_ENTRY

#define MIP_GUI_XCB
#define MIP_PAINTER_OPENGL

#define MIP_DEBUG_PRINT_SOCKET
// nc -U -l -k /tmp/mip.socket

//----------------------------------------------------------------------

#include "base/system/mip_time.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"
#include "gui/mip_surface.h"
#include "gui/mip_window.h"
#include "gui/opengl/mip_opengl_painter.h"
#include "gui/opengl/mip_opengl_utils.h"
#include "gui/nanovg/mip_nanovg.h"



//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myWidget
: public MIP_Widget {

public:

  myWidget(MIP_DRect ARect)
  : MIP_Widget(ARect) {
  }

  //----------

  virtual ~myWidget() {
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myEditor
: public MIP_Editor {

//------------------------------
private:
//------------------------------

  GLfloat MPositionData[3*2] = {
     0,  1,
     1,  0,
    -1, -1,
  };

  //----------

  GLubyte MColorData[3*3] = {
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

//------------------------------
private:
//------------------------------

  MIP_OpenGLPainter*  GL              = nullptr;
  NVGcontext*         MNvgContext     = nullptr;
  //uint32_t            MWidth          = 0;
  //uint32_t            MHeight         = 0;
  GLuint              MVertexArray    = 0;
  GLuint              MPosBuffer      = 0;
  GLuint              MColBuffer      = 0;
  GLuint              MVertexShader   = 0;
  GLuint              MFragmentShader = 0;
  GLuint              MProgram        = 0;

//------------------------------
public:
//------------------------------

  /*
    opengl want context creation and processing in the same thread.
    since we start out own event thread, and paint from there,
    we need to do some trickery.. we set a few callback pointers,
    (using setThreadCallbacks), and when thr event thread starts,
    it calls these, so that we can setup things the right thread..
  */

  myEditor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight)
  : MIP_Editor(AListener,AWidth,AHeight) {
    MIP_PRINT;
    //setWindowFillBackground();
    setThreadCallbacks(thread_start,thread_stop);
  }

  //----------

  virtual ~myEditor() {
    MIP_PRINT;
  }

//------------------------------
public:
//------------------------------

  static
  void thread_start(void* AUserPtr) {
    MIP_PRINT;
    myEditor* editor = (myEditor*)AUserPtr;
    uint32_t width = editor->getWindowWidth();
    uint32_t height = editor->getWindowHeight();
    editor->init_gl(width,height);
    editor->prepare_gl(width,height);
  }

  //----------

  static
  void thread_stop(void* AUserPtr) {
    MIP_PRINT;
    myEditor* editor = (myEditor*)AUserPtr;
    editor->exit_gl();
  }

//------------------------------
public: // window listener
//------------------------------

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    GL->makeCurrent();
    int32_t window_width = getWindowWidth();
    int32_t window_height = getWindowHeight();
    MIP_Print("window size: %i,%i\n",window_width,window_height);
    double start_time = MIP_GetTimeMS();
    glViewport(0,0,window_width,window_height);
    glClearColor(0.5,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    renderTriangle();
    renderNanoVG(window_width,window_height);
    double elapsed = MIP_GetTimeMS() - start_time;
    MIP_Print("Elapsed (ms): %f\n",elapsed);
    GL->swapBuffers();
    GL->resetCurrent();
    //blitDrawable(0,0,MSurface->drawable_getXcbDrawable(),AXpos,AYpos,AWidth,AHeight);
  }

  //----------

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    MIP_PRINT;
    exit_gl();
    delete GL;
    init_gl(AWidth,AHeight);
    glViewport(0,0,AWidth,AHeight);
    prepare_gl(AWidth,AHeight);
  }

//------------------------------
private:
//------------------------------

  void init_gl(int32_t AWidth, int32_t AHeight) {
    MIP_PRINT;
    GL = new MIP_OpenGLPainter(this,this);
    GL->resetCurrent();
  }

  //----------

  void exit_gl() {
    MIP_PRINT;
    nvgDeleteGL3(MNvgContext);
    MIP_GL_ERROR_CHECK;
    GL->destroyContext();
  }

  //----------

  void prepare_gl(int32_t AWidth, int32_t AHeight) {
    MIP_PRINT;
    GL->makeCurrent();
    MIP_GL_ERROR_CHECK;
    setupTriangle();
    glClearColor(1,0,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    renderTriangle();
    MNvgContext = nvgCreateGL3(NVG_ANTIALIAS);// | NVG_STENCIL_STROKES);
    nvgCreateFont(MNvgContext,"font1","/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf");
    GL->resetCurrent();
    MIP_GL_ERROR_CHECK;
  }

  //----------

  void setupTriangle() {
    MIP_PRINT;
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
    //
    int params = -1;
    glGetProgramiv(MProgram,GL_LINK_STATUS,&params);
    if (params != GL_TRUE) {
      MIP_Print("Program did not link!\n");
    }
  }

  //----------

  void renderTriangle() {
    MIP_PRINT;
    glBindVertexArray(MVertexArray);
    glUseProgram(MProgram);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

  //----------

  void renderNanoVG(int32_t w, int32_t h) {
    MIP_PRINT;
    nvgBeginFrame(MNvgContext,w,h,1.0);
    for (uint32_t i=0; i<100; i++) {

      nvgBeginPath(MNvgContext);

      float x1 = MIP_RandomRange(0,w);
      float y1 = MIP_RandomRange(0,h);
      nvgMoveTo(MNvgContext,x1,y1);
      float x2 = MIP_RandomRange(0,w);
      float y2 = MIP_RandomRange(0,h);
      nvgLineTo(MNvgContext,x2,y2);

      uint8_t r = MIP_RandomRangeInt(0,255);
      uint8_t g = MIP_RandomRangeInt(0,255);
      uint8_t b = MIP_RandomRangeInt(0,255);
      nvgStrokeColor(MNvgContext, nvgRGBA(r,g,b,128));
      nvgStrokeWidth(MNvgContext,3);
      nvgStroke(MNvgContext);

    }

    nvgFontSize(MNvgContext,64);
    float x = MIP_RandomRange(0,w);
    float y = MIP_RandomRange(0,h);
    nvgFillColor(MNvgContext,nvgRGBA(0,0,0,255));
    nvgText(MNvgContext,x,y,"MIP2",0);
    nvgFillColor(MNvgContext,nvgRGBA(255,255,255,255));
    nvgText(MNvgContext,x+1,y+1,"MIP2",0);

    nvgEndFrame(MNvgContext);
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t template_descriptor = {
   .clap_version  = CLAP_VERSION,
   .id            = "me/template/0",
   .name          = "template",
   .vendor        = "me",
   .url           = "https://my_website.com",
   .manual_url    = "",
   .support_url   = "",
   .version       = "0.0.0",
   .description   = "",
   .features      =  (const char *[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class template_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

//  enum myParameterEnums {
//    MY_PARAM1 = 0,
//    MY_PARAM2,
//    MY_PARAM3,
//    MY_PARAM_COUNT
//  };
//
//  const clap_param_info_t myParameters[MY_PARAM_COUNT] = {
//    { MY_PARAM1, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "param1", "", 0, 1, 0 },
//    { MY_PARAM2, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "param2", "", -1, 1, 1 },
//    { MY_PARAM3, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED, nullptr, "param3", "", -12, 12, 0 }
//  };
//
//  const clap_audio_port_info_t myAudioInputPorts[1] = {
//    { 0, "audio in 1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
//  };
//
//  const clap_audio_port_info_t myAudioOutputPorts[1] = {
//    { 0, "audio out 1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
//  };
//
//  const clap_note_port_info_t myNoteInputPorts[1] = {
//    { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "note in 1" }
//  };
//
//  const clap_note_port_info_t myNoteOutputPorts[1] = {
//    { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "note out 1" }
//  };

//------------------------------
public:
//------------------------------

  template_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~template_plugin() {
  }

//------------------------------
public: // plugin
//------------------------------

  //

  bool init() final {
    //appendAudioInputPort(&myAudioInputPorts[0]);
    //appendAudioOutputPort(&myAudioOutputPorts[0]);
    //appendNoteInputPort(&myNoteInputPorts[0]);
    //appendNoteOutputPort(&myNoteOutputPorts[0]);
    //for (uint32_t i=0; i<MY_PARAM_COUNT; i++) {
    //  appendParameter(new MIP_Parameter(&myParameters[i]));
    //}
//    mip_old_x_error_handler = XSetErrorHandler(mip_x_error_handler);
    return true;
  }

  //void destroy() final {
  //  XSetErrorHandler(old_x_error_handler);
  //}

  //bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
  //  return true;
  //}

//------------------------------
public: // gui
//------------------------------

  bool gui_create(const char *api, bool is_floating) override {
    MEditor = new myEditor(this,MEditorWidth,MEditorHeight);
    return true;
  }

  //----------

  void gui_destroy() override {
    delete MEditor;
  }

//------------------------------
public:
//------------------------------

  //bool start_processing() final {
  //  return true;
  //}

  //----------

  //void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {}
  //void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {}
  //void processEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {}
  //void processEvent(const clap_event_header_t* header) final {}
  //void processNoteOnEvent(const clap_event_note_t* event) final {}
  //void processNoteOffEvent(const clap_event_note_t* event) final {}
  //void processNoteChokeEvent(const clap_event_note_t* event) final {}
  //void processNoteEndEvent(const clap_event_note_t* event) final {}
  //void processNoteExpressionEvent(const clap_event_note_expression_t* event) final {}
  //void processParamValueEvent(const clap_event_param_value_t* event) final {}
  //void processParamModEvent(const clap_event_param_mod_t* event) final {}
  //void processParamGestureBeginEvent(const clap_event_param_gesture_t* event) final {}
  //void processParamGestureEndEvent(const clap_event_param_gesture_t* event) final {}
  //void processTransportEvent(const clap_event_transport_t* event) final {}
  //void processMidiEvent(const clap_event_midi_t* event) final {}
  //void processMidiSysexEvent(const clap_event_midi_sysex_t* event) final {}
  //void processMidi2Event(const clap_event_midi2_t* event) final {}

  //void processAudioBlock(const clap_process_t* process) final {}
  //void processTransport(const clap_event_transport_t* transport) final {}

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef MIP_NO_ENTRY

  #include "plugin/clap/mip_clap_entry.h"
  #include "plugin/mip_registry.h"

  //----------

  void MIP_Register(MIP_Registry* ARegistry) {
    ARegistry->appendDescriptor(&template_descriptor);
  };

  //----------

  MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
    if (AIndex == 0) {
      return new template_plugin(ADescriptor,AHost);
    }
    return nullptr;
  }

#endif

