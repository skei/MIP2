#ifndef mip_vst2_plugin_entry_included
#define mip_vst2_plugin_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/vst2/mip_vst2.h"
#include "plugin/vst2/mip_vst2_plugin.h"
#include "plugin/vst2/mip_vst2_host_proxy.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Vst2PluginEntry {

//------------------------------
public:
//------------------------------

  MIP_Vst2PluginEntry() {
    MIP_PRINT;
  }

  //----------

  virtual ~MIP_Vst2PluginEntry() {
    MIP_PRINT;
  }

//------------------------------
public:
//------------------------------

  AEffect* entry(audioMasterCallback audioMaster) {
    MIP_PRINT;
    MIP_Descriptor* descriptor = MIP_GLOBAL_PLUGIN_LIST.getPluginDescriptor(0);

    MIP_Vst2HostProxy* hostproxy = new MIP_Vst2HostProxy(/*audioMaster*/);
    MIP_Plugin* plugin = MIP_GLOBAL_PLUGIN_LIST.createPlugin(0,hostproxy); // deleted in MIP_Vst2Plugin destructor

    MIP_Vst2Plugin* vst2plugin = new MIP_Vst2Plugin(plugin,audioMaster);
    AEffect* effect = vst2plugin->getAEffect();
    memset(effect,0,sizeof(AEffect));

    int32_t flags = effFlagsCanReplacing;
    if (descriptor->isSynth())    flags |= effFlagsIsSynth;
    if (descriptor->hasEditor())  flags |= effFlagsHasEditor;
    //if (MDescriptor->options.use_chunks)) flags |= effFlagsProgramChunks;
    //if (MPlugin->hasFlag(kpf_silentStop)) flags |= effFlagsNoSoundInStop;

    //#ifndef MIP_PLUGIN_VST2_VESTIGE
    //if (MDescriptor->options.can_process_double) flags |= effFlagsCanDoubleReplacing;
    //#endif

    effect->magic                     = kEffectMagic;
    effect->uniqueID                  = descriptor->getShortId();
    effect->flags                     = flags;
    effect->numInputs                 = descriptor->getNumAudioInputs();
    effect->numOutputs                = descriptor->getNumAudioOutputs();
    effect->numParams                 = descriptor->getNumParameters();
    effect->numPrograms               = 0;//descriptor->getNumPresets();
    effect->version                   = descriptor->getVersion();
    effect->initialDelay              = 0;
    effect->object                    = vst2plugin;
    effect->user                      = nullptr;//instance;
    effect->dispatcher                = vst2_dispatcher_callback;
    effect->setParameter              = vst2_setParameter_callback;
    effect->getParameter              = vst2_getParameter_callback;
    effect->processReplacing          = vst2_process_callback;
    effect->processDoubleReplacing    = vst2_processDouble_callback;

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

MIP_Vst2PluginEntry GLOBAL_VST2_PLUGIN_ENTRY;

//----------

__MIP_EXPORT
AEffect* mip_vst2_entry(audioMasterCallback audioMaster) {
  MIP_PRINT;
  if (!audioMaster(0,audioMasterVersion,0,0,0,0)) return 0;
  return GLOBAL_VST2_PLUGIN_ENTRY.entry(audioMaster);
}

//----------------------------------------------------------------------
#endif
