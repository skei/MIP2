
#define MIP_GUI_XCB
#define MIP_PAINTER_CAIRO

#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

//----------

#include "mip.h"
#include "audio/mip_voice_manager.h"
#include "audio/mip_audio_math.h"
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
#define SUPPORTED_DIALECTS  CLAP_NOTE_DIALECT_CLAP

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
    //bend = 0.0;
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
      float out = ph;
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

  const char* buttonrow_text[6] = { "1", "2", "III", "Four", "5", "6" };

//------------------------------
public:
//------------------------------

  myEditor(MIP_EditorListener* AListener, MIP_ClapPlugin* APlugin, uint32_t AWidth, uint32_t AHeight, bool AEmbedded)
  : MIP_Editor(AListener,APlugin,AWidth,AHeight,AEmbedded) {

    setCanResize();

    // menu

    MIP_MenuWidget* menu1 = new MIP_MenuWidget( MIP_FRect() );
    menu1->appendMenuItem("first");
    menu1->appendMenuItem("item2");
    menu1->appendMenuItem("item3");
    menu1->appendMenuItem("4");
    menu1->appendMenuItem("five");
    menu1->setItemSize(90,20);
    menu1->setItemLayout(1,5);
    menu1->setMenuMirror(true,false);

    // panel

    MIP_PanelWidget* MEditorPanel = new MIP_PanelWidget(MIP_FRect());
    MEditorPanel->setBackgroundColor(0.6);
    MEditorPanel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    MEditorPanel->layout.innerBorder = MIP_FRect(10,10,10,10);
    MEditorPanel->layout.spacing = 5;

    // knob 1

    MIP_KnobWidget* knob1 = new MIP_KnobWidget( MIP_FRect( 50,50 ));
    MEditorPanel->appendWidget(knob1);
    knob1->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
    connect(knob1,0);

    // knob 2

    MIP_KnobWidget* knob2 = new MIP_KnobWidget( MIP_FRect( 50,50 ));
    MEditorPanel->appendWidget(knob2);
    knob2->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
    connect(knob2,1);

    // knob 3

    MIP_KnobWidget* knob3 = new MIP_KnobWidget( MIP_FRect( 50,50 ));
    MEditorPanel->appendWidget(knob3);
    knob3->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
    connect(knob3,2);

    // knob 4

    MIP_KnobWidget* knob4 = new MIP_KnobWidget( MIP_FRect( 50,50 ));
    MEditorPanel->appendWidget(knob4);
    knob4->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
    connect(knob4,3);

    // window sizer

    MIP_SizerWidget* MSizer = new MIP_SizerWidget(MIP_FRect( 15,15),MIP_SIZER_WINDOW);
    MSizer->layout.alignment = MIP_WIDGET_ALIGN_BOTTOM_RIGHT;
    MEditorPanel->appendWidget(MSizer);

    // button row

    MIP_ButtonRowWidget* button_row = new MIP_ButtonRowWidget(MIP_FRect(230,20), 6, buttonrow_text, MIP_BUTTON_ROW_MULTI );
    MEditorPanel->appendWidget(button_row);
    button_row->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;

    // slider

    MIP_SliderWidget* slider = new MIP_SliderWidget(MIP_FRect(110,20), "Slider", 0.5 );
    MEditorPanel->appendWidget(slider);
    slider->layout.alignment  = MIP_WIDGET_ALIGN_FILL_TOP;

    // selector

    MIP_SelectorWidget* selector = new MIP_SelectorWidget(MIP_FRect(110,20));
    MEditorPanel->appendWidget(selector);
    selector->setMenu(menu1);
    selector->layout.alignment  = MIP_WIDGET_ALIGN_FILL_TOP;

    // append menu last (so it draws on top of everything else)

    MEditorPanel->appendWidget(menu1);

    // window

    MIP_Window* win = getWindow();
    MSizer->setTarget(win);
    win->appendWidget(MEditorPanel);

  }

  //----------

  virtual ~myEditor() {
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myPlugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  clap_param_info_t myParameters[NUM_PARAMS] = {

    { 0,
      CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE | CLAP_PARAM_IS_PER_NOTE,
      nullptr,
      "param1",
      "Params",
      0.0,
      1.0,
      0.2
    },

    { 1,
      CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE,
      nullptr,
      "param2",
      "Params",
      0.0,
      1.0,
      0.4
    },

    { 2,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "param3",
      "Params",
      0.0,
      1.0,
      0.6
    },

    { 3,
      0,
      nullptr,
      "param4",
      "Params",
      0.0,
      1.0,
      0.8
    }

  };

  MIP_VoiceManager<myVoice,16>  MVoices = {};

  // todo: -> descriptor..
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
    //for (uint32_t i=0; i<NUM_PARAMS; i++) {
    //  appendParameter(new MIP_Parameter( &myParameters[i] ));
    //}
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
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {

        switch (header->type) {
          case CLAP_EVENT_PARAM_VALUE:
            handle_parameter_event((clap_event_param_value_t*)header);
            break;
          case CLAP_EVENT_PARAM_MOD:
            handle_modulation_event((clap_event_param_mod_t*)header);
            break;
        }

        MVoices.on_event(header);
      }
    }
  }

  //----------

  //void handle_events_output(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
  //  //float v0 = MParameterValues[0] + MParameterModulations[0];
  //  //v0 = MIP_Clamp(v0,0,1);
  //  //send_param_mod_event(0,v0,out_events);
  //  MIP_Plugin::handle_events_output(in_events,out_events);
  //}

  //----------

  //void handle_parameter_event(const clap_event_param_value_t* param_value) final {
  //  MIP_Plugin::handle_parameter_event(param_value);
  //}

  //----------

  //void handle_modulation_event(const clap_event_param_mod_t* param_mod) final {
  //  MIP_Plugin::handle_modulation_event(param_mod);
  //}

  //----------

  void handle_process(const clap_process_t *process) final {
    float** outputs = process->audio_outputs[0].data32;
    uint32_t length = process->frames_count;
    MIP_ClearStereoBuffer(outputs,length);
    MVoices.process(process);
    // post-process?
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

//void MIP_Unregister(MIP_ClapRegistry* ARegistry) {
//}

//----------------------------------------------------------------------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    case 0: return new myPlugin(ADescriptor,AHost);
  }
  return nullptr;
}
