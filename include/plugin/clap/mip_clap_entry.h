#ifndef mip_clap_entry_included
#define mip_clap_entry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "extern/clap/clap.h"
#include "plugin/clap/mip_clap_list.h"
#include "plugin/clap/mip_clap_factory.h"

//extern void MIP_RegisterPlugins(MIP_ClapList* AList);
//extern void MIP_UnregisterPlugins(MIP_ClapList* AList);

#ifndef MIP_NO_DEFAULT_PLUGIN_ENTRY

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

bool clap_entry_init_callback(const char *plugin_path) {
  //MIP_Print("plugin_path: %s\n",plugin_path);
  //MIP_RegisterPlugins(&MIP_GLOBAL_CLAP_LIST);
  return true;
}

//----------

void clap_entry_deinit_callback() {
  //MIP_PRINT;
  //MIP_UnregisterPlugins(&MIP_GLOBAL_CLAP_LIST);
}

//----------

// MIP_ClapList->registerFactory( CLAP_PLUGIN_INVALIDATION_FACTORY_ID, &MIP_GLOBAL_CLAP_INVALIDATION );

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

//----------------------------------------------------------------------
//
// test 1
//
//----------------------------------------------------------------------

#if 0

template <class E>
class MIP_ClapEntry {

public:

  static
  bool init_callback(const char *plugin_path) {
    MIP_PRINT;
    return E::init(plugin_path);
  }

  static
  void deinit_callback() {
    MIP_PRINT;
    E::deinit();
  }

  static
  const void* get_factory_callback(const char *factory_id) {
    MIP_PRINT;
    return E::get_factory(factory_id);
  }

public:

  // get rid of "warning: ‘visibility’ attribute ignored [-Wattributes]"
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wattributes"

  static constexpr
  CLAP_EXPORT const clap_plugin_entry clap_entry asm("clap_entry") = {
    CLAP_VERSION,
    init_callback,
    deinit_callback,
    get_factory_callback,
  };

  #pragma GCC diagnostic pop

};

//----------------------------------------------------------------------

MIP_ClapEntry<myEntry> MIP_GLOBAL_CLAP_ENTRY = {};

#endif // 0

//----------------------------------------------------------------------
//
// test 2
//
//----------------------------------------------------------------------

#if 0

class myPluginEntry {

//public:
//
//  myPluginEntry() { MIP_PRINT; }
//  ~myPluginEntry() { MIP_PRINT; }

public:

  static
  bool init(const char *plugin_path) {
    //MIP_PRINT;
    return true;
  }

  static
  void deinit() {
    //MIP_PRINT;
  }

  static
  const void* get_factory(const char *factory_id) {
    //MIP_PRINT;
    if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) return &MIP_GLOBAL_CLAP_FACTORY;
    else return nullptr;
  }

public:

  //static constexpr
  //CLAP_EXPORT const clap_plugin_entry clap_entry2 asm("clap_entry2") = {
  //  CLAP_VERSION,
  //  init,
  //  deinit,
  //  get_factory,
  //};

};

//----------------------------------------------------------------------

myPluginEntry MIP_GLOBAL_CLAP_ENTRY = {};

// get rid of "warning: ‘visibility’ attribute ignored [-Wattributes]"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"

//static constexpr
CLAP_EXPORT const clap_plugin_entry clap_entry /*asm("clap_entry")*/ = {
  CLAP_VERSION,
  MIP_GLOBAL_CLAP_ENTRY.init,
  MIP_GLOBAL_CLAP_ENTRY.deinit,
  MIP_GLOBAL_CLAP_ENTRY.get_factory,
};

#pragma GCC diagnostic pop

#endif // 0

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#endif // MIP_NO_DEFAULT_PLUGIN_ENTRY

//----------------------------------------------------------------------
#endif
