
/*
  todo / maybe-do:
  * parameter/modulation dezipping
  * polyphonic parameter modulation (should in theory already work, but can't test it)
  * note off velocity, timbre expression
  * examine -faligned=new (slice/cache processing, buffers)
  * clap extensions: quick controls, note ports (no host suppors it yet)
  * better synthesis? (probably not)
  * trim down event handling (handle_input_events)
  * zoomable editor (widget scaling and bitmap stretching is in there, but need to connect everything together)
*/

//----------------------------------------------------------------------

#define MIP_GUI_XCB
#define MIP_PAINTER_CAIRO

#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

//----------

#include "mip.h"
#include "audio/mip_voice_manager.h"
#include "audio/mip_audio_math.h"
#include "audio/filters/mip_svf_filter.h"
#include "audio/modulation/mip_envelope.h"
#include "audio/waveforms/mip_polyblep_waveform.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"
#include "gui/mip_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define NUM_PARAMS        12
#define NUM_AUDIO_INPUTS  0
#define NUM_AUDIO_OUTPUTS 2
#define NUM_NOTE_INPUTS   1
#define NUM_NOTE_OUTPUTS  0

#define NUM_VOICES        16
#define EDITOR_WIDTH      (270 + 150)
#define EDITOR_HEIGHT     (296 + 60)

//#define SUPPORTED_DIALECTS (CLAP_NOTE_DIALECT_CLAP | CLAP_NOTE_DIALECT_MIDI | CLAP_NOTE_DIALECT_MIDI_MPE | CLAP_NOTE_DIALECT_MIDI2)

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const char* myFeatures[] = {
  "instrument",
  nullptr
};

//----------

const clap_plugin_descriptor_t myDescriptor = {
  CLAP_VERSION,
  "skei.audio/test_synth1",
  "test_synth1",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.1",
  "simple mip2 test synth",
  myFeatures
};

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

class myVoice {

//------------------------------
private:
//------------------------------

  MIP_VoiceContext*   context   = nullptr;
  MIP_SvfFilter       filter    = {};
  MIP_ExpAdsrEnvelope amp_env  = {};
  MIP_ExpAdsrEnvelope flt_env  = {};

  float   filter_freq   = 0.5;
  float   filter_res    = 0.5;

  float   filter_freq_mod = 0.0;
  float   filter_res_mod  = 0.0;

  float   hz      = 0.0;  // note hz
  float   ph      = 0.0;  // phase
  float   phadd   = 0.0;  // phase add

  int32_t note_key    = -1;
  float   note_onvel  = 0.0;
  float   note_offvel = 0.0;
  float   note_vol    = 0.0;
  float   note_pan    = 0.0;
  float   note_vibr   = 0.0;
  float   note_expr   = 0.0;
  float   note_bright = 0.0;
  float   note_press  = 0.0;


//------------------------------
public:
//------------------------------

  void prepare(MIP_VoiceContext* AContext) {
    context = AContext;
    amp_env.setSampleRate(context->samplerate);
    flt_env.setSampleRate(context->samplerate);
  }

  uint32_t note_on(int32_t key, float velocity) {
    note_key = key;
    note_onvel = velocity;
    ph = 0.0;
    hz = MIP_NoteToHz(key);
    phadd = hz / context->samplerate;
    amp_env.noteOn();
    flt_env.noteOn();
    return MIP_VOICE_PLAYING;
  }

  uint32_t note_off(float velocity) {
    note_offvel = velocity;
    amp_env.noteOff();
    flt_env.noteOff();
    return MIP_VOICE_RELEASED;
  }

  void note_choke() {
  }

  void note_end() {
  }

  void tuning(float amount) {
    hz = MIP_NoteToHz(note_key + amount);
    phadd = hz / context->samplerate;
  }

  void volume(float amount) {
    note_vol = amount;
  }

  void pan(float amount) {
    note_pan = amount;
  }

  void vibrato(float amount) {
    note_vibr = amount;
  }

  void expression(float amount) {
    note_expr = amount;
  }

  void brightness(float amount) {
    note_bright = amount;
  }

  void pressure(float amount) {
    note_press = amount;
  }

  void parameter(uint32_t index, float value) {
    switch (index) {
      case  2:  filter_freq = value;            break;
      case  3:  filter_res = value;             break;
      case  4:  amp_env.setAttack(value * 5);   break;
      case  5:  amp_env.setDecay(value * 5);    break;
      case  6:  amp_env.setSustain(value);      break;
      case  7:  amp_env.setRelease(value * 5);  break;
      case  8:  flt_env.setAttack(value * 5);   break;
      case  9:  flt_env.setDecay(value * 5);    break;
      case 10:  flt_env.setSustain(value);      break;
      case 11:  flt_env.setRelease(value * 5);  break;
    }
  }

  void modulation(uint32_t index, float value) {
    switch (index) {
      case  2:  filter_freq_mod = value;  break;
      case  3:  filter_res_mod = value;   break;
    }
  }

  // ASize  = 0..15
  // AState = MIP_VOICE_PLAYING/MIP_VOICE_RELEASED

  uint32_t process(uint32_t AState, uint32_t ASize) {
    float* output = MIP_VoiceSliceBuffer;
    for (uint32_t i = 0; i < ASize; i++) {
      float t = ph + 0.5f;
      t = MIP_Fract(t);
      float o = 2.0 * t - 1.0;
      o -= MIP_PolyBlep(t,phadd);
      ph += phadd;
      ph = MIP_Fract(ph);
      float ae = amp_env.process();
      float fe = flt_env.process();
      float ff = MIP_Clamp(filter_freq + filter_freq_mod, 0,1);
      float fr = MIP_Clamp(filter_res + filter_res_mod, 0,1);
      ff *= fe;
      filter.setMode(MIP_SVF_LP);
      filter.setFreq(ff * ff);
      filter.setBW(1.0 - fr);
      o = filter.process(o);
      o *= (note_onvel + note_press);
      o *= ae;
      *output++ += o;

    }
    if (amp_env.getStage() == MIP_ENVELOPE_FINISHED) return MIP_VOICE_FINISHED;
    //else if (flt_env.getStage() == MIP_ENVELOPE_FINISHED) return MIP_VOICE_FINISHED;
    else return AState;
  }

  uint32_t process(uint32_t AState) {
    return process(AState,MIP_VOICE_SLICE_SIZE);
  }

};

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class myEditor
: public MIP_Editor {

//------------------------------
public:
//------------------------------

  myEditor(MIP_EditorListener* AListener, MIP_ClapPlugin* APlugin, uint32_t AWidth, uint32_t AHeight, bool AEmbedded)
  : MIP_Editor(AListener,APlugin,AWidth,AHeight,AEmbedded) {

    MIP_Window* window = getWindow();

    // panel
    MIP_PanelWidget* MEditorPanel = new MIP_PanelWidget(MIP_FRect());
    MEditorPanel->setDrawBorder(false);
    MEditorPanel->setFillBackground(false);
    //MEditorPanel->setBackgroundColor(0.6);
    MEditorPanel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    //MEditorPanel->layout.innerBorder = MIP_FRect(10,10,10,10);
    //MEditorPanel->layout.spacing = 5;

    // header

    MIP_SAHeaderWidget* sa_header = new MIP_SAHeaderWidget(60,window);
    MEditorPanel->appendWidget(sa_header);
    sa_header->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    sa_header->setPluginName("test_synth1");
    sa_header->setPluginVersion("v0.0.1");

    // controls panel (knobs)

    MIP_PanelWidget* controls = new MIP_PanelWidget(MIP_FRect());
    MEditorPanel->appendWidget(controls);
    controls->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    controls->setDrawBorder(false);
    controls->setFillBackground(true);
    controls->setBackgroundColor(0.6);
    controls->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    //controls->layout.innerBorder = MIP_FRect(10,10,10,10);
    //controls->layout.spacing = 5;

    //

      // vol
      MIP_Knob2Widget* vol_knob = new MIP_Knob2Widget( MIP_FRect(10,10,50,82),"Vol");
      controls->appendWidget(vol_knob);
      connect(vol_knob,0);

      // pan
      MIP_Knob2Widget* pan_knob = new MIP_Knob2Widget( MIP_FRect(70,10,50,82),"Pan");
      controls->appendWidget(pan_knob);
      connect(pan_knob,1);

      // freq
      MIP_Knob2Widget* freq_knob = new MIP_Knob2Widget( MIP_FRect(130,10,50,82),"Freq");
      controls->appendWidget(freq_knob);
      connect(freq_knob,2);

      // res
      MIP_Knob2Widget* res_knob = new MIP_Knob2Widget( MIP_FRect(190,10,50,82),"Res");
      controls->appendWidget(res_knob);
      connect(res_knob,3);

      //

      // ampl att
      MIP_Knob2Widget* amp_att_knob = new MIP_Knob2Widget( MIP_FRect(10,102,50,82),"A.Att");
      controls->appendWidget(amp_att_knob);
      connect(amp_att_knob,4);

      // ampl dec
      MIP_Knob2Widget* amp_dec_knob = new MIP_Knob2Widget( MIP_FRect(70,102,50,82),"A.Dec");
      controls->appendWidget(amp_dec_knob);
      connect(amp_dec_knob,5);

      // ampl sus
      MIP_Knob2Widget* amp_sus_knob = new MIP_Knob2Widget( MIP_FRect(130,102,50,82),"A.Sus");
      controls->appendWidget(amp_sus_knob);
      connect(amp_sus_knob,6);

      // ampl rel
      MIP_Knob2Widget* amp_rel_knob = new MIP_Knob2Widget( MIP_FRect(190,102,50,82),"A.Rel");
      controls->appendWidget(amp_rel_knob);
      connect(amp_rel_knob,7);

      //

      // flt att
      MIP_Knob2Widget* flt_att_knob = new MIP_Knob2Widget( MIP_FRect(10,194,50,82),"F.Att");
      controls->appendWidget(flt_att_knob);
      connect(flt_att_knob,8);

      // flt dec
      MIP_Knob2Widget* flt_dec_knob = new MIP_Knob2Widget( MIP_FRect(70,194,50,82),"F.Dec");
      controls->appendWidget(flt_dec_knob);
      connect(flt_dec_knob,9);

      // flt sus
      MIP_Knob2Widget* flt_sus_knob = new MIP_Knob2Widget( MIP_FRect(130,194,50,82),"F.Sus");
      controls->appendWidget(flt_sus_knob);
      connect(flt_sus_knob,10);

      // flt rel
      MIP_Knob2Widget* flt_rel_knob = new MIP_Knob2Widget( MIP_FRect(190,194,50,82),"F.Rel");
      controls->appendWidget(flt_rel_knob);
      connect(flt_rel_knob,11);

    //

    window->appendWidget(MEditorPanel);
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  clap_param_info_t myParameters[NUM_PARAMS] = {
    { 0,  CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE, nullptr, "Vol",    "", 0.0, 1.0, 0.5  },
    { 1,  CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE, nullptr, "Pan",    "", 0.0, 1.0, 0.5  },
    { 2,  CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE, nullptr, "F.Freq", "", 0.0, 1.0, 0.7  },
    { 3,  CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE, nullptr, "F.Res",  "", 0.0, 1.0, 0.5  },
    { 4,  CLAP_PARAM_IS_AUTOMATABLE,                             nullptr, "A.Att",  "", 0.0, 1.0, 0.05 },
    { 5,  CLAP_PARAM_IS_AUTOMATABLE,                             nullptr, "A.Dec",  "", 0.0, 1.0, 0.5  },
    { 6,  CLAP_PARAM_IS_AUTOMATABLE,                             nullptr, "A.Sus",  "", 0.0, 1.0, 0.5  },
    { 7,  CLAP_PARAM_IS_AUTOMATABLE,                             nullptr, "A.Rel",  "", 0.0, 1.0, 0.5  },
    { 8,  CLAP_PARAM_IS_AUTOMATABLE,                             nullptr, "F.Att",  "", 0.0, 1.0, 0.05 },
    { 9,  CLAP_PARAM_IS_AUTOMATABLE,                             nullptr, "F.Dec",  "", 0.0, 1.0, 0.5  },
    { 10, CLAP_PARAM_IS_AUTOMATABLE,                             nullptr, "F.Sus",  "", 0.0, 1.0, 0.5  },
    { 11, CLAP_PARAM_IS_AUTOMATABLE,                             nullptr, "F.Rel",  "", 0.0, 1.0, 0.5  }
  };

  clap_audio_port_info_t myAudioOutputs[NUM_AUDIO_OUTPUTS] = {
    { 0, "Audio Out", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  clap_note_port_info_t myNoteInputs[NUM_NOTE_INPUTS] = {
    { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "Note In" }
  };

  MIP_VoiceManager<myVoice,NUM_VOICES>  MVoices = {};

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

//------------------------------
public: // clap
//------------------------------

  bool init() final {
    setupParameters(myParameters,NUM_PARAMS);
    setupAudioOutputs(myAudioOutputs,NUM_AUDIO_OUTPUTS);
    setupNoteInputs(myNoteInputs,NUM_NOTE_INPUTS);
    return MIP_Plugin::init();
  }

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MVoices.prepare(sample_rate);
    for (uint32_t i=0; i<NUM_PARAMS; i++) {
      float v = MParameterValues[i];
      MVoices.handle_master_param(i,v);
    }
    return MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  const void* get_extension(const char *id) final {
    if (strcmp(id,CLAP_EXT_GUI) == 0) return &MGui;
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0) return &MAudioPorts;
    if (strcmp(id,CLAP_EXT_NOTE_PORTS) == 0) return &MNotePorts;
    return MIP_Plugin::get_extension(id);
  }

  clap_process_status process(const clap_process_t *process) final {
    flushAudioParams();
    handle_input_events(process->in_events,process->out_events);
    handle_process(process);
    handle_output_events(process->in_events,process->out_events);
    return CLAP_PROCESS_CONTINUE;
  }

//------------------------------
public: // gui
//------------------------------

  bool gui_create(const char *api, bool is_floating) final {
    if (strcmp(api,CLAP_WINDOW_API_X11) != 0) { MIP_Print("error.. !x11\n");  return false; }
    if (is_floating) { MIP_Print("error.. is_floating\n"); return false; }
    MEditorIsOpen = false;
    MEditor = new myEditor(this,this,EDITOR_WIDTH,EDITOR_HEIGHT,true);
    return (MEditor);
  }

//------------------------------
public:
//------------------------------

  /*
    if param or mod changes, we need to update the editor
    the rest of the events are handled by the voice manager
  */

  void handle_input_events(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        switch (header->type) {
          case CLAP_EVENT_PARAM_VALUE:
            MIP_Plugin::handle_parameter_event((clap_event_param_value_t*)header);
            break;
          case CLAP_EVENT_PARAM_MOD:
            MIP_Plugin::handle_modulation_event((clap_event_param_mod_t*)header);
            break;
        }
      }
    }
  }

  /*
    called from MIP_Plugin.on_updateParameterFromEditor
    widget has changed, so we need to notify the voices..
  */

  void handle_editor_parameter(uint32_t AIndex, float AValue) final {
    MVoices.handle_master_param(AIndex,AValue);

  }

//------------------------------
public: // process
//------------------------------

  void handle_process(const clap_process_t *process) {
    float** outputs = process->audio_outputs[0].data32;
    uint32_t length = process->frames_count;
    MIP_ClearStereoBuffer(outputs,length);
    MVoices.process(process);
    float v = MParameterValues[0];  // vol
    float p = MParameterValues[1];  // pan
    float l = v * (1.0 - p);
    float r = v * (      p);
    MIP_ScaleStereoBuffer(outputs,l,r,length);
  }

};

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

void MIP_Register(MIP_ClapRegistry* ARegistry) {
  ARegistry->appendPlugin(&myDescriptor);
}

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    case 0: return new myPlugin(ADescriptor,AHost);
  }
  return nullptr;
}
