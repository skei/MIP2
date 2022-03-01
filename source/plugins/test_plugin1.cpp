
#define MIP_USE_XCB
#define MIP_GUI_XCB
//#define MIP_PAINTER_XCB

#define MIP_USE_CAIRO
#define MIP_PAINTER_CAIRO

//#define MIP_NO_WINDOW_BUFFERING

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

#define NUM_PARAMS          4
#define NUM_AUDIO_INPUTS    2
#define NUM_AUDIO_OUTPUTS   2
#define NUM_NOTE_INPUTS     2
#define NUM_NOTE_OUTPUTS    2
#define NUM_QUICK_CONTROLS  2

#define ALL_DIALECTS (CLAP_NOTE_DIALECT_CLAP | CLAP_NOTE_DIALECT_MIDI | CLAP_NOTE_DIALECT_MIDI_MPE | CLAP_NOTE_DIALECT_MIDI2)

//----------

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

  clap_param_info_t myParameters[NUM_PARAMS] = {
    { 0, CLAP_PARAM_IS_MODULATABLE, nullptr, "Gain",   "Params", 0.0, 1.0, 0.5 },
    { 1, 0,                         nullptr, "param2", "Params", 0.0, 1.0, 0.5 },
    { 2, 0,                         nullptr, "param3", "Params", 0.0, 1.0, 0.5 },
    { 3, 0,                         nullptr, "param4", "Params", 0.0, 1.0, 0.5 }
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

  MIP_PanelWidget*  MEditorPanel  = nullptr;
  MIP_SizerWidget*  MSizer        = nullptr;

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
public:
//------------------------------

  /*
    if we want to do something special with incoming parameter values or
      modulations, we can do it here.. otherwise we can just use the values
      in MParameterValues and MParameterModulations arrays
      (set by MIP_Plugin, so careful with ordering if you override it)
  */

  //----------

  //void handle_parameter_event(const clap_event_param_value_t* param_value) final {
  //  MIP_Plugin::handle_parameter_event(param_value);
  //}

  //----------

  //void handle_modulation_event(const clap_event_param_mod_t* param_mod) final {
  //  MIP_Plugin::handle_modulation_event(param_mod);
  //}

//------------------------------
public:
//------------------------------

  /*
    thread pool experiment that didn't go so well..
    need to find a more suitable test setup..
  */

  //----------

  //void thread_pool_exec(uint32_t task_index) final {
  //  float f = 0.0;
  //  for (uint32_t i=0; i<NUM_RANDOM; i++) {
  //    f += (MIP_Random() * 0.00001);
  //  }
  //  MSum += (1.0 / NUM_THREADS) + f;
  //}

  //----------

  //#define NUM_THREADS 16
  //#define NUM_RANDOM  512
  //
  //void test_thread_pool() {
  //  if (MHost->thread_pool) {
  //    bool didComputeVoices = false;
  //    //didComputeVoices = MHost->thread_pool->request_exec(MHost->host,NUM_THREADS);
  //    if (!didComputeVoices) {
  //      for (uint32_t i=0; i<NUM_THREADS;i++) {
  //        thread_pool_exec(i);
  //        //MThreadPool.exec(&MPlugin,i);
  //      }
  //    }
  //  }
  //}

//------------------------------
public:
//------------------------------

  void handle_process(const clap_process_t *process) final {
    float**  inputs  = process->audio_inputs[0].data32;
    float**  outputs = process->audio_outputs[0].data32;
    uint32_t length  = process->frames_count;
    float    scale   = getParameterValue(0) + getParameterModulation(0);
    //MSum = 0.0;
    //test_thread_pool();
    //scale *= MSum;
    MIP_CopyStereoBuffer(outputs,inputs,length);
    MIP_ScaleStereoBuffer(outputs,scale,length);
  }

  //----------

  // called from end of process()
  // or param_flush..
  // can we send param_mod events from flush()?

  void handle_events_output(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
    { // send modulation value for parameter 0 to host
      float v0 = MParameterValues[0] + MParameterModulations[0];
      v0 = MIP_Clamp(v0,0,1);
      send_param_mod_event(0,v0,out_events);
      //float v1 = MParameterValues[1] + MParameterModulations[1];
      //v0 = MIP_Clamp(v1,0,1);
      //send_param_mod_event(1,v1,out_events);
    }
    MIP_Plugin::handle_events_output(in_events,out_events);
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
  }

  //----------

  const char* buttonrow_text[6] = { "1", "2", "3", "four", "5", "6" };

  //----------

  bool gui_create() final {

    MIP_PRINT;
    //bool result = MIP_Plugin::gui_create();

    MEditor = new MIP_Editor(this,this,400,400);
    MIsEditorOpen = false;

    MEditor->setCanResize();
    bool result = (MEditor);
    if (result) {
      MEditorPanel = new MIP_PanelWidget(MIP_FRect(0));
      MEditorPanel->setBackgroundColor(0.6);
      //MEditorPanel->layout.innerBorder = 2;
      MEditorPanel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
      MIP_KnobWidget*   knob1 = new MIP_KnobWidget(MIP_FRect( 10,10, 50,50));
      MIP_KnobWidget*   knob2 = new MIP_KnobWidget(MIP_FRect( 70,10, 50,50));
      MIP_KnobWidget*   knob3 = new MIP_KnobWidget(MIP_FRect(130,10, 50,50));
      MIP_KnobWidget*   knob4 = new MIP_KnobWidget(MIP_FRect(190,10, 50,50));
      MSizer = new MIP_SizerWidget(MIP_FRect(15,15),MIP_SIZER_WINDOW);//MEditor->getWindow());
      MSizer->layout.alignment = MIP_WIDGET_ALIGN_BOTTOM_RIGHT;
      MEditorPanel->appendWidget(knob1);
      MEditorPanel->appendWidget(knob2);
      MEditorPanel->appendWidget(knob3);
      MEditorPanel->appendWidget(knob4);
      MEditorPanel->appendWidget(MSizer);

      // test

      MIP_MenuWidget* menu1 = new MIP_MenuWidget( MIP_FRect(100,100) );
      menu1->appendMenuItem("first");
      menu1->appendMenuItem("item2");
      menu1->appendMenuItem("item3");
      menu1->appendMenuItem("4");
      menu1->appendMenuItem("five");
      menu1->setItemSize(90,20);
      menu1->setItemLayout(1,5);
      menu1->setMenuMirror(true,false);

      MEditorPanel->appendWidget( new MIP_ButtonRowWidget(MIP_FRect(  10, 70, 230, 20 ), 6, buttonrow_text, MIP_BUTTON_ROW_MULTI ));
      MEditorPanel->appendWidget( new MIP_SliderWidget(   MIP_FRect(  10,100, 110, 20 ), "Slider", 0.5 ));

      MIP_SelectorWidget* selector = (MIP_SelectorWidget*)MEditorPanel->appendWidget( new MIP_SelectorWidget( MIP_FRect( 130,100, 110, 20 )  ));
      selector->setMenu(menu1);

      MEditorPanel->appendWidget(menu1);

      if (MEditor) {
        MEditor->connect(knob1,0);
        MEditor->connect(knob2,1);
        MEditor->connect(knob3,2);
        MEditor->connect(knob4,3);
        MIP_Window* win = MEditor->getWindow();
        MSizer->setTarget(win);
        win->appendWidget(MEditorPanel);
        //win->alignWidgets();
        //win->on_window_paint(0,0,640,480);
      }
    }
    return result;
  }

  //----------

  void gui_show() final {
    // (MIP_Plugin doesn't know about myParameters)
    setEditorParameterValues(myParameters,NUM_PARAMS);
    MIP_Plugin::gui_show();
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

//----------------------------------------------------------------------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    case 0: return new myPlugin(ADescriptor,AHost);
    case 1: return new myPlugin2(ADescriptor,AHost);
  }
  return nullptr;
}
