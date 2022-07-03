#ifndef mip_clap_entry_included
#define mip_clap_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "extern/clap/clap.h"
#include "plugin/mip_registry.h"
#include "plugin/clap/mip_clap_factory.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

bool clap_entry_init_callback(const char *plugin_path) {
  CLAP_Print("plugin_path %s\n",plugin_path);
  return true;
}

//----------

void clap_entry_deinit_callback() {
  CLAP_PRINT;
}

//----------

const void* clap_entry_get_factory_callback(const char *factory_id) {
  CLAP_Print("factory_id %s\n",factory_id);
  int32_t index = MIP_REGISTRY.findFactoryById(factory_id);
  if (index >= 0) {
    //CLAP_DPrint("  found at index %i\n",index);
    return MIP_REGISTRY.getFactory(index);
  }
  if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) {
    //CLAP_DPrint("  not found. returns default\n");
    return &MIP_CLAP_FACTORY;
  }
  return nullptr;
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// get rid of "warning: ‘visibility’ attribute ignored [-Wattributes]"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"

CLAP_EXPORT const clap_plugin_entry clap_entry = {
  .clap_version = CLAP_VERSION,
  .init         = clap_entry_init_callback,
  .deinit       = clap_entry_deinit_callback,
  .get_factory  = clap_entry_get_factory_callback
};

#pragma GCC diagnostic pop

//----------------------------------------------------------------------
#endif
