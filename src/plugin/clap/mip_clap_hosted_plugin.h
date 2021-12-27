#ifndef mip_clap_hosted_plugin_included
#define mip_clap_hosted_plugin_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------

class MIP_ClapHostedPlugin {

//------------------------------
private:
//------------------------------

  const clap_plugin_entry*  MPluginEntry = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ClapHostedPlugin(const clap_plugin_entry* plugin_entry) {
    MPluginEntry = plugin_entry;
  }

  //----------

  ~MIP_ClapHostedPlugin() {
  }

};

//----------------------------------------------------------------------
#endif
