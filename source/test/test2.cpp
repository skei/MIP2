
#include "mip.h"
#include "plugin/clap/mip_clap.h"

#include "plugin/clap/mip_clap_list.h"
#include "plugin/clap/mip_clap_entry.h"
#include "plugin/clap/mip_clap_factory.h"
#include "plugin/clap/mip_clap_plugin.h"

int main() {
  return 0;
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

clap_plugin_descriptor descriptor1 = {};

class myPlugin
: public MIP_ClapPlugin {

public:

  myPlugin(const clap_host_t* AHost)
  : MIP_ClapPlugin(AHost) {
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


void MIP_RegisterPlugins(MIP_ClapList* AList) {
  AList->appendPlugin(&descriptor1);
}

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_host_t* AHost) {
  return new myPlugin(AHost);
}

