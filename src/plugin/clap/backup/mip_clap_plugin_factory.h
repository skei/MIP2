#ifndef mip_clap_plugin_factory_included
#define mip_clap_plugin_factory_included
//----------------------------------------------------------------------

#include "mip.h"
//#include "base/types/mip_array.h"
#include "plugin/clap/mip_clap.h"
//#include "plugin/clap/mip_clap_plugin.h"

//----------------------------------------------------------------------
//
// plugin info
//
//----------------------------------------------------------------------

//struct MIP_ClapPluginInfo {
//  uint32_t                      index = 0;
//  const clap_plugin_descriptor* desc  = nullptr;
//};

//----------

//typedef MIP_Array<MIP_ClapPluginInfo*> MIP_ClapPluginList;

//----------------------------------------------------------------------
//
// plugin factory
//
//----------------------------------------------------------------------

class MIP_ClapPluginFactory {

//------------------------------
private:
//------------------------------

  //MIP_ClapPluginList  MPlugins;

//------------------------------
public:
//------------------------------

  MIP_ClapPluginFactory() {
    // call registration function..
  }

  //----------

  ~MIP_ClapPluginFactory() {
    //deletePlugins();
  }

//------------------------------
public:
//------------------------------

//  void appendPlugin(MIP_ClapPluginInfo* info) {
//    uint32_t index = MPlugins.size();
//    info->index = index;
//    MPlugins.append(info);
//  }
//
//  //void appendPlugin(const clap_plugin_descriptor* desc) {
//  //  MIP_ClapPluginInfo* info = new MIP_ClapPluginInfo();
//  //  info->desc = desc;
//  //  MPlugins.append(info);
//  //}
//
//  //----------
//
//  void deletePlugins() {
//    for (uint32_t i=0; i<MPlugins.size(); i++) {
//      delete MPlugins[i];
//    }
//    MPlugins.clear(false);
//  }
//
//  //----------
//
//  MIP_ClapPluginInfo* getPluginInfo(uint32_t index) {
//    return MPlugins[index];
//  }
//
//  //----------
//
//  // return -1 if not found
//
//  int32_t findPluginInfoById(const char* id) {
//    for (uint32_t i=0; i<MPlugins.size(); i++) {
//      if (strstr(id,MPlugins[i]->desc->id) == 0) { return i; }
//    }
//    return -1;
//  }
//
//  //----------
//
//  // return NULL if not found
//
//  MIP_ClapPluginInfo* getPluginInfoById(const char* id) {
//    for (uint32_t i=0; i<MPlugins.size(); i++) {
//      if (strstr(id,MPlugins[i]->desc->id) == 0) {
//        return MPlugins[i];
//      }
//    }
//    return nullptr;
//  }

//------------------------------
public:
//------------------------------

  virtual uint32_t get_plugin_count() {
    //return MPlugins.size();
    return 0;
  }

  //----------

  virtual const clap_plugin_descriptor* get_plugin_descriptor(uint32_t index) {
    //if (index >= MPlugins.size()) return nullptr;
    //return MPlugins[index]->desc;
    return nullptr;
  }

  //----------

  virtual const clap_plugin* create_plugin(const clap_host *host, const char *plugin_id) {
    //MIP_ClapPluginInfo* info = getPluginInfoById(plugin_id);
    //if (info) {
    //  MIP_ClapPlugin* plugin = new MIP_ClapPlugin(info->desc,host);
    //  return plugin->getClapPlugin();
    //}
    return nullptr;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_SimplePluginFactory
: public MIP_ClapPluginFactory {

private:

  myDescriptor  MDescriptor;

public:

  uint32_t get_plugin_count() final {
    return 1;
  }

  const clap_plugin_descriptor* get_plugin_descriptor(uint32_t index) final {
    return MDescriptor.getClapDescriptor();
  }

  const clap_plugin* create_plugin(const clap_host *host, const char *plugin_id) final {
    const clap_plugin_descriptor* desc = MDescriptor.getClapDescriptor();
    if (strstr(plugin_id,desc->id)) {
      myPlugin* plugin = new myPlugin(desc,host);
      return plugin->get_clap_plugin();
    }
    return nullptr;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define MIP_CLAP_PLUGIN_FACTORY                                                                                 \
                                                                                                                \
  MIP_ClapPluginFactory GLOBAL_CLAP_PLUGIN_FACTORY;                                                             \
                                                                                                                \
  /* ---------- */                                                                                              \
                                                                                                                \
  uint32_t clap_plugin_factory_get_plugin_count_callback(void) {                                                \
    return GLOBAL_CLAP_PLUGIN_FACTORY.get_plugin_count();                                                       \
  }                                                                                                             \
                                                                                                                \
  const clap_plugin_descriptor* clap_plugin_factory_get_plugin_descriptor_callback(uint32_t index) {            \
    return GLOBAL_CLAP_PLUGIN_FACTORY.get_plugin_descriptor(index);                                             \
  }                                                                                                             \
                                                                                                                \
  const clap_plugin* clap_plugin_factory_create_plugin_callback(const clap_host *host, const char *plugin_id) { \
    return GLOBAL_CLAP_PLUGIN_FACTORY.create_plugin(host,plugin_id);                                            \
  }                                                                                                             \
                                                                                                                \
  /* ---------- */                                                                                              \
                                                                                                                \
  struct clap_plugin_factory MClapPluginFactory = {                                                             \
    clap_plugin_factory_get_plugin_count_callback,                                                              \
    clap_plugin_factory_get_plugin_descriptor_callback,                                                         \
    clap_plugin_factory_create_plugin_callback                                                                  \
  };                                                                                                            \

//----------------------------------------------------------------------
#endif

