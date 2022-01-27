
#include "mip.h"
#include "plugin/clap/mip_clap_entry.h"
#include "plugin/clap/mip_clap_plugin.h"

//#include "plugin/exe/mip_exe_wrapper.h"
//#include "plugin/lv2/mip_lv2_wrapper.h"
//#include "plugin/vst2/mip_vst2_wrapper.h"
//#include "plugin/vst3/mip_vst3_wrapper.h"

int main() {
  return 0;
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* myFeatures[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t myDescriptor = {
  CLAP_VERSION,
  "plugin_id",
  "name",
  "vendor",
  "url",
  "manual_url",
  "support_url",
  "0.0.0",
  "description",
  myFeatures
};

//----------

class myPlugin
: public MIP_ClapPlugin {
public:
  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_ClapPlugin(ADescriptor,AHost) {}
  //myPlugin() : MIP_ClapPlugin() {}
  virtual ~myPlugin() {}
};

//myPlugin MY_PLUGIN;


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

uint32_t MIP_GetNumPlugins() {
  return 1;
}

//----------

const clap_plugin_descriptor_t* MIP_GetDescriptor(uint32_t index) {
  return &myDescriptor;
}

//----------

const clap_plugin_t* MIP_CreatePlugin(const clap_host_t *host, const char *plugin_id) {
  myPlugin* plugin = new myPlugin(&myDescriptor,host); // who deletes this?
  //MY_PLUGIN.set(&myDescriptor,host);
  //return MY_PLUGIN.getPtr();
  return plugin->getPtr();
}
