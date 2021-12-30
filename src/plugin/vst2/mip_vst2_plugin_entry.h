#ifndef mip_vst2_plugin_entry_included
#define mip_vst2_plugin_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/vst2/mip_vst2.h"

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
    return nullptr;
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
  AEffect* effect = GLOBAL_VST2_PLUGIN_ENTRY.entry(audioMaster);
  return effect;
}

//----------------------------------------------------------------------
#endif




