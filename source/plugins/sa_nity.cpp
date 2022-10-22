
#define MIP_GUI_XCB
#define MIP_PAINTER_NANOVG

#ifndef MIP_EXE
  #define MIP_DEBUG_PRINT_SOCKET
  // nc -U -l -k /tmp/mip.socket
#endif

//----------------------------------------------------------------------

#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"
#include "gui/widgets/mip_widgets.h"

#include "sa_nity/sa_nity_editor.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_nity_descriptor = {
   .clap_version  = CLAP_VERSION,
   .id            = "skei.audio/sa_nity/0",
   .name          = "sa_nity",
   .vendor        = "skei.audio",
   .url           = "https://torhelgeskei.com/sa",
   .manual_url    = "",
   .support_url   = "",
   .version       = "0.0.0",
   .description   = "",
   .features      =  (const char *[]){ CLAP_PLUGIN_FEATURE_INSTRUMENT, nullptr }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_nity_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  enum sa_nity_parameter_enums {
    SA_NITY_PARAM1 = 0,
    SA_NITY_PARAM2,
    SA_NITY_PARAM3,
    SA_NITY_PARAM_COUNT
  };

  const clap_param_info_t sa_nity_parameters[SA_NITY_PARAM_COUNT] = {
    { SA_NITY_PARAM1, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "param1", "", 0, 1, 0 },
    { SA_NITY_PARAM2, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "param2", "", -1, 1, 1 },
    { SA_NITY_PARAM3, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED, nullptr, "param3", "", -12, 12, 0 }
  };

  const clap_audio_port_info_t sa_nity_audio_input_ports[1] = {
    { 0, "audio in 1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  const clap_audio_port_info_t sa_nity_audio_output_ports[1] = {
    { 0, "audio out 1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  const clap_note_port_info_t sa_nity_note_input_ports[1] = {
    { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "note in 1" }
  };

  const clap_note_port_info_t sa_nity_note_output_ports[1] = {
    { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "note out 1" }
  };

//------------------------------
public:
//------------------------------

  sa_nity_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    MEditorWidth  = 640;
    MEditorHeight = 480;
  }

  //----------

  virtual ~sa_nity_plugin() {
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    appendAudioInputPort( &sa_nity_audio_input_ports[0] );
    appendAudioOutputPort(&sa_nity_audio_output_ports[0]);
    appendNoteInputPort(  &sa_nity_note_input_ports[0]  );
    appendNoteOutputPort( &sa_nity_note_output_ports[0] );
    for (uint32_t i=0; i<SA_NITY_PARAM_COUNT; i++) {
      appendParameter( new MIP_Parameter(&sa_nity_parameters[i]) );
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
  //void processParamValue(const clap_event_param_value_t* event) final {}
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
      *out0++ = *in0++;
      *out1++ = *in1++;
    }
  }

//------------------------------
public: // gui
//------------------------------

  bool gui_create(const char *api, bool is_floating) override {
    MEditor = new sa_nity_editor(this,MEditorWidth,MEditorHeight);
    return (MEditor);
    //bool result = MIP_Plugin::gui_create(api,is_floating);
    //if (result) {
    //  MEditor->setWindowFillBackground(false);
    //  MIP_PanelWidget* background = new MIP_PanelWidget(0);
    //  background->Layout.alignment = MIP_WIDGET_ALIGN_FILL_PARENT;
    //  MEditor->appendChildWidget(background);
    //}
    //return result;
  }

  //----------

  //void gui_destroy() override {
  //  //delete MEditor;
  //  MIP_Plugin::gui_destroy();
  //
  //}

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef MIP_NO_ENTRY

  #include "plugin/mip_registry.h"
  #include "plugin/clap/mip_clap_entry.h"
  #include "plugin/exe/mip_exe_entry.h"
  //#include "plugin/vst2/mip_vst2_entry.h"
  //#include "plugin/vst3/mip_vst3_entry.h"

  //----------

  void MIP_Register() {
    MIP_REGISTRY.appendDescriptor(&sa_nity_descriptor);
  };

  //----------

  MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
    if (AIndex == 0) {
      return new sa_nity_plugin(ADescriptor,AHost);
    }
    return nullptr;
  }

#endif
