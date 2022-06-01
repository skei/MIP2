
#define MIP_GUI_XCB
#define MIP_PAINTER_CAIRO
#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

//#define MIP_VOICE_USE_SLICES

#define NUM_PARAMETERS    1
#define NUM_AUDIO_OUTPUTS 1
#define NUM_NOTE_INPUTS   1
#define NUM_VOICES        16
//#define EDITOR_WIDTH      640
//#define EDITOR_HEIGHT     480

//----------------------------------------------------------------------

#include "plugin/mip_plugin.h"

#include "plugin/mip_editor.h"
#include "gui/mip_widgets.h"

#include "audio/mip_audio_math.h"
#include "audio/modulation/mip_envelope.h"
#include "plugin/mip_voice_manager.h"

//----------

#include "sa_tyr/sa_tyr_voice.h"
#include "sa_tyr/sa_tyr_editor.h"
#include "sa_tyr/sa_tyr_widgets.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const char* sa_tyr_features[] = {
  "instrument",
  nullptr
};

//----------

const clap_plugin_descriptor_t sa_tyr_descriptor = {
  CLAP_VERSION,
  "skei.audio/sa_tyr",
  "sa_tyr",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.1",
  "sa_tyr...",
  sa_tyr_features
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

  bool need_recalc = false;
  float MSampleRate = 0.0;

  clap_param_info_t myParameters[NUM_PARAMETERS] = {
    { 0, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "param1", "", 0.0, 1.0, 0.5 }
  };

  clap_audio_port_info_t myAudioOutputs[NUM_AUDIO_OUTPUTS] = {
    { 0, "Audio Out", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  clap_note_port_info_t myNoteInputs[NUM_NOTE_INPUTS] = {
    { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "Note In" }
  };

  //----------

  MIP_VoiceManager<sa_tyr_voice,NUM_VOICES> MVoiceManager;


//------------------------------
public:
//------------------------------

  sa_tyr_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    MEditorDefaultWidth   = 800;
    MEditorDefaultHeight  = 600;
  }

  //----------

  virtual ~sa_tyr_plugin() {
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    setupParameters(myParameters,NUM_PARAMETERS);
    setupAudioOutputs(myAudioOutputs,NUM_AUDIO_OUTPUTS);
    setupNoteInputs(myNoteInputs,NUM_NOTE_INPUTS);
    need_recalc = true;
    return MIP_Plugin::init();
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MSampleRate = sample_rate;
    MVoiceManager.prepareVoices(sample_rate);
    // send initial parameter values to the voices
    for (uint32_t i=0; i<NUM_PARAMETERS; i++) {
      float v = MParameterValues[i];
      MVoiceManager.setParameter(i,v);
    }
    return MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  const void* get_extension(const char *id) final {
    if (strcmp(id,CLAP_EXT_GUI) == 0)         return &MGui;
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0) return &MAudioPorts;
    if (strcmp(id,CLAP_EXT_NOTE_PORTS) == 0)  return &MNotePorts;
    if (strcmp(id,CLAP_EXT_VOICE_INFO) == 0)  return &MVoiceInfo;
    return MIP_Plugin::get_extension(id);
  }

  //----------

  //clap_process_status process(const clap_process_t *process) final {
  //  flushAudioParams();
  //  handle_input_events(process->in_events,process->out_events);
  //  handle_process(process);
  //  handle_output_events(process->in_events,process->out_events);
  //  return CLAP_PROCESS_CONTINUE;
  // }

  //----------

  bool voice_info_get(clap_voice_info_t *info) final {
    info->voice_count     = NUM_VOICES;
    info->voice_capacity  = NUM_VOICES;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }

  //----------

  bool gui_create(const char *api, bool is_floating) final {
    MEditor = new sa_tyr_editor(this,this,MEditorDefaultWidth,MEditorDefaultHeight,true,&sa_tyr_descriptor);
    return (MEditor);
  }

//------------------------------
private:
//------------------------------

  //void precalc() {
  //}

  //----------

  void recalc(float srate) {
    need_recalc = false;
  }

//------------------------------
private:
//------------------------------

  void handle_parameter_event(clap_event_param_value_t* param_value) final {
    MIP_Plugin::handle_parameter_event(param_value);
    need_recalc = true;
  }

  //----------

  void handle_process(const clap_process_t *process) final {
    float** outputs = process->audio_outputs[0].data32;
    uint32_t length = process->frames_count;
    MIP_ClearStereoBuffer(outputs,length);
    #ifdef MIP_VOICE_USE_SLICES
    MVoiceManager.processSlices(process);
    #else
    MVoiceManager.processBlock(process);
    #endif
    //float v = MParameterValues[0];  // vol
    //float p = MParameterValues[1];  // pan
    //float l = v * (1.0 - p);
    //float r = v * (      p);
    //MIP_ScaleStereoBuffer(outputs,l,r,length);
  }

  //----------


  //void on_editor_listener_parameter(uint32_t AIndex, double AValue) overrude {}

  void handle_editor_parameter(uint32_t AIndex, float AValue) final {
    MVoiceManager.setParameter(AIndex,AValue);
  }

};

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

void MIP_Register(MIP_ClapRegistry* ARegistry) {
  ARegistry->appendDescriptor(&sa_tyr_descriptor);
}

//----------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    case 0: return new sa_tyr_plugin(ADescriptor,AHost);
  }
  return nullptr;
}
