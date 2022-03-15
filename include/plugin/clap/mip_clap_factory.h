#ifndef mip_clap_factory_included
#define mip_clap_factory_included
//----------------------------------------------------------------------

#include "mip.h"
#include "extern/clap/clap.h"
#include "plugin/clap/mip_clap_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

/*
  Get the number of plugins available.
  [thread-safe]
*/

uint32_t clap_factory_get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  return MIP_CLAP_REGISTRY.getNumPlugins();
}

//----------

/*
  Retrieves a plugin descriptor by its index.
  Returns null in case of error.
  The descriptor must not be freed.
  [thread-safe]
*/

const clap_plugin_descriptor_t* clap_factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  return MIP_CLAP_REGISTRY.getPlugin(index);
}

//----------

/*
  Create a clap_plugin by its plugin_id.
  The returned pointer must be freed by calling plugin->destroy(plugin);
  The plugin is not allowed to use the host callbacks in the create method.
  Returns null in case of error.
  [thread-safe]
*/

const clap_plugin_t* clap_factory_create_plugin_callback(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
  if (MIP_CreatePlugin) {
    int32_t index = MIP_CLAP_REGISTRY.findPluginById(plugin_id);
    const clap_plugin_descriptor_t* descriptor = MIP_CLAP_REGISTRY.getPlugin(index);
    MIP_ClapPlugin* plugin = MIP_CreatePlugin(index,descriptor,host);
    return plugin->ptr();
  }
  else {
    //MIP_Print("no MIP_CreatePlugin()\n");
    return nullptr;
  }
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_factory MIP_CLAP_FACTORY = {
  clap_factory_get_plugin_count_callback,
  clap_factory_get_plugin_descriptor_callback,
  clap_factory_create_plugin_callback
};

//----------------------------------------------------------------------
#endif
