
/*
  TODO:
  - the voices are still in mono..
  - proper db and freq (params/mods)
  - scalable editor

  - parameter modulation..
  don't draw anything until we receives the first modulation event?

*/

//----------------------------------------------------------------------

#define MIP_GUI_XCB
#define MIP_PAINTER_CAIRO

//#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket
//tail -F ~/.BitwigStudio/log/engine.log | pv --rate --bytes > /dev/null

//#define MIP_DEBUG_PRINT_TIME
//#define MIP_DEBUG_WATCHES
//#define MIP_DEBUG_CALLSTACK
//#define MIP_DEBUG_CRASH_HANDLER

//#define MIP_DEBUG_CLAP
#define MIP_VST2
#define MIP_VST3

//----------------------------------------------------------------------

#define NUM_VOICES    128
#define EDITOR_WIDTH  816
#define EDITOR_HEIGHT 683

//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_plugin.h"

//#include "plugin/mip_voice_manager.h"
//#include "plugin/mip_block_voice_manager.h"
#include "plugin/mip_split_voice_manager.h"
//#include "plugin/mip_threaded_voice_manager.h"

//#include "base/utils/mip_lua.h"
//#include "base/utils/mip_vm.h"
//#include "base/utils/mip_vm_compiler.h"

//----------

#include "sa_ts1/sa_ts1_parameters.h"
#include "sa_ts1/sa_ts1_editor.h"
#include "sa_ts1/sa_ts1_voice.h"

typedef MIP_SplitVoiceManager<sa_ts1_Voice<double>,NUM_VOICES> sa_ts1_VoiceManager;

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

//----------

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "skei.audio/sa_ts1",
  #ifdef MIP_DEBUG
    .name       = "sa_ts1 (debug)",
  #else
    .name       = "sa_ts1",
  #endif
  .vendor       = "skei.audio",
  .url          = "https://torhelgeskei.com",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.7",
  //myFeatures
  .features     = (const char*[]){
    CLAP_PLUGIN_FEATURE_INSTRUMENT,
    CLAP_PLUGIN_FEATURE_SYNTHESIZER,
    nullptr
  }
};

//----------------------------------------------------------------------
//
// processor
//
//----------------------------------------------------------------------

//class myProcessor {
//};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin
//: public MIP_Plugin<myProcessor> {
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  // -> sa_ts1_parameters.h
  //clap_param_info_t myParameters[NUM_PARAMS] = {};

  //----------

  #define NUM_AUDIO_OUTPUTS 1

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

  #define NUM_NOTE_INPUTS 1

  clap_note_port_info_t myNoteInputs[NUM_NOTE_INPUTS] = {
    { 0,
      CLAP_NOTE_DIALECT_CLAP,
      CLAP_NOTE_DIALECT_CLAP,
      "Note In"
    }
  };

  //----------

  sa_ts1_VoiceManager MVoiceManager = {};
  clap_id MSelectedQuickControlsPage = 0;

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  //: MIP_Plugin<myProcessor>(ADescriptor,AHost) {
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
    setupParameters(sa_ts1_Parameters,NUM_PARAMS);
    setupAudioOutputs(myAudioOutputs,NUM_AUDIO_OUTPUTS);
    setupNoteInputs(myNoteInputs,NUM_NOTE_INPUTS);
    //setupNoteOutputs(myNoteOutputs,NUM_NOTE_OUTPUTS);

    //supportsExtension(CLAP_EXT_AUDIO_PORTS);

//    test_lua();

    return MIP_Plugin::init();
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MVoiceManager.prepareVoices(sample_rate,&MParameters);
    // send initial parameter values to the voices
    for (uint32_t i=0; i<NUM_PARAMS; i++) {
      //float v = MParameterValues[i];
      float v = MParameters[i]->getValue();
      MVoiceManager.setParameter(i,v);
    }
    return MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  //TODO: make more automatic..
  // uint32_t MSupportedExtensions in MIP_Plugin ?
  // set bits in constructor, cgeck and return ptr in MIP_Plugin::getextension..

  const void* get_extension(const char *id) final {
    //MIP_Print("host asks for: %s\n",id);
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0)     return &MAudioPorts;
    if (strcmp(id,CLAP_EXT_GUI) == 0)             return &MGui;
    if (strcmp(id,CLAP_EXT_NOTE_PORTS) == 0)      return &MNotePorts;
    if (MIP_IsEqual(id,CLAP_EXT_QUICK_CONTROLS))  return &MQuickControls;
    if (strcmp(id,CLAP_EXT_THREAD_POOL) == 0)     return &MThreadPool;
    if (strcmp(id,CLAP_EXT_VOICE_INFO) == 0)      return &MVoiceInfo;
    return MIP_Plugin::get_extension(id);
  }

  //----------

  clap_process_status process(const clap_process_t *process) final {
    flushAudioParams();
    preProcessEvents(process->in_events,process->out_events);

    // processes into MFrameBuffer, which is then copied into 'outputs'
    // todo: do the copy heer, sp we can add effects..
    MVoiceManager.process(process);

    float v = MParameters[PAR_MASTER_VOL]->getValue();  // vol
    float p = MParameters[PAR_MASTER_PAN]->getValue();  // pan
    float l = v * (1.0 - p);
    float r = v * (      p);

    float** outputs = process->audio_outputs[0].data32;
    uint32_t length = process->frames_count;
    MIP_ScaleStereoBuffer(outputs,l,r,length);

    postProcessEvents(process->in_events,process->out_events);
    flushHostParams(process->out_events);

    // hack!
    // update gui (state only) in process!
    if (MEditor && MIsEditorOpen) {
      uint32_t num_playing = 0;
      uint32_t num_released = 0;
      for (uint32_t i=0; i<NUM_VOICES; i++) {
        uint32_t state = MVoiceManager.getVoiceState(i);
        ((sa_ts1_Editor*)MEditor)->MVoiceWidget->voice_state[i] = state;
        if (state == MIP_VOICE_PLAYING) num_playing += 1;
        if (state == MIP_VOICE_RELEASED) num_released += 1;
      }
      ((sa_ts1_Editor*)MEditor)->MPlayingVoicesWidget->setValue(num_playing);
      ((sa_ts1_Editor*)MEditor)->MReleasedVoicesWidget->setValue(num_released);
      ((sa_ts1_Editor*)MEditor)->MTotalVoicesWidget->setValue(num_playing + num_released);
    }

    return CLAP_PROCESS_CONTINUE;
  }

  //----------
  // gui
  //----------

  bool gui_create(const char *api, bool is_floating) final {
    MEditor = new sa_ts1_Editor(this,this,EDITOR_WIDTH,EDITOR_HEIGHT,true,&myDescriptor);
    if (!MEditor) return false;
    return true;
  }

  //----------
  // quick-controls
  //----------

  uint32_t quick_controls_count() final {
    return 1;
  }

  bool quick_controls_get(uint32_t page_index, clap_quick_controls_page_t *page) final {
    page->id = 0;
    strcpy(page->name,"MIP: page1");
    page->param_ids[0] = PAR_OSC1_SHAPE;
    page->param_ids[1] = PAR_OSC1_WIDTH;
    page->param_ids[2] = PAR_OSC2_SHAPE;
    page->param_ids[3] = PAR_OSC2_WIDTH;
    page->param_ids[4] = PAR_RES1_FB;
    page->param_ids[5] = PAR_RES1_DAMP;
    page->param_ids[6] = PAR_RES2_FB;
    page->param_ids[7] = PAR_RES2_DAMP;
    return true;
  }

  void quick_controls_select(clap_id page_id) final {
    MSelectedQuickControlsPage = page_id;
  }

  clap_id quick_controls_get_selected() final {
    return MSelectedQuickControlsPage;
  }

  //----------
  // thread-pool
  //----------

  void thread_pool_exec(uint32_t task_index) final {
    //MVoiceManager.processVoiceThread(task_index);
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


//------------------------------
protected:
//------------------------------

  /*
    parameter has changed via gui
    we need to tell the voices about it
  */

  //void handle_editor_parameter(uint32_t AIndex, float AValue) final {
  //  //MIP_Print("%i = %.3f\n",AIndex,AValue);
  //  MVoiceManager.setParameter(AIndex,AValue);
  //}

  void on_editor_listener_parameter(uint32_t AIndex, double AValue) override {
    MIP_Plugin::on_editor_listener_parameter(AIndex,AValue);
    MVoiceManager.setParameter(AIndex,AValue);
  }

};

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

void MIP_Register(MIP_Registry* ARegistry) {
  ARegistry->appendDescriptor(&myDescriptor);
}

//----------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    case 0: return new myPlugin(ADescriptor,AHost);
  }
  return nullptr;
}

