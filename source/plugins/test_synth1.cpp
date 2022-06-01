
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

#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket
//tail -F ~/.BitwigStudio/log/engine.log | pv --rate --bytes > /dev/null

//#define MIP_DEBUG_PRINT_TIME
//#define MIP_DEBUG_WATCHES
//#define MIP_DEBUG_CALLSTACK
//#define MIP_DEBUG_CRASH_HANDLER

//#define MIP_VST2
//#define MIP_VST3

//----------------------------------------------------------------------

#define PAR_VOL         0
#define PAR_PAN         1
#define PAR_OSC1_OUT    2
#define PAR_RES1_OUT    3

#define PAR_OSC1_PULSE  4
#define PAR_OSC1_WIDTH  5
#define PAR_OSC1_TRI    6
#define PAR_OSC1_SIN    7
#define PAR_OSC1_OCT    8
#define PAR_OSC1_SEMI   9
#define PAR_OSC1_CENT   10

#define PAR_RES1_NOISE  11
#define PAR_RES1_NSHAPE 12
#define PAR_RES1_FB     13
#define PAR_RES1_DAMP   14
#define PAR_RES1_OCT    15
#define PAR_RES1_SEMI   16
#define PAR_RES1_CENT   17
#define PAR_RES1_ROUGH  18

#define PAR_FLT1_TYPE   19
#define PAR_FLT1_FREQ   20
#define PAR_FLT1_RES    21

#define PAR_ENV1_ATT    22
#define PAR_ENV1_DEC    23
#define PAR_ENV1_SUS    24
#define PAR_ENV1_REL    25

//----------

#define NUM_PARAMS      26
#define NUM_VOICES      128

//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_voice_manager.h"

#include "test_synth1/test_synth1_voice.h"
#include "test_synth1/test_synth1_editor.h"

typedef MIP_VoiceManager<myVoice,NUM_VOICES> myVoiceManager;

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
  #ifdef MIP_DEBUG
    "test_synth1 (debug)",
  #else
    "test_synth1",
  #endif
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.4",
  "simple mip2 test synth",
  myFeatures
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

    //---------- global ----------

    { PAR_VOL,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Vol",
      "",
      0.0,
      1.0,
      0.5
    },

    { PAR_PAN,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Pan",
      "",
      0.0,
      1.0,
      0.5
    },

    { PAR_OSC1_OUT,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "O1",
      "",
      0.0,
      1.0,
      0.5
    },

    { PAR_RES1_OUT,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "R1",
      "",
      0.0,
      1.0,
      0.0
    },

    //---------- osc1 ----------

    { PAR_OSC1_PULSE,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Pulse",
      "",
      0.0,
      1.0,
      0.0
    },

    { PAR_OSC1_WIDTH,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Width",
      "",
      0.0,
      1.0,
      0.5
    },

    { PAR_OSC1_TRI,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Tri",
      "",
      0.0,
      1.0,
      0.0
    },

    { PAR_OSC1_SIN,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Sin",
      "",
      0.0,
      1.0,
      0.0
    },

//    { PAR_OSC1_NOISE,
//      CLAP_PARAM_IS_AUTOMATABLE,
//      nullptr,
//      "Noise",
//      "",
//      0.0,
//      1.0,
//      0.0
//    },

    { PAR_OSC1_OCT,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Oct",
      "",
      0.0,
      4.0,
      0.5
    },

    { PAR_OSC1_SEMI,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Semi",
      "",
      0.0,
      1.0,
      0.5
    },

    { PAR_OSC1_CENT,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Cent",
      "",
      0.0,
      1.0,
      0.5
    },

    //---------- res1 ----------

    { PAR_RES1_NOISE,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Noise",
      "",
      0.0,
      1.0,
      0.0
    },

    { PAR_RES1_NSHAPE,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "NShape",
      "",
      0.0,
      1.0,
      0.0
    },

    { PAR_RES1_FB,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "FB",
      "",
      0.0,
      1.0,
      0.9
    },

    { PAR_RES1_DAMP,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Damp",
      "",
      0.0,
      1.0,
      0.5
    },

    { PAR_RES1_OCT,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Oct",
      "",
      0.0,
      1.0,
      0.5
    },

    { PAR_RES1_SEMI,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Semi",
      "",
      0.0,
      1.0,
      0.5

    },

    { PAR_RES1_CENT,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Cent",
      "",
      0.0,
      1.0,
      0.5
    },

    { PAR_RES1_ROUGH,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Rough",
      "",
      0.0,
      1.0,
      0.0
    },

    //---------- flt1 ----------

    { PAR_FLT1_TYPE,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Type",
      "",
      0.0,
      1.0,
      0.0
    },

    { PAR_FLT1_FREQ,
      CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Freq",
      "",
      0.0,
      1.0,
      1.0
    },

    { PAR_FLT1_RES,
      CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Res",
      "",
      0.0,
      1.0,
      0.0
    },

    //---------- env1 ----------

    { PAR_ENV1_ATT,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Att",
      "",
      0.0,
      1.0,
      0.01
    },

    { PAR_ENV1_DEC,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Dec",
      "",
      0.0,
      1.0,
      0.0
    },

    { PAR_ENV1_SUS,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Sus",
      "",
      0.0,
      1.0,
      1.0
    },

    { PAR_ENV1_REL,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Rel",
      "",
      0.0,
      1.0,
      0.5
    }

  };

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

  //TODO: make more automatic..
  // uint32_t MSupportedExtensions in MIP_Plugin ?
  // set bits in constructor, cgeck and return ptr in MIP_Plugin::getextension..

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

    // hack!!!!!
    // update gui (state only) in process!

    if (MEditor && MEditorIsOpen) {
      uint32_t num_playing = 0;
      uint32_t num_released = 0;
      for (uint32_t i=0; i<NUM_VOICES; i++) {
        uint32_t state = MVoiceManager.getVoiceState(i);
        ((myEditor*)MEditor)->MVoiceWidget->voice_state[i] = state;
        if (state == MIP_VOICE_PLAYING) num_playing += 1;
        if (state == MIP_VOICE_RELEASED) num_released += 1;
      }
      ((myEditor*)MEditor)->MPlayingVoicesWidget->setValue(num_playing);
      ((myEditor*)MEditor)->MReleasedVoicesWidget->setValue(num_released);
      ((myEditor*)MEditor)->MTotalVoicesWidget->setValue(num_playing + num_released);
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
    MEditor = new myEditor(this,this,EDITOR_WIDTH,EDITOR_HEIGHT,true,&myDescriptor);
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

  //----------

  void handle_process(const clap_process_t *process) final {
    float** outputs = process->audio_outputs[0].data32;
    uint32_t length = process->frames_count;

    #ifdef MIP_VOICE_PREPARE_EVENTS
      MVoiceManager.processPrepared(process,MHost);
    #else
      MVoiceManager.processBlock(process);
    #endif

    float v = MParameterValues[PAR_VOL];  // vol
    float p = MParameterValues[PAR_PAN];  // pan
    float l = v * (1.0 - p);
    float r = v * (      p);
    MIP_ScaleStereoBuffer(outputs,l,r,length);
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

