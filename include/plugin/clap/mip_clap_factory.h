#ifndef mip_clap_factory_included
#define mip_clap_factory_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

uint32_t clap_factory_get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  CLAP_PRINT;
  return MIP_REGISTRY.getNumDescriptors();
}

//----------

const clap_plugin_descriptor_t* clap_factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  CLAP_Print("index %i\n",index);
  return MIP_REGISTRY.getDescriptor(index);
}

//----------

const clap_plugin_t* clap_factory_create_plugin_callback(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
  CLAP_Print("plugin_id %s\n",plugin_id);
  if (MIP_CreatePlugin) {
    int32_t index = MIP_REGISTRY.findDescriptorById(plugin_id);
    const clap_plugin_descriptor_t* descriptor = MIP_REGISTRY.getDescriptor(index);
    MIP_ClapPlugin* plugin = MIP_CreatePlugin(index,descriptor,host);
    return plugin->getPlugin();
  }
  else {
    return nullptr;
  }
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_factory MIP_CLAP_FACTORY = {
  .get_plugin_count       = clap_factory_get_plugin_count_callback,
  .get_plugin_descriptor  = clap_factory_get_plugin_descriptor_callback,
  .create_plugin          = clap_factory_create_plugin_callback
};

//----------------------------------------------------------------------
#endif
