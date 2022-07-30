
#ifndef MIP_EXE
  #define MIP_DEBUG_PRINT_SOCKET
  // nc -U -l -k /tmp/mip.socket
#endif // MIP_EXE

#define MIP_GUI_XCB
#define MIP_PAINTER_NANOVG

//----------------------------------------------------------------------

#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"

#include "gui/widgets/mip_widgets.h"
#include "gui/mip_bitmap.h"

#include "base/system/mip_timer.h"


//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t template_descriptor = {
   .clap_version  = CLAP_VERSION,
   .id            = "me/test_editor2/0",
   .name          = "test_editor2",
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
// editor
//
//----------------------------------------------------------------------

class test_editor2_editor
: public MIP_Editor
, public MIP_TimerListener {

private:

  MIP_ColorWidget*      background = nullptr;
  MIP_ColorWidget*      color1 = nullptr;
  MIP_PanelWidget*      panel1 = nullptr;
  MIP_TextWidget*       text1 = nullptr;
  MIP_ValueWidget*      value1 = nullptr;
  MIP_DragValueWidget*  dragvalue1 = nullptr;
  MIP_SliderWidget*     slider1 = nullptr;
  MIP_ButtonWidget*     button1 = nullptr;
  MIP_ButtonWidget*     button2 = nullptr;

  MIP_KnobWidget*       knob1 = nullptr;
  MIP_ImageWidget*      image1 = nullptr;

  mip_voxel_t*    voxel                 = nullptr;
  uint32_t        voxelbuffer[320*240]  = {0};
  MIP_Timer       timer                 = MIP_Timer(this);
  MIP_Bitmap      bitmap                = {};

  int32_t px  =  160;
  int32_t py  =  100;
  float pa    = 0.0;

public:

  test_editor2_editor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight)
  : MIP_Editor(AListener,AWidth,AHeight) {
    setup_widgets(AWidth,AHeight);
    setWindowFillBackground(false);
    voxel = voxel_create();
    voxel_make_map(voxel);
  }

  virtual ~test_editor2_editor() {
    voxel_destroy(voxel);
  }

//------------------------------
public:
//------------------------------

  void setup_widgets(uint32_t AWidth, uint32_t AHeight) {
    // background
    background = new MIP_ColorWidget( MIP_DRect( 0,0,500,400), MIP_COLOR_GRAY );
    appendChildWidget(background);
    background->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;

    #define H  22
    #define H2 25

    color1      = new MIP_ColorWidget(      MIP_DRect(  10, 10 + (0 * H2), 150, H ), MIP_COLOR_RED );
    panel1      = new MIP_PanelWidget(      MIP_DRect(  10, 10 + (1 * H2), 150, H ) );
    text1       = new MIP_TextWidget(       MIP_DRect(  10, 10 + (2 * H2), 150, H ), "Text" );
    value1      = new MIP_ValueWidget(      MIP_DRect(  10, 10 + (3 * H2), 150, H ), "Value", 0.3 );
    dragvalue1  = new MIP_DragValueWidget(  MIP_DRect(  10, 10 + (4 * H2), 150, H ), "DragValue", 0.3 );
    slider1     = new MIP_SliderWidget(     MIP_DRect(  10, 10 + (5 * H2), 150, H ), "Slider", 0.3 );
    button1     = new MIP_ButtonWidget(     MIP_DRect(  10, 10 + (6 * H2), 150, H ), "On", "Off", 0 );
    button2     = new MIP_ButtonWidget(     MIP_DRect(  10, 10 + (7 * H2), 150, H ), "On", "Off", 0 );

    knob1       = new MIP_KnobWidget(       MIP_DRect( 170, 10, 40,40),   "Knob", 0.3 );
    image1      = new MIP_ImageWidget(      MIP_DRect( 170, 10, 320,240), voxelbuffer, 320,240);

    #undef H
    #undef H2

    slider1->setMinValue(-5);
    slider1->setMaxValue(5);
    slider1->scaleDragSensitivity(10);
    knob1->setFillBackground(false);
    knob1->setDrawBorder(false);
    button1->setTextColor(MIP_COLOR_BLACK);
    button2->setTextColor(MIP_COLOR_RED);
    button2->setIsToggle(false);

    background->appendChildWidget(color1);
    background->appendChildWidget(panel1);
    background->appendChildWidget(text1);
    background->appendChildWidget(value1);
    background->appendChildWidget(dragvalue1);
    background->appendChildWidget(slider1);
    background->appendChildWidget(button1);
    background->appendChildWidget(button2);

    background->appendChildWidget(knob1);
    background->appendChildWidget(image1);
  }

  //

  bool gui_show() override {
    bool result = MIP_Editor::gui_show();
    timer.start(50);
    return result;
  }

  bool gui_hide() override {
    timer.stop();
    bool result = MIP_Editor::gui_hide();
    return result;
  }

  void on_timerCallback() final {
    MIP_PRINT;
    memset(voxelbuffer,0,sizeof(voxelbuffer));
    voxel_render_frame(voxel,voxelbuffer,px * 65536,py * 65536,pa);
    px += 1;
    //py += 1;
    pa += 0.05;
    if (px >= 320) px = 0;
    if (py >= 240) py = 0;
    if (pa >= MIP_PI2)   pa -= MIP_PI2;
    do_widget_redraw(image1);

  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class test_editor2_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  enum myParameterEnums {
    PARAM1 = 0,
    PARAM2,
    PARAM3,
    PARAM_COUNT
  };

  const clap_param_info_t myParameters[PARAM_COUNT] = {
    { PARAM1, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "param1", "", 0, 1, 0 },
    { PARAM2, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "param2", "", -1, 1, 1 },
    { PARAM3, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED, nullptr, "param3", "", -12, 12, 0 }
  };

  const clap_audio_port_info_t myAudioInputPorts[1] = {
    { 0, "audio in 1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  const clap_audio_port_info_t myAudioOutputPorts[1] = {
    { 0, "audio out 1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  const clap_note_port_info_t myNoteInputPorts[1] = {
    { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "note in 1" }
  };

  const clap_note_port_info_t myNoteOutputPorts[1] = {
    { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "note out 1" }
  };

//------------------------------
public:
//------------------------------

  test_editor2_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    MEditorWidth = 500;
    MEditorHeight = 300;
  }

  //----------

  virtual ~test_editor2_plugin() {
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    appendAudioInputPort( &myAudioInputPorts[0] );
    appendAudioOutputPort(&myAudioOutputPorts[0]);
    appendNoteInputPort(  &myNoteInputPorts[0]  );
    appendNoteOutputPort( &myNoteOutputPorts[0] );
    for (uint32_t i=0; i<PARAM_COUNT; i++) {
      appendParameter( new MIP_Parameter(&myParameters[i]) );
    }

    MIP_Parameter* param4 = new MIP_Parameter(0,"param4","",-1,1,0,CLAP_PARAM_IS_AUTOMATABLE);
    appendParameter(param4);

    return true;
  }

  //bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
  //  return true;
  //}

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

  //void processAudioBlock(const clap_process_t* process) final { MIP_PRINT; }
  //void processTransport(const clap_event_transport_t* transport) final {}

//------------------------------
public: // gui
//------------------------------

  bool gui_create(const char *api, bool is_floating) override {
    MEditor = new test_editor2_editor(this,MEditorWidth,MEditorHeight);
    return true;
  }

  //----------

  void gui_destroy() override {
    delete MEditor;
  }

//  bool gui_show() override {
//    bool result = MIP_Plugin::gui_show();
//    return result;
//  }
//
//  bool gui_hide() override {
//    bool result = MIP_Plugin::gui_hide();
//    return result;
//  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#include "plugin/clap/mip_clap_entry.h"
#include "plugin/exe/mip_exe_entry.h"

#include "plugin/mip_registry.h"

//----------

void MIP_Register(MIP_Registry* ARegistry) {
  ARegistry->appendDescriptor(&template_descriptor);
};

//----------

MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  if (AIndex == 0) {
    return new test_editor2_plugin(ADescriptor,AHost);
  }
  return nullptr;
}

