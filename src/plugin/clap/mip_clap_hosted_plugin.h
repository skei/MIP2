#ifndef mip_clap_hosted_plugin_included
#define mip_clap_hosted_plugin_included
//----------------------------------------------------------------------

// aka ClapPluginProxy
//TODO: extensions..

#include "mip.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------

class MIP_ClapHostedPlugin {

//------------------------------
private:
//------------------------------

  const clap_plugin*  MPlugin = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ClapHostedPlugin(const clap_plugin* plugin) {
    MPlugin = plugin;
  }

  //----------

  ~MIP_ClapHostedPlugin() {
  }

//------------------------------
public:
//------------------------------

  const clap_plugin* getClapPlugin() {
    return MPlugin;
  }

//------------------------------
public:
//------------------------------

  bool init() {
    return MPlugin->init(MPlugin);
  }

  void destroy() {
    MPlugin->destroy(MPlugin);
  }

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    return MPlugin->activate(MPlugin,sample_rate,min_frames_count,max_frames_count);
  }

  void deactivate() {
    MPlugin->deactivate(MPlugin);
  }

  bool start_processing() {
    return MPlugin->start_processing(MPlugin);
  }

  void stop_processing() {
    MPlugin->stop_processing(MPlugin);
  }

  clap_process_status process(const clap_process_t *process) {
    return MPlugin->process(MPlugin,process);
  }

  const void* get_extension(const char *id) {
    return MPlugin->get_extension(MPlugin,id);
  }

  void on_main_thread() {
    MPlugin->on_main_thread(MPlugin);
  }

};

//----------------------------------------------------------------------
#endif
