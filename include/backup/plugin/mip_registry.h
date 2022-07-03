#ifndef mip_clap_registry_included
#define mip_clap_registry_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"
#include "extern/clap/clap.h"
#include "plugin/clap/mip_clap_plugin.h"
//#include <vector>

//----------

typedef MIP_Array<const clap_plugin_descriptor_t*> clap_descriptor_array;
typedef MIP_Array<const void*> clap_factory_array;
typedef MIP_Array<const char*> clap_factory_id_array;

class MIP_Registry;

extern void MIP_Register(MIP_Registry* ARegistry) __MIP_WEAK;
extern void MIP_Unregister(MIP_Registry* ARegistry) __MIP_WEAK;
extern MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) __MIP_WEAK;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Registry {

//------------------------------
private:
//------------------------------

    clap_descriptor_array MDescriptors          = {};
    clap_factory_array    MFactories            = {};
    clap_factory_id_array MFactoryIds           = {};

    clap_descriptor_array MAutoFreeDescriptors  = {};

//------------------------------
public:
//------------------------------

  MIP_Registry() {
    //MIP_PRINT;
    if (MIP_Register) MIP_Register(this);
  }

  //----------

  ~MIP_Registry() {
    //MIP_PRINT;
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

  //void autoFreeDescriptor(const clap_plugin_descriptor_t* ADescriptor) {
  //  MAutoFreeDescriptors.push_back(ADescriptor);
  //}

  //----------

  void appendDescriptor(const clap_plugin_descriptor_t* ADescriptor, bool AAutoFree=false) {
    //MIP_PRINT;
    MDescriptors.push_back(ADescriptor);
    if (AAutoFree) {
      MAutoFreeDescriptors.push_back(ADescriptor);
    }
  }

  //----------

  uint32_t getNumDescriptors() {
    //MIP_PRINT;
    return MDescriptors.size();
  }

  //----------

  const clap_plugin_descriptor_t* getDescriptor(uint32_t AIndex) {
    //MIP_PRINT;
    return MDescriptors[AIndex];
  }

  //----------

  // returns index of plugin with plugin_id, or -1 if not found

  int32_t findDescriptorById(const char *plugin_id) {
    //MIP_PRINT;
    for (uint32_t i=0; i<MDescriptors.size(); i++) {
      if (strcmp(plugin_id,MDescriptors[i]->id) == 0) return i;
    }
    return -1;
  }

//------------------------------
public: // factories
//------------------------------

  void appendFactory(const char* AId, const void* AFactory) {
    //MIP_PRINT;
    MFactoryIds.push_back(AId);
    MFactories.push_back(AFactory);
  }

  //----------

  uint32_t getNumFactories() {
    //MIP_PRINT;
    return MDescriptors.size();
  }

  //----------

  const void* getFactory(uint32_t AIndex) {
    //MIP_PRINT;
    return MFactories[AIndex];
  }

  //----------

  // returns index of plugin with plugin_id, or -1 if not found

  int32_t findFactoryById(const char *id) {
    //MIP_PRINT;
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

MIP_Registry MIP_REGISTRY = {};

//----------------------------------------------------------------------
#endif
