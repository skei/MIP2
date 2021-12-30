#ifndef mip_plugin_list_included
#define mip_plugin_list_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"

//----------------------------------------------------------------------

class MIP_Plugin;
class MIP_PluginList;
class MIP_HostProxy;

void MIP_RegisterPlugins(MIP_PluginList* AList);
MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, MIP_Descriptor* ADescriptor);

//----------------------------------------------------------------------
//
// plugin info
//
//----------------------------------------------------------------------

struct MIP_PluginInfo {
  uint32_t        index       = 0;
  MIP_Descriptor* descriptor  = nullptr;
  void*           ptr         = nullptr;
};

//----------

typedef MIP_Array<MIP_PluginInfo*> MIP_PluginInfoArray;

//----------------------------------------------------------------------
//
// plugin list
//
//----------------------------------------------------------------------

class MIP_PluginList {

//------------------------------
private:
//------------------------------

  MIP_PluginInfoArray MPlugins = {};

//------------------------------
public:
//------------------------------

  MIP_PluginList() {
    MIP_PRINT;
  }

  //----------

  ~MIP_PluginList() {
    MIP_PRINT;
    deletePlugins();
  }

//------------------------------
public:
//------------------------------

  uint32_t appendPlugin(MIP_PluginInfo* info) {
    MIP_PRINT;
    uint32_t index = MPlugins.size();
    info->index = index;
    MPlugins.append(info);
    return index;
  }

  //----------

  uint32_t appendPlugin(MIP_Descriptor* descriptor) {
    MIP_PRINT;
    MIP_PluginInfo* info = new MIP_PluginInfo();
    info->descriptor = descriptor;
    uint32_t index = MPlugins.size();
    info->index = index;
    MPlugins.append(info);
    return index;
  }

  //----------

  void deletePlugins() {
    MIP_PRINT;
    for (uint32_t i=0; i<MPlugins.size(); i++) {
      delete MPlugins[i];
    }
    MPlugins.clear();
  }

  //----------

  uint32_t getNumPlugins() {
    MIP_PRINT;
    return MPlugins.size();
  }

  //----------

  MIP_PluginInfo* getPluginInfo(uint32_t index) {
    MIP_PRINT;
    return MPlugins[index];
  }

  //----------

  MIP_Descriptor* getPluginDescriptor(uint32_t index) {
    MIP_PRINT;
    MIP_PluginInfo* info = MPlugins[index];
    if (info) return info->descriptor;
    return nullptr;
  }

  //----------

  MIP_PluginInfo* findPluginById(const char* id) {
    MIP_PRINT;
    for (uint32_t i=0; i<MPlugins.size(); i++) {
      MIP_Descriptor* descriptor = MPlugins[i]->descriptor;
      if (strcmp(id,descriptor->getStringId()) == 0) return MPlugins[i];
    }
    return nullptr;
  }

  //----------

  MIP_Plugin* createPlugin(uint32_t AIndex) {
    MIP_PRINT;
    MIP_Descriptor* descriptor = MPlugins[AIndex]->descriptor;
    return MIP_CreatePlugin(AIndex,descriptor);
  }


};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

MIP_PluginList  MIP_GLOBAL_PLUGIN_LIST = {};

//----------------------------------------------------------------------
#endif
