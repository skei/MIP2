#ifndef mip_clap_factory_included
#define mip_clap_factory_included
//----------------------------------------------------------------------

#include "mip.h"
#include "extern/clap/clap.h"
#include "plugin/clap/mip_clap_list.h"
#include "plugin/clap/mip_clap_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

uint32_t clap_factory_get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  return MIP_GLOBAL_CLAP_LIST.getNumPlugins();
}

//----------

const clap_plugin_descriptor_t* clap_factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  return MIP_GLOBAL_CLAP_LIST.getPlugin(index);
}

//----------

const clap_plugin_t* clap_factory_create_plugin_callback(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
  int32_t index = MIP_GLOBAL_CLAP_LIST.findPlugin(plugin_id);
  //MIP_ClapHostProxy* hostproxy = new MIP_ClapHostProxy(host);
  const clap_plugin_descriptor_t* descriptor = MIP_GLOBAL_CLAP_LIST.getPlugin(index);
  MIP_ClapPlugin* plugin = MIP_CreatePlugin(index,descriptor,host); // deleted in MIP_ClapPlugin.clap_plugin_destroy_callback()
  return plugin->getPtr();
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
