#ifndef mip_clap_invalidation_included
#define mip_clap_invalidation_included
//----------------------------------------------------------------------

#include "mip.h"
#include "extern/clap/clap.h"

//----------------------------------------------------------------------

//#define NUM_CLAP_INVALIDATION_SOURCES 2
//const clap_plugin_invalidation_source_t MIP_CLAP_INVALIDATION_SOURCES[NUM_CLAP_INVALIDATION_SOURCES] = {
//  { "/usr/lib/ladspa/", ".so", true },
//  { "~/.ladspa",        ".so", true }
//};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

uint32_t clap_invalidation_count_callback(const struct clap_plugin_invalidation_factory *factory) {
  //return NUM_CLAP_INVALIDATION_SOURCES;
  return 0;
}

const clap_plugin_invalidation_source_t* clap_invalidation_get_callback(const struct clap_plugin_invalidation_factory *factory, uint32_t index) {
  //return &MIP_CLAP_INVALIDATION_SOURCES[index];
  return nullptr;
}

bool clap_invalidation_refresh_callback(const struct clap_plugin_invalidation_factory *factory) {
  return false;
}


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//static
//constexpr
const clap_plugin_invalidation_factory MIP_GLOBAL_CLAP_INVALIDATION = {
  clap_invalidation_count_callback,
  clap_invalidation_get_callback,
  clap_invalidation_refresh_callback
};

//----------------------------------------------------------------------
#endif
