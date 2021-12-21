#ifndef mip_clap_plugin_entry16_included
#define mip_clap_plugin_entry16_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

typedef struct clap_plugin_entry16 {
  struct clap_version clap_version;
  bool (*init)(const char *plugin_path);
  void (*deinit)(void);
  const void *(*get_factory)(const char *factory_id);
} clap_plugin_entry_16;

struct clap_plugin_factory {
  uint32_t (*get_plugin_count)(void);
  const clap_plugin_descriptor *(*get_plugin_descriptor)(uint32_t index);
  const clap_plugin *(*create_plugin)(const clap_host *host, const char *plugin_id);
};

static const CLAP_CONSTEXPR char CLAP_PLUGIN_FACTORY_ID[] = "clap.plugin-factory";

//#include "plugin/clap/mip_clap_plugin_factory.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


template <class FACTORY>
class MIP_ClapPluginEntry16 {

private:

  FACTORY   MFactory;

//------------------------------
public:
//------------------------------

  MIP_ClapPluginEntry16() {}
  virtual ~MIP_ClapPluginEntry16() {}

public:

  virtual bool init(const char *plugin_path) {
    return false;
  }

  virtual void deinit() {
  }

  virtual const void* get_factory(const char *factory_id) {
    if (strstr(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) {
      return &MFactory;
    }
    return nullptr;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define MIP_CLAP_PLUGIN_ENTRY16(FACTORY)                                                    \
                                                                                            \
  MIP_ClapPluginEntry16<FACTORY> GLOBAL_CLAP_PLUGIN_ENTRY16;                                \
                                                                                            \
  /* ---------- */                                                                          \
                                                                                            \
  bool clap_plugin_entry16_init_callback(const char *plugin_path) {                         \
    return GLOBAL_CLAP_PLUGIN_ENTRY16.init(plugin_path);                                    \
  }                                                                                         \
                                                                                            \
  void clap_plugin_entry16_deinit_callback(void) {                                          \
    GLOBAL_CLAP_PLUGIN_ENTRY16.deinit();                                                    \
  }                                                                                         \
                                                                                            \
  const void* clap_plugin_entry16_get_factory_callback(const char* factory_id) {            \
    return GLOBAL_CLAP_PLUGIN_ENTRY16.get_factory(factory_id);                              \
  }                                                                                         \
                                                                                            \
  /* ---------- */                                                                          \
                                                                                            \
  __MIP_EXPORT                                                                              \
  const struct clap_plugin_entry16 CLAP_PLUGIN_ENTRY16_STRUCT asm("clap_plugin_entry16") = {  \
    CLAP_VERSION,                                                                           \
    clap_plugin_entry16_init_callback,                                                      \
    clap_plugin_entry16_deinit_callback,                                                    \
    clap_plugin_entry16_get_factory_callback                                                \
  };                                                                                        \


//----------------------------------------------------------------------
#endif

