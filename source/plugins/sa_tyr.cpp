
#ifdef __gnu_linux__
  #define MIP_GUI_XCB
#else
  #define MIP_GUI_WIN32
#endif

#define MIP_PAINTER_NANOVG

//#define MIP_PLUGIN_GENERIC_EDITOR

#ifndef MIP_EXE
  #define MIP_DEBUG_PRINT_SOCKET
  // nc -U -l -k /tmp/mip.socket
#endif

//----------------------------------------------------------------------

#include "audio/mip_audio_utils.h"

#include "plugin/mip_plugin.h"
#include "plugin/mip_note.h"
#include "plugin/mip_voice_manager.h"
#include "plugin/mip_voice.h"
#include "plugin/mip_editor.h"

#include "gui/widgets/mip_widgets.h"

#include "sa_tyr/sa_tyr_voice.h"

//----------------------------------------------------------------------

#define EDITOR_WIDTH  420
#define EDITOR_HEIGHT 620

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_tyr_descriptor = {
  .clap_version  = CLAP_VERSION,
  .id            = "skei.audio/sa_tyr/0",
  .name          = "sa_tyr",
  .vendor        = "skei.audio",
  .url           = "https://torhelgeskei.com",
  .manual_url    = "",
  .support_url   = "",
  .version       = "0.0.1",
  .description   = "",
  .features      = (const char*[]){ CLAP_PLUGIN_FEATURE_INSTRUMENT, nullptr }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_tyr_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  __MIP_ALIGNED(MIP_ALIGNMENT_CACHE)
  MIP_VoiceManager<sa_tyr_voice,16> MVoices = {};

  //---------------
  // parameters
  //---------------

  enum gain_parameter_enums {
    PAR_GAIN = 0,
    PARAM_COUNT
  };

  const clap_param_info_t gain_parameters[PARAM_COUNT] = {
    { PAR_GAIN, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "Gain", "", 0, 1, 0 }
  };

  //---------------
  // ports
  //---------------

  const clap_audio_port_info_t gain_audioInputPorts[1] = {
    { 0, "audio in 1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  const clap_audio_port_info_t gain_audioOutputPorts[1] = {
    { 0, "audio out 1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  const clap_note_port_info_t gain_noteInputPorts[1] = {
    { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "note in 1" }
  };

  const clap_note_port_info_t gain_noteOutputPorts[1] = {
    { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "note out 1" }
  };

  //---------------
  //
  //---------------

  double par_gain = 0.0;

//------------------------------
public:
//------------------------------

  sa_tyr_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    MEditorWidth = EDITOR_WIDTH;
    MEditorHeight = EDITOR_HEIGHT;
  }

  //----------

  virtual ~sa_tyr_plugin() {
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    bool result = MIP_Plugin::init();
    appendAudioInputPort( &gain_audioInputPorts[0] );
    appendAudioOutputPort(&gain_audioOutputPorts[0]);
    appendNoteInputPort(  &gain_noteInputPorts[0]  );
    appendNoteOutputPort( &gain_noteOutputPorts[0] );
    for (uint32_t i=0; i<PARAM_COUNT; i++) {
      appendParameter( new MIP_Parameter(&gain_parameters[i]) );
    }
    return result;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MVoices.setup(sample_rate,min_frames_count,max_frames_count,&MParameters);
    return true;
  }

//------------------------------
public: // gui
//------------------------------

  #ifndef MIP_PLUGIN_GENERIC_EDITOR

  bool gui_create(const char *api, bool is_floating) override {
    MIP_Plugin::gui_create(api,is_floating);

    MEditor->setCanResizeEditor(true);
    //MEditor->setResizeProportional(false);
    //MEditor->setProportionalSize(EDITOR_WIDTH,EDITOR_HEIGHT);

    MIP_Window* window = MEditor->getWindow();

    MIP_ColorWidget* background = new MIP_ColorWidget( MIP_DRect(EDITOR_WIDTH,EDITOR_HEIGHT), MIP_COLOR_RED );
    window->appendChildWidget(background);
    background->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    background->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      //background->Layout.border = MIP_DRect(10,10,10,10);
      MIP_Knob2Widget* gain_knob = new MIP_Knob2Widget( MIP_DRect(10,10,EDITOR_WIDTH-20,EDITOR_HEIGHT-20), "Gain", 0.0 );
      background->appendChildWidget(gain_knob);
      //gain_knob->Layout.alignment = MIP_WIDGET_ALIGN_PARENT;
      gain_knob->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      gain_knob->Layout.aspectRatio = (4.0 / 6.0);
    MEditor->connectWidget(MParameters[PAR_GAIN],gain_knob);

    return true;
  }

  #endif

//------------------------------
public:
//------------------------------


//------------------------------
public:
//------------------------------

  void processNoteOn(const clap_event_note_t* event) final {
    MVoices.handleNoteOn(event);
  }

  void processNoteOff(const clap_event_note_t* event) final {
    MVoices.handleNoteOff(event);
  }

  void processNoteChoke(const clap_event_note_t* event) final {
    MVoices.handleNoteChoke(event);
  }

  void processNoteExpression(const clap_event_note_expression_t* event) final {
    MVoices.handleNoteExpression(event);
  }

  //----------

  void processParamValue(const clap_event_param_value_t* event) final {
    MVoices.handleParamValue(event);
  }

  void processParamMod(const clap_event_param_mod_t* event) final {
    MVoices.handleParamMod(event);
  }

  //----------

  //void processParamGestureBegin(const clap_event_param_gesture_t* event) final {
  //}

  //void processParamGestureEnd(const clap_event_param_gesture_t* event) final {
  //}

  //----------

  void processTransport(const clap_event_transport_t* event) final {
  }

  //----------

  void processMidi(const clap_event_midi_t* event) final {
    MVoices.handleMidi(event);
  }

  void processMidiSysex(const clap_event_midi_sysex_t* event) final {
    MVoices.handleMidiSysex(event);
  }

  void processMidi2(const clap_event_midi2_t* event) final {
    MVoices.handleMidi2(event);
  }

//------------------------------
public: // events
//------------------------------

  void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
    //handleParamValueModEvents(in_events);
    MVoices.preProcessEvents(in_events,out_events);
  }

  //----------

  //#ifndef MIP_NO_GUI

  void processEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {

    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {

        switch (header->type) {

          case CLAP_EVENT_PARAM_VALUE: {
            const clap_event_param_value_t* event = (const clap_event_param_value_t*)header;
            uint32_t index = event->param_id;
            double value = event->value;
            setParameterValue(index,value);
            #ifndef MIP_NO_GUI
            queueGuiParam(index,value);
            #endif
            break;
          }

          case CLAP_EVENT_PARAM_MOD: {
            const clap_event_param_mod_t* event = (const clap_event_param_mod_t*)header;
            uint32_t index = event->param_id;
            double value = event->amount;
            setParameterModulation(index,value);
            #ifndef MIP_NO_GUI
            queueGuiMod(index,value);
            #endif
            break;
          }

        } // switch

      }
    }

  }

  //#endif // MIP_NO_GUI

  //----------

  void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
    MVoices.postProcessEvents(in_events,out_events);
  }

//------------------------------
public: // process
//------------------------------

  void processAudioBlock(MIP_ProcessContext* AContext) final {
    MVoices.processAudioBlock(AContext);
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#include "plugin/clap/mip_clap_entry.h"
#include "plugin/exe/mip_exe_entry.h"
//#include "plugin/vst2/mip_vst2_entry.h"
//#include "plugin/vst3/mip_vst3_entry.h"

//----------

MIP_DEFAULT_ENTRY(sa_tyr_descriptor,sa_tyr_plugin);
