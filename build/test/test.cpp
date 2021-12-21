

// nc -U -l -k /tmp/mip.socket
#define MIP_DEBUG
//#define MIP_DEBUG_PRINT_SOCKET

#include "mip.h"
#include "plugin/clap/mip_clap_plugin_entry.h"
#include "plugin/clap/mip_clap_plugin_entry16.h"
#include "plugin/clap/mip_clap_plugin_factory.h"
#include "plugin/clap/mip_clap_plugin_descriptor.h"
#include "plugin/clap/mip_clap_plugin.h"

//class myClapPluginEntry;
//class myClapPluginEntry16;
//class myClapPluginFactory;

//class MIP_ClapPluginEntry<myClapPluginEntry>;
//class MIP_ClapPluginEntry16<myClapPluginEntry16>;
//class MIP_ClapPluginFactory<myClapPluginFactory>;

//----------------------------------------------------------------------

class myClapDescriptor : public MIP_ClapPluginDescriptor {
};

//----------------------------------------------------------------------

class myClapPlugin : public MIP_ClapPlugin {
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myClapPluginEntry {

public:

  static bool init(const char *plugin_path) { return false; }
  static void deinit(void) {}
  static uint32_t get_plugin_count(void) { return 0; }
  static const clap_plugin_descriptor* get_plugin_descriptor(uint32_t index) { return nullptr; }
  static const clap_plugin* create_plugin(const clap_host *host, const char *plugin_id) { return nullptr; }
  static uint32_t get_invalidation_source_count(void) { return 0; }
  static const clap_plugin_invalidation_source* get_invalidation_source(uint32_t index) { return nullptr; }
  static void refresh(void) {}

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myClapPluginFactory {

public:

  static uint32_t get_plugin_count() {
    MIP_PRINT;
    return 0;
  }

  static const clap_plugin_descriptor* get_plugin_descriptor(uint32_t index) {
    MIP_PRINT;
    return nullptr;
  }

  static const clap_plugin* create_plugin(const clap_host *host, const char *plugin_id) {
    MIP_PRINT;
    return nullptr;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myClapPluginEntry16 {

private:

  static constexpr myClapPluginFactory MFactory = {};


public:

  static bool init(const char *plugin_path) {
    MIP_PRINT;
    return true;
  }

  static void deinit() {
    MIP_PRINT;
  }

  static const void* get_factory(const char* factory_id) {
    MIP_PRINT;
    return &MFactory;
  }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//MIP_ClapPluginEntry<myClapPluginEntry> ENTRY;
MIP_ClapPluginEntry16<myClapPluginEntry16> ENTRY16;

//----------------------------------------------------------------------

int main() {
  MIP_PRINT;

  ENTRY16.init("./");
  ENTRY16.deinit();
  const clap_plugin_factory16* factory = (const clap_plugin_factory16*)ENTRY16.get_factory(CLAP_PLUGIN_FACTORY_ID);

  return 0;
}
