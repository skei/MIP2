#ifndef mip_clap_factory_included
#define mip_clap_factory_included
//----------------------------------------------------------------------

#include "mip.h"
#include "extern/clap/clap.h"
#include "plugin/clap/mip_clap_list.h"
#include "plugin/clap/mip_clap_plugin.h"

extern MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost);

//----------

#ifndef MIP_NO_DEFAULT_PLUGIN_FACTORY

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
  return MIP_GLOBAL_CLAP_LIST.getNumPlugins();
}

//----------

/*
  Retrieves a plugin descriptor by its index.
  Returns null in case of error.
  The descriptor must not be freed.
  [thread-safe]
*/

const clap_plugin_descriptor_t* clap_factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  return MIP_GLOBAL_CLAP_LIST.getPlugin(index);
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
  int32_t index = MIP_GLOBAL_CLAP_LIST.findPluginById(plugin_id);
  //MIP_ClapHostProxy* hostproxy = new MIP_ClapHostProxy(host);
  const clap_plugin_descriptor_t* descriptor = MIP_GLOBAL_CLAP_LIST.getPlugin(index);
  MIP_ClapPlugin* plugin = MIP_CreatePlugin(index,descriptor,host); // deleted in MIP_ClapPlugin.clap_plugin_destroy_callback()

//  MIP_GLOBAL_CLAP_LIST.appendInstance(plugin);

  return plugin->ptr();
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

#endif // MIP_NO_DEFAULT_PLUGIN_FACTORY

//----------------------------------------------------------------------
#endif
