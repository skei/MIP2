#ifndef mip_clap_plugin_factory_included
#define mip_clap_plugin_factory_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_plugin.h"
#include "plugin/clap/mip_clap_plugin_list.h"

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t index, const clap_plugin_descriptor* desc, const clap_host_t *host);

//----------------------------------------------------------------------

uint32_t clap_plugin_factory_get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  //MIP_PRINT;
  return GLOBAL_CLAP_PLUGIN_LIST.getNumPlugins();
}

//----------

const clap_plugin_descriptor_t* clap_plugin_factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  //MIP_PRINT;
  return GLOBAL_CLAP_PLUGIN_LIST.getPluginDescriptor(index);
}

//----------

const clap_plugin_t* clap_plugin_factory_create_plugin_callback(const struct clap_plugin_factory *factory,const clap_host_t *host,const char *plugin_id) {
  //MIP_PRINT;
  MIP_ClapPluginInfo* info = GLOBAL_CLAP_PLUGIN_LIST.findPluginById(plugin_id);
  const clap_plugin_descriptor* desc = info->desc->getClapDescriptor();
  if (desc && (strcmp(plugin_id,desc->id) == 0)) {
    MIP_ClapPlugin* plugin = MIP_CreatePlugin(info->index,desc,host);
    if (plugin) {
      return plugin->getClapPlugin();
    }
  }
  return nullptr;
}

//----------------------------------------------------------------------

const clap_plugin_factory GLOBAL_CLAP_PLUGIN_FACTORY = {
  clap_plugin_factory_get_plugin_count_callback,
  clap_plugin_factory_get_plugin_descriptor_callback,
  clap_plugin_factory_create_plugin_callback
};

//----------------------------------------------------------------------
#endif

