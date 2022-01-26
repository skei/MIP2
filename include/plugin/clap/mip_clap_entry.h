#ifndef mip_clap_entry_included
#define mip_clap_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------
//
// factory
//
//----------------------------------------------------------------------

class MIP_ClapFactory {

//------------------------------
public:
//------------------------------

  static
  uint32_t get_plugin_count(const struct clap_plugin_factory *factory) {
    return MIP_GetNumPlugins();
  }

  //----------

  static
  const clap_plugin_descriptor_t* get_plugin_descriptor(const struct clap_plugin_factory *factory, uint32_t index) {
    return MIP_GetDescriptor(index);

  }

  //----------

  static
  const clap_plugin_t* create_plugin(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
    return MIP_CreatePlugin(host,plugin_id);
  }

};

//----------------------------------------------------------------------

const MIP_ClapFactory GLOBAL_CLAP_FACTORY;

//----------

//const clap_plugin_factory CLAP_FACTORY = {
const clap_plugin_factory clap_factory = {
  GLOBAL_CLAP_FACTORY.get_plugin_count,
  GLOBAL_CLAP_FACTORY.get_plugin_descriptor,
  GLOBAL_CLAP_FACTORY.create_plugin
};


//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

class MIP_ClapEntry {

//------------------------------
public:
//------------------------------

  static
  bool init(const char *plugin_path) {
    return true;
  }

  //----------

  static
  void deinit() {
  }

  //----------

  static
  const void* get_factory(const char *factory_id) {
    if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) {
      return &clap_factory;
    }
    return nullptr;
  }

};

//----------------------------------------------------------------------

const MIP_ClapEntry GLOBAL_CLAP_ENTRY;

//----------

// get rid of "warning: ‘visibility’ attribute ignored [-Wattributes]"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"

CLAP_EXPORT const clap_plugin_entry clap_entry = {
  CLAP_VERSION,
  GLOBAL_CLAP_ENTRY.init,
  GLOBAL_CLAP_ENTRY.deinit,
  GLOBAL_CLAP_ENTRY.get_factory
};

#pragma GCC diagnostic pop

//----------------------------------------------------------------------
#endif
