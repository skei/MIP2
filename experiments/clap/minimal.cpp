

// nc -U -l -k /tmp/mip.socket
#define MIP_DEBUG
#define MIP_DEBUG_PRINT
#define MIP_DEBUG_PRINT_SOCKET

#include "mip.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

uint32_t clap_plugin_factory_get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  MIP_PRINT;
  return 1;
}

const clap_plugin_descriptor_t* clap_plugin_factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  MIP_PRINT;
  return nullptr;
}

const clap_plugin_t* clap_plugin_factory_create_plugin_callback(const struct clap_plugin_factory *factory,const clap_host_t *host,const char *plugin_id) {
  MIP_PRINT;
  return nullptr;
}

//----------------------------------------------------------------------

const clap_plugin_factory MY_FACTORY = {
  clap_plugin_factory_get_plugin_count_callback,
  clap_plugin_factory_get_plugin_descriptor_callback,
  clap_plugin_factory_create_plugin_callback
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

bool clap_plugin_entry_init_callback(const char *plugin_path) {
  MIP_PRINT; // this is not printed
  return true;
}

void clap_plugin_entry_deinit_callback() {
  MIP_PRINT;
}

const void* clap_plugin_entry_get_factory_callback(const char *factory_id) {
  MIP_Print("factory_id: '%s'\n",factory_id);
  if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) {
    return &MY_FACTORY;
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

