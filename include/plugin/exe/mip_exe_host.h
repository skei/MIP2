#ifndef mip_exe_host_included
#define mip_exe_host_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_host_implementation.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ExeHost
: public MIP_ClapHostImplementation {

public:

  MIP_ExeHost(int argc, char** argv)
  : MIP_ClapHostImplementation() {
    MHost.host_data      = this;
    MHost.name           = "MIP_ExeHost";
  }

  virtual ~MIP_ExeHost() {
  }

public:

  const clap_host_t* getHost() {
    return &MHost;
  }

public:

  //const void* get_extension(const char *extension_id) override {
  //  return nullptr;
  //}

  //virtual void request_restart() override {
  //}

  //virtual void request_process() override {
  //}

  //virtual void request_callback() override {
  //}

private:

  clap_host_t MHost = {
    .clap_version   = CLAP_VERSION,
    .host_data      = this,
    .name           = "MIP_ExeHost",
    .vendor         = "skei.audio",
    .url            = "torhelgeskei.com",
    .version        = "1",
    .get_extension    = clap_host_get_extension_callback,
    .request_restart  = clap_host_request_restart_callback,
    .request_process  = clap_host_request_process_callback,
    .request_callback = clap_host_request_callback_callback
  };

  //----------

  static const void* clap_host_get_extension_callback(const struct clap_host *host, const char *extension_id) {
    MIP_ExeHost* exehost = (MIP_ExeHost*)host->host_data;
    return exehost->get_extension(extension_id);
  }

  static void clap_host_request_restart_callback(const struct clap_host *host) {
    MIP_ExeHost* exehost = (MIP_ExeHost*)host->host_data;
    return exehost->request_restart();
  }

  static void clap_host_request_process_callback(const struct clap_host *host) {
    MIP_ExeHost* exehost = (MIP_ExeHost*)host->host_data;
    return exehost->request_process();
  }

  static void clap_host_request_callback_callback(const struct clap_host *host) {
    MIP_ExeHost* exehost = (MIP_ExeHost*)host->host_data;
    return exehost->request_callback();
  }

};

//----------------------------------------------------------------------
#endif
