#ifndef mip_lv2_entry_included
#define mip_lv2_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/lv2/mip_lv2.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Lv2PluginEntry {

//------------------------------
public:
//------------------------------

  MIP_Lv2PluginEntry() {
    MIP_PRINT;
  }

  //----------

  virtual ~MIP_Lv2PluginEntry() {
    MIP_PRINT;
  }

//------------------------------
public:
//------------------------------

  const LV2_Descriptor* entry(unsigned long Index) {
    MIP_PRINT;
    return nullptr;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const LV2_Descriptor* mip_lv2_entry(  uint32_t Index) asm ("lv2_descriptor");
//const LV2UI_Descriptor* mip_lv2ui_entrypoint(uint32_t index) asm ("lv2ui_descriptor"); // MIP_LV2_UI_SYMBOL

//----------

MIP_Lv2PluginEntry GLOBAL_LV2_PLUGIN_ENTRY;

//----------

//__MIP_EXPORT
__attribute__ ((visibility ("default")))
const LV2_Descriptor* mip_lv2_entry(unsigned long Index) {
  MIP_PRINT;
  return GLOBAL_LV2_PLUGIN_ENTRY.entry(Index);
}

//----------------------------------------------------------------------
#endif
