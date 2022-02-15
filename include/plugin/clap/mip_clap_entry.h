#ifndef mip_clap_entry_included
#define mip_clap_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "extern/clap/clap.h"
#include "plugin/clap/mip_clap_list.h"
#include "plugin/clap/mip_clap_factory.h"

extern void MIP_RegisterPlugins(MIP_ClapList* AList);
extern void MIP_UnregisterPlugins(MIP_ClapList* AList);

//----------

#ifndef MIP_NO_DEFAULT_PLUGIN_ENTRY

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//bool entry_init_has_already_been_called = false;

/*
  This function must be called fist, and can only be called once.
  It should be as fast as possible, in order to perform very quick scan of the
  plugin descriptors.
  It is forbidden to display graphical user interface in this call.
  It is forbidden to perform user inter-action in this call.
  If the initialization depends upon expensive computation, maybe try to do
  them ahead of time and cache the result.
*/

bool clap_entry_init_callback(const char *plugin_path) {
  //if (entry_init_has_already_been_called) {
  //  MIP_Print("entry.init has already been called..\n");
  //}
  //else {
  //  MIP_Print("entry.init..\n");
    MIP_RegisterPlugins(&MIP_GLOBAL_CLAP_LIST);
  //  entry_init_has_already_been_called = true;
  //}
  return true;
}

//----------

/*
  No more calls into the DSO must be made after calling deinit().
*/

void clap_entry_deinit_callback() {
  MIP_PRINT;
  MIP_UnregisterPlugins(&MIP_GLOBAL_CLAP_LIST);
}

//----------

/*
  Get the pointer to a factory.
  See plugin-factory.h, vst2-converter.h ...
  Returns null if the factory is not provided.
  The returned pointer must *not* be freed by the caller.
*/

const void* clap_entry_get_factory_callback(const char *factory_id) {
  #ifdef MIP_PLUGIN_USE_INVALIDATION
  if (strcmp(factory_id,CLAP_PLUGIN_INVALIDATION_FACTORY_ID) == 0) {
    return &MIP_GLOBAL_CLAP_INVALIDATION;
  }
  #endif
  if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) {
    return &MIP_GLOBAL_CLAP_FACTORY;
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

// MIP_GLOBAL_CLAP_ENTRY
CLAP_EXPORT const clap_plugin_entry clap_entry = {
  CLAP_VERSION,
  clap_entry_init_callback,
  clap_entry_deinit_callback,
  clap_entry_get_factory_callback,
};

#pragma GCC diagnostic pop

#endif // MIP_NO_DEFAULT_PLUGIN_ENTRY

//----------------------------------------------------------------------
#endif
