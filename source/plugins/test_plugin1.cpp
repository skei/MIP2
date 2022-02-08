
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
#include "gui/cairo/mip_cairo_painter.h"

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
  "torhelgeskei/test_plugin1/v0.0.0",
  "test_plugin1",
  "torhelgeskei",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.0",
  "simple mip2 test plugin",
  myFeatures
};

#define ALL_DIALECTS (CLAP_NOTE_DIALECT_CLAP | CLAP_NOTE_DIALECT_MIDI | CLAP_NOTE_DIALECT_MIDI_MPE | CLAP_NOTE_DIALECT_MIDI2)

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
  "simple mip2 test plugin 2",
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

  #define NUM_PARAMS          4
  #define NUM_AUDIO_INPUTS    2
  #define NUM_AUDIO_OUTPUTS   2
  #define NUM_NOTE_INPUTS     2
  #define NUM_NOTE_OUTPUTS    2
  #define NUM_QUICK_CONTROLS  2

  #define NUM_THREADS         16
  #define NUM_RANDOM          1024

  clap_param_info_t myParameters[NUM_PARAMS] = {
    { 0, CLAP_PARAM_IS_MODULATABLE, nullptr, "Gain",     "Params",   0.0, 1.0, 0.5 },
    { 1, 0,                         nullptr, "(param2)", "(unused)", 0.0, 1.0, 0.5 },
    { 2, 0,                         nullptr, "(param3)", "(unused)", 0.0, 1.0, 0.5 },
    { 3, 0,                         nullptr, "(param4)", "(unused)", 0.0, 1.0, 0.5 }
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
   { 0, ALL_DIALECTS, CLAP_NOTE_DIALECT_CLAP, "Notes 1" },
   { 1, ALL_DIALECTS, CLAP_NOTE_DIALECT_CLAP, "Notes 2" }
  };

  clap_note_port_info_t myNoteOutputs[NUM_NOTE_OUTPUTS] = {
   { 0, ALL_DIALECTS, CLAP_NOTE_DIALECT_CLAP, "Notes 1" },
   { 1, ALL_DIALECTS, CLAP_NOTE_DIALECT_CLAP, "Notes 2" }
  };

  clap_quick_controls_page_t myQuickControls[NUM_QUICK_CONTROLS] = {
    { 0, "Quick 1", "preset", {0,1,2,3,0,1,2,3} },
    { 1, "Quick 2", "device", {0,1,2,3,0,1,2,3} }
  };

  //----------

  MIP_PanelWidget* MEditorPanel = nullptr;
  float MSum = 0.0;

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

  void thread_pool_exec(uint32_t task_index) {
    float f = 0.0;
    for (uint32_t i=0; i<NUM_RANDOM; i++) f = MIP_RandomRange(-1.0,1.0);
     f *= (task_index * 0.00001);
    MSum += (1.0 / NUM_THREADS) + f;
  }

  //----------

  void handle_process(const clap_process_t *process) final {
    float**  inputs  = process->audio_inputs[0].data32;
    float**  outputs = process->audio_outputs[0].data32;
    uint32_t length  = process->frames_count;
    float    scale   = getParamVal(0) + getParamMod(0);

    // test thread pool
    if (MHost->thread_pool) {
      MSum = 0.0;
      bool didComputeVoices = false;
      didComputeVoices = MHost->thread_pool->request_exec(MHost->host,NUM_THREADS);
      if (!didComputeVoices) {
        for (uint32_t i=0; i<NUM_THREADS;i++) {
          thread_pool_exec(i);
        }
      }
      scale *= MSum;
    }
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

  const void* get_extension(const char *id) final {
    const void* ext = MIP_Plugin::get_extension(id);
    if (!ext) {
      if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0)     return &MAudioPorts;
      if (strcmp(id,CLAP_EXT_GUI) == 0)             return &MGui;
      if (strcmp(id,CLAP_EXT_GUI_X11) == 0)         return &MGuiX11;
      if (strcmp(id,CLAP_EXT_NOTE_PORTS) == 0)      return &MNotePorts;
      if (strcmp(id,CLAP_EXT_QUICK_CONTROLS) == 0)  return &MQuickControls;
      if (strcmp(id,CLAP_EXT_THREAD_POOL) == 0)     return &MThreadPool;
    }
    return ext;
    //return nullptr;
  }


  //----------

  bool gui_create() final {
    bool result = MIP_Plugin::gui_create();
    if (result) {
      // setup gui
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
      // connect widgets/parameters
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

// test

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

void MIP_RegisterPlugins(MIP_ClapList* AList) {
  AList->appendPlugin(&myDescriptor);
  AList->appendPlugin(&myDescriptor2);
}

//----------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    case 0: return new myPlugin(ADescriptor,AHost);
    case 1: return new myPlugin2(ADescriptor,AHost);
  }
  //else return new myPlugin2(ADescriptor,AHost);
  return nullptr;
}
