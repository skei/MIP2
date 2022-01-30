#ifndef mip_plugin_included
#define mip_plugin_included
//----------------------------------------------------------------------

#include "mip.h"

#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_plugin.h"

//

#include "plugin/clap/mip_clap_entry.h"
#include "plugin/clap/mip_clap_factory.h"

#include "plugin/wrapper/mip_exe_wrapper.h"
#include "plugin/wrapper/mip_lv2_wrapper.h"
#include "plugin/wrapper/mip_vst2_wrapper.h"
#include "plugin/wrapper/mip_vst3_wrapper.h"

//----------------------------------------------------------------------

class MIP_Plugin
: public MIP_ClapPlugin {

//------------------------------
private:
//------------------------------

  //MIP_Editor* MEditor = nullptr;

//------------------------------
public:
//------------------------------

  MIP_Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_ClapPlugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~MIP_Plugin() {
  }

};

//----------------------------------------------------------------------
#endif
