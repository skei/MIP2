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

  //const clap_plugin_entry*  MPluginEntry = nullptr;
  const clap_plugin*  MPlugin= nullptr;

//------------------------------
public:
//------------------------------

  //MIP_ClapHostedPlugin(const clap_plugin_entry* plugin_entry) {
  //  MPluginEntry = plugin_entry;
  //}

  //----------

  MIP_ClapHostedPlugin(const clap_plugin* plugin) {
    MPlugin = plugin;
  }

  //----------

  ~MIP_ClapHostedPlugin() {
  }

};

//----------------------------------------------------------------------
#endif
