
//#define MIP_NO_ENTRY

#ifndef MIP_EXE

#define MIP_DEBUG_PRINT_SOCKET
// nc -U -l -k /tmp/mip.socket

#endif // MIP_EXE

//----------------------------------------------------------------------

#define MIP_GUI_XCB
#define MIP_PAINTER_NANOVG

#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"

#include "gui/widgets/mip_widgets.h"

//----------------------------------------------------------------------
//
//
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
//
//
//----------------------------------------------------------------------

class test_editor2_editor
: public MIP_Editor {

public:

  test_editor2_editor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight)
  : MIP_Editor(AListener,AWidth,AHeight) {
    setup_widgets(AWidth,AHeight);
    setWindowFillBackground(false);
  }

  virtual ~test_editor2_editor() {
  }

//------------------------------
public:
//------------------------------

  void setup_widgets(uint32_t AWidth, uint32_t AHeight) {
    // background
    MIP_ColorWidget* background = new MIP_ColorWidget( MIP_DRect( 0,0,500,400), MIP_COLOR_GRAY );
    appendChildWidget(background);
    background->Layout.alignment = MIP_WIDGET_ALIGN_CLIENT;

    #define H  22
    #define H2 25

    MIP_ColorWidget*      color1      = new MIP_ColorWidget(      MIP_DRect(  10, 10 + (0 * H2), 150, H ), MIP_COLOR_RED );
    MIP_PanelWidget*      panel1      = new MIP_PanelWidget(      MIP_DRect(  10, 10 + (1 * H2), 150, H ) );
    MIP_TextWidget*       text1       = new MIP_TextWidget(       MIP_DRect(  10, 10 + (2 * H2), 150, H ), "Text" );
    MIP_ValueWidget*      value1      = new MIP_ValueWidget(      MIP_DRect(  10, 10 + (3 * H2), 150, H ), "Value", 0.3 );
    MIP_DragValueWidget*  dragvalue1  = new MIP_DragValueWidget(  MIP_DRect(  10, 10 + (4 * H2), 150, H ), "DragValue", 0.3 );
    MIP_SliderWidget*     slider1     = new MIP_SliderWidget(     MIP_DRect(  10, 10 + (5 * H2), 150, H ), "Slider", 0.3 );
    MIP_ButtonWidget*     button1     = new MIP_ButtonWidget(     MIP_DRect(  10, 10 + (6 * H2), 150, H ), "On", "Off", 0 );
    MIP_KnobWidget*       knob1       = new MIP_KnobWidget(       MIP_DRect( 170, 10, 40,40), "Knob", 0.3 );

    #undef H
    #undef H2

    background->appendChildWidget(knob1);
    knob1->setFillBackground(false);
    knob1->setDrawBorder(false);
    button1->setTextColor(MIP_COLOR_BLACK);

    background->appendChildWidget(color1);
    background->appendChildWidget(panel1);
    background->appendChildWidget(text1);
    background->appendChildWidget(value1);
    background->appendChildWidget(dragvalue1);
    background->appendChildWidget(slider1);
    background->appendChildWidget(button1);


  }

};

//----------------------------------------------------------------------
//
//
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

  void processTransport(const clap_event_transport_t* transport) final {}

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


};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef MIP_NO_ENTRY

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

#endif

