#ifndef mip_clap_factory_included
#define mip_clap_factory_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/mip_registry.h"

//----------------------------------------------------------------------

uint32_t clap_plugin_factory_get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  return MIP_REGISTRY.getNumDescriptors();
}

//----------

const clap_plugin_descriptor_t* clap_plugin_factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  return MIP_REGISTRY.getDescriptor(index);
}

//----------

const clap_plugin_t* clap_plugin_factory_create_plugin_callback(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
  int32_t index = MIP_REGISTRY.findDescriptorById(plugin_id);
  if (index >= 0) {
    const clap_plugin_descriptor_t* descriptor = MIP_REGISTRY.getDescriptor(index);
    MIP_Plugin* plugin = MIP_CreatePlugin(index,descriptor,host);
    return plugin->getPlugin();
  }
  return nullptr;
}

//----------------------------------------------------------------------

const clap_plugin_factory_t MIP_CLAP_FACTORY {
  .get_plugin_count      = clap_plugin_factory_get_plugin_count_callback,
  .get_plugin_descriptor = clap_plugin_factory_get_plugin_descriptor_callback,
  .create_plugin         = clap_plugin_factory_create_plugin_callback
};

//----------------------------------------------------------------------
#endif
