#ifndef mip_exe_host_proxy_included
#define mip_exe_host_proxy_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_host_proxy.h"

class MIP_ExeHostProxy
: public MIP_HostProxy {

//------------------------------
public:
//------------------------------

  MIP_ExeHostProxy()
  : MIP_HostProxy() {
  }

  //----------

  virtual ~MIP_ExeHostProxy() {
  }

//------------------------------
public:
//------------------------------

  uint32_t getPluginFormat() final {
    return MIP_PLUGIN_FORMAT_EXE;
  }

};

//----------------------------------------------------------------------
#endif
