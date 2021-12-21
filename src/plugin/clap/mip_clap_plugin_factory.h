#ifndef mip_clap_plugin_factory_included
#define mip_clap_plugin_factory_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
//#include "plugin/clap/mip_clap_plugin.h"
//#include "plugin/clap/mip_clap_plugin_list.h"

//----------------------------------------------------------------------

template <class FACTORY>
class MIP_ClapPluginFactory {

private:

  static constexpr FACTORY MPluginFactory = {};

public:

  static uint32_t clap_plugin_factory_get_plugin_count_callback(void) {
    return MPluginFactory.get_plugin_count();
  }

  static const clap_plugin_descriptor* clap_plugin_factory_get_plugin_descriptor_callback(uint32_t index) {
    return MPluginFactory.get_plugin_descriptor(index);
  }

  static const clap_plugin* clap_plugin_factory_create_plugin_callback(const clap_host *host, const char *plugin_id) {
    return MPluginFactory.create_plugin(host,plugin_id);
  }

public:

  __MIP_EXPORT
  static constexpr struct clap_plugin_factory16 MClapPluginFactory asm("clap_plugin_factory") = {
    clap_plugin_factory_get_plugin_count_callback,
    clap_plugin_factory_get_plugin_descriptor_callback,
    clap_plugin_factory_create_plugin_callback
  };

};

//----------------------------------------------------------------------
#endif

