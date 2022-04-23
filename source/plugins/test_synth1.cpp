// -faligned=new


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

#define NUM_PARAMS  12
#define NUM_VOICES  16

//#define SUPPORTED_DIALECTS (CLAP_NOTE_DIALECT_CLAP | CLAP_NOTE_DIALECT_MIDI | CLAP_NOTE_DIALECT_MIDI_MPE | CLAP_NOTE_DIALECT_MIDI2)
//#define SUPPORTED_DIALECTS  CLAP_NOTE_DIALECT_CLAP

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

  float   ffreq   = 0.5;
  float   fres    = 0.5;

  float   ffreq_mod = 0.0;
  float   fres_mod  = 0.0;

  //float   envatt = 0.0;
  //float   envdec = 0.0;
  //float   envsus = 1.0;
  //float   envrel = 0.0;

  float   hz      = 0.0;  // note hz
  float   ph      = 0.0;  // phase
  float   phadd   = 0.0;  // phase add

  int32_t _key    = -1;
  float   _onvel  = 0.0;
  float   _offvel = 0.0;
  float   _vol    = 0.0;
  float   _pan    = 0.0;
  float   _vibr   = 0.0;
  float   _expr   = 0.0;
  float   _bright = 0.0;
  float   _press  = 0.0;


//------------------------------
public:
//------------------------------

  void prepare(MIP_VoiceContext* AContext) {
    context = AContext;
    amp_env.setSampleRate(context->samplerate);
    flt_env.setSampleRate(context->samplerate);
    //amp_env.setADSR(0,0,1,0);
    //flt_env.setADSR(0,0,1,0);
  }

  //----------

  uint32_t note_on(int32_t key, float velocity) {
    //MIP_Print("key %i valocity %.3f\n",key,velocity);
    _key = key;
    _onvel = velocity;
    ph = 0.0;
    hz = MIP_NoteToHz(key);
    phadd = hz / context->samplerate;
    amp_env.noteOn();
    flt_env.noteOn();
    //MIP_Print("amp_env note on\n");
    return MIP_VOICE_PLAYING;
  }

  //----------

  uint32_t note_off(float velocity) {
    //MIP_Print("velocity %.3f\n",velocity);
    _offvel = velocity;
    amp_env.noteOff();
    flt_env.noteOff();
    //MIP_Print("amp_env note off\n");
    //return MIP_VOICE_FINISHED;
    return MIP_VOICE_RELEASED;
  }

  //----------

  void note_choke() {
  }

  //----------

  void note_end() {
  }

  //----------

  void tuning(float amount) {
    hz = MIP_NoteToHz(_key + amount);
    phadd = hz / context->samplerate;
  }

  //----------

  void volume(float amount) {
    _vol = amount;
  }

  //----------

  void pan(float amount) {
    _pan = amount;
  }

  //----------

  void vibrato(float amount) {
    _vibr = amount;
  }

  //----------

  void expression(float amount) {
    _expr = amount;
  }

  //----------

  void brightness(float amount) {
    _bright = amount;
  }

  //----------

  void pressure(float amount) {
    _press = amount;
  }

  //----------

  void parameter(uint32_t index, float value) {
    switch (index) {
      case 2:  ffreq = value; break;
      case 3:  fres = value; break;
      case 4:  amp_env.setAttack(value * 5); break;
      case 5:  amp_env.setDecay(value * 5); break;
      case 6:  amp_env.setSustain(value); break;
      case 7:  amp_env.setRelease(value * 5); break;
      case 8:  flt_env.setAttack(value * 5); break;
      case 9:  flt_env.setDecay(value * 5); break;
      case 10: flt_env.setSustain(value); break;
      case 11: flt_env.setRelease(value * 5); break;
    }
  }

  //----------

  void modulation(uint32_t index, float value) {
    switch (index) {
      case 2: ffreq_mod = value; break;
      //case 3: fres_mod = value; break;
    }
  }

  //----------

//  uint32_t process(uint32_t AState) {
//    //float* output0 = context->process->audio_outputs[0].data32[0];
//    //float* output1 = context->process->audio_outputs[0].data32[1];
//    float* output = context->voicebuffer;
//    uint32_t length  = context->process->frames_count;
//    for (uint32_t i=0; i<length; i++) {
//      filter.setMode(MIP_SVF_LP);
//      filter.setFreq(ffreq * ffreq);
//      filter.setBW(1.0 - fres);
//      float out = filter.process(ph);
//      ph += phadd;
//      ph = MIP_Fract(ph);
//      float v = _onvel + _press;
//      v = MIP_Clamp(v,0,1);
//      *output++ = out * v;
//      //*output++ += out * v;
//      //*output1++ += out * v;
//    }
//    return MIP_VOICE_PLAYING;
//  }

  //----------

  uint32_t process(uint32_t AState, uint32_t ASize) {
    //float* output = context->voicebuffer;
    float* output = MIP_VoiceBuffer;
    for (uint32_t i=0; i<ASize; i++) {

      //float o = ph - MIP_PolyBlep(t,dt);

      float t1 = ph + 0.5f;
      t1 = MIP_Fract(t1);
      float o = 2.0f * t1 - 1.0f;
      o -= MIP_PolyBlep(t1,phadd);

      float ae = amp_env.process();
      float fe = flt_env.process();

      float ff = MIP_Clamp(ffreq + ffreq_mod, 0,1);
      float fr = MIP_Clamp(fres + fres_mod, 0,1);

      ff *= fe;

      filter.setMode(MIP_SVF_LP);
      filter.setFreq(ff * ff);
      filter.setBW(1.0 - fr);


      o = filter.process(o);
      o *= (_onvel + _press);
      o *= ae;

      *output++ = o;

      ph += phadd;
      ph = MIP_Fract(ph);

    }
    uint32_t stage = amp_env.getStage();
    //MIP_DPrint("e %.3f stage %i\n",amp_env,es);

    if (stage == MIP_ENVELOPE_FINISHED) {
      //MIP_Print("amp_env finished\n");
      //amp_env.reset();
      return MIP_VOICE_FINISHED;
    }
    else return AState;//MIP_VOICE_PLAYING;
  }

  //----------

  uint32_t process(uint32_t AState) {
    return process(AState,MIP_VOICE_TICKSIZE);
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
private:
//------------------------------



//------------------------------
public:
//------------------------------

  myEditor(MIP_EditorListener* AListener, MIP_ClapPlugin* APlugin, uint32_t AWidth, uint32_t AHeight, bool AEmbedded)
  : MIP_Editor(AListener,APlugin,AWidth,AHeight,AEmbedded) {
    MIP_Window* window = getWindow();
    // panel
    MIP_PanelWidget* MEditorPanel = new MIP_PanelWidget(MIP_FRect());
    MEditorPanel->setBackgroundColor(0.6);
    MEditorPanel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    MEditorPanel->layout.innerBorder = MIP_FRect(10,10,10,10);
    MEditorPanel->layout.spacing = 5;
    // vol
    MIP_Knob2Widget* vol_knob = new MIP_Knob2Widget( MIP_FRect(10,10,50,82),"Vol");
    MEditorPanel->appendWidget(vol_knob);
    connect(vol_knob,0);
    // pan
    MIP_Knob2Widget* pan_knob = new MIP_Knob2Widget( MIP_FRect(70,10,50,82),"Pan");
    MEditorPanel->appendWidget(pan_knob);
    connect(pan_knob,1);
    // freq
    MIP_Knob2Widget* freq_knob = new MIP_Knob2Widget( MIP_FRect(130,10,50,82),"Freq");
    MEditorPanel->appendWidget(freq_knob);
    connect(freq_knob,2);
    // res
    MIP_Knob2Widget* res_knob = new MIP_Knob2Widget( MIP_FRect(190,10,50,82),"Res");
    MEditorPanel->appendWidget(res_knob);
    connect(res_knob,3);

    // att
    MIP_Knob2Widget* amp_att_knob = new MIP_Knob2Widget( MIP_FRect(10,102,50,82),"A.Att");
    MEditorPanel->appendWidget(amp_att_knob);
    connect(amp_att_knob,4);
    // dec
    MIP_Knob2Widget* amp_dec_knob = new MIP_Knob2Widget( MIP_FRect(70,102,50,82),"A.Dec");
    MEditorPanel->appendWidget(amp_dec_knob);
    connect(amp_dec_knob,5);
    // sus
    MIP_Knob2Widget* amp_sus_knob = new MIP_Knob2Widget( MIP_FRect(130,102,50,82),"A.Sus");
    MEditorPanel->appendWidget(amp_sus_knob);
    connect(amp_sus_knob,6);
    // rel
    MIP_Knob2Widget* amp_rel_knob = new MIP_Knob2Widget( MIP_FRect(190,102,50,82),"A.Rel");
    MEditorPanel->appendWidget(amp_rel_knob);
    connect(amp_rel_knob,7);

    // att
    MIP_Knob2Widget* flt_att_knob = new MIP_Knob2Widget( MIP_FRect(10,194,50,82),"F.Att");
    MEditorPanel->appendWidget(flt_att_knob);
    connect(flt_att_knob,8);
    // dec
    MIP_Knob2Widget* flt_dec_knob = new MIP_Knob2Widget( MIP_FRect(70,194,50,82),"F.Dec");
    MEditorPanel->appendWidget(flt_dec_knob);
    connect(flt_dec_knob,9);
    // sus
    MIP_Knob2Widget* flt_sus_knob = new MIP_Knob2Widget( MIP_FRect(130,194,50,82),"F.Sus");
    MEditorPanel->appendWidget(flt_sus_knob);
    connect(flt_sus_knob,10);
    // rel
    MIP_Knob2Widget* flt_rel_knob = new MIP_Knob2Widget( MIP_FRect(190,194,50,82),"F.Rel");
    MEditorPanel->appendWidget(flt_rel_knob);
    connect(flt_rel_knob,11);

//    // test
//    MIP_Knob2Widget* test_knob = new MIP_Knob2Widget( MIP_FRect(50,82),"Test");
//    MEditorPanel->appendWidget(test_knob);
//    test_knob->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
    //
    window->appendWidget(MEditorPanel);
  }

  //----------

  virtual ~myEditor() {
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
    { 0,  CLAP_PARAM_IS_AUTOMATABLE, nullptr, "Vol",  "", 0.0, 1.0, 0.5 },
    { 1,  CLAP_PARAM_IS_AUTOMATABLE, nullptr, "Pan",  "", 0.0, 1.0, 0.5 },
    { 2,  CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE | CLAP_PARAM_IS_MODULATABLE_PER_CHANNEL, nullptr, "Freq", "", 0.0, 1.0, 0.7 },
    { 3,  CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE, nullptr, "Res",  "", 0.0, 1.0, 0.5 },
    { 4,  CLAP_PARAM_IS_AUTOMATABLE, nullptr, "A.Att",  "", 0.0, 1.0, 0.05 },
    { 5,  CLAP_PARAM_IS_AUTOMATABLE, nullptr, "A.Dec",  "", 0.0, 1.0, 0.5 },
    { 6,  CLAP_PARAM_IS_AUTOMATABLE, nullptr, "A.Sus",  "", 0.0, 1.0, 0.5 },
    { 7,  CLAP_PARAM_IS_AUTOMATABLE, nullptr, "A.Rel",  "", 0.0, 1.0, 0.5 },
    { 8,  CLAP_PARAM_IS_AUTOMATABLE, nullptr, "F.Att",  "", 0.0, 1.0, 0.05 },
    { 9,  CLAP_PARAM_IS_AUTOMATABLE, nullptr, "F.Dec",  "", 0.0, 1.0, 0.5 },
    { 10, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "F.Sus",  "", 0.0, 1.0, 0.5 },
    { 11, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "F.Rel",  "", 0.0, 1.0, 0.5 }
  };

  MIP_VoiceManager<myVoice,NUM_VOICES>  MVoices = {};

  uint32_t MDefaultEditorWidth  = 270;
  uint32_t MDefaultEditorHeight = 296;

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~myPlugin() {
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    setupParameters(myParameters,NUM_PARAMS);
    return MIP_Plugin::init();
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MVoices.prepare(sample_rate);
    return MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  const void* get_extension(const char *id) final {
    if (strcmp(id,CLAP_EXT_GUI) == 0) return &MGui;
    return MIP_Plugin::get_extension(id);
  }

//------------------------------
public: // gui
//------------------------------

  bool gui_create(const char *api, bool is_floating) final {
    if (strcmp(api,CLAP_WINDOW_API_X11) != 0) {
      MIP_Print("not x11\n");
      return false;
    }
    if (is_floating) {
      MIP_Print("floating\n");
    return false;
    }
    MEditorIsOpen = false;
    MEditor = new myEditor(this,this,MDefaultEditorWidth,MDefaultEditorHeight,true);
    return (MEditor);
  }

//------------------------------
public:
//------------------------------

//  void handle_input_events(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
//    MIP_Plugin::handle_input_events(in_events,out_events);
//    uint32_t num_events = in_events->size(in_events);
//    for (uint32_t i=0; i<num_events; i++) {
//      const clap_event_header_t* header = in_events->get(in_events,i);
//      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
//        MVoices.on_event(header);
//      }
//    }
//  }

  //----------

  void handle_note_on_event(clap_event_note_t* event) final {
    //MIP_PRINT;
    MVoices.on_note_on(event);
  }

  void handle_note_off_event(clap_event_note_t* event) final {
    MVoices.on_note_off(event);
  }

  void handle_note_end_event(clap_event_note_t* event) final {
    MVoices.on_note_end(event);
  }

  void handle_note_choke_event(clap_event_note_t* event) final {
    MVoices.on_note_choke(event);
  }

  void handle_note_expression_event(clap_event_note_expression_t* event) final {
    switch (event->expression_id) {
      case CLAP_NOTE_EXPRESSION_VOLUME:     MVoices.on_note_volume_expression(event);     break;
      case CLAP_NOTE_EXPRESSION_PAN:        MVoices.on_note_pan_expression(event);        break;
      case CLAP_NOTE_EXPRESSION_TUNING:     MVoices.on_note_tuning_expression(event);     break;
      case CLAP_NOTE_EXPRESSION_VIBRATO:    MVoices.on_note_vibrato_expression(event);    break;
      case CLAP_NOTE_EXPRESSION_EXPRESSION: MVoices.on_note_expression_expression(event); break;
      case CLAP_NOTE_EXPRESSION_BRIGHTNESS: MVoices.on_note_brightness_expression(event); break;
      case CLAP_NOTE_EXPRESSION_PRESSURE:   MVoices.on_note_pressure_expression(event);   break;
    }
  }

  void handle_midi_event(clap_event_midi_t* event) final {
    MVoices.on_midi(event);
  }

  void handle_midi2_event(clap_event_midi2_t* event) final {
    MVoices.on_midi2(event);
  }

  void handle_midi_sysex_event(clap_event_midi_sysex_t* event) final {
    MVoices.on_midi_sysex(event);
  }

  void handle_transport_event(clap_event_transport_t* event) final {
    //MVoices.transport(event);
    }

  void handle_parameter_event(clap_event_param_value_t* event) final {
    MIP_Plugin::handle_parameter_event(event);
    MVoices.on_parameter_value(event);
  }

  void handle_modulation_event(clap_event_param_mod_t* event) final {
    MIP_Plugin::handle_modulation_event(event);
    MVoices.on_parameter_modulation(event);
  }

  //----------

  void handle_process(const clap_process_t *process) final {

    // todo: fix this..
    // send freq/res to voices..
    for (uint32_t i=2; i<12; i++) {
      MVoices.handle_master_param(i,MParameterValues[i]);
    }

    float** outputs = process->audio_outputs[0].data32;
    uint32_t length = process->frames_count;
    MIP_ClearStereoBuffer(outputs,length);

    //MVoices.process(process);
    MVoices.processTicks(process);

    float v = MParameterValues[0];  // vol
    float p = MParameterValues[1];  // pan
    float l = v * (1.0 - p);
    float r = v * (      p);
    MIP_ScaleStereoBuffer(outputs,l,r,length);

//      if (amp_env.getStage() == MIP_ENVELOPE_FINISHED) {
//        //result = MIP_VOICE_FINISHED;
//      }

  }

};

//----------------------------------------------------------------------

#undef NUM_PARAMS
#undef ALL_DIALECTS

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

void MIP_Register(MIP_ClapRegistry* ARegistry) {
  ARegistry->appendPlugin(&myDescriptor);
}

//----------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    case 0: return new myPlugin(ADescriptor,AHost);
  }
  return nullptr;
}
