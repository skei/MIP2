#ifndef mip_plugin_entry_included
#define mip_plugin_entry_included
//----------------------------------------------------------------------

#include "mip.h"

//----------------------------------------------------------------------

#define MIP_CLAP_PLUGIN_ENTRY(DESC,PLUG)
#define MIP_EXE_PLUGIN_ENTRY(DESC,PLUG)
#define MIP_LV2_PLUGIN_ENTRY(DESC,PLUG)
#define MIP_VST2_PLUGIN_ENTRY(DESC,PLUG)
#define MIP_VST2_PLUGIN_ENTRY(DESC,PLUG)

//----------

#ifdef MIP_PLUGIN_CLAP
  #undef MIP_CLAP_PLUGIN_ENTRY
  #include "plugin/clap/mip_clap_plugin_entry.h"
#endif

#ifdef MIP_PLUGIN_EXE
  #undef MIP_EXE_PLUGIN_ENTRY
  #include "plugin/exe/mip_exe_plugin_entry.h"
#endif

#ifdef MIP_PLUGIN_LV2
  #undef MIP_LV2_PLUGIN_ENTRY
  #include "plugin/lv2/mip_lv2_plugin_entry.h"
#endif

#ifdef MIP_PLUGIN_VST2
  #undef MIP_VST2_PLUGIN_ENTRY
  #include "plugin/vst2/mip_vst2_plugin_entry.h"
#endif

#ifdef MIP_PLUGIN_VST3
  #undef MIP_VST3_PLUGIN_ENTRY
  #include "plugin/vst3/mip_vst3_plugin_entry.h"
#endif

//----------------------------------------------------------------------


//----------

#define MIP_PLUGIN_ENTRY(DESC,PLUG) \
  MIP_CLAP_PLUGIN_ENTRY(DESC,PLUG); \
  MIP_EXE_PLUGIN_ENTRY(DESC,PLUG);  \
  MIP_LV2_PLUGIN_ENTRY(DESC,PLUG);  \
  MIP_VST2_PLUGIN_ENTRY(DESC,PLUG); \
  MIP_VST2_PLUGIN_ENTRY(DESC,PLUG); \

//----------------------------------------------------------------------
#endif
