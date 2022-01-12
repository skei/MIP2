#ifndef mip_plugin_list_included
#define mip_plugin_list_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"
#include "plugin/mip_plugin.h"

//----------------------------------------------------------------------

class MIP_Plugin;
class MIP_PluginList;
class MIP_HostProxy;

void MIP_RegisterPlugins(MIP_PluginList* AList);
MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, MIP_Descriptor* ADescriptor, MIP_HostProxy* AHost);

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
    //MIP_PRINT;
    MIP_RegisterPlugins(this);
  }

  //----------

  ~MIP_PluginList() {
    //MIP_PRINT;
    #ifndef MIP_NO_AUTODELETE
    deletePlugins();
    #endif
  }

//------------------------------
public:
//------------------------------

  uint32_t appendPlugin(MIP_PluginInfo* info) {
    //MIP_PRINT;
    uint32_t index = MPlugins.size();
    info->index = index;
    MPlugins.append(info);
    return index;
  }

  //----------

  uint32_t appendPlugin(MIP_Descriptor* descriptor) {
    //MIP_PRINT;
    MIP_PluginInfo* info = new MIP_PluginInfo();
    info->descriptor = descriptor;
    uint32_t index = MPlugins.size();
    info->index = index;
    MPlugins.append(info);
    return index;
  }

  //----------

  void deletePlugins() {
    //MIP_PRINT;
    for (uint32_t i=0; i<MPlugins.size(); i++) {
      delete MPlugins[i];
    }
    MPlugins.clear();
  }

  //----------

  uint32_t getNumPlugins() {
    //MIP_PRINT;
    return MPlugins.size();
  }

  //----------

  MIP_PluginInfo* getPluginInfo(uint32_t index) {
    //MIP_PRINT;
    return MPlugins[index];
  }

  //----------

  MIP_Descriptor* getPluginDescriptor(uint32_t index) {
    //MIP_PRINT;
    MIP_PluginInfo* info = MPlugins[index];
    if (info) return info->descriptor;
    return nullptr;
  }

  //----------

  // clap, lv2?
  MIP_PluginInfo* findPluginById(const char* id) {
    //MIP_PRINT;
    for (uint32_t i=0; i<MPlugins.size(); i++) {
      MIP_Descriptor* descriptor = MPlugins[i]->descriptor;
      if (strcmp(id,descriptor->getStringId()) == 0) return MPlugins[i];
    }
    return nullptr;
  }

  // vst3
  MIP_PluginInfo* findPluginByLongId(const char* id) {
    for (uint32_t i=0; i<MPlugins.size(); i++) {
      MIP_Descriptor* descriptor = MPlugins[i]->descriptor;
      if (id16_equal(id,descriptor->getLongId())) return MPlugins[i];
    }
    return nullptr;
  }


  //----------

  MIP_Plugin* createPlugin(uint32_t AIndex, MIP_HostProxy* AHost) {
    //MIP_PRINT;
    MIP_Descriptor* descriptor = MPlugins[AIndex]->descriptor;
    return MIP_CreatePlugin(AIndex,descriptor,AHost);
  }

//------------------------------
private:
//------------------------------

  bool id16_equal(const void* id1, const void* id2) {
    uint32_t* ptr1 = (uint32_t*)id1;
    uint32_t* ptr2 = (uint32_t*)id2;
    if (ptr1[0] != ptr2[0]) return false;
    if (ptr1[1] != ptr2[1]) return false;
    if (ptr1[2] != ptr2[2]) return false;
    if (ptr1[3] != ptr2[3]) return false;
    return true;
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
