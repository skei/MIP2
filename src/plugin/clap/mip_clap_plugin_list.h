#ifndef clap_plugin_list_included
#define clap_plugin_list_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_plugin_descriptor.h"

//----------------------------------------------------------------------

struct MIP_ClapPluginInfo {
  uint32_t                  index = 0;
  MIP_ClapPluginDescriptor* desc  = nullptr;
};

//----------

typedef MIP_Array<MIP_ClapPluginInfo*> MIP_ClapPluginInfos;

//----------------------------------------------------------------------

class MIP_ClapPluginList {

//------------------------------
private:
//------------------------------

  MIP_ClapPluginInfos  MPlugins = {};

//------------------------------
public:
//------------------------------

  MIP_ClapPluginList() {
  }

  //----------

  ~MIP_ClapPluginList() {
    deletePlugins();
  }

//------------------------------
public:
//------------------------------

  uint32_t appendPlugin(MIP_ClapPluginInfo* info) {
    uint32_t index = MPlugins.size();
    info->index = index;
    MPlugins.append(info);
    return index;
  }

  //----------

  void deletePlugins() {
    for (uint32_t i=0; i<MPlugins.size(); i++) {
      delete MPlugins[i];
    }
    MPlugins.clear();
  }

  //----------

  uint32_t getNumPlugins() {
    return MPlugins.size();
  }

  //----------

  MIP_ClapPluginInfo* getPluginInfo(uint32_t index) {
    return MPlugins[index];
  }

  //----------

  const clap_plugin_descriptor_t* getPluginDescriptor(uint32_t index) {
    MIP_ClapPluginInfo* info = MPlugins[index];
    if (info) {
      MIP_ClapPluginDescriptor* desc = info->desc;
      if (desc) return desc->getClapDescriptor();
    }
    return nullptr;
  }

  //----------

  MIP_ClapPluginInfo* findPluginById(const char* id) {
    for (uint32_t i=0; i<MPlugins.size(); i++) {
      if (strcmp(id,MPlugins[i]->desc->getId()) == 0) return MPlugins[i];
    }
    return nullptr;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

MIP_ClapPluginList  GLOBAL_CLAP_PLUGIN_LIST = {};

//------------------------------
//
//------------------------------

void registerPlugin(MIP_ClapPluginDescriptor* desc) {
  MIP_ClapPluginInfo* info = new MIP_ClapPluginInfo();
  info->desc = desc;
  GLOBAL_CLAP_PLUGIN_LIST.appendPlugin(info);
}

//----------------------------------------------------------------------
#endif
