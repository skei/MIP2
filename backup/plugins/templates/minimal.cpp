#define MIP_NO_GUI
#define MIP_PLUGIN_ALL

#include "mip.h"
#include "plugin/mip_plugin_entry.h"

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

void MIP_RegisterPlugins(MIP_PluginList* AList) {
  MIP_Descriptor* descriptor = new MIP_Descriptor();
  AList->appendPlugin(descriptor);
}

//----------

MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, MIP_Descriptor* ADescriptor, MIP_HostProxy* AHost) {
  if (AIndex == 0) return new MIP_Plugin(ADescriptor,AHost);
  return nullptr;
}
