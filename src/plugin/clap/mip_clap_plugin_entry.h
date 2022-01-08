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
  uint32_t num = MIP_GLOBAL_PLUGIN_LIST.getNumPlugins();
  MIP_ClapPrint("-> %i\n",num);
  return num;
}

//----------

const clap_plugin_descriptor_t* clap_plugin_factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  MIP_PluginInfo* info = MIP_GLOBAL_PLUGIN_LIST.getPluginInfo(index);
  if (info) {
    void* ptr = info->ptr;
    MIP_ClapPrint("index %i -> %p\n",index,ptr);
    return (const clap_plugin_descriptor*)ptr;
  }
  MIP_ClapPrint("index %i -> null\n",index);
  return nullptr;
}

//----------

const clap_plugin_t* clap_plugin_factory_create_plugin_callback(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
  //MIP_Print("id: %s host: %s, %s, %s\n",plugin_id,host->name,host->vendor,host->version);
  MIP_PluginInfo* info = MIP_GLOBAL_PLUGIN_LIST.findPluginById(plugin_id);
  const clap_plugin_descriptor* clapdesc = (const clap_plugin_descriptor*)info->ptr;
  if (clapdesc && (strcmp(plugin_id,clapdesc->id) == 0)) {
    MIP_ClapHostProxy* hostproxy = new MIP_ClapHostProxy(host);                     // deleted in MIP_Plugin() destructor
    MIP_Plugin* plugin = MIP_CreatePlugin(info->index,info->descriptor,hostproxy);  // deleted in MIP_ClapPlugin destructor
    MIP_ClapPlugin* clapplugin = new MIP_ClapPlugin(plugin,clapdesc,hostproxy);     // --"--
    if (plugin) {
      const clap_plugin_t* p = clapplugin->getClapPlugin();
      MIP_ClapPrint("plugin_id '%s' host.name '%s' host.vendor '%s' host.version '%s' -> %p\n",plugin_id,host->name,host->vendor,host->version,p);
      return p;
    }
  }
  MIP_ClapPrint("plugin_id '%s' host.name '%s' host.vendor '%s' host.version '%s' -> null\n",plugin_id,host->name,host->vendor,host->version);
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
  MIP_ClapPrint("plugin_path: '%s' -> true\n",plugin_path);
  uint32_t num = MIP_GLOBAL_PLUGIN_LIST.getNumPlugins();
  for (uint32_t i=0; i<num; i++) {
    clap_plugin_descriptor_t* clapdesc = (clap_plugin_descriptor_t*)malloc(sizeof(clap_plugin_descriptor_t)); // not ptr to desc!
    if (clapdesc) {
      MIP_PluginInfo* info = MIP_GLOBAL_PLUGIN_LIST.getPluginInfo(i);
      if (info) {
        //MIP_Descriptor* desc = MIP_GLOBAL_PLUGIN_LIST.getPluginDescriptor(i);
        MIP_Descriptor* desc    = info->descriptor;//MIP_GLOBAL_PLUGIN_LIST.getPluginDescriptor(i);
        clapdesc->clap_version  = CLAP_VERSION;
        clapdesc->id            = desc->getStringId();      //"";
        clapdesc->name          = desc->getName();          //"";
        clapdesc->vendor        = desc->getAuthor();        //"";
        clapdesc->url           = desc->getUrl();           //"";
        clapdesc->manual_url    = desc->getManualUrl();     //"";
        clapdesc->support_url   = desc->getSupportUrl();    //"";
        clapdesc->version       = desc->getVersionText();   //"";
        clapdesc->description   = desc->getDescription();   //"";

        if (desc->isSynth()) clapdesc->features = "instrument";
        else clapdesc->features = "audio effect";

        //clapdesc->plugin_type   = CLAP_PLUGIN_AUDIO_EFFECT;
        //MIP_PluginInfo* info = MIP_GLOBAL_PLUGIN_LIST.getPluginInfo(i);

        info->ptr = clapdesc;
      }
    }
    //else return false;
  }
  return true;
}

//----------

void clap_plugin_entry_deinit_callback() {
  MIP_CLAPPRINT;
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
  if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) {
    MIP_ClapPrint("id: '%s' -> %p\n",factory_id,&GLOBAL_CLAP_PLUGIN_FACTORY);
    return &GLOBAL_CLAP_PLUGIN_FACTORY;
  }
  MIP_ClapPrint("id: '%s' -> null\n",factory_id);
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
