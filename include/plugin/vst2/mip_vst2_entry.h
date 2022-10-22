#ifndef mip_vst2_entry_included
#define mip_vst2_entry_included
//----------------------------------------------------------------------

// work in progress..

#include "mip.h"
#include "base/system/mip_paths.h"
#include "plugin/mip_registry.h"
#include "plugin/clap/mip_clap.h"
//#include "plugin/clap/mip_clap_host.h"
#include "plugin/vst2/mip_vst2.h"
#include "plugin/vst2/mip_vst2_host_implementation.h"
#include "plugin/vst2/mip_vst2_plugin.h"
#include "plugin/vst2/mip_vst2_utils.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Vst2Entry {

//------------------------------
private:
//------------------------------

  //MIP_ClapHost* MHost = nullptr;

//------------------------------
public:
//------------------------------

  MIP_Vst2Entry() {
    //MIP_Print("\n");
  }

  //----------

  ~MIP_Vst2Entry() {
    //MIP_Print("\n");
  }

//------------------------------
public:
//------------------------------

// host cando "shellCategory"
// ask the host about shellCategorycurid;


  AEffect* entry(audioMasterCallback audioMaster) {
    MIP_Print("\n");

    // shell

    uint32_t index = 0;

    #ifndef MIP_VST2_NO_SHELL
      uint32_t current_id = 0;
      if ( audioMaster(nullptr,audioMasterCanDo,0,0,(void*)"shellCategory",0) == 1) {
        //MIP_Print("host supports shellCategory\n");
        current_id = audioMaster(nullptr,audioMasterCurrentId,0,0,nullptr,0);
        MIP_Print("shellCategory.. current_id: %i\n",current_id);
        for (int32_t i=0; i<MIP_REGISTRY.getNumDescriptors(); i++) {
          const clap_plugin_descriptor_t* desc = MIP_REGISTRY.getDescriptor(i);
          if (current_id == mip_vst2_create_unique_id(desc)) {
            MIP_Print("found plugin %i = '%s'\n",i,desc->name);
            index = i;
            break;
          }
        }
      }
      //    if ( audioMaster(effect,audioMasterCanDo,0,0,(void*)"shellCategorycurID",0) == 1) {
      //      MIP_Print("host supports shellCategorycurID\n");
      //    }
    #endif

    char path[1024] = {};
    const char* plugin_path = MIP_GetLibFilename(path);

    MIP_Print("plugin_path '%s'\n",plugin_path);
    MIP_REGISTRY.setPath(plugin_path);

    MIP_Vst2Host* host = new MIP_Vst2Host(audioMaster); // deleted in MIP_Vst2Plugin destructor
    //const clap_plugin_descriptor_t* descriptor = MIP_GetDescriptor(0);

    const clap_plugin_descriptor_t* descriptor = MIP_REGISTRY.getDescriptor(index);

    //const clap_plugin_t* plugin = MIP_CreatePlugin(host->ptr(),descriptor->id); // deleted in MIP_Vst2Plugin destructor
    MIP_ClapPlugin* plugin = MIP_CreatePlugin(0,descriptor,host->getHost());
    const clap_plugin_t* clap_plugin = plugin->getPlugin();

    clap_plugin->init(clap_plugin); // destroy called in effClose
    //MIP_GLOBAL_CLAP_LIST.appendInstance(plugin);
    MIP_Vst2Plugin* vst2plugin  = new MIP_Vst2Plugin(host,clap_plugin/*plugin->ptr()*/,audioMaster); // deleted in vst2_dispatcher_callback(effClose)

    #ifndef MIP_VST2_NO_SHELL
      vst2plugin->MShellPluginCurrentId = current_id;
    #endif

    /*
      assumes stereo in & out
      TODO: check clap.audio-ports
      use number of channels in port with 'is_main'
    */

    uint32_t  num_inputs  = 2;
    uint32_t  num_outputs = 2;
    uint32_t  num_params  = 0;
    int32_t   flags       = effFlagsCanReplacing;

    /*
    if (strstr(descriptor->features,"instrument")) {
      flags |= effFlagsIsSynth;
      num_inputs = 0;
    }
    */

    const clap_plugin_gui_t* gui = (const clap_plugin_gui_t*)plugin->get_extension(CLAP_EXT_GUI);
    if (gui) {
      flags |= effFlagsHasEditor;
    }

    // is_synth..
    // flags |= effFlagsProgramChunks;
    // flags |= effFlagsNoSoundInStop;
    // flags |= effFlagsCanDoubleReplacing;

    const clap_plugin_params_t* params = (const clap_plugin_params_t*)plugin->get_extension(CLAP_EXT_PARAMS);
    if (params) {
      num_params = params->count(clap_plugin/*plugin->ptr()*/);
    }

    // move all of the below to vst2plugin ?

    AEffect* effect = vst2plugin->getAEffect();
    host->setAEffect(effect);
    memset(effect,0,sizeof(AEffect));

    //

    effect->magic                   = kEffectMagic;
    effect->uniqueID                = mip_vst2_create_unique_id(descriptor);
    effect->flags                   = flags;
    effect->numInputs               = num_inputs;
    effect->numOutputs              = num_outputs;
    effect->numParams               = num_params;
    effect->numPrograms             = 0;
    effect->version                 = 0x00000000; // TODO
    effect->initialDelay            = 0;
    effect->object                  = vst2plugin;
    effect->user                    = nullptr;//this;
    effect->dispatcher              = vst2_dispatcher_callback;
    effect->setParameter            = vst2_setParameter_callback;
    effect->getParameter            = vst2_getParameter_callback;
    effect->processReplacing        = vst2_process_callback;
    effect->processDoubleReplacing  = vst2_processDouble_callback;

    return effect;
  }

//------------------------------
private: // vst2 callbacks
//------------------------------

  static
  VstIntPtr vst2_dispatcher_callback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt) {
    VstIntPtr result = 0;
    MIP_Vst2Plugin* vst2_plugin = (MIP_Vst2Plugin*)effect->object;
    result = vst2_plugin->vst2_dispatcher(opcode,index,value,ptr,opt);
    if (opcode==effClose) {
      delete (MIP_Vst2Plugin*)vst2_plugin;
    }
    return result;
  }

  //----------

  static
  void vst2_setParameter_callback(AEffect* effect, VstInt32 index, float parameter) {
    MIP_Vst2Plugin* vst2_plugin = (MIP_Vst2Plugin*)effect->object;
    vst2_plugin->vst2_setParameter(index,parameter);
  }

  //----------

  static
  float vst2_getParameter_callback(AEffect* effect, VstInt32 index) {
    MIP_Vst2Plugin* vst2_plugin = (MIP_Vst2Plugin*)effect->object;
    return vst2_plugin->vst2_getParameter(index);
  }

  //----------

  static
  void vst2_process_callback(AEffect* effect, float** inputs, float** outputs, VstInt32 sampleFrames) {
    MIP_Vst2Plugin* vst2_plugin = (MIP_Vst2Plugin*)effect->object;
    vst2_plugin->vst2_process(inputs,outputs,sampleFrames);
  }

  //----------

  static
  void vst2_processDouble_callback(AEffect* effect, double** inputs, double** outputs, VstInt32 sampleFrames) {
    MIP_Vst2Plugin* vst2_plugin = (MIP_Vst2Plugin*)effect->object;
    vst2_plugin->vst2_processDouble(inputs,outputs,sampleFrames);
  }

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

#define MIP_VST2_ENTRY_SYMBOL asm ("VSTPluginMain");
AEffect* mip_vst2_entry(audioMasterCallback audioMaster) MIP_VST2_ENTRY_SYMBOL

//----------

MIP_Vst2Entry GLOBAL_VST2_PLUGIN_ENTRY;

//----------

//__MIP_EXPORT
__attribute__ ((visibility ("default")))
AEffect* mip_vst2_entry(audioMasterCallback audioMaster) {
  //MIP_Print("\n");
  if (!audioMaster(0,audioMasterVersion,0,0,0,0)) return 0;
  return GLOBAL_VST2_PLUGIN_ENTRY.entry(audioMaster);
}

//----------------------------------------------------------------------
#endif
