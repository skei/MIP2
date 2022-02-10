#ifndef mip_clap_list_included
#define mip_clap_list_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"
#include "extern/clap/clap.h"
#include "plugin/clap/mip_clap_plugin.h"
#include <vector>

//----------

typedef MIP_Array<const clap_plugin_descriptor_t*> clap_descriptors;
typedef MIP_Array<MIP_ClapPlugin*> mip_clap_plugins;

#ifdef MIP_PLUGIN_USE_INVALIDATION
typedef MIP_Array<const clap_plugin_invalidation_source_t*> clap_invalidation_sources;
#endif


class MIP_ClapList;
extern void MIP_RegisterPlugins(MIP_ClapList* AList);
extern MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost);

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ClapList {

//------------------------------
private:
//------------------------------

    clap_descriptors          MDescriptors;
    #ifdef MIP_PLUGIN_USE_INVALIDATION
    clap_invalidation_sources MInvalidationSources;
    #endif
    mip_clap_plugins          MPluginInstances;



//------------------------------
public:
//------------------------------

  MIP_ClapList() {
    MIP_RegisterPlugins(this);
  }

  //----------

  ~MIP_ClapList() {
  }

//------------------------------
public:
//------------------------------

  void appendPlugin(const clap_plugin_descriptor_t* ADescriptor) {
    MDescriptors.push_back(ADescriptor);
  }

  //----------

  uint32_t getNumPlugins() {
    return MDescriptors.size();
  }

  //----------

  const clap_plugin_descriptor_t* getPlugin(uint32_t AIndex) {
    return MDescriptors[AIndex];
  }

  //----------

  // returns index of plugin with plugin_id, or -1 if not found

  int32_t findPluginById(const char *plugin_id) {
    for (uint32_t i=0; i<MDescriptors.size(); i++) {
      if (strcmp(plugin_id,MDescriptors[i]->id) == 0) return i;
    }
    return -1;
  }

//------------------------------
public:
//------------------------------

  #ifdef MIP_PLUGIN_USE_INVALIDATION

  void appendInvalidationSource(const clap_plugin_invalidation_source_t* ASource) {
    MInvalidationSources.push_back(ASource);
  }

  //----------

  uint32_t getNumInvalidationSources() {
    return MInvalidationSources.size();
  }

  //----------

  const clap_plugin_invalidation_source_t* getInvalidationSource(uint32_t AIndex) {
    return MInvalidationSources[AIndex];
  }

  #endif // MIP_PLUGIN_USE_INVALIDATION

//------------------------------
public:
//------------------------------

  void appendInstance(MIP_ClapPlugin* AInstance) {
    MPluginInstances.push_back(AInstance);
  }

  //----------

  int32_t findInstance(MIP_ClapPlugin* AInstance) {
    for (uint32_t i=0; i<MPluginInstances.size(); i++) {
      if (AInstance == MPluginInstances[i]) return i;
    }
    return -1;
  }

  //----------

  void removeInstance(MIP_ClapPlugin* AInstance) {
    int32_t index = findInstance(AInstance);
    if (index >= 0) {
      MPluginInstances.remove(index);
    }
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

MIP_ClapList MIP_GLOBAL_CLAP_LIST = {};

//----------------------------------------------------------------------
#endif
