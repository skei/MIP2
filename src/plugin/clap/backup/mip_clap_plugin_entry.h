#ifndef mip_clap_plugin_entry_included
#define mip_clap_plugin_entry_included
//----------------------------------------------------------------------

#include "mip.h"
//#include "base/types/mip_array.h"
#include "plugin/clap/mip_clap.h"

//extern void MIP_RegisterPlugins();

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
// plugin entry
//
//----------------------------------------------------------------------

template <class PLUGIN>
class MIP_ClapPluginEntry {

//------------------------------
private:
//------------------------------

  //MIP_ClapPluginList  MPlugins;

//------------------------------
public:
//------------------------------

  MIP_ClapPluginEntry() {
    //#ifdef MIP_REGISTER_PLUGINS
    //  MIP_RegisterPlugins();
    //#endif
  }

  virtual ~MIP_ClapPluginEntry() {
    //deletePlugins();
  }

//------------------------------
public:
//------------------------------

  virtual bool init(const char *plugin_path) {
    return false;
  }

  //----------

  virtual void deinit(void) {
  }

  //----------

  virtual uint32_t get_plugin_count(void) {
    //return MPlugins.size();
    return 0;
  }

  //----------

  virtual const clap_plugin_descriptor* get_plugin_descriptor(uint32_t index) {
    return nullptr;
  }

  //----------

  virtual const clap_plugin* create_plugin(const clap_host *host, const char *plugin_id) {
    return nullptr;
  }

  //----------

  virtual uint32_t get_invalidation_source_count(void) {
    return 0;
  }

  //----------

  virtual const clap_plugin_invalidation_source* get_invalidation_source(uint32_t index) {
    return nullptr;
  }

  //----------

  virtual void refresh(void) {
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

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define MIP_CLAP_PLUGIN_ENTRY(PLUGIN)                                                                         \
                                                                                                              \
  MIP_ClapPluginEntry<PLUGIN> GLOBAL_CLAP_PLUGIN_ENTRY;                                                       \
                                                                                                              \
  /* ---------- */                                                                                            \
                                                                                                              \
  /*void MIP_AppendPlugin(MIP_ClapPluginInfo* info) {                                                         \
    GLOBAL_CLAP_PLUGIN_ENTRY.appendPlugin(info);                                                              \
  }*/                                                                                                         \
                                                                                                              \
  /* ---------- */                                                                                            \
                                                                                                              \
  bool clap_plugin_entry_init_callback(const char *plugin_path) {                                             \
    return GLOBAL_CLAP_PLUGIN_ENTRY.init(plugin_path);                                                        \
  }                                                                                                           \
                                                                                                              \
  void clap_plugin_entry_deinit_callback(void) {                                                              \
    return GLOBAL_CLAP_PLUGIN_ENTRY.deinit();                                                                 \
  }                                                                                                           \
                                                                                                              \
  uint32_t clap_plugin_entry_get_plugin_count_callback(void) {                                                \
    return GLOBAL_CLAP_PLUGIN_ENTRY.get_plugin_count();                                                       \
  }                                                                                                           \
                                                                                                              \
  const clap_plugin_descriptor* clap_plugin_entry_get_plugin_descriptor_callback(uint32_t index) {            \
    return GLOBAL_CLAP_PLUGIN_ENTRY.get_plugin_descriptor(index);                                             \
  }                                                                                                           \
                                                                                                              \
  const clap_plugin* clap_plugin_entry_create_plugin_callback(const clap_host *host, const char *plugin_id) { \
    return GLOBAL_CLAP_PLUGIN_ENTRY.create_plugin(host,plugin_id);                                            \
  }                                                                                                           \
                                                                                                              \
  uint32_t clap_plugin_entry_get_invalidation_source_count_callback(void) {                                   \
    return GLOBAL_CLAP_PLUGIN_ENTRY.get_invalidation_source_count();                                          \
  }                                                                                                           \
                                                                                                              \
  const clap_plugin_invalidation_source* clap_plugin_entry_get_invalidation_source_callback(uint32_t index) { \
    return GLOBAL_CLAP_PLUGIN_ENTRY.get_invalidation_source(index);                                           \
  }                                                                                                           \
                                                                                                              \
  void clap_plugin_entry_refresh_callback(void) {                                                             \
    return GLOBAL_CLAP_PLUGIN_ENTRY.refresh();                                                                \
  }                                                                                                           \
                                                                                                              \
  /* ---------- */                                                                                            \
                                                                                                              \
__MIP_EXPORT                                                                                                  \
const struct clap_plugin_entry CLAP_PLUGIN_ENTRY_STRUCT asm("clap_plugin_entry") = {                          \
  CLAP_VERSION,                                                                                               \
  clap_plugin_entry_init_callback,                                                                            \
  clap_plugin_entry_deinit_callback,                                                                          \
  clap_plugin_entry_get_plugin_count_callback,                                                                \
  clap_plugin_entry_get_plugin_descriptor_callback,                                                           \
  clap_plugin_entry_create_plugin_callback,                                                                   \
  clap_plugin_entry_get_invalidation_source_count_callback,                                                   \
  clap_plugin_entry_get_invalidation_source_callback,                                                         \
  clap_plugin_entry_refresh_callback                                                                          \
};                                                                                                            \


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


//----------------------------------------------------------------------
#endif

