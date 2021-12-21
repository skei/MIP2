#ifndef CLAP_plugin_entry_included
#define CLAP_plugin_entry_included
//----------------------------------------------------------------------

//// nc -U -l -k /tmp/mip.socket
////#define MIP_DEBUG_PRINT_SOCKET
//#define MIP_NO_PLUGIN
//#define MIP_NO_GUI
//#include "mip.h"

#include "extern/clap/all.h"
#include "extern/clap/ext/draft/check-for-update.h"
#include "extern/clap/ext/draft/midi-mappings.h"

#include "editor.h"

//----------------------------------------------------------------------

class CLAP_PluginEntry {

//------------------------------
private:
//------------------------------

//  clap_plugin_descriptor  MDescriptor = {
//    CLAP_VERSION,
//    "CLAP_Plugin",              // id
//    "CLAP_Plugin",              // name
//    "Tor-Helge Skei",           // vendor
//    "https://torhelgeskei.com", // url
//    "manual_url",               // manual url
//    "support_url",              // support url
//    "0.0.1",                    // version
//    "clap test plugin",         // description
//    "clap;test;plugin",         // keywords
//    CLAP_PLUGIN_AUDIO_EFFECT
//  };

//------------------------------
public:
//------------------------------

  CLAP_PluginEntry() {}
  virtual ~CLAP_PluginEntry() {}

//------------------------------
public: // your plugin
//------------------------------

  virtual bool init(const char *plugin_path) { return false; }
  virtual void deinit(void) {}
  virtual uint32_t get_plugin_count(void) { return 0; }
  virtual const clap_plugin_descriptor* get_plugin_descriptor(uint32_t index) { return nullptr; }
  virtual const clap_plugin* create_plugin(const clap_host *host, const char *plugin_id) { return nullptr; }
  virtual uint32_t get_invalidation_source_count(void) { return 0; }
  virtual const clap_plugin_invalidation_source* get_invalidation_source(uint32_t index) { return nullptr; }
  virtual void refresh(void) {}

};

//----------------------------------------------------------------------

// our plugin entry
//CLAP_PluginEntry GLOBAL_CLAP_PLUGIN_ENTRY_STRUCT;

//----------------------------------------------------------------------

// clap callbacks
// trampoline into our entry class

//----------

#define CLAP_PLUGIN(ENTRY)                                                                                    \
                                                                                                              \
  ENTRY   CLAP_PLUGIN_ENTRY;                                                                                  \
                                                                                                              \
  /* */                                                                                                       \
                                                                                                              \
  bool clap_plugin_entry_init_callback(const char *plugin_path) {                                             \
    return CLAP_PLUGIN_ENTRY.init(plugin_path);                                                               \
  }                                                                                                           \
                                                                                                              \
  void clap_plugin_entry_deinit_callback(void) {                                                              \
    CLAP_PLUGIN_ENTRY.deinit();                                                                               \
  }                                                                                                           \
                                                                                                              \
  uint32_t clap_plugin_entry_get_plugin_count_callback(void) {                                                \
    return CLAP_PLUGIN_ENTRY.get_plugin_count();                                                              \
  }                                                                                                           \
                                                                                                              \
  const clap_plugin_descriptor *clap_plugin_entry_get_plugin_descriptor_callback(uint32_t index) {            \
    return CLAP_PLUGIN_ENTRY.get_plugin_descriptor(index);                                                    \
  }                                                                                                           \
                                                                                                              \
  const clap_plugin *clap_plugin_entry_create_plugin_callback(const clap_host *host, const char *plugin_id) { \
    return CLAP_PLUGIN_ENTRY.create_plugin(host,plugin_id);                                                   \
  }                                                                                                           \
                                                                                                              \
  uint32_t clap_plugin_entry_get_invalidation_source_count_callback(void) {                                   \
    return CLAP_PLUGIN_ENTRY.get_invalidation_source_count();                                                 \
  }                                                                                                           \
                                                                                                              \
  const clap_plugin_invalidation_source *clap_plugin_entry_get_invalidation_source_callback(uint32_t index) { \
    return CLAP_PLUGIN_ENTRY.get_invalidation_source(index);                                                  \
  }                                                                                                           \
                                                                                                              \
  void clap_plugin_entry_refresh_callback(void) {                                                             \
    CLAP_PLUGIN_ENTRY.refresh();                                                                              \
  }                                                                                                           \
                                                                                                              \
  /* ---------- */                                                                                            \
                                                                      \
  __attribute__((visibility("default")))                              \
  struct clap_plugin_entry ENTRY_STRUCT asm("clap_plugin_entry") = {  \
    CLAP_VERSION,                                                     \
    clap_plugin_entry_init_callback,                                  \
    clap_plugin_entry_deinit_callback,                                \
    clap_plugin_entry_get_plugin_count_callback,                      \
    clap_plugin_entry_get_plugin_descriptor_callback,                 \
    clap_plugin_entry_create_plugin_callback,                         \
    clap_plugin_entry_get_invalidation_source_count_callback,         \
    clap_plugin_entry_get_invalidation_source_callback,               \
    clap_plugin_entry_refresh_callback                                \
  };                                                                  \


//----------------------------------------------------------------------
#endif
