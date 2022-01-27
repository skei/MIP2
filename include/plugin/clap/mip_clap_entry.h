#ifndef mip_clap_entry_included
#define mip_clap_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
//#include "plugin/clap/mip_clap_factory.h"

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

bool clap_entry_init_callback(const char *plugin_path) {
  return true;
}

//----------

void clap_entry_deinit_callback() {
}

//----------

const void* clap_entry_get_factory_callback(const char *factory_id) {
  if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) {
    return &MIP_GLOBAL_CLAP_FACTORY;
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
  CLAP_VERSION,
  clap_entry_init_callback,
  clap_entry_deinit_callback,
  clap_entry_get_factory_callback,
};

#pragma GCC diagnostic pop

//----------------------------------------------------------------------
#endif
