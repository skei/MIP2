
#define MIP_GUI_XCB
#define MIP_PAINTER_CAIRO

#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

//----------

#include "mip.h"
#include "audio/mip_voice_manager.h"
#include "audio/mip_audio_math.h"
#include "audio/filters/mip_svf_filter.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"
#include "gui/mip_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define NUM_PARAMS 4

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

  MIP_VoiceContext* context = nullptr;
  MIP_SvfFilter     filter  = {};

  float   ffreq   = 0.5;
  float   fres    = 0.5;

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
  }

  //----------

  uint32_t note_on(int32_t key, float velocity) {
    _key = key;
    _onvel = velocity;
    ph = 0.0;
    hz = MIP_NoteToHz(key);
    phadd = hz / context->samplerate;
    return MIP_VOICE_PLAYING;
  }

  //----------

  uint32_t note_off(float velocity) {
    _offvel = velocity;
    return MIP_VOICE_FINISHED;
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
      case 2: ffreq = value; break;
      case 3: fres = value; break;
    }
  }

  //----------

  void modulation(uint32_t index, float value) {
  }

  //----------

  uint32_t process(uint32_t AState) {
    float* output0 = context->process->audio_outputs[0].data32[0];
    float* output1 = context->process->audio_outputs[0].data32[1];
    uint32_t length  = context->process->frames_count;
    for (uint32_t i=0; i<length; i++) {
      filter.setMode(MIP_SVF_LP);
      filter.setFreq(ffreq * ffreq);
      filter.setBW(1.0 - fres);
      float out = filter.process(ph);
      ph += phadd;
      ph = MIP_Fract(ph);
      float v = _onvel + _press;
      v = MIP_Clamp(v,0,1);
      *output0++ += out * v;
      *output1++ += out * v;
    }
    return MIP_VOICE_PLAYING;
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
    MIP_Knob2Widget* vol_knob = new MIP_Knob2Widget( MIP_FRect(50,82),"Vol");
    MEditorPanel->appendWidget(vol_knob);
    vol_knob->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
    connect(vol_knob,0);
    // pan
    MIP_Knob2Widget* pan_knob = new MIP_Knob2Widget( MIP_FRect(50,82),"Pan");
    MEditorPanel->appendWidget(pan_knob);
    pan_knob->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
    connect(pan_knob,1);
    // freq
    MIP_Knob2Widget* freq_knob = new MIP_Knob2Widget( MIP_FRect(50,82),"Freq");
    MEditorPanel->appendWidget(freq_knob);
    freq_knob->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
    connect(freq_knob,2);
    // res
    MIP_Knob2Widget* res_knob = new MIP_Knob2Widget( MIP_FRect(50,82),"Res");
    MEditorPanel->appendWidget(res_knob);
    res_knob->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
    connect(res_knob,3);
    // test
    MIP_Knob2Widget* test_knob = new MIP_Knob2Widget( MIP_FRect(50,82),"Test");
    MEditorPanel->appendWidget(test_knob);
    test_knob->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
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
    { 0, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "Vol",  "", 0.0, 1.0, 0.5 },
    { 1, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "Pan",  "", 0.0, 1.0, 0.5 },
    { 2, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "Freq", "", 0.0, 1.0, 0.5 },
    { 3, CLAP_PARAM_IS_AUTOMATABLE, nullptr, "Res",  "", 0.0, 1.0, 0.5 }
  };

  MIP_VoiceManager<myVoice,16>  MVoices = {};

  uint32_t MDefaultEditorWidth  = 400;
  uint32_t MDefaultEditorHeight = 400;

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

  void handle_events_input(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
    MIP_Plugin::handle_events_input(in_events,out_events);
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        MVoices.on_event(header);
      }
    }
  }

  //----------

  void handle_process(const clap_process_t *process) final {

    // send freq/res to voices..
    // hmmmm.. this doesn't feel right..
    MVoices.handle_master_param(2,MParameterValues[2]);
    MVoices.handle_master_param(3,MParameterValues[3]);

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
