
#define MIP_GUI_XCB
#define MIP_PAINTER_CAIRO

#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

//#define MIP_DEBUG_PRINT_TIME
//#define MIP_DEBUG_WATCHES
//#define MIP_DEBUG_CALLSTACK
//#define MIP_DEBUG_CRASH_HANDLER

//#define MIP_VST2
//#define MIP_VST3

//----------------------------------------------------------------------

#include "mip.h"
#include "audio/mip_audio_math.h"
#include "audio/filters/mip_rc_filter.h"
#include "audio/filters/mip_svf_filter.h"
#include "audio/modulation/mip_envelope.h"
#include "audio/waveforms/mip_polyblep_waveform.h"
#include "gui/mip_widgets.h"
#include "plugin/mip_editor.h"
#include "plugin/mip_parameter.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_voice_manager.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define NUM_PARAMS          11
#define NUM_NOTE_INPUTS     1
#define NUM_AUDIO_OUTPUTS   1
#define NUM_VOICES          16
#define EDITOR_WIDTH        420
#define EDITOR_HEIGHT       350

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
  "0.0.2",
  "simple mip2 test synth",
  myFeatures
};

//----------------------------------------------------------------------
//
// voice widget
//
//----------------------------------------------------------------------

class MIP_VoiceWidget
: public MIP_PanelWidget {

//------------------------------
public:

  MIP_VoiceWidget(MIP_FRect ARect)
  : MIP_PanelWidget(ARect) {
    setFillBackground(false);
  }

  //----------

  virtual ~MIP_VoiceWidget() {
  }

//------------------------------
public:
//------------------------------

  uint32_t voice_state[NUM_VOICES]  = {0};

  //----------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) final {
    MIP_FRect rect = MIP_FRect(MRect.x,MRect.y,MRect.w / NUM_VOICES, MRect.h);
    MIP_Color color = MIP_COLOR_DARK_GRAY;
    for (uint32_t i=0; i<NUM_VOICES; i++) {
      APainter->rectangle(rect);
      switch (voice_state[i]) {
        case MIP_VOICE_OFF:       color = MIP_COLOR_DARK_GRAY;    break;
        case MIP_VOICE_WAITING:   color = MIP_COLOR_YELLOW;       break;
        case MIP_VOICE_PLAYING:   color = MIP_COLOR_BRIGHT_GREEN; break;
        case MIP_VOICE_RELEASED:  color = MIP_COLOR_GREEN;        break;
        case MIP_VOICE_FINISHED:  color = MIP_COLOR_LIGHT_GRAY;   break;
      }
      APainter->setColor(color);
      APainter->fillPath();
      rect.x += 8;
    }
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

  MIP_VoiceWidget* MVoiceWidget = nullptr;

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
    MEditorPanel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    // header
    MIP_SAHeaderWidget* sa_header = new MIP_SAHeaderWidget(60,window);
    MEditorPanel->appendWidget(sa_header);
    sa_header->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    sa_header->setPluginName(myDescriptor.name);
    sa_header->setPluginVersion(myDescriptor.version);
    // controls panel (knobs)
    MIP_PanelWidget* controls = new MIP_PanelWidget(MIP_FRect());
    MEditorPanel->appendWidget(controls);
    controls->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    controls->setDrawBorder(false);
    controls->setFillBackground(true);
    controls->setBackgroundColor(0.6);
    controls->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
      // voice widget
      MVoiceWidget = new MIP_VoiceWidget( MIP_FRect(250,10,NUM_VOICES*8,8) );
      controls->appendWidget(MVoiceWidget);
      // vol
      MIP_Knob2Widget* vol_knob = new MIP_Knob2Widget( MIP_FRect(10,10,50,82),"Vol");
      controls->appendWidget(vol_knob);
      connect(vol_knob,0);
      // pan
      MIP_Knob2Widget* pan_knob = new MIP_Knob2Widget( MIP_FRect(70,10,50,82),"Pan");
      controls->appendWidget(pan_knob);
      connect(pan_knob,1);
      // pitch
      MIP_Knob2Widget* pitch_knob = new MIP_Knob2Widget( MIP_FRect(130,10,50,82),"Pitch");
      controls->appendWidget(pitch_knob);
      pitch_knob->getKnobWidget()->setSnap(true);
      pitch_knob->getKnobWidget()->setSnapPos(0.5);
      connect(pitch_knob,10);
      // freq
      MIP_Knob2Widget* freq_knob = new MIP_Knob2Widget( MIP_FRect(10,102,50,82),"Freq");
      controls->appendWidget(freq_knob);
      connect(freq_knob,2);
      // res
      MIP_Knob2Widget* res_knob = new MIP_Knob2Widget( MIP_FRect(70,102,50,82),"Res");
      controls->appendWidget(res_knob);
      connect(res_knob,3);
      // squ
      MIP_Knob2Widget* pulse_knob = new MIP_Knob2Widget( MIP_FRect(130,102,50,82),"Pulse");
      controls->appendWidget(pulse_knob);
      connect(pulse_knob,4);
      // width
      MIP_Knob2Widget* width_knob = new MIP_Knob2Widget( MIP_FRect(190,102,50,82),"Width");
      controls->appendWidget(width_knob);
      connect(width_knob,5);
      // amp att
      MIP_Knob2Widget* amp_att_knob = new MIP_Knob2Widget( MIP_FRect(10,194,50,82),"Att");
      controls->appendWidget(amp_att_knob);
      connect(amp_att_knob,6);
      // amp dec
      MIP_Knob2Widget* amp_dec_knob = new MIP_Knob2Widget( MIP_FRect(70,194,50,82),"Dec");
      controls->appendWidget(amp_dec_knob);
      connect(amp_dec_knob,7);
      // amp sus
      MIP_Knob2Widget* amp_sus_knob = new MIP_Knob2Widget( MIP_FRect(130,194,50,82),"Sus");
      controls->appendWidget(amp_sus_knob);
      connect(amp_sus_knob,8);
      // amp rel
      MIP_Knob2Widget* amp_rel_knob = new MIP_Knob2Widget( MIP_FRect(190,194,50,82),"Rel");
      controls->appendWidget(amp_rel_knob);
      connect(amp_rel_knob,9);
    window->appendWidget(MEditorPanel);
  }

  //----------

  void on_timerCallback(void) final {
    MVoiceWidget->redraw();
    MIP_Editor::on_timerCallback();
  }

};

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

// todo: redo the smoothing things..

#define SMOOTHER_FACTOR (1.0 / 250.0)

//----------

class myVoice {

//------------------------------
private:
//------------------------------

  MIP_VoiceContext* context           = nullptr;

  MIP_SvfFilter     filter            = {};
  MIP_Envelope      amp_env           = {};
  MIP_RcFilter      flt_freq_smoother = {};
  MIP_RcFilter      flt_res_smoother  = {};
  MIP_RcFilter      vol_smoother      = {};

  int32_t           note_key          = -1;
  float             note_onvel        = 0.0;
  float             note_offvel       = 0.0;
  float             note_vol          = 0.0;
  float             note_pan          = 0.0;
  float             note_tuning       = 0.0;
  float             note_vibr         = 0.0;
  float             note_expr         = 0.0;
  float             note_bright       = 0.0;
  float             note_press        = 0.0;

  float             hz                = 0.0;  // note hz
  float             ph                = 0.0;  // phase
  float             phadd             = 0.0;  // phase add
  float             pulse             = 1.0;
  float             width             = 0.5;
  float             filter_freq       = 0.5;
  float             filter_res        = 0.5;
  float             pitch             = 0.5;

  float             pulse_mod         = 0.0;
  float             width_mod         = 0.0;
  float             filter_freq_mod   = 0.0;
  float             filter_res_mod    = 0.0;
  float             pitch_mod         = 0.0;

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
    flt_freq_smoother.setup(filter_freq,filter_freq, SMOOTHER_FACTOR);
    flt_res_smoother.setup(filter_res,filter_res, SMOOTHER_FACTOR);
    vol_smoother.setup(0,0,SMOOTHER_FACTOR);
  }

  //----------

  uint32_t note_on(int32_t key, float velocity) {
    note_key        = key;
    note_onvel      = velocity;
    note_press      = 0.0;
    note_bright     = 0.0;
    note_tuning     = 0.0;
    ph              = 0.0;
    filter_freq_mod = 0.0;
    filter_res_mod  = 0.0;
    pulse_mod       = 0.0;
    width_mod       = 0.0;
    pitch_mod       = 0.0;
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

  void volume(float amount) {
    note_vol = amount;
  }

  //----------

  void pan(float amount) {
    note_pan = amount;
  }

  //----------

  void tuning(float amount) {
    note_tuning = amount;
  }

  //----------

  void vibrato(float amount) {
    note_vibr = amount;
  }

  //----------

  void expression(float amount) {
    note_expr = amount;
  }

  //----------

  void brightness(float amount) {
    note_bright = (amount * 2.0) - 1.0;
  }

  //----------

  void pressure(float amount) {
    note_press = amount;
  }

  //----------

  void parameter(uint32_t index, float value) {
    //MIP_Print("%i = %.3f\n",index,value);
    switch (index) {
      case  2:  filter_freq = value;              break;
      case  3:  filter_res = value;               break;
      case  4:  pulse = value;                    break;
      case  5:  width = value;                    break;
      case  6:  amp_env.setAttack(value*5);       break;
      case  7:  amp_env.setDecay(value*5);        break;
      case  8:  amp_env.setSustain(value*value);  break;
      case  9:  amp_env.setRelease(value*5);      break;
      case 10:  pitch = value;                    break;
    }
  }

  void modulation(uint32_t index, float value) {
    switch (index) {
      case  2:  filter_freq_mod = value;  break;
      case  3:  filter_res_mod = value;   break;
      case  4:  pulse_mod = value;        break;
      case  5:  width_mod = value;        break;
      case 10:  pitch_mod = value;        break;
    }
  }

  //----------

  uint32_t process(uint32_t AIndex, uint32_t AState, uint32_t ALength, uint32_t AOffset) {
    MIP_Assert(note_key >= 0);
    if (note_key >= 0) {
      float p = (pitch * 2.0) - 1.0;
      p += pitch_mod;
      //todo: smoother
      hz = MIP_NoteToHz(note_key + note_tuning + p);
      phadd = hz * context->invsamplerate;
      float* output = context->voicebuffer;

      // this should already have been applied
      // maybe when we setup the voicebuffer ptr (voice manager)

      #ifdef MIP_VOICE_PROCESS_THREADED
        output += (context->process->frames_count * AIndex);
      #endif

      output += AOffset;
      MIP_Assert(output);
      for (uint32_t i = 0; i < ALength; i++) {
        float t1 = ph + 0.5f;
        t1 = MIP_Fract(t1);
        float saw1 = 2.0 * t1 - 1.0;
        saw1 -= MIP_PolyBlep(t1,phadd);
        float w = width + width_mod;
        w = MIP_Clamp(w,0,1);
        float t2 = t1 + w;
        t2 = MIP_Fract(t2);
        float saw2 = 2.0 * t2 - 1.0;
        saw2 -= MIP_PolyBlep(t2,phadd);
        float sq = pulse + pulse_mod;
        sq = MIP_Clamp(sq,0,1);
        float squ = saw1 - (saw2 * sq);
        float ff = filter_freq + filter_freq_mod;
        ff += note_bright;
        ff = MIP_Clamp(ff,0,1);
        ff = (ff * ff); // ugh..
        ff = flt_freq_smoother.process(ff);
        float fr = filter_res + filter_res_mod;
        fr = MIP_Clamp(fr,0,1);
        fr = 1.0 - fr; // hhhh...
        fr = flt_res_smoother.process(fr);
        float amp = note_onvel + note_press;
        amp = MIP_Clamp(amp,0,1);
        amp *= amp_env.process();
        amp = vol_smoother.process(amp);
        filter.setMode(MIP_SVF_LP);
        filter.setFreq(ff);
        filter.setBW(fr);
        float out = squ;

        #ifdef MIP_VOICE_PROCESS_THREADED
          *output++ = (filter.process(out) * amp);
        #else
          *output++ += (filter.process(out) * amp);
        #endif

        ph += phadd;
        ph = MIP_Fract(ph);
      }
    }
    uint32_t stage = amp_env.getStage();
    if (stage == MIP_ENVELOPE_FINISHED) return MIP_VOICE_FINISHED;
    else return AState;
  }

};

//----------------------------------------------------------------------

typedef MIP_VoiceManager<myVoice,NUM_VOICES> myVoiceManager;

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

    { 0,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Vol",
      "",
      0.0,
      1.0,
      0.5
    },
    { 1,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Pan",
      "",
      0.0,
      1.0,
      0.5
    },

    { 2,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        //| CLAP_PARAM_IS_MODULATABLE_PER_KEY
        //| CLAP_PARAM_IS_MODULATABLE_PER_CHANNEL
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "F.Freq",
      "",
      0.0,
      1.0,
      0.5
    },
    { 3,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        //| CLAP_PARAM_IS_MODULATABLE_PER_KEY
        //| CLAP_PARAM_IS_MODULATABLE_PER_CHANNEL
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "F.Res",
      "",
      0.0,
      1.0,
      0.5
    },
    { 4,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        //| CLAP_PARAM_IS_MODULATABLE_PER_KEY
        //| CLAP_PARAM_IS_MODULATABLE_PER_CHANNEL
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Pulse",
      "",
      0.0,
      1.0,
      1.0
    },
    { 5,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        //| CLAP_PARAM_IS_MODULATABLE_PER_KEY
        //| CLAP_PARAM_IS_MODULATABLE_PER_CHANNEL
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Width",
      "",
      0.0,
      1.0,
      0.7
    },

    { 6,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "A.Att",
      "",
      0.0,
      1.0,
      0.01
    },
    { 7,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "A.Dec",
      "",
      0.0,
      1.0,
      0.0
    },
    { 8,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "A.Sus",
      "",
      0.0,
      1.0,
      1.0
    },
    { 9,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "A.Rel",
      "",
      0.0,
      1.0,
      0.5
    },

    { 10,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        //| CLAP_PARAM_IS_MODULATABLE_PER_KEY
        //| CLAP_PARAM_IS_MODULATABLE_PER_CHANNEL
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Pitch",
      "",
      0.0,
      1.0,
      0.5
    }

  };

  //----------

  clap_audio_port_info_t myAudioOutputs[NUM_AUDIO_OUTPUTS] = {
    { 0,
      "Audio Out",
      CLAP_AUDIO_PORT_IS_MAIN,
      2,
      CLAP_PORT_STEREO,
      CLAP_INVALID_ID
    }
  };

  //----------

  clap_note_port_info_t myNoteInputs[NUM_NOTE_INPUTS] = {
    { 0,
      CLAP_NOTE_DIALECT_CLAP,
      CLAP_NOTE_DIALECT_CLAP,
      "Note In"
    }
  };

  //----------

  myVoiceManager MVoiceManager = {};

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    MIP_Print("Hello world!\n");
  }

//------------------------------
public: // clap
//------------------------------

  bool init() final {
    //clap_version_t ver = MHost->host->clap_version;
    //MIP_Print("host name: %s\n",MHost->host->name);
    //MIP_Print("host version: %s\n",MHost->host->version);
    //MIP_Print("host clap version: %i.%i.%i\n",ver.major,ver.minor,ver.revision);
    //MHost->printSupportedExtensions();
    setupParameters(myParameters,NUM_PARAMS);
    setupAudioOutputs(myAudioOutputs,NUM_AUDIO_OUTPUTS);
    setupNoteInputs(myNoteInputs,NUM_NOTE_INPUTS);
    return MIP_Plugin::init();
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MVoiceManager.prepareVoices(sample_rate);
    // send initial parameter values to the voices
    for (uint32_t i=0; i<NUM_PARAMS; i++) {
      float v = MParameterValues[i];
      MVoiceManager.setParameter(i,v);
    }
    return MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  const void* get_extension(const char *id) final {
    //MIP_Print("host asks for: %s\n",id);
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0) return &MAudioPorts;
    if (strcmp(id,CLAP_EXT_GUI) == 0)         return &MGui;
    if (strcmp(id,CLAP_EXT_NOTE_PORTS) == 0)  return &MNotePorts;
    if (strcmp(id,CLAP_EXT_THREAD_POOL) == 0) return &MThreadPool;
    if (strcmp(id,CLAP_EXT_VOICE_INFO) == 0)  return &MVoiceInfo;
    return MIP_Plugin::get_extension(id);
  }

  //----------

  clap_process_status process(const clap_process_t *process) final {
    flushAudioParams();
    handle_input_events(process->in_events,process->out_events);
    handle_process(process);
    handle_output_events(process->in_events,process->out_events);
    // update gui (state)
    for (uint32_t i=0; i<NUM_VOICES; i++) {
      uint32_t state = MVoiceManager.getVoiceState(i);
      if (MEditor && MEditorIsOpen) {
        ((myEditor*)MEditor)->MVoiceWidget->voice_state[i] = state;
      }
    }
    return CLAP_PROCESS_CONTINUE;
  }

  //----------
  // gui
  //----------

  bool gui_create(const char *api, bool is_floating) final {
    if (strcmp(api,CLAP_WINDOW_API_X11) != 0) return false;
    if (is_floating) return false;
    MEditorIsOpen = false;
    MEditor = new myEditor(this,this,EDITOR_WIDTH,EDITOR_HEIGHT,true);
    return (MEditor);
  }

  //----------
  // voice-info
  //----------

  bool voice_info_get(clap_voice_info_t *info) final {
    info->voice_count     = NUM_VOICES;
    info->voice_capacity  = NUM_VOICES;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }

  //----------
  // thread-pool
  //----------

  void thread_pool_exec(uint32_t task_index) final {
    MVoiceManager.processVoiceThread(task_index);
  }

//------------------------------
public:
//------------------------------

  void handle_process(const clap_process_t *process) final {
    float** outputs = process->audio_outputs[0].data32;
    uint32_t length = process->frames_count;

    #ifdef MIP_VOICE_PROCESS_THREADED
      MVoiceManager.processThreaded(process,MHost);
    #else
      MVoiceManager.process(process);
    #endif

    float v = MParameterValues[0];  // vol
    float p = MParameterValues[1];  // pan
    float l = v * (1.0 - p);
    float r = v * (      p);
    MIP_ScaleStereoBuffer(outputs,l,r,length);
  }

//------------------------------
protected:
//------------------------------

  /*
    parameter has changed via gui
    we need to tell the voices about it
  */

  void handle_editor_parameter(uint32_t AIndex, float AValue) final {
    //MIP_Print("%i = %.3f\n",AIndex,AValue);
    MVoiceManager.setParameter(AIndex,AValue);
  }

};

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

void MIP_Register(MIP_ClapRegistry* ARegistry) {
  ARegistry->appendDescriptor(&myDescriptor);
}

//----------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    case 0: return new myPlugin(ADescriptor,AHost);
  }
  return nullptr;
}

