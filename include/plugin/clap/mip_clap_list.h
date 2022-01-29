#ifndef mip_clap_list_included
#define mip_clap_list_included
//----------------------------------------------------------------------

#include "mip.h"
#include "extern/clap/clap.h"
#include "plugin/clap/mip_clap_host_proxy.h"
#include "plugin/clap/mip_clap_plugin.h"
#include <vector>

//----------

typedef std::vector<const clap_plugin_descriptor_t*> clap_descriptors;
class MIP_ClapList;

extern void MIP_RegisterPlugins(MIP_ClapList* AList);
MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost);

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ClapList {

//------------------------------
private:
//------------------------------

    clap_descriptors  MDescriptors;

//------------------------------
public:
//------------------------------

  MIP_ClapList() {
    MIP_RegisterPlugins(this);
  }

  //----------

  ~MIP_ClapList() {
    //for (uint32_t i=0; i<MDescriptors.size(); i++) {
    //  free(MDescriptors[i]);
    //}
  }

//------------------------------
public:
//------------------------------

  void appendPlugin(const clap_plugin_descriptor_t* ADescriptor) {
    MDescriptors.push_back(ADescriptor);
  }

  uint32_t getNumPlugins() {
    return MDescriptors.size();
  }

  const clap_plugin_descriptor_t* getPlugin(uint32_t AIndex) {
    return MDescriptors[AIndex];
  }

  int32_t findPlugin(const char *plugin_id) {
    for (uint32_t i=0; i<MDescriptors.size(); i++) {
      if (strcmp(plugin_id,MDescriptors[i]->id) == 0) return i;
    }
    return -1;
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
