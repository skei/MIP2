#ifndef mip_simple_plugin_included
#define mip_simple_plugin_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_plugin.h"

class MIP_SimplePlugin
: public MIP_Plugin {

//------------------------------
public:
//------------------------------

  MIP_SimplePlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost, uint32_t ANumParams)
  : MIP_Plugin(ADescriptor,AHost,ANumParams) {
  }

  //----------

  virtual ~MIP_SimplePlugin() {
  }

};

//----------------------------------------------------------------------
#endif
