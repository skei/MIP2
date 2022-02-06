
//#define MIP_USE_CAIRO
#define MIP_USE_XCB
#define MIP_GUI_XCB

#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

//----------

#include "mip.h"

#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"
#include "gui/mip_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* myFeatures[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t myDescriptor = {
  CLAP_VERSION,
  "torhelgeskei/test/v0.0.0",
  #ifdef MIP_DEBUG
    "mip_debug",
  #else
    "mip_release",
  #endif
  "torhelgeskei",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.0",
  "simple mip2 test plugin",
  myFeatures
};

#define ALL_DIALECTS (CLAP_NOTE_DIALECT_CLAP | CLAP_NOTE_DIALECT_MIDI | CLAP_NOTE_DIALECT_MIDI_MPE | CLAP_NOTE_DIALECT_MIDI2)

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

  #define NUM_PARAMS          4
  #define NUM_AUDIO_INPUTS    2
  #define NUM_AUDIO_OUTPUTS   2
  #define NUM_NOTE_INPUTS     2
  #define NUM_NOTE_OUTPUTS    2
  #define NUM_QUICK_CONTROLS  2

  clap_param_info_t
  myParameters[NUM_PARAMS] = {
    { 0, 0 /*CLAP_PARAM_IS_MODULATABLE*/, nullptr, "param1", "", 0.0, 1.0, 0.5 },
    { 1, 0 /*CLAP_PARAM_IS_MODULATABLE*/, nullptr, "param2", "", 0.0, 1.0, 0.5 },
    { 2, 0 /*CLAP_PARAM_IS_MODULATABLE*/, nullptr, "param3", "", 0.0, 1.0, 0.5 },
    { 3, 0 /*CLAP_PARAM_IS_MODULATABLE*/, nullptr, "param4", "", 0.0, 1.0, 0.5 }
  };

  clap_audio_port_info_t myAudioInputs[NUM_AUDIO_INPUTS] = {
    { 0, "input1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID },
    { 1, "input2", 0,                       2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  clap_audio_port_info_t myAudioOutputs[NUM_AUDIO_OUTPUTS] = {
    { 0, "output1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID },
    { 1, "output2", 0,                       2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };


  clap_note_port_info_t myNoteInputs[NUM_NOTE_INPUTS] = {
   { 0, ALL_DIALECTS, CLAP_NOTE_DIALECT_CLAP, "notes1" },
   { 1, ALL_DIALECTS, CLAP_NOTE_DIALECT_CLAP, "notes2" }
  };

  clap_note_port_info_t myNoteOutputs[NUM_NOTE_OUTPUTS] = {
   { 0, ALL_DIALECTS, CLAP_NOTE_DIALECT_CLAP, "notes1" },
   { 1, ALL_DIALECTS, CLAP_NOTE_DIALECT_CLAP, "notes2" }
  };

  clap_quick_controls_page_t myQuickControls[NUM_QUICK_CONTROLS] = {
    { 0, "quick1", "preset", {0,1,2,3,0,1,2,3} },
    { 1, "quick2", "device", {0,1,2,3,0,1,2,3} }
  };

  //----------

  MIP_PanelWidget* MEditorPanel = nullptr;

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
private:
//------------------------------

  //void handle_parameter_event(const clap_event_param_value_t* param_value) final {
  //  MIP_Plugin::handle_parameter_event(param_value);
  //}

  //----------

  //void handle_modulation_event(const clap_event_param_mod_t* param_mod) final {
  //  MIP_Plugin::handle_modulation_event(param_mod);
  //}

  //----------

  void handle_process(const clap_process_t *process) final {
    //float* in0 = process->audio_inputs[0].data32[0];
    //float* in1 = process->audio_inputs[0].data32[1];
    //float* out0 = process->audio_outputs[0].data32[0];
    //float* out1 = process->audio_outputs[0].data32[1];
    //uint32_t num = process->frames_count;
    //for (uint32_t i=0; i<num; i++) {
    //  float v = getParamVal(0) + getParamMod(0);
    //  v = MIP_Clamp(v,0,1);
    //  *out0++ = *in0++ * v;
    //  *out1++ = *in1++ * v;
    //}
    float** inputs = process->audio_inputs[0].data32;
    float** outputs = process->audio_outputs[0].data32;
    uint32_t length = process->frames_count;
    float scale = getParamVal(0) + getParamMod(0);
    MIP_CopyStereoBuffer(outputs,inputs,length);
    MIP_ScaleStereoBuffer(outputs,scale,length);
  }

  //----------

  void handle_output_events(const clap_output_events_t* out_events) final {
    if (MEditor && MIsEditorOpen) {
      float v0 = MParamVal[0] + MParamMod[0];
      v0 = MIP_Clamp(v0,0,1);
      MEditor->send_param_mod(0,v0,out_events);
    }
    MIP_Plugin::handle_output_events(out_events);
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    setupParameters(myParameters,NUM_PARAMS);
    setupAudioInputs(myAudioInputs,NUM_AUDIO_INPUTS);
    setupAudioOutputs(myAudioOutputs,NUM_AUDIO_OUTPUTS);
    setupNoteInputs(myNoteInputs,NUM_NOTE_INPUTS);
    setupNoteOutputs(myNoteOutputs,NUM_NOTE_OUTPUTS);
    setupQuickControls(myQuickControls,NUM_QUICK_CONTROLS);
    bool result = MIP_Plugin::init();
    if (result) {
      setDefaultParameterValues(myParameters,NUM_PARAMS);
    }
    return result;
  }

  //----------

  bool gui_create() final {
    bool result = MIP_Plugin::gui_create();
    if (result) {
      MEditorPanel = new MIP_PanelWidget(MIP_FRect(0));
      MEditorPanel->setBackgroundColor(0.6);
      MEditorPanel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
      MIP_KnobWidget* knob1 = new MIP_KnobWidget(MIP_FRect( 10,10, 50,50));
      MIP_KnobWidget* knob2 = new MIP_KnobWidget(MIP_FRect( 70,10, 50,50));
      MIP_KnobWidget* knob3 = new MIP_KnobWidget(MIP_FRect(130,10, 50,50));
      MIP_KnobWidget* knob4 = new MIP_KnobWidget(MIP_FRect(190,10, 50,50));
      MEditorPanel->appendWidget(knob1);
      MEditorPanel->appendWidget(knob2);
      MEditorPanel->appendWidget(knob3);
      MEditorPanel->appendWidget(knob4);
      if (MEditor) {
        MEditor->connect(knob1,0);
        MEditor->connect(knob2,1);
        MEditor->connect(knob3,2);
        MEditor->connect(knob4,3);
      }
    }
    return result;
  }

  void gui_show() final {
    setEditorParameterValues(myParameters,NUM_PARAMS);
    MIP_Plugin::gui_show();
  }

  //----------

  bool gui_x11_attach(const char *display_name, unsigned long window) final {
    bool result = MIP_Plugin::gui_x11_attach(display_name,window);
    if (result) {
      if (MEditor) {
        MIP_Window* win = MEditor->getWindow();
        win->appendWidget(MEditorPanel);
      }
    }
    return result;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void MIP_RegisterPlugins(MIP_ClapList* AList) {
  AList->appendPlugin(&myDescriptor);
}

//----------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  if (AIndex == 0) {
    return new myPlugin(ADescriptor,AHost);
  }
  return nullptr;
}
