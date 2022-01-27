#ifndef mip_clap_factory_included
#define mip_clap_factory_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"

//----------

extern uint32_t MIP_GetNumPlugins();
extern const clap_plugin_descriptor_t* MIP_GetDescriptor(uint32_t index);
extern const clap_plugin_t* MIP_CreatePlugin(const clap_host_t *host, const char *plugin_id);

//----------------------------------------------------------------------
//
// factory
//
//----------------------------------------------------------------------

uint32_t clap_factory_get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  return MIP_GetNumPlugins();
}

//----------

const clap_plugin_descriptor_t* clap_factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  return MIP_GetDescriptor(index);
}

//----------

const clap_plugin_t* clap_factory_create_plugin_callback(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
  return MIP_CreatePlugin(host,plugin_id);
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//static
//constexpr
const clap_plugin_factory MIP_GLOBAL_CLAP_FACTORY = {
  clap_factory_get_plugin_count_callback,
  clap_factory_get_plugin_descriptor_callback,
  clap_factory_create_plugin_callback
};

//----------------------------------------------------------------------
#endif
