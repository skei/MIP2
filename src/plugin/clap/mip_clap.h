#ifndef mip_clap_included
#define mip_clap_included
//----------------------------------------------------------------------

#include "mip.h"
#include "extern/clap/all.h"
#include "extern/clap/ext/draft/check-for-update.h"
#include "extern/clap/ext/draft/midi-mappings.h"

//#ifndef MIP_NO_PLUGIN
//  #include "plugin/clap/mip_clap_plugin.h"
//  #include "plugin/clap/mip_clap_utils.h"
//#endif

typedef struct clap_plugin_entry16 {
  struct clap_version clap_version;
  bool (*init)(const char *plugin_path);
  void (*deinit)(void);
  const void *(*get_factory)(const char *factory_id);
} clap_plugin_entry_16;

//----------

struct clap_plugin_factory16 {
  uint32_t (*get_plugin_count)(void);
  const clap_plugin_descriptor *(*get_plugin_descriptor)(uint32_t index);
  const clap_plugin *(*create_plugin)(const clap_host *host, const char *plugin_id);
};

//----------

static const CLAP_CONSTEXPR char CLAP_PLUGIN_FACTORY_ID[] = "clap.plugin-factory";

//----------------------------------------------------------------------
#endif
