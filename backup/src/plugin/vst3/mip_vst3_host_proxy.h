#ifndef mip_vst3_host_proxy_included
#define mip_vst3_host_proxy_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_host_proxy.h"

class MIP_Vst3HostProxy
: public MIP_HostProxy {

public:

  uint32_t getPluginFormat() final {
    return MIP_PLUGIN_FORMAT_VST3;
  }

};

//----------------------------------------------------------------------
#endif

