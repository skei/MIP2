#ifndef mip_clap_entry_included
#define mip_clap_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_registry.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_factory.h"

//----------------------------------------------------------------------

bool clap_entry_init_callback(const char *plugin_path) {
  //MIP_Print("path: %s\n",plugin_path);
  MIP_REGISTRY.setPath(plugin_path);
  return true;
}

//----------

void clap_entry_deinit_callback(void) {
}

//----------

/*
  bitwig asks for
    clap.plugin-factory
    clap.plugin-invalidation-factory/draft0
  reaper asks for
    clap.plugin-factory
*/

const void* clap_entry_get_factory_callback(const char* factory_id) {
  //MIP_DPrint("%s\n",factory_id);
  const void* factory = MIP_REGISTRY.findFactoryById(factory_id);
  if (!factory) {
    if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) return &MIP_CLAP_FACTORY;
    return nullptr;
  }
  return factory;
}

//----------------------------------------------------------------------

//__MIP_EXPORT
CLAP_EXPORT extern const clap_plugin_entry_t clap_entry {
  .clap_version = CLAP_VERSION,
  .init         = clap_entry_init_callback,
  .deinit       = clap_entry_deinit_callback,
  .get_factory  = clap_entry_get_factory_callback
};

//----------------------------------------------------------------------
#endif
