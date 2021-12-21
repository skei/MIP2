#ifndef clap_plugin_list_included
#define clap_plugin_list_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"
#include "plugin/clap/mip_clap.h"


struct MIP_ClapPluginInfo {
  uint32_t                      index = 0;
  const clap_plugin_descriptor* desc  = nullptr;
};

//-----------

typedef MIP_Array<MIP_ClapPluginInfo*> MIP_ClapPluginList;

//----------------------------------------------------------------------
#endif
