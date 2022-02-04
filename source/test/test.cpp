
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
    "test_debug",
  #else
    "test",
  #endif
  "torhelgeskei",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.0",
  "simple mip2 test plugin",
  myFeatures
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

  clap_param_info_t
  myParameters[4] = {
    { 0, 0 /*CLAP_PARAM_IS_MODULATABLE*/, nullptr, "param1", "", 0.0, 1.0, 0.5 },
    { 1, 0 /*CLAP_PARAM_IS_MODULATABLE*/, nullptr, "param2", "", 0.0, 1.0, 0.5 },
    { 2, 0 /*CLAP_PARAM_IS_MODULATABLE*/, nullptr, "param3", "", 0.0, 1.0, 0.5 },
    { 3, 0 /*CLAP_PARAM_IS_MODULATABLE*/, nullptr, "param4", "", 0.0, 1.0, 0.5 }
  };

  clap_audio_port_info_t
  myAudioInputs[2] = {
    { 0, "input1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID },
    { 1, "input2", 0,                       2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  clap_audio_port_info_t
  myAudioOutputs[2] = {
    { 0, "output1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID },
    { 1, "output2", 0,                       2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

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

  void handle_parameter_event(const clap_event_param_value_t* param_value) override {
    MIP_Plugin::handle_parameter_event(param_value);
  }

  //----------

  void handle_modulation_event(const clap_event_param_mod_t* param_mod) override {
    MIP_Plugin::handle_modulation_event(param_mod);
  }

  //----------

  void handle_process(const clap_process_t *process) override {
    float* in0 = process->audio_inputs[0].data32[0];
    float* in1 = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    uint32_t num = process->frames_count;
    for (uint32_t i=0; i<num; i++) {
      float v = getParamVal(0) + getParamMod(0);
      v = MIP_Clamp(v,0,1);
      *out0++ = *in0++ * v;
      *out1++ = *in1++ * v;
    }
  }

//------------------------------
public: // plugin
//------------------------------

  #define ARRAY_SIZE(x) ( sizeof(x) / sizeof((x)[0]) )

  bool init() override {
    setupParameters(myParameters,ARRAY_SIZE(myParameters));
    setupAudioInputs(myAudioInputs,ARRAY_SIZE(myAudioInputs));
    setupAudioOutputs(myAudioOutputs,ARRAY_SIZE(myAudioOutputs));
    return MIP_Plugin::init();
  }

  #undef ARRAY_SIZE

  //----------

  void gui_show() override {
    MIP_Window* window = MEditor->getWindow();
    window->setFillBackground();
    window->setBackgroundColor(0.6);
      MIP_PanelWidget* panel = new MIP_PanelWidget(MIP_FRect(0));
      panel->setBackgroundColor(0.6);
      panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
      window->appendWidget(panel);
        MIP_KnobWidget* knob1 = new MIP_KnobWidget(MIP_FRect( 10,  10, 50,50 ));
        //knob1->layout.alignment = MIP_WIDGET_ALIGN_PARENT;
        knob1->setValue(getParamVal(0));
        panel->appendWidget(knob1);
        MIP_KnobWidget* knob2 = new MIP_KnobWidget(MIP_FRect( 70,  10, 50,50 ));
        //knob2->layout.alignment = MIP_WIDGET_ALIGN_PARENT;
        knob2->setValue(getParamVal(1));
        panel->appendWidget(knob2);
        MIP_KnobWidget* knob3 = new MIP_KnobWidget(MIP_FRect( 130, 10, 50,50 ));
        //knob3->layout.alignment = MIP_WIDGET_ALIGN_PARENT;
        knob3->setValue(getParamVal(2));
        panel->appendWidget(knob3);
        MIP_KnobWidget* knob4 = new MIP_KnobWidget(MIP_FRect( 190, 10, 50,50 ));
        //knob4->layout.alignment = MIP_WIDGET_ALIGN_PARENT;
        knob4->setValue(getParamVal(3));
        panel->appendWidget(knob4);
    MEditor->connect(knob1,0);
    MEditor->connect(knob2,1);
    MEditor->connect(knob3,2);
    MEditor->connect(knob4,3);
    MIP_Plugin::gui_show();
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
