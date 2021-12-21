#ifndef mip_clap_plugin_entry16_included
#define mip_clap_plugin_entry16_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_plugin_factory.h"

//----------------------------------------------------------------------

template < class ENTRY>
class MIP_ClapPluginEntry16 {

private:

  static constexpr ENTRY MPluginEntry = {};

public:

  bool init(const char *plugin_path) { return MPluginEntry.init(plugin_path); }
  void deinit() { MPluginEntry.deinit(); }
  const void* get_factory(const char* factory_id) { return MPluginEntry.get_factory(factory_id); }

public:

  static bool clap_plugin_entry16_init_callback(const char *plugin_path) {
    return MPluginEntry.init(plugin_path);
  }

  static void clap_plugin_entry16_deinit_callback(void) {
    MPluginEntry.deinit();
  }

  static const void* clap_plugin_entry16_get_factory_callback(const char* factory_id) {
    return MPluginEntry.get_factory(factory_id);
  }

public:

  __MIP_EXPORT
    static constexpr const struct clap_plugin_entry16 MClapPluginEntry16 asm("clap_plugin_entry16") = {
    CLAP_VERSION,
    clap_plugin_entry16_init_callback,
    clap_plugin_entry16_deinit_callback,
    clap_plugin_entry16_get_factory_callback
  };

};

//----------------------------------------------------------------------
#endif
