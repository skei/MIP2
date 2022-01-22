
#define MIP_NO_GUI

#include "mip.h"
#include "plugin/mip_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class Plugin
: public MIP_ClapPlugin {

//------------------------------
public:
//------------------------------

  Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_ClapPlugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~Plugin() {
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t Descriptor = {
  CLAP_VERSION,
  "plugin_id",
  "name",
  "vendor",
  "url",
  "manual_url",
  "support_url",
  "0.0.0",
  "description",
  "audio_effect"
};

//----------------------------------------------------------------------

uint32_t MIP_GetNumPlugins() {
  return 1;
}

//----------

const clap_plugin_descriptor_t* MIP_GetDescriptor(uint32_t index) {
  return &Descriptor;
}

//----------

const clap_plugin_t* MIP_CreatePlugin(const clap_host_t *host, const char *plugin_id) {
  MIP_ClapPlugin* plugin = new Plugin(&Descriptor,host);
  return  plugin->getPlugin();
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

int main(int argc, char** argv) {
  return 0;
}
