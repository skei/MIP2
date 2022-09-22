
//#define MIP_NO_GUI
//#define MIP_NO_PAINTER

//#define MIP_GUI_XCB
//#define MIP_PAINTER_XCB
//#define MIP_WINDOW_BUFFERED
//#define MIP_PAINTER_NANOVG

#define MIP_GUI_WIN32
#define MIP_PAINTER_GDI

//----------

#ifndef MIP_EXE
  #define MIP_DEBUG_PRINT_SOCKET
  // nc -U -l -k /tmp/mip.socket
#endif

//----------------------------------------------------------------------

#include "plugin/mip_plugin.h"

#ifndef MIP_NO_GUI
  #include "plugin/mip_editor.h"
  #include "gui/widgets/mip_widgets.h"
#endif

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
   .clap_version  = CLAP_VERSION,
   .id            = "MIP/Template/0",
   .name          = "Template",
   .vendor        = "MIP2",
   .url           = "https://github.com/skei/MIP2",
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

/*
#ifndef MIP_NO_GUI

class myEditor
: public MIP_Editor {

//------------------------------
public:
//------------------------------

  myEditor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight)
  : MIP_Editor(AListener,AWidth,AHeight) {
    setWindowFillBackground(false);
    MIP_ColorWidget* background = new MIP_ColorWidget( MIP_DRect( 0,0,500,400), MIP_COLOR_GRAY );
    appendChildWidget(background);
    background->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
  }

  //----------

  virtual ~myEditor() {
  }

};

#endif
*/

//----------------------------------------------------------------------
//
// gui
//
//----------------------------------------------------------------------

class myPlugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  enum myParameterEnums {
    PAR_LEFT = 0,
    PAR_RIGHT,
    PAR_COUNT
  };

  const clap_param_info_t myParameters[PAR_COUNT] = {
    { PAR_LEFT,  CLAP_PARAM_IS_AUTOMATABLE, nullptr, "Left",  "",  0, 1, 1 },
    { PAR_RIGHT, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "Right", "",  0, 1, 1 }
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
private:
//------------------------------

  double MLeft  = 0.0;
  double MRight = 0.0;

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {

    #ifndef MIP_NO_GUI
      MEditorWidth  = 300;
      MEditorHeight = 300;
    #endif

    MLeft  = 1.0;
    MRight = 1.0;

  }

  //----------

  virtual ~myPlugin() {
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    appendAudioInputPort( &myAudioInputPorts[0] );
    appendAudioOutputPort(&myAudioOutputPorts[0]);
    appendNoteInputPort(  &myNoteInputPorts[0]  );
    appendNoteOutputPort( &myNoteOutputPorts[0] );
    for (uint32_t i=0; i<PAR_COUNT; i++) {
      appendParameter( new MIP_Parameter(&myParameters[i]) );
    }
    return true;
  }

  //bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {}
  //bool start_processing() final {}

  //----------

  //void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {}
  //void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {}
  //void processEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {}

  //void processNoteOn(const clap_event_note_t* event) final {}
  //void processNoteOff(const clap_event_note_t* event) final {}
  //void processNoteChoke(const clap_event_note_t* event) final {}
  //void processNoteEnd(const clap_event_note_t* event) final {}
  //void processNoteExpression(const clap_event_note_expression_t* event) final {}

  void processParamValue(const clap_event_param_value_t* event) final {
    double v2 = event->value * event->value;
    switch(event->param_id) {
      case PAR_LEFT:  MLeft  = v2; break;
      case PAR_RIGHT: MRight = v2; break;
    }
  }

  //void processParamMod(const clap_event_param_mod_t* event) final {}
  //void processParamGestureBegin(const clap_event_param_gesture_t* event) final {}
  //void processParamGestureEnd(const clap_event_param_gesture_t* event) final {}
  //void processTransport(const clap_event_transport_t* event) final {}
  //void processMidi(const clap_event_midi_t* event) final {}
  //void processMidiSysex(const clap_event_midi_sysex_t* event) final {}
  //void processMidi2(const clap_event_midi2_t* event) final {}

  //----------

  void processAudioBlock(MIP_ProcessContext* AContext) final {
    const clap_process_t* process = AContext->process;
    uint32_t len = process->frames_count;
    float* in0 = process->audio_inputs[0].data32[0];
    float* in1 = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<len; i++) {
      //*out0++ = *in0++ * MParameters[PAR_LEFT ]->getValue();
      //*out1++ = *in1++ * MParameters[PAR_RIGHT]->getValue();
      *out0++ = *in0++ * MLeft;
      *out1++ = *in1++ * MRight;
    }
  }

//------------------------------
public: // gui
//------------------------------

  #ifndef MIP_NO_GUI

  bool gui_create(const char *api, bool is_floating) final {
    //MEditor = new myEditor(this,MEditorWidth,MEditorHeight);
    bool result = MIP_Plugin::gui_create(api,is_floating);
    if (result) {

      MIP_Window* window = MEditor->getWindow();

      //window->setWindowFillBackground(true);

      MIP_ColorWidget* background = new MIP_ColorWidget( MIP_DRect( 0,0,500,400), MIP_COLOR_RED );
      background->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
      window->appendChildWidget(background);

      MIP_Knob2Widget* knob1 = new MIP_Knob2Widget( MIP_DRect(10,10,40,60), "Left", 0.0 );
      background->appendChildWidget(knob1);
      MEditor->connectWidget(MParameters[0],knob1);

      MIP_Knob2Widget* knob2 = new MIP_Knob2Widget( MIP_DRect(60,10,40,60), "Right", 0.0 );
      background->appendChildWidget(knob2);
      MEditor->connectWidget(MParameters[1],knob2);

    }
    return result;
  }

  //----------

  //void gui_destroy() override {
  //  //delete MEditor;
  //  MIP_Plugin::gui_destroy();
  //
  //}

  #endif

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#ifndef MIP_NO_ENTRY

  #include "plugin/clap/mip_clap_entry.h"
  //#include "plugin/exe/mip_exe_entry.h"
  #include "plugin/vst2/mip_vst2_entry.h"
  #include "plugin/vst3/mip_vst3_entry.h"

  //----------

  void MIP_Register() {
    MIP_REGISTRY.appendDescriptor(&myDescriptor);
  };

  //----------

  MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
    if (AIndex == 0) {
      return new myPlugin(ADescriptor,AHost);
    }
    return nullptr;
  }

//#endif
