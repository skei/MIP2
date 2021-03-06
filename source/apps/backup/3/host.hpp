#ifndef host_included
#define host_included
//----------------------------------------------------------------------

//#include <windows.h>
#include <dlfcn.h>

#include "clap/clap.h"
#include "clap/ext/draft/ambisonic.h"
#include "clap/ext/draft/check-for-update.h"


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class Host {


//------------------------------
private:
//------------------------------

    //HINSTANCE                   MLibHandle      = nullptr;

    void*                       MLibrary    = nullptr;
    const clap_plugin_entry*    MEntry      = nullptr;
    const clap_plugin_factory*  MFactory    = nullptr;
    char                        MPathOnly[512]  = {0};

//  clap_host                     MHost
//  clap_host_ambisonic           MAmbisonic
//  clap_host_audio_ports         MAudioPorts
//  clap_host_audio_ports_config  MAudioPortsConfig
//  clap_host_check_for_update    MChekForUpdate
//  clap_host_event_filter        MEventFilter
//  clap_host_event_registry      MEventRegistry
//  clap_host_file_reference      MFileReference
//  clap_host_gui                 MGui
//  clap_host_latency             MLatency
//  clap_host_log                 MLog
//  clap_host_midi_mappings       MMidiMappings
//  clap_host_note_name           MNoteName
//  clap_host_note_ports          MNotePorts
//  clap_host_params              MParams
//  clap_host_posix_fd_support    MPosixFdSupport
//  clap_host_quick_controls      MQuickControls
//  clap_host_state               MState
//  clap_host_surround            MSurround
//  clap_host_thread_check        MThreadCheck
//  clap_host_thread_pool         MThreadPool
//  clap_host_timer_support       MTimerSupport
//  clap_host_track_info          MTrackInfo
//  clap_host_tuning              MTuning

//------------------------------
public:
//------------------------------

  Host() {
  }

  //----------

  ~Host() {
  }

//------------------------------
public:
//------------------------------

  const clap_host*              getClapHost()     { return &MHost; }
  const clap_plugin_entry_t*    getClapEntry()    { return MEntry; }
  const clap_plugin_factory_t*  getClapFactory()  { return MFactory; }

  //----------

  void  setHostData(void* data)         { MHost.host_data = data; }
  void  setHostName(const char* txt)    { MHost.name      = txt; }
  void  setHostVendor(const char* txt)  { MHost.vendor    = txt; }
  void  setHostUrl(const char* txt)     { MHost.url       = txt; }
  void  setHostVersion(const char* txt) { MHost.version   = txt; }

//------------------------------
public:
//------------------------------

  bool loadPlugin(const char* path) {
    //MPluginPath = path;
    //MLibrary = LoadLibrary(path);
    //MEntry = (struct clap_plugin_entry*)GetProcAddress(MLibrary,"clap_entry");
    MLibrary = dlopen(path,RTLD_LAZY|RTLD_LOCAL); // RTLD_NOW
    MEntry = (struct clap_plugin_entry*)dlsym(MLibrary,"clap_entry");
    get_path_only(MPathOnly,path);
    MEntry->init(MPathOnly);
    MFactory = (const clap_plugin_factory*)MEntry->get_factory(CLAP_PLUGIN_FACTORY_ID);
    return true;
  }

  //----------

  void unloadPlugin() {
    //if (MEntry) MEntry->deinit();
    if (MLibrary) dlclose(MLibrary);
  }

  //----------

  const clap_plugin_t* createPlugin(const char* path, uint32_t index) {
    if (!MFactory) return nullptr;
    if (index >= MFactory->get_plugin_count(MFactory)) return nullptr;
    const clap_plugin_descriptor* descriptor = MFactory->get_plugin_descriptor(MFactory,index);
    const clap_plugin* plugin = MFactory->create_plugin(MFactory,&MHost,descriptor->id);
    plugin->init(plugin);
    return plugin;
  }

  //----------

  //void destroyPlugin(const clap_plugin* plugin) {
  //  plugin->destroy(plugin);
  //}

//------------------------------
private:
//------------------------------

  // /home/skei/test.so -> test.so
  // returns ptr to first character after last /

  /*
  const char* get_filename_from_path(const char* path) {
    if (path) {
      const char* slash     = strrchr(path,'/');
      const char* backslash = strrchr(path,'\\');
      if (slash) {
        return slash + 1;
      }
      else if (backslash) {
        return backslash + 1;
      }
    }
    return NULL;
  }
  */

  //----------

  // /home/skei/test.so -> /home/skei/
  // makes a copy of dst, inserts a 0 after the last /

  char* get_path_only(char* dst, const char* src) {
    if (dst && src) {
      strcpy(dst,src);
      char* slash     = strrchr(dst,'/');
      char* backslash = strrchr(dst,'\\');
      if (slash) {
        slash[1] = 0;
        return dst;
      }
      else if (backslash) {
        backslash[1] = 0;
        return dst;
      }
    }
    return nullptr;
  }

//------------------------------
public: // timer listener
//------------------------------

  //void on_timerCallback(void) override {
  //  //MIP_CLAPPRINT;
  //  //hosted_plugin->timer()
  //}

//------------------------------
public: // clap plugin
//------------------------------

  virtual const void* get_extension(const char *extension_id) { return nullptr; }
  virtual void request_restart() {}
  virtual void request_process() {}
  virtual void request_callback() {}

//------------------------------
public: // extensions
//------------------------------

  virtual void ambisonic_changed() {}
  virtual uint32_t audio_ports_get_preferred_sample_size() { return 0; }
  virtual void audio_ports_rescan(uint32_t flags) {}
  virtual void audio_ports_config_rescan() {}
  virtual void check_for_update_on_new_version(const clap_check_for_update_info *update_info) {}
  virtual void event_filter_changed() {}
  virtual bool event_registry_query(const char* space_name, uint16_t* space_id) { return false; }
  virtual void file_reference_changed() {}
  virtual void file_reference_set_dirty(clap_id resource_id) {}
  virtual bool gui_resize(uint32_t width, uint32_t height) { return false; }
  virtual void latency_changed() {}
  virtual void log_log(clap_log_severity severity, const char *msg) {}
  virtual void midi_mappings_changed() {}
  virtual void note_name_changed() {}
  virtual void note_ports_rescan(uint32_t flags) {}
  virtual void params_rescan(clap_param_rescan_flags flags) {}
  virtual void params_clear(clap_id param_id, clap_param_clear_flags flags) {}
  virtual void params_request_flush() {}
  virtual bool posix_fd_support_register_fd(int fd, int flags) { return false; }
  virtual bool posix_fd_support_modify_fd(int fd, int flags) { return false; }
  virtual bool posix_fd_support_unregister_fd(int fd) { return false; }
  virtual void quick_controls_changed(clap_quick_controls_changed_flags flags) {}
  virtual void state_mark_dirty() {}
  virtual void surround_changed() {}
  virtual void surround_get_preferred_channel_map(uint8_t* channel_map, uint32_t channel_map_capacity, uint32_t* channel_count) {}
  virtual bool thread_check_is_main_thread() { return false; }
  virtual bool thread_check_is_audio_thread() { return false; }
  virtual bool thread_pool_request_exec(uint32_t num_tasks) { return false; }
  virtual bool timer_support_register_timer(uint32_t period_ms, clap_id *timer_id) { return false; }
  virtual bool timer_support_unregister_timer(clap_id timer_id) { return false; }
  virtual bool track_info_get(clap_track_info *info) { return false; }
  virtual double tuning_get(int32_t key, int32_t channel) { return 0.0; }

//------------------------------
private:
//------------------------------

  static const void* clap_host_get_extension_callback(const struct clap_host *host, const char *extension_id) {
    Host* host_ = (Host*)host->host_data;
    return host_->get_extension(extension_id);
  }

  static void clap_host_request_restart_callback(const struct clap_host *host) {
    Host* host_ = (Host*)host->host_data;
    host_->request_restart();
  }

  static void clap_host_request_process_callback(const struct clap_host *host) {
    Host* host_ = (Host*)host->host_data;
    host_->request_process();
  }

  static void clap_host_request_callback_callback(const struct clap_host *host) {
    Host* host_ = (Host*)host->host_data;
    host_->request_callback();
  }

  //const
  clap_host MHost = {
    CLAP_VERSION,
    this,
    "Host",
    "Tor-Helge Skei",
    "https://torhelgeskei.com",
    "0.0.1",
    clap_host_get_extension_callback,
    clap_host_request_restart_callback,
    clap_host_request_process_callback,
    clap_host_request_callback_callback
  };

//------------------------------
private: // extensions
//------------------------------

  // ambisonic

  static void clap_host_ambisonic_changed_callback(const clap_host_t *host) {
    Host* host_ = (Host*)host->host_data;
    return host_->ambisonic_changed();
  }

  clap_host_ambisonic MAmbisonic = {
    clap_host_ambisonic_changed_callback
  };

  // audio-ports

  static uint32_t clap_host_audio_ports_get_preferred_sample_size_callback(const clap_host *host) {
    Host* host_ = (Host*)host->host_data;
    return host_->audio_ports_get_preferred_sample_size();
  }

  static void clap_host_audio_ports_rescan_callback(const clap_host *host, uint32_t flags) {
    Host* host_ = (Host*)host->host_data;
    host_->audio_ports_rescan(flags);
  }

  clap_host_audio_ports MAudioPorts = {
    clap_host_audio_ports_get_preferred_sample_size_callback,
    clap_host_audio_ports_rescan_callback
  };

  // audio-ports-config

  static void clap_host_audio_ports_config_rescan_callback(const struct clap_host *host) {
    Host* host_ = (Host*)host->host_data;
    host_->audio_ports_config_rescan();
  }

  clap_host_audio_ports_config MAudioPortsConfig = {
   clap_host_audio_ports_config_rescan_callback
  };

  // check-for-update.draft/0

  static void clap_host_check_for_update_on_new_version_callback(const clap_host *host, const clap_check_for_update_info *update_info) {
    Host* host_ = (Host*)host->host_data;
    host_->check_for_update_on_new_version(update_info);
  }

  clap_host_check_for_update MChekForUpdate = {
    clap_host_check_for_update_on_new_version_callback
  };

  // event-filter

  static void clap_host_event_filter_changed_callback(const clap_host *host) {
    Host* host_ = (Host*)host->host_data;
    host_->event_filter_changed();
  }

  clap_host_event_filter MEventFilter = {
    clap_host_event_filter_changed_callback
  };

  // event-registry

  static bool clap_host_event_registry_query_callback(const clap_host_t *host, const char *space_name, uint16_t *space_id) {
    Host* host_ = (Host*)host->host_data;
    return host_->event_registry_query(space_name,space_id);
  }

  clap_host_event_registry MEventRegistry = {
    clap_host_event_registry_query_callback
  };

  // file-reference.draft/0

  static void clap_host_file_reference_changed_callback(const clap_host *host) {
    Host* host_ = (Host*)host->host_data;
    host_->file_reference_changed();
  }

  static void clap_host_file_reference_set_dirty_callback(const clap_host *host, clap_id resource_id) {
    Host* host_ = (Host*)host->host_data;
    host_->file_reference_set_dirty(resource_id);
  }

  clap_host_file_reference MFileReference = {
    clap_host_file_reference_changed_callback,
    clap_host_file_reference_set_dirty_callback
  };

  // gui

  static bool clap_host_gui_resize_callback(const clap_host *host, uint32_t width, uint32_t height) {
    Host* host_ = (Host*)host->host_data;
    return host_->gui_resize(width,height);
  }

  clap_host_gui MGui = {
    clap_host_gui_resize_callback
  };

  // latency

  static void clap_host_latency_changed_callback(const clap_host *host) {
    Host* host_ = (Host*)host->host_data;
    host_->latency_changed();
  }

  clap_host_latency MLatency = {
    clap_host_latency_changed_callback
  };

  // log

  static void clap_host_log_log_callback(const clap_host *host, clap_log_severity severity, const char *msg) {
    Host* host_ = (Host*)host->host_data;
    host_->log_log(severity,msg);
  }

  clap_host_log MLog = {
    clap_host_log_log_callback
  };

  // midi-mappings.draft/0

  static void clap_host_midi_mappings_changed_callback(const clap_host *host) {
    Host* host_ = (Host*)host->host_data;
    host_->midi_mappings_changed();
  }

  clap_host_midi_mappings MMidiMappings = {
    clap_host_midi_mappings_changed_callback
  };

  // note-name

  static void clap_host_note_name_changed_callback(const clap_host *host) {
    Host* host_ = (Host*)host->host_data;
    host_->note_name_changed();
  }

  clap_host_note_name MNoteName = {
    clap_host_note_name_changed_callback
  };

  // note-ports

  static void clap_host_note_ports_rescan_callback(const clap_host *host, uint32_t flags) {
    Host* host_ = (Host*)host->host_data;
    host_->note_ports_rescan(flags);
  }

  clap_host_note_ports MNotePorts = {
    clap_host_note_ports_rescan_callback
  };

  // params

  static void clap_host_params_rescan_callback(const clap_host *host, clap_param_rescan_flags flags) {
    Host* host_ = (Host*)host->host_data;
    host_->params_rescan(flags);
  }

  static void clap_host_params_clear_callback(const clap_host *host, clap_id param_id, clap_param_clear_flags flags) {
    Host* host_ = (Host*)host->host_data;
    host_->params_clear(param_id,flags);
  }

  static void clap_host_params_request_flush_callback(const clap_host *host) {
    Host* host_ = (Host*)host->host_data;
    host_->params_request_flush();
  }

  clap_host_params MParams = {
    clap_host_params_rescan_callback,
    clap_host_params_clear_callback,
    clap_host_params_request_flush_callback
  };

  // posix-fd-support

  static bool clap_host_posix_fd_support_register_fd_callback(const clap_host *host, int fd, int flags) {
    Host* host_ = (Host*)host->host_data;
    return host_->posix_fd_support_register_fd(fd,flags);
  }

  static bool clap_host_posix_fd_support_modify_fd_callback(const clap_host *host, int fd, int flags) {
    Host* host_ = (Host*)host->host_data;
    return host_->posix_fd_support_modify_fd(fd,flags);
  }

  static bool clap_host_posix_fd_support_unregister_fd_callback(const clap_host *host, int fd) {
    Host* host_ = (Host*)host->host_data;
    return host_->posix_fd_support_unregister_fd(fd);
  }

  clap_host_posix_fd_support MPosixFdSupport = {
    clap_host_posix_fd_support_register_fd_callback,
    clap_host_posix_fd_support_modify_fd_callback,
    clap_host_posix_fd_support_unregister_fd_callback
  };

  // quick-controls.draft/0

  static void clap_host_quick_controls_changed_callback(const clap_host *host, clap_quick_controls_changed_flags flags) {
    Host* host_ = (Host*)host->host_data;
    host_->quick_controls_changed(flags);
  }

  clap_host_quick_controls MQuickControls = {
    clap_host_quick_controls_changed_callback
  };

  // state

  static void clap_host_state_mark_dirty_callback(const clap_host *host) {
    Host* host_ = (Host*)host->host_data;
    host_->state_mark_dirty();
  }

  clap_host_state MState = {
    clap_host_state_mark_dirty_callback
  };

  // surround

  static void clap_host_surround_changed_callback(const clap_host_t *host) {
    Host* host_ = (Host*)host->host_data;
    host_->surround_changed();
  }

  static void clap_host_surround_get_preferred_channel_map_callback(const clap_host_t *host, uint8_t* channel_map, uint32_t channel_map_capacity, uint32_t* channel_count) {
    Host* host_ = (Host*)host->host_data;
    host_->surround_get_preferred_channel_map(channel_map,channel_map_capacity,channel_count);
  }

  clap_host_surround MSurround = {
    clap_host_surround_changed_callback,
    clap_host_surround_get_preferred_channel_map_callback
  };

  // thread-check

  static bool clap_host_thread_check_is_main_thread_callback(const clap_host *host) {
    Host* host_ = (Host*)host->host_data;
    return host_->thread_check_is_main_thread();
  }

  static bool clap_host_thread_check_is_audio_thread_callback(const clap_host *host) {
    Host* host_ = (Host*)host->host_data;
    return host_->thread_check_is_audio_thread();
  }

  clap_host_thread_check MThreadCheck = {
    clap_host_thread_check_is_main_thread_callback,
    clap_host_thread_check_is_audio_thread_callback
  };

  // thread-pool.draft/0

  static bool clap_host_thread_pool_request_exec_callback(const clap_host *host, uint32_t num_tasks) {
    Host* host_ = (Host*)host->host_data;
    return host_->thread_pool_request_exec(num_tasks);
  }

  clap_host_thread_pool MThreadPool = {
    clap_host_thread_pool_request_exec_callback
  };

  // timer-support

  static bool clap_host_timer_support_register_timer_callback(const clap_host *host, uint32_t period_ms, clap_id *timer_id) {
    Host* host_ = (Host*)host->host_data;
    return host_->timer_support_register_timer(period_ms,timer_id);
  }

  static bool clap_host_timer_support_unregister_timer_callback(const clap_host *host, clap_id timer_id) {
    Host* host_ = (Host*)host->host_data;
    return host_->timer_support_unregister_timer(timer_id);
  }

  clap_host_timer_support MTimerSupport = {
    clap_host_timer_support_register_timer_callback,
    clap_host_timer_support_unregister_timer_callback
  };

  // track-info.draft/0

  static bool clap_host_track_info_get_callback(const clap_host *host, clap_track_info *info) {
    Host* host_ = (Host*)host->host_data;
    return host_->track_info_get(info);
  }

  clap_host_track_info MTrackInfo = {
    clap_host_track_info_get_callback
  };

  // tuning.draft/0

  static double clap_host_tuning_get_callback(const clap_host *host, int32_t key, int32_t channel) {
    Host* host_ = (Host*)host->host_data;
    return host_->tuning_get(key,channel);
  }

  clap_host_tuning MTuning = {
    clap_host_tuning_get_callback
  };


};

//----------------------------------------------------------------------
#endif
