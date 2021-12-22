

// nc -U -l -k /tmp/mip.socket
#define MIP_DEBUG
#define MIP_DEBUG_PRINT
#define MIP_DEBUG_PRINT_SOCKET

#include "mip.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class myDescriptor
: public MIP_ClapPluginDescriptor {

//------------------------------
public:
//------------------------------

  myDescriptor() {
    setName("myPlugin");
    setVendor("Tor-Helge Skei");
    setUrl("https://torhelgeskei.com");
    setVersion("0.0.0");
    setType(CLAP_PLUGIN_AUDIO_EFFECT);
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin : public MIP_ClapPlugin {

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor* desc)
  : MIP_ClapPlugin(desc) {
    MIP_PRINT;
  }

  //----------

  virtual ~myPlugin() {
    MIP_PRINT;
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    MIP_PRINT;
    return true;
  }

  void destroy() final {
    MIP_PRINT;
  }

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MIP_PRINT;
    return true;
  }

  void deactivate() final {
    MIP_PRINT;
  }

  bool start_processing() final {
    MIP_PRINT;
    return true;
  }

  void stop_processing() final {
    MIP_PRINT;
  }

  clap_process_status process(const clap_process_t* process) final {
    //MIP_PRINT;
    return CLAP_PROCESS_CONTINUE;
  }

  const void* get_extension(const char* id) final {
    MIP_PRINT;
    return nullptr;
  }

  void on_main_thread() final {
    MIP_PRINT;
  }


};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void MIP_RegisterPlugins() {
  MIP_ClapPluginDescriptor* desc = new myDescriptor();
  registerPlugin(desc);
}

//----------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t index, const clap_plugin_descriptor* desc) {
  if (index == 0) return new myPlugin(desc);
  return nullptr;
}
