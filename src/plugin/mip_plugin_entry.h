#ifndef mip_plugin_entry_included
#define mip_plugin_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_plugin_list.h"

//----------------------------------------------------------------------

#define MIP_CLAP_PLUGIN_ENTRY(DESC,PLUG)
#define MIP_EXE_PLUGIN_ENTRY(DESC,PLUG)
#define MIP_LV2_PLUGIN_ENTRY(DESC,PLUG)
#define MIP_VST2_PLUGIN_ENTRY(DESC,PLUG)
#define MIP_VST3_PLUGIN_ENTRY(DESC,PLUG)

#undef MIP_PLUGIN_DEFINED

//----------

#ifdef MIP_NO_PLUGIN
  #define MIP_PLUGIN_DEFINED
#endif

#ifdef MIP_PLUGIN_CLAP
  #define MIP_PLUGIN_DEFINED
  #undef MIP_CLAP_PLUGIN_ENTRY
  #include "plugin/clap/mip_clap_plugin_entry.h"
#endif

#ifdef MIP_PLUGIN_EXE
  #define MIP_PLUGIN_DEFINED
  #undef MIP_EXE_PLUGIN_ENTRY
  #include "plugin/exe/mip_exe_plugin_entry.h"
#endif

#ifdef MIP_PLUGIN_LV2
  #define MIP_PLUGIN_DEFINED
  #undef MIP_LV2_PLUGIN_ENTRY
  #include "plugin/lv2/mip_lv2_plugin_entry.h"
#endif

#ifdef MIP_PLUGIN_VST2
  #define MIP_PLUGIN_DEFINED
  #undef MIP_VST2_PLUGIN_ENTRY
  #include "plugin/vst2/mip_vst2_plugin_entry.h"
#endif

#ifdef MIP_PLUGIN_VST3
  #define MIP_PLUGIN_DEFINED
  #undef MIP_VST3_PLUGIN_ENTRY
  #include "plugin/vst3/mip_vst3_plugin_entry.h"
#endif

//----------

#ifndef MIP_PLUGIN_DEFINED
  //#warning No plugin format defined
  #error No plugin format defined
#endif

//----------------------------------------------------------------------
#endif
