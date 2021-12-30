#ifndef mip_clap_plugin_entry_incuded
#define mip_clap_plugin_entry_incuded
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_plugin.h"
#include "plugin/mip_plugin_list.h"

//----------------------------------------------------------------------
//
// plugin factory
//
//----------------------------------------------------------------------

uint32_t clap_plugin_factory_get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  MIP_PRINT;
  return MIP_GLOBAL_PLUGIN_LIST.getNumPlugins();
}

//----------

const clap_plugin_descriptor_t* clap_plugin_factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  MIP_PRINT;
  MIP_PluginInfo* info = MIP_GLOBAL_PLUGIN_LIST.getPluginInfo(index);
  if (info) {
    return  (const clap_plugin_descriptor*)info->ptr;
  }
  return nullptr;
}

//----------

const clap_plugin_t* clap_plugin_factory_create_plugin_callback(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
  MIP_PRINT;
  MIP_PluginInfo* info = MIP_GLOBAL_PLUGIN_LIST.findPluginById(plugin_id);
  const clap_plugin_descriptor* clapdesc = (const clap_plugin_descriptor*)info->ptr;
  if (clapdesc && (strcmp(plugin_id,clapdesc->id) == 0)) {
    // deleted in MIP_ClapPlugin  (?)
    MIP_Plugin* plugin = MIP_CreatePlugin(info->index,info->descriptor);
    MIP_ClapPlugin* clapplugin = new MIP_ClapPlugin(plugin,clapdesc,host);
    if (plugin) return clapplugin->getClapPlugin();
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
//
// plugin entry
//
//----------------------------------------------------------------------

bool clap_plugin_entry_init_callback(const char *plugin_path) {
  MIP_PRINT;
  MIP_RegisterPlugins(&MIP_GLOBAL_PLUGIN_LIST);
  uint32_t num = MIP_GLOBAL_PLUGIN_LIST.getNumPlugins();
  for (uint32_t i=0; i<num; i++) {
    clap_plugin_descriptor_t* clapdesc = (clap_plugin_descriptor_t*)malloc(sizeof(clap_plugin_descriptor_t*));
    if (clapdesc) {
      clapdesc->clap_version  = CLAP_VERSION;
      clapdesc->id            = "";
      clapdesc->name          = "";
      clapdesc->vendor        = "";
      clapdesc->url           = "";
      clapdesc->manual_url    = "";
      clapdesc->support_url   = "";
      clapdesc->version       = "";
      clapdesc->description   = "";
      clapdesc->keywords      = "";
      clapdesc->plugin_type   = CLAP_PLUGIN_AUDIO_EFFECT;
      MIP_PluginInfo* info = MIP_GLOBAL_PLUGIN_LIST.getPluginInfo(i);
      if (info) {
        info->ptr = clapdesc;
      }
    }
    //else return false;
  }
  return true;
}

//----------

void clap_plugin_entry_deinit_callback() {
  MIP_PRINT;
  uint32_t num = MIP_GLOBAL_PLUGIN_LIST.getNumPlugins();
  for (uint32_t i=0; i<num; i++) {
    MIP_PluginInfo* info = MIP_GLOBAL_PLUGIN_LIST.getPluginInfo(i);
    if (info) {
      if (info->ptr) {
        free(info->ptr);
        info->ptr = nullptr;
      }
    }
  }
  //MIP_UnregisterPlugins();
}

//----------

const void* clap_plugin_entry_get_factory_callback(const char *factory_id) {
  MIP_PRINT;
  if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) {
    return &GLOBAL_CLAP_PLUGIN_FACTORY;
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
  clap_plugin_entry_init_callback,
  clap_plugin_entry_deinit_callback,
  clap_plugin_entry_get_factory_callback
};

#pragma GCC diagnostic pop

//----------------------------------------------------------------------
#endif
