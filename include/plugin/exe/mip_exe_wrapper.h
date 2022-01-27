#ifndef mip_exe_wrapper_included
#define mip_exe_wrapper_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/exe/mip_exe.h"
#include "plugin/exe/mip_exe_host.h"

//----------------------------------------------------------------------

int main(int argc, char** argv) {
  MIP_ExeHost* exehost = new MIP_ExeHost(argc,argv);
  const clap_plugin_descriptor_t* descriptor = MIP_GetDescriptor(0);
  const clap_host_t* host = exehost->getPtr();;
  const clap_plugin_t* plugin = MIP_CreatePlugin(host,descriptor->id);
  plugin->init(plugin);
  //
  plugin->destroy(plugin);
  delete exehost;
  return 0;
}

//----------------------------------------------------------------------
#endif
