
#define MIP_GUI_XCB
#define MIP_PAINTER_NANOVG

#define MIP_DEBUG_PRINT_SOCKET
// nc -U -l -k /tmp/mip.socket

//----------------------------------------------------------------------

#include "plugin/mip_plugin.h"

#include "sa_botage/sa_botage_editor.h"
#include "sa_botage/sa_botage_params.h"
#include "sa_botage/sa_botage_process.h"

#define EDITOR_WIDTH  500
#define EDITOR_HEIGHT 300

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
   .clap_version  = CLAP_VERSION,
   .id            = "skei.audio/sa_botage/0.0.11",
   .name          = "sa_botage",
   .vendor        = "skei.audio",
   .url           = "https://torhelgeskei.com",
   .manual_url    = "",
   .support_url   = "",
   .version       = "0.0.11",
   .description   = "",
   .features      =  (const char *[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_botage_plugin
: public MIP_Plugin
, public MIP_TimerListener {

//------------------------------
private:
//------------------------------

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

  MIP_Timer         MTimer    = MIP_Timer(this);
  sa_botage_process MProcess  = {};

//------------------------------
public:
//------------------------------

  sa_botage_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    MEditorWidth = EDITOR_WIDTH;
    MEditorHeight = EDITOR_HEIGHT;
  }

  //----------

  virtual ~sa_botage_plugin() {
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    appendAudioInputPort( &myAudioInputPorts[0] );
    appendAudioOutputPort(&myAudioOutputPorts[0]);
    appendNoteInputPort(  &myNoteInputPorts[0]  );
    appendNoteOutputPort( &myNoteOutputPorts[0] );
    for (uint32_t i=0; i<SA_BOTAGE_PARAM_COUNT; i++) {
      appendParameter( new MIP_Parameter(&sa_botage_params[i]) );
    }
    return true;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MProcess.activate(sample_rate);
    return true;
  }

  //----------

  //bool start_processing() final {
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

  //----------

  void processParamValueEvent(const clap_event_param_value_t* event) final {
    MProcess.setParamValue(event->param_id,event->value);
  }

  //----------

  //void processParamModEvent(const clap_event_param_mod_t* event) final {}
  //void processParamGestureBeginEvent(const clap_event_param_gesture_t* event) final {}
  //void processParamGestureEndEvent(const clap_event_param_gesture_t* event) final {}
  //void processTransportEvent(const clap_event_transport_t* event) final {}
  //void processMidiEvent(const clap_event_midi_t* event) final {}
  //void processMidiSysexEvent(const clap_event_midi_sysex_t* event) final {}
  //void processMidi2Event(const clap_event_midi2_t* event) final {}

  //----------

  void processTransport(const clap_event_transport_t* transport) final {
    MProcess.transport(transport->flags);

  }

  //----------

  void processAudioBlock(const clap_process_t* process) final {
    MProcess.processAudioBlock(process);

  }

//------------------------------
public: // gui
//------------------------------

  bool gui_create(const char *api, bool is_floating) override {
    // don't call MIP_Plugin::, we create the editor ourselves..
    MEditor = new sa_botage_editor(this,MEditorWidth,MEditorHeight);
    return true;
  }

  //----------

  void gui_destroy() override {
    // same..
    MTimer.stop();
    delete MEditor;
  }

  //----------

  bool gui_show() override {
    MTimer.start(20);
    return MIP_Plugin::gui_show();
  }

  //----------

  bool gui_hide() override {
    MTimer.stop();
    return MIP_Plugin::gui_hide();
  }

//------------------------------
public: // timer
//------------------------------

  // we (just) read from MProcess directly...
  //

  void on_timerCallback() override {
    sa_botage_editor* editor = (sa_botage_editor*)MEditor;
    editor->MWaveform->setBuffer(MProcess.MBuffer,MProcess.MBufferLength);
    editor->MWaveform->setNumGrid(MProcess.par_num_beats);
    editor->MWaveform->setNumSubGrid(MProcess.par_num_slices);

    editor->MWaveform->setMarkerActive(0,true);
    editor->MWaveform->setMarkerPos(0,MProcess.MWritePos);
    editor->MWaveform->setMarkerColor(0, MIP_Color(1,0,0,1) );
    editor->MWaveform->setMarkerWidth(0,1);

    editor->MWaveform->setMarkerActive(1,true);
    editor->MWaveform->setMarkerPos(1,MProcess.MReadPos);
    editor->MWaveform->setMarkerColor(1, MIP_Color(0,1,0,1) );
    editor->MWaveform->setMarkerWidth(1,1);

    editor->MWaveform->setAreaActive(0,true);
    editor->MWaveform->setAreaStart(0,MProcess.MCurrentSliceStart);
    editor->MWaveform->setAreaLength(0,MProcess.MCurrentSliceLength);
    editor->MWaveform->setAreaColor(0, MIP_Color(0,0,0,0.3) );

    editor->MWaveform->do_widget_redraw(editor->MWaveform);
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "plugin/mip_registry.h"
#include "plugin/clap/mip_clap_entry.h"
#include "plugin/exe/mip_exe_entry.h"
//#include "plugin/vst2/mip_vst2_entry.h"
//#include "plugin/vst3/mip_vst3_entry.h"

//----------

void MIP_Register(MIP_Registry* ARegistry) {
  ARegistry->appendDescriptor(&myDescriptor);
};

//----------

MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  if (AIndex == 0) {
    return new sa_botage_plugin(ADescriptor,AHost);
  }
  return nullptr;
}

