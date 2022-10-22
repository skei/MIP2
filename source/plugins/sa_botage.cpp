
#ifdef __gnu_linux__
  #define MIP_GUI_XCB
  #define MIP_PAINTER_NANOVG
  //#define MIP_PAINTER_XCB
  //#define MIP_WINDOW_BUFFERED
#else
  #define MIP_GUI_WIN32
  #define MIP_PAINTER_NANOVG
  //#define MIP_PAINTER_GDI
  //#define MIP_PAINTER_WGL
#endif

// nc -U -l -k /tmp/mip.socket

#ifndef MIP_EXE
  #define MIP_DEBUG_PRINT_SOCKET
#endif

//----------------------------------------------------------------------

#define EDITOR_WIDTH  550
#define EDITOR_HEIGHT 630

#include "plugin/mip_plugin.h"

#include "sa_botage/sa_botage_editor.h"
#include "sa_botage/sa_botage_params.h"
#include "sa_botage/sa_botage_process.h"


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
/*, public MIP_TimerListener*/ {

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
      const clap_param_info_t* param_info = &sa_botage_params[i];
      MIP_Parameter* parameter = new MIP_Parameter(param_info);
      //MIP_Print("param %i: %s\n",i,parameter->getName());
      appendParameter(parameter);
    }
    setDefaultParameterValues();
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

  void processParamValue/*Event*/(const clap_event_param_value_t* event) final {
    //MIP_Plugin::processParamValueEvent(event);
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
    MIP_Plugin::processTransport(transport);
    MProcess.transport(transport->flags);

  }

  //----------

  void processAudioBlock(MIP_ProcessContext* AContext) final {
    MProcess.processAudioBlock(AContext);
  }

//------------------------------
public: // gui
//------------------------------

  bool gui_create(const char *api, bool is_floating) override {
    //MIP_Print("\n");
    MEditor = new sa_botage_editor(this,MEditorWidth,MEditorHeight,MParameters);
    return true;
  }

  //----------

  void gui_destroy() override {
    //MIP_Print("\n");
    gui_hide(); // stops timer, etc..
    delete MEditor; // delete (sa_botage_editor*)MEditor;
    MEditor = nullptr;
  }

  //----------

//  bool gui_show() override {
//    MTimer.start(20);
//    return MIP_Plugin::gui_show();
//  }

  //----------

//  bool gui_hide() override {
//    MTimer.stop();
//    return MIP_Plugin::gui_hide();
//  }

//------------------------------
public: // timer
//------------------------------

  // we read from MProcess directly... :-/

  void on_timerCallback(MIP_Timer* ATimer) override {
    // we override MIP_Plugin timer callback, so be sure to call the
    // original, so the gui is properly updated, etc..
    MIP_Plugin::on_timerCallback(ATimer); // flush queues
    if (ATimer == &MGuiTimer) {
      //MIP_Plugin::on_timerCallback(ATimer); // flush queues
      sa_botage_editor* editor = (sa_botage_editor*)MEditor;
      if (editor) editor->timer_update(&MProcess);
    }
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
#include "plugin/vst2/mip_vst2_entry.h"
#include "plugin/vst3/mip_vst3_entry.h"

//----------

MIP_DEFAULT_ENTRY(myDescriptor,sa_botage_plugin)

//void MIP_Register() {
//  MIP_REGISTRY.appendDescriptor(&myDescriptor);
//};
//
////----------
//
//MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
//  if (AIndex == 0) {
//    return new sa_botage_plugin(ADescriptor,AHost);
//  }
//  return nullptr;
//}

