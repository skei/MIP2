#ifndef mip_host_included
#define mip_host_included
//----------------------------------------------------------------------

//#define MIP_HOST_NO_LOADING

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_host.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Host {
: public MIP_ClapHost {

//------------------------------
public:
//------------------------------

  MIP_Host()
  : MIP_ClapHost() {
  }

  //----------

  virtual ~MIP_Host() {
  }

//------------------------------
public:
//------------------------------

  /*

  //void initialize(const clap_plugin_entry* entry, const clap_plugin_factory* factory) {
  //  MClapEntry = entry;
  //  MClapFactory = factory;
  //}

  //----------

  #ifndef MIP_HOST_NO_LOADING

  bool loadPlugin(const char* path) {
    MPluginPath = path;
    //MLibHandle = LoadLibrary(path);
    //MClapEntry = (struct clap_plugin_entry*)GetProcAddress(MLibHandle,"clap_entry");
    MLibHandle = dlopen(path,RTLD_LAZY|RTLD_LOCAL); // RTLD_NOW
    MClapEntry = (struct clap_plugin_entry*)dlsym(MLibHandle,"clap_entry");
    MIP_GetPathOnly(MPathOnly,path);
    MClapEntry->init(MPathOnly);
    MClapFactory = (const clap_plugin_factory*)MClapEntry->get_factory(CLAP_PLUGIN_FACTORY_ID);
    return true;
  }

  //----------

  void unloadPlugin() {
    //if (MClapEntry) MClapEntry->deinit();
    if (MLibHandle) dlclose(MLibHandle);
  }

  #endif

  //----------

  const clap_plugin_t* createPlugin(const char* path, uint32_t index, const clap_host_t* host=nullptr) {
    if (!MClapFactory) return nullptr;
    if (index >= MClapFactory->get_plugin_count(MClapFactory)) return nullptr;
    const clap_plugin_descriptor* descriptor = MClapFactory->get_plugin_descriptor(MClapFactory,index);
    const clap_plugin* plugin = nullptr;
    if (host) {
      plugin = MClapFactory->create_plugin(MClapFactory,host,descriptor->id);
    }
    else {
      plugin = MClapFactory->create_plugin(MClapFactory,&MHost,descriptor->id);
    }
    plugin->init(plugin);
    return plugin;
  }

  //----------

  //void destroyPlugin(const clap_plugin* plugin) {
  //  plugin->destroy(plugin);
  //}

  */



};

//----------------------------------------------------------------------
#endif
