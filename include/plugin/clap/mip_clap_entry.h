#ifndef mip_clap_entry_included
#define mip_clap_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "extern/clap/clap.h"
#include "plugin/clap/mip_clap_registry.h"
#include "plugin/clap/mip_clap_factory.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

/*
  This function must be called fist, and can only be called once.
  It should be as fast as possible, in order to perform very quick scan of the plugin
  descriptors.
  It is forbidden to display graphical user interface in this call.
  It is forbidden to perform user inter-action in this call.
  If the initialization depends upon expensive computation, maybe try to do them ahead of time
  and cache the result.
*/

bool clap_entry_init_callback(const char *plugin_path) {
  //MIP_Print("clap_entry_init_callback\n");
  return true;
}

//----------

/*
  No more calls into the DSO must be made after calling deinit().
*/

void clap_entry_deinit_callback() {
}

//----------

/*
  Get the pointer to a factory.
  See plugin-factory.h, vst2-converter.h ...
  Returns null if the factory is not provided.
  The returned pointer must *not* be freed by the caller.
*/

const void* clap_entry_get_factory_callback(const char *factory_id) {
  int32_t index = MIP_CLAP_REGISTRY.findFactoryById(factory_id);
  if (index >= 0) return MIP_CLAP_REGISTRY.getFactory(index);
  if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) {
    return &MIP_CLAP_FACTORY;
  }
  return nullptr;
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// get rid of "warning: ‘visibility’ attribute ignored [-Wattributes]"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"

CLAP_EXPORT const clap_plugin_entry clap_entry = {
  CLAP_VERSION,
  clap_entry_init_callback,
  clap_entry_deinit_callback,
  clap_entry_get_factory_callback,
};

#pragma GCC diagnostic pop

//----------------------------------------------------------------------
#endif
