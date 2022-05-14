#ifndef mip_clap_registry_included
#define mip_clap_registry_included
//----------------------------------------------------------------------

/*
  MIP_CLAP_REGISTRY is global/static, so it should be initialized before any
  functions in the .so is called.. in the constructor, we call MIP_Register()
  so that the user can register plugins (and factories)..
  we could move the MIP_Register call to MIP_Plugin.init() ...
*/

//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"
#include "extern/clap/clap.h"
#include "plugin/clap/mip_clap_plugin.h"
#include <vector>

//----------

typedef MIP_Array<const clap_plugin_descriptor_t*> clap_descriptor_array;
typedef MIP_Array<const void*> clap_factory_array;
typedef MIP_Array<const char*> clap_factory_id_array;

//----------

class MIP_ClapRegistry;

extern void MIP_Register(MIP_ClapRegistry* ARegistry) __MIP_WEAK;
extern void MIP_Unregister(MIP_ClapRegistry* ARegistry) __MIP_WEAK;

extern MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) __MIP_WEAK;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ClapRegistry {

//------------------------------
private:
//------------------------------

    clap_descriptor_array MDescriptors          = {};
    clap_descriptor_array MAutoFreeDescriptors  = {};
    clap_factory_array    MFactories            = {};
    clap_factory_id_array MFactoryIds           = {};

    //#ifdef MIP_PLUGIN_USE_INVALIDATION
    //clap_invalidation_sources MInvalidationSources;
    //#endif
    //mip_clap_plugins MPluginInstances;

//------------------------------
public:
//------------------------------

  MIP_ClapRegistry() {
    if (MIP_Register) MIP_Register(this);
  }

  //----------

  ~MIP_ClapRegistry() {
    for (uint32_t i=0; i<MAutoFreeDescriptors.size(); i++) {
      clap_plugin_descriptor_t* desc = (clap_plugin_descriptor_t*)MAutoFreeDescriptors[i];
      if (desc) {
        free((void*)desc->id);
        free((void*)desc->name);
        free((void*)desc->vendor);
        free(desc);
      }
    }
    if (MIP_Unregister) MIP_Unregister(this);
  }

//------------------------------
public: // descriptors
//------------------------------

  void autoFreeDescriptor(const clap_plugin_descriptor_t* ADescriptor) {
    MAutoFreeDescriptors.push_back(ADescriptor);
  }

  //----------

  void appendDescriptor(const clap_plugin_descriptor_t* ADescriptor) {
    MDescriptors.push_back(ADescriptor);
  }

  //----------

  uint32_t getNumDescriptors() {
    return MDescriptors.size();
  }

  //----------

  const clap_plugin_descriptor_t* getDescriptor(uint32_t AIndex) {
    return MDescriptors[AIndex];
  }

  //----------

  // returns index of plugin with plugin_id, or -1 if not found

  int32_t findDescriptorById(const char *plugin_id) {
    for (uint32_t i=0; i<MDescriptors.size(); i++) {
      if (strcmp(plugin_id,MDescriptors[i]->id) == 0) return i;
    }
    return -1;
  }

//------------------------------
public: // factories
//------------------------------

  void appendFactory(const char* AId, const void* AFactory) {
    MFactoryIds.push_back(AId);
    MFactories.push_back(AFactory);
  }

  //----------

  uint32_t getNumFactories() {
    return MDescriptors.size();
  }

  //----------

  const void* getFactory(uint32_t AIndex) {
    return MFactories[AIndex];
  }

  //----------

  // returns index of plugin with plugin_id, or -1 if not found

  int32_t findFactoryById(const char *id) {
    for (uint32_t i=0; i<MFactoryIds.size(); i++) {
      if (strcmp(id,MFactoryIds[i]) == 0) return i;
    }
    return -1;
  }

  //----------

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#ifndef MIP_NO_CLAP_REGISTRY

MIP_ClapRegistry MIP_CLAP_REGISTRY = {};

//#endif

//----------------------------------------------------------------------
#endif
