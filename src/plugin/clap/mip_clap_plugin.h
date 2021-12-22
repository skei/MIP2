#ifndef mip_clap_plugin_included
#define mip_clap_plugin_included
//----------------------------------------------------------------------

class MIP_ClapPlugin {

//------------------------------
private:
//------------------------------

  struct host {
    clap_host_audio_ports*        audio_ports         = nullptr;
    clap_host_audio_ports_config* audio_ports_config  = nullptr;
    clap_host_event_filter*       event_filter        = nullptr;
    clap_host_fd_support*         fd_support          = nullptr;
    clap_host_gui*                gui                 = nullptr;
    clap_host_latency*            latency             = nullptr;
    clap_host_log*                log                 = nullptr;
    clap_host_note_name*          note_name           = nullptr;
    clap_host_note_ports*         note_ports          = nullptr;
    clap_host_params*             params              = nullptr;
    clap_host_state*              state               = nullptr;
    clap_host_thread_check*       thread_check        = nullptr;
    clap_host_thread_pool*        thread_pool         = nullptr;
    clap_host_timer_support*      timer_support       = nullptr;
  };

  struct host_draft {
    clap_host_check_for_update*   check_for_update    = nullptr;
    clap_host_file_reference*     file_reference      = nullptr;
    clap_host_midi_mappings*      midi_mappings       = nullptr;
    clap_host_quick_controls*     quick_controls      = nullptr;
    clap_host_surround*           surround            = nullptr;
    clap_host_track_info*         track_info          = nullptr;
    clap_host_tuning*             tuning              = nullptr;
  };

  clap_plugin* get_clap_plugin() { return &MClapPlugin; }

//------------------------------
public:
//------------------------------

  MIP_ClapPlugin(const clap_plugin_descriptor* clapdesc) {
    MClapPlugin.desc = clapdesc;
  }

  virtual ~MIP_ClapPlugin() {
  }

  const clap_plugin_t* getClapPlugin() {
    return &MClapPlugin;
  }

//------------------------------
public:
//------------------------------

  virtual bool init() { return true; }
  virtual void destroy() {}
  virtual bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) { return true; }
  virtual void deactivate() {}
  virtual bool start_processing() { return true; }
  virtual void stop_processing() {}
  virtual clap_process_status process(const clap_process_t *process) { return CLAP_PROCESS_CONTINUE; }
  virtual const void* get_extension(const char *id) { return nullptr; }
  virtual void on_main_thread() {}

//------------------------------
private:
//------------------------------

  static bool clap_plugin_init_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->init();
  }

  static void clap_plugin_destroy_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->destroy();
  }

  static bool clap_plugin_activate_callback(const struct clap_plugin *plugin, double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->activate(sample_rate,min_frames_count,max_frames_count);
  }

  static void clap_plugin_deactivate_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->deactivate();
  }

  static bool clap_plugin_start_processing_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->start_processing();
  }

  static void clap_plugin_stop_processing_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->stop_processing();
  }

  static clap_process_status clap_plugin_process_callback(const struct clap_plugin *plugin, const clap_process_t *process) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->process(process);
  }

  static const void* clap_plugin_get_extension_callback(const struct clap_plugin *plugin, const char *id) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->get_extension(id);
  }

  static void clap_plugin_on_main_thread_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->on_main_thread();
  }

//------------------------------
private:
//------------------------------

  //const
  clap_plugin_t MClapPlugin = {
    nullptr,
    this,
    clap_plugin_init_callback,
    clap_plugin_destroy_callback,
    clap_plugin_activate_callback,
    clap_plugin_deactivate_callback,
    clap_plugin_start_processing_callback,
    clap_plugin_stop_processing_callback,
    clap_plugin_process_callback,
    clap_plugin_get_extension_callback,
    clap_plugin_on_main_thread_callback
  };

};

//----------------------------------------------------------------------
#endif

