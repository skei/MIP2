#ifndef mip_plugin_included
#define mip_plugin_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_plugin.h"

class MIP_Plugin
: public MIP_ClapPlugin {

//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  MIP_Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_ClapPlugin(ADescriptor,AHost) {
  }

  virtual ~MIP_Plugin() {
  }

};

//----------------------------------------------------------------------
#endif
