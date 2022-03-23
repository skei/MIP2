
#define MIP_USE_XCB
#define MIP_GUI_XCB
//#define MIP_PAINTER_XCB
#define MIP_USE_CAIRO
#define MIP_PAINTER_CAIRO

//#define MIP_NO_WINDOW_BUFFERING
//#define MIP_XCB_WAIT_FOR_MAPNOTIFY

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

#define NUM_PARAMS          4
#define NUM_AUDIO_INPUTS    2
#define NUM_AUDIO_OUTPUTS   2
#define NUM_NOTE_INPUTS     2
#define NUM_NOTE_OUTPUTS    2
#define NUM_QUICK_CONTROLS  2

//#define SUPPORTED_DIALECTS (CLAP_NOTE_DIALECT_CLAP | CLAP_NOTE_DIALECT_MIDI | CLAP_NOTE_DIALECT_MIDI_MPE | CLAP_NOTE_DIALECT_MIDI2)
#define SUPPORTED_DIALECTS  CLAP_NOTE_DIALECT_CLAP

//----------

const char* myFeatures1[] = {
  "instrument",
  nullptr
};

const char* myFeatures2[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t myDescriptor = {
  CLAP_VERSION,
  "torhelgeskei/test_plugin1/v0.0.0",
  "test_plugin1",
  "torhelgeskei",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.0",
  "simple mip2 test instrument",
  myFeatures1
};

//test
const clap_plugin_descriptor_t myDescriptor2 = {
  CLAP_VERSION,
  "torhelgeskei/test_plugin2/v0.0.0",
  "test_plugin2",
  "torhelgeskei",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.0",
  "simple mip2 test effect",
  myFeatures2
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myVoice {

private:

  MIP_VoiceContext* context = nullptr;
  float             hz      = 0.0;  // note hz
  float             ph      = 0.0;  // phase
  float             phadd   = 0.0;  // phase add

  int32_t _key  = -1;
  float   _onvel  = 0.0;
  float   _offvel = 0.0;
  float   _vol    = 0.0;
  float   _pan    = 0.0;
  float   _vibr   = 0.0;
  float   _expr   = 0.0;
  float   _bright = 0.0;
  float   _press  = 0.0;

public:

  void prepare(MIP_VoiceContext* AContext) {
    context = AContext;
  }

  //----------

  uint32_t note_on(int32_t key, float velocity) {
    //MIP_Print("note on %i %.2f\n",key,velocity);
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
    //float**  inputs = context->process->audio_inputs[0].data32;
    float* output0 = context->process->audio_outputs[0].data32[0];
    float* output1 = context->process->audio_outputs[0].data32[1];
    uint32_t length  = context->process->frames_count;
    for (uint32_t i=0; i<length; i++) {
      float out = ph;
      ph += phadd;
      ph = MIP_Fract(ph);
      // hack
      //float v = _onvel;
      //v += (0.0 + (1.0 * _press));
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
      0.5
    },

    { 1,
      CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE,
      nullptr,
      "param2",
      "Params",
      0.0,
      1.0,
      0.5
    },

    { 2,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "param3",
      "Params",
      0.0,
      1.0,
      0.5
    },

    { 3,
      0,
      nullptr,
      "param4",
      "Params",
      0.0,
      1.0,
      0.5
    }

  };

  clap_audio_port_info_t myAudioInputs[NUM_AUDIO_INPUTS] = {
    { 0, "Input 1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID },
    { 1, "Input 2", 0,                       2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  clap_audio_port_info_t myAudioOutputs[NUM_AUDIO_OUTPUTS] = {
    { 0, "Output 1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID },
    { 1, "Output 2", 0,                       2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  clap_note_port_info_t myNoteInputs[NUM_NOTE_INPUTS] = {
    { 0, SUPPORTED_DIALECTS, CLAP_NOTE_DIALECT_CLAP, "Notes 1" },
    { 1, SUPPORTED_DIALECTS, CLAP_NOTE_DIALECT_CLAP, "Notes 2" }
  };

  clap_note_port_info_t myNoteOutputs[NUM_NOTE_OUTPUTS] = {
    { 0, SUPPORTED_DIALECTS, CLAP_NOTE_DIALECT_CLAP, "Notes 1" },
    { 1, SUPPORTED_DIALECTS, CLAP_NOTE_DIALECT_CLAP, "Notes 2" }
  };

  clap_quick_controls_page_t myQuickControls[NUM_QUICK_CONTROLS] = {
    { 0, "Quick 1", "preset", {0,1,2,3,0,1,2,3} },
    { 1, "Quick 2", "device", {0,1,2,3,0,1,2,3} }
  };

  //----------

  MIP_PanelWidget*  MEditorPanel  = nullptr;
  MIP_SizerWidget*  MSizer        = nullptr;

  float MSum = 0.0;

  MIP_VoiceManager<myVoice,16>  MVoices = {};

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
public:
//------------------------------

//  void handle_parameter_event(const clap_event_param_value_t* param_value) final {
//    MIP_Plugin::handle_parameter_event(param_value);
//  }
//
//  //----------
//
//  void handle_modulation_event(const clap_event_param_mod_t* param_mod) final {
//    MIP_Plugin::handle_modulation_event(param_mod);
//  }

  void handle_events_input(const clap_input_events_t* in_events, const clap_output_events_t* out_events) override {
    //MIP_Plugin::handle_events_input(in_events,out_events);
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        switch (header->type) {

          case CLAP_EVENT_NOTE_ON:
            MVoices.on_note_on((clap_event_note_t*)header);
            break;

          case CLAP_EVENT_NOTE_OFF:
            MVoices.on_note_off((clap_event_note_t*)header);
            break;

          case CLAP_EVENT_NOTE_END:
            MVoices.on_note_end((clap_event_note_t*)header);
            break;

          case CLAP_EVENT_NOTE_CHOKE:
            MVoices.on_note_choke((clap_event_note_t*)header);
            break;

          case CLAP_EVENT_PARAM_VALUE:
            handle_parameter_event((clap_event_param_value_t*)header);
            MVoices.on_parameter_value((clap_event_param_value_t*)header);
            break;

          case CLAP_EVENT_PARAM_MOD:
            handle_modulation_event((clap_event_param_mod_t*)header);
            MVoices.on_parameter_modulation((clap_event_param_mod_t*)header);
            break;

          //case CLAP_EVENT_MIDI:
          //case CLAP_EVENT_MIDI2:
          //case CLAP_EVENT_MIDI_SYSEX:
          //case CLAP_EVENT_TRANSPORT:

          case CLAP_EVENT_NOTE_EXPRESSION:
            {
              clap_event_note_expression_t* event = (clap_event_note_expression_t*)header;
              switch (event->expression_id) {
                case CLAP_NOTE_EXPRESSION_VOLUME:       MVoices.on_note_volume_expression(event);      break;
                case CLAP_NOTE_EXPRESSION_PAN:          MVoices.on_note_pan_expression(event);         break;
                case CLAP_NOTE_EXPRESSION_TUNING:       MVoices.on_note_tuning_expression(event);      break;
                case CLAP_NOTE_EXPRESSION_VIBRATO:      MVoices.on_note_vibrato_expression(event);     break;
                case CLAP_NOTE_EXPRESSION_EXPRESSION:   MVoices.on_note_expression_expression(event);  break;
                case CLAP_NOTE_EXPRESSION_BRIGHTNESS:   MVoices.on_note_brightness_expression(event);  break;
                case CLAP_NOTE_EXPRESSION_PRESSURE:     MVoices.on_note_pressure_expression(event);    break;
              }
            }

          //default:
          //  break;

        }
      }
    }
  }

//------------------------------
public:
//------------------------------

  void handle_process(const clap_process_t *process) final {
    //float** inputs = process->audio_inputs[0].data32;
    float** outputs = process->audio_outputs[0].data32;
    uint32_t length = process->frames_count;
    //float scale = getParameterValue(0) + getParameterModulation(0);
    //MIP_CopyStereoBuffer(outputs,inputs,length);
    //MIP_ScaleStereoBuffer(outputs,scale,length);
    MIP_ClearStereoBuffer(outputs,length);
    MVoices.process(process);
  }

  //----------

  //void handle_events_output(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
  //  //float v0 = MParameterValues[0] + MParameterModulations[0];
  //  //v0 = MIP_Clamp(v0,0,1);
  //  //send_param_mod_event(0,v0,out_events);
  //  MIP_Plugin::handle_events_output(in_events,out_events);
  //}

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    for (uint32_t i=0; i<NUM_PARAMS; i++) {
      appendParameter(new MIP_Parameter( &myParameters[i] ));
    }
    setupAudioInputs(myAudioInputs,NUM_AUDIO_INPUTS);
    setupAudioOutputs(myAudioOutputs,NUM_AUDIO_OUTPUTS);
    setupNoteInputs(myNoteInputs,NUM_NOTE_INPUTS);
    setupNoteOutputs(myNoteOutputs,NUM_NOTE_OUTPUTS);
    setupQuickControls(myQuickControls,NUM_QUICK_CONTROLS);
    if (MIP_Plugin::init()) {
      //setDefaultParameterValues(myParameters,NUM_PARAMS);
      setDefaultParameterValues();
    return true;
    }
    return false;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MVoices.prepare(sample_rate);
    return MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }


  //----------

  const void* get_extension(const char *id) final {
    const void* ext = MIP_Plugin::get_extension(id);
    if (!ext) {
      if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0)     return &MAudioPorts;
      if (strcmp(id,CLAP_EXT_GUI) == 0)             return &MGui;
      if (strcmp(id,CLAP_EXT_NOTE_PORTS) == 0)      return &MNotePorts;
      if (strcmp(id,CLAP_EXT_QUICK_CONTROLS) == 0)  return &MQuickControls;
      if (strcmp(id,CLAP_EXT_THREAD_POOL) == 0)     return &MThreadPool;
    }
    return ext;
  }

//------------------------------
public: // gui
//------------------------------

  const char* buttonrow_text[6] = { "1", "2", "3", "four", "5", "6" };

  //----------

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
    MEditor = new MIP_Editor(this,this,400,400,true);
    // -> myEditor()
    bool result = (MEditor);
    if (result) {

      MEditor->setCanResize();

      MIP_MenuWidget* menu1 = new MIP_MenuWidget( MIP_FRect(100,100) );
      menu1->appendMenuItem("first");
      menu1->appendMenuItem("item2");
      menu1->appendMenuItem("item3");
      menu1->appendMenuItem("4");
      menu1->appendMenuItem("five");
      menu1->setItemSize(90,20);
      menu1->setItemLayout(1,5);
      menu1->setMenuMirror(true,false);
      MEditorPanel = new MIP_PanelWidget(MIP_FRect(100,100));
      MEditorPanel->setBackgroundColor(0.6);
      MEditorPanel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
      MEditorPanel->layout.innerBorder = MIP_FRect(10,10,10,10);
      MEditorPanel->layout.spacing = 5;
      MIP_KnobWidget*   knob1 = new MIP_KnobWidget( MIP_FRect( 50,50 ));
      MIP_KnobWidget*   knob2 = new MIP_KnobWidget( MIP_FRect( 50,50 ));
      MIP_KnobWidget*   knob3 = new MIP_KnobWidget( MIP_FRect( 50,50 ));
      MIP_KnobWidget*   knob4 = new MIP_KnobWidget( MIP_FRect( 50,50 ));
      knob1->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
      knob2->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
      knob3->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
      knob4->layout.alignment  = MIP_WIDGET_ALIGN_STACK_HORIZ;
      MEditorPanel->appendWidget(knob1);
      MEditorPanel->appendWidget(knob2);
      MEditorPanel->appendWidget(knob3);
      MEditorPanel->appendWidget(knob4);
      MSizer = new MIP_SizerWidget(MIP_FRect( 15,15),MIP_SIZER_WINDOW);
      MSizer->layout.alignment = MIP_WIDGET_ALIGN_BOTTOM_RIGHT;
      MEditorPanel->appendWidget(MSizer);
      MIP_ButtonRowWidget* button_row = new MIP_ButtonRowWidget(MIP_FRect(230,20), 6, buttonrow_text, MIP_BUTTON_ROW_MULTI );
      button_row->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      MIP_SliderWidget* slider = new MIP_SliderWidget(MIP_FRect(110,20), "Slider", 0.5 );
      slider->layout.alignment  = MIP_WIDGET_ALIGN_FILL_TOP;
      MIP_SelectorWidget* selector = new MIP_SelectorWidget(MIP_FRect(110,20));
      selector->setMenu(menu1);
      selector->layout.alignment  = MIP_WIDGET_ALIGN_FILL_TOP;
      MEditorPanel->appendWidget(button_row);
      MEditorPanel->appendWidget(slider);
      MEditorPanel->appendWidget(selector);
      MEditorPanel->appendWidget(menu1);
      MEditor->connect(knob1,0);
      MEditor->connect(knob2,1);
      MEditor->connect(knob3,2);
      MEditor->connect(knob4,3);
      MIP_Window* win = MEditor->getWindow();
      MSizer->setTarget(win);
      win->appendWidget(MEditorPanel);
    }
    return result;
  }

  //----------

  bool gui_show() final {
    // (MIP_Plugin doesn't know about myParameters)
    //setEditorParameterValues(myParameters,NUM_PARAMS);
    setEditorParameterValues();
    MIP_Plugin::gui_show();
    return true;
  }

};

//----------------------------------------------------------------------

//#undef NUM_THREADS
//#undef NUM_RANDOM

#undef NUM_PARAMS
#undef NUM_AUDIO_INPUTS
#undef NUM_AUDIO_OUTPUTS
#undef NUM_NOTE_INPUTS
#undef NUM_NOTE_OUTPUTS
#undef NUM_QUICK_CONTROLS
#undef ALL_DIALECTS

//----------------------------------------------------------------------
//
// plugin 2
//
//----------------------------------------------------------------------

class myPlugin2
: public MIP_Plugin {
public:
  myPlugin2(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) : MIP_Plugin(ADescriptor,AHost) {}
  virtual ~myPlugin2() {}
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void MIP_Register(MIP_ClapRegistry* ARegistry) {
  ARegistry->appendPlugin(&myDescriptor);
  ARegistry->appendPlugin(&myDescriptor2);
}

//----------

//void MIP_Unregister(MIP_ClapRegistry* ARegistry) {
//}

//----------------------------------------------------------------------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  MIP_PRINT;
  switch (AIndex) {
    case 0: return new myPlugin(ADescriptor,AHost);
    case 1: return new myPlugin2(ADescriptor,AHost);
  }
  return nullptr;
}
