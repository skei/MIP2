#ifndef mip_plugin_included
#define mip_plugin_included
//----------------------------------------------------------------------

#include "plugin/clap/mip_clap.h"

extern uint32_t                         MIP_GetNumPlugins();
extern const clap_plugin_descriptor_t*  MIP_GetDescriptor(uint32_t index);
extern const clap_plugin_t*             MIP_CreatePlugin(const clap_host_t *host, const char *plugin_id);

#include "plugin/clap/mip_clap_entry.h"

// adapters:
#include "plugin/vst2/mip_vst2_entry.h"
#include "plugin/vst3/mip_vst3_entry.h"
#include "plugin/lv2/mip_lv2_entry.h"
#include "plugin/exe/mip_exe_entry.h"

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

//TODO: simplified plugin

//class MIP_Plugin
//: public MIP_ClapPlugin {
//
//public:
//
//  MIP_Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
//  : MIP_ClapPlugin(ADescriptor,AHost) {
//  }
//
//  virtual ~MIP_Plugin() {
//  }
//
//};

//----------------------------------------------------------------------
#endif
