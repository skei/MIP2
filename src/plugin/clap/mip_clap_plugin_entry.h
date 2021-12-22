#ifndef mip_clap_plugin_entry_included
#define mip_clap_plugin_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_plugin_factory.h"

void MIP_RegisterPlugins();

//----------------------------------------------------------------------

bool clap_plugin_entry_init_callback(const char *plugin_path) {
  MIP_RegisterPlugins();
  return true;
}

//----------

void clap_plugin_entry_deinit_callback() {
}

//----------

const void* clap_plugin_entry_get_factory_callback(const char *factory_id) {
  if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) {
    return &GLOBAL_CLAP_PLUGIN_FACTORY;
  }
  return nullptr;
}

//----------------------------------------------------------------------

CLAP_EXPORT const clap_plugin_entry clap_entry = {
  CLAP_VERSION,
  clap_plugin_entry_init_callback,
  clap_plugin_entry_deinit_callback,
  clap_plugin_entry_get_factory_callback
};

//----------------------------------------------------------------------
#endif
