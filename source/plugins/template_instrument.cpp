
#define MIP_GUI_XCB
#define MIP_PAINTER_CAIRO
#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

//#define MIP_VOICE_USE_SLICES

#define NUM_PARAMETERS    1
#define NUM_AUDIO_OUTPUTS 1
#define NUM_NOTE_INPUTS   1
#define NUM_VOICES        16
#define EDITOR_WIDTH      640
#define EDITOR_HEIGHT     480

//----------------------------------------------------------------------

#include "mip.h"
#include "audio/mip_audio_math.h"
#include "audio/modulation/mip_envelope.h"
#include "gui/mip_widgets.h"
#include "plugin/mip_editor.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_voice_manager.h"


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* template_features[] = {
  "instrument",
  nullptr
};

//----------

const clap_plugin_descriptor_t template_descriptor = {
  CLAP_VERSION,
  "skei.audio/instrument_template",
  "instrument_template",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.1",
  "instrument_template description",
  template_features
};

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

class myVoice {

//------------------------------
public:
//------------------------------

  uint32_t  state = MIP_VOICE_OFF;
  MIP_Note  note  = {};

//------------------------------
private:
//------------------------------

  MIP_VoiceContext* context = nullptr;
  MIP_Envelope amp_env = {};

  int32_t note_key        = -1;

  float   note_volume     = 0.0;
  float   note_onvel      = 0.0;
  float   note_offvel     = 0.0;
  float   note_pan        = 0.0;
  float   note_tuning     = 0.0;
  float   note_vibrato    = 0.0;
  float   note_expression = 0.0;
  float   note_brightness = 0.0;
  float   note_pressure   = 0.0;

  float   hz              = 0.0;
  float   ph              = 0.0;
  float   phadd           = 0.0;

//------------------------------
public:
//------------------------------

  float getEnvLevel() {
    return amp_env.getValue();
  }

  //----------

  void prepare(MIP_VoiceContext* AContext) {
    context = AContext;
    amp_env.setSampleRate(context->samplerate);
    amp_env.setAttack(0);
    amp_env.setDecay(0);
    amp_env.setSustain(1);
    amp_env.setRelease(2);
  }

  //----------

  uint32_t note_on(int32_t key, float velocity) {
    ph = 0.0;
    note_key = key;
    note_onvel = velocity;
    note_tuning = 0.0;
    amp_env.noteOn();
    return MIP_VOICE_PLAYING;
  }

  //----------

  uint32_t note_off(float velocity) {
    note_offvel = velocity;
    amp_env.noteOff();
    return MIP_VOICE_RELEASED;
  }

  //----------

  void note_choke() {
  }

  //----------

  void volume(float amount)     { note_volume = amount; }
  void pan(float amount)        { note_pan = amount;}
  void tuning(float amount)     { note_tuning = amount; }
  void vibrato(float amount)    { note_vibrato = amount; }
  void expression(float amount) { note_expression = amount; }
  void brightness(float amount) { note_brightness = amount; }
  void pressure(float amount)   { note_pressure = amount; }

  //----------

  void parameter(uint32_t index, float value) {
    //switch (index) {
    //  case: break;
    //}
  }

  void modulation(uint32_t index, float value) {
    //switch (index) {
    //  case: break;
    //}
  }

  //----------

  // ASize  = 0..15
  // AState = MIP_VOICE_PLAYING/MIP_VOICE_RELEASED

  uint32_t process(uint32_t AIndex, uint32_t AState, uint32_t ASize, uint32_t AOffset) {

    hz = MIP_NoteToHz(note_key + note_tuning);
    phadd = hz * context->invsamplerate;

    float* output = context->voicebuffer;
    for (uint32_t i = 0; i < ASize; i++) {

      float t1 = ph + 0.5f;
      t1 = MIP_Fract(t1);
      float saw1 = 2.0 * t1 - 1.0;
      //saw1 -= MIP_PolyBlep(t1,phadd);

      float amp = note_onvel + note_pressure;
      amp = MIP_Clamp(amp,0,1);
      amp *= amp_env.process();
      //amp = vol_smoother.process(amp);

      float out = saw1;
      *output++ += (out * amp);

      ph += phadd;
      ph = MIP_Fract(ph);
    }

    if (amp_env.getStage() == MIP_ENVELOPE_FINISHED) return MIP_VOICE_FINISHED;
    else return AState;

  }

  //----------

  //uint32_t process(uint32_t AIndex,uint32_t AState) {
  //  return process(AIndex,AState,MIP_VOICE_SLICE_SIZE);
  //}

};

//----------------------------------------------------------------------

typedef MIP_VoiceManager<myVoice,NUM_VOICES>  myVoiceManager;

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

  myVoiceManager  MVoiceManager = {};

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

  clap_process_status process(const clap_process_t *process) final {
    flushAudioParams();
    handle_input_events(process->in_events,process->out_events);
    handle_process(process);
    handle_output_events(process->in_events,process->out_events);
    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  bool voice_info_get(clap_voice_info_t *info) final {
    info->voice_count     = NUM_VOICES;
    info->voice_capacity  = NUM_VOICES;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }

  //----------

  bool gui_create(const char *api, bool is_floating) final {
    MEditor = new MIP_Editor(this,this,MEditorDefaultWidth,MEditorDefaultHeight,true);
    if (MEditor) {
      MIP_Window* window = MEditor->getWindow();
        MIP_PanelWidget* panel = new MIP_PanelWidget(MIP_FRect());
        panel->setFillBackground(true);
        panel->setBackgroundColor(MIP_COLOR_GRAY);
        panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;

          MIP_Knob2Widget* knob = new MIP_Knob2Widget(MIP_FRect(50,80),"param");
          knob->layout.alignment = MIP_WIDGET_ALIGN_CENTER;
          panel->appendWidget(knob);

          MEditor->connect(knob,0);

      window->appendWidget(panel);
    }
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
    MVoiceManager.process(process);
    #endif
    //float v = MParameterValues[0];  // vol
    //float p = MParameterValues[1];  // pan
    //float l = v * (1.0 - p);
    //float r = v * (      p);
    //MIP_ScaleStereoBuffer(outputs,l,r,length);
  }

  //----------

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
  ARegistry->appendDescriptor(&template_descriptor);
}

//----------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    case 0: return new template_plugin(ADescriptor,AHost);
  }
  return nullptr;
}
