
#define MIP_NO_GUI

#include "mip.h"
#include "plugin/mip_plugin.h"
#include "plugin/clap/mip_clap_plugin.h"

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
    MIP_Print("Plugin()\n");
  }

  //----------

  virtual ~Plugin() {
    MIP_Print("~Plugin()\n");
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
  MIP_Print("MIP_GetNumPlugins()\n");
  return 1;
}

//----------

const clap_plugin_descriptor_t* MIP_GetDescriptor(uint32_t index) {
  MIP_Print("MIP_GetDescriptor(%i)\n",index);
  return &Descriptor;
}

//----------

const clap_plugin_t* MIP_CreatePlugin(const clap_host_t *host, const char *plugin_id) {
  MIP_Print("MIP_CreatePlugin(\"%s\")\n",plugin_id);
  MIP_ClapPlugin* plugin = new Plugin(&Descriptor,host);
  return  plugin->getPlugin();
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//int main(int argc, char** argv) {
//  return 0;
//}
