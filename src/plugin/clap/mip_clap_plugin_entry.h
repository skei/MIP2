#ifndef mip_clap_plugin_entry_included
#define mip_clap_plugin_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_plugin_factory.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class ENTRY>
class MIP_ClapPluginEntry {

private:

  static constexpr ENTRY MPluginEntry = {};

public:

  bool init(const char *plugin_path) {
    return MPluginEntry.clap_plugin_entry_init_callback(plugin_path);
  }

public:

  static bool clap_plugin_entry_init_callback(const char *plugin_path) {
    return MPluginEntry.init(plugin_path);
  }

  static void clap_plugin_entry_deinit_callback(void) {
    return MPluginEntry.deinit();
  }

  static uint32_t clap_plugin_entry_get_plugin_count_callback(void) {
    return MPluginEntry.get_plugin_count();
  }

  static const clap_plugin_descriptor* clap_plugin_entry_get_plugin_descriptor_callback(uint32_t index) {
    return MPluginEntry.get_plugin_descriptor(index);
  }

  static const clap_plugin* clap_plugin_entry_create_plugin_callback(const clap_host *host, const char *plugin_id) {
    return MPluginEntry.create_plugin(host,plugin_id);
  }

  static uint32_t clap_plugin_entry_get_invalidation_source_count_callback(void) {
    return MPluginEntry.get_invalidation_source_count();
  }

  static const clap_plugin_invalidation_source* clap_plugin_entry_get_invalidation_source_callback(uint32_t index) {
    return MPluginEntry.get_invalidation_source(index);
  }

  static void clap_plugin_entry_refresh_callback(void) {
    return MPluginEntry.refresh();
  }

public:

  __MIP_EXPORT
  static constexpr const struct clap_plugin_entry CLAP_PLUGIN_ENTRY_STRUCT asm("clap_plugin_entry") = {
    CLAP_VERSION,
    clap_plugin_entry_init_callback,
    clap_plugin_entry_deinit_callback,
    clap_plugin_entry_get_plugin_count_callback,
    clap_plugin_entry_get_plugin_descriptor_callback,
    clap_plugin_entry_create_plugin_callback,
    clap_plugin_entry_get_invalidation_source_count_callback,
    clap_plugin_entry_get_invalidation_source_callback,
    clap_plugin_entry_refresh_callback
  };

};


//----------------------------------------------------------------------
#endif
