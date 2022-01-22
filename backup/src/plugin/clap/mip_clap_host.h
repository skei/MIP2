#ifndef mip_clap_host_included
#define mip_clap_host_included
//----------------------------------------------------------------------

/*
  TODO / may-do?:
  - inherit from MIP_BasicClapHost
  - load dll/so via MIP_LoadLibrary
*/

//----------

#ifdef MIP_LINUX
  #include <dlfcn.h>
#else
  #include <windows.h>
#endif

//----------

#include "mip.h"
//#include "base/system/mip_time.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_hosted_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ClapHost
/*: public MIP_TimerListener*/ {


//------------------------------
private:
//------------------------------

  #ifdef MIP_WIN32
    HINSTANCE                 MLibHandle      = nullptr;
  #else
    void*                     MLibHandle      = nullptr;
  #endif

  const char*                 MPluginPath     = "";
  char                        MPathOnly[512]  = {0};
  const clap_plugin_entry*    MClapEntry      = nullptr;
  const clap_plugin_factory*  MClapFactory    = nullptr;

  //MIP_Timer*                  MTimer          = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ClapHost() {
  }

  //----------

  ~MIP_ClapHost() {
  }

//------------------------------
public:
//------------------------------

  const clap_host*            getClapHost()     { return &MClapHost; }
  const clap_plugin_entry*    getClapEntry()    { return MClapEntry; }
  const clap_plugin_factory*  getClapFactory()  { return MClapFactory; }

  void setHostData(void* data)          { MClapHost.host_data = data; }
  void setHostName(const char* txt)     { MClapHost.name      = txt; }
  void setHostVendor(const char* txt)   { MClapHost.vendor    = txt; }
  void setHostUrl(const char* txt)      { MClapHost.url       = txt; }
  void setHostVersion(const char* txt)  { MClapHost.version   = txt; }

  //----------

  bool _load_error(const char* str, bool cl=true) {
    printf("%s\n",str);
    #ifdef MIP_WIN32
      //if (cl) dlclose(MLibHandle);
    #else
      if (cl) dlclose(MLibHandle);
    #endif
    MLibHandle = nullptr;
    return false;
  }

  //----------

  bool loadPlugin(const char* path) {
    MPluginPath = path;

    #ifdef MIP_WIN32
      MLibHandle = LoadLibrary(path);
    #else
      MLibHandle = dlopen(path,RTLD_LAZY|RTLD_LOCAL); // RTLD_NOW
    #endif
    if (!MLibHandle) return _load_error("couldn't load plugin",false);

    #ifdef MIP_WIN32
      MClapEntry = (struct clap_plugin_entry*)GetProcAddress(MLibHandle,"clap_entry");
    #else
      MClapEntry = (struct clap_plugin_entry*)dlsym(MLibHandle,"clap_entry");
    #endif

    if (!MClapEntry) return _load_error("couldn't find 'clap_entry'");

    get_path_only(MPathOnly,path);
    if (!MClapEntry->init(MPathOnly)) return _load_error("entry.init error");

    MClapFactory = (const clap_plugin_factory*)MClapEntry->get_factory(CLAP_PLUGIN_FACTORY_ID);
    if (!MClapFactory) return _load_error("couldn't get plugin factory");

    return true;
  }

  //----------

  void unloadPlugin() {
    //if (MClapEntry) MClapEntry->deinit();
    #ifdef MIP_WIN32
    #else
      if (MLibHandle) dlclose(MLibHandle);
    #endif
  }

  //----------

  MIP_ClapHostedPlugin* instantiatePlugin(const char* path, uint32_t index) {
    if (!MClapFactory) return nullptr;
    if (index >= MClapFactory->get_plugin_count(MClapFactory)) return nullptr;
    const clap_plugin_descriptor* descriptor = MClapFactory->get_plugin_descriptor(MClapFactory,index);
    if (!descriptor) {
      printf("couldn't get descriptor\n");
      return nullptr;
    }
    const clap_plugin* plugin = MClapFactory->create_plugin(MClapFactory,&MClapHost,descriptor->id);
    if (!plugin) {
      printf("couldn't create plugin\n");
      return nullptr;
    }
    bool result = plugin->init(plugin);
    if (result) {
      return new MIP_ClapHostedPlugin(plugin);
    }
    else {
      printf("couldn't init plugin\n");
      //plugin->destroy(plugin);
      return nullptr;
    }
    return nullptr;
  }

  //----------

  //void destroyPlugin(const clap_plugin* plugin) {
  //  if (plugin) plugin->destroy(plugin);
  //}

  void destroyPlugin(MIP_ClapHostedPlugin* plugin) {
    const clap_plugin* clapplug = plugin->getClapPlugin();
    if (clapplug) clapplug->destroy(clapplug);
  }

//------------------------------
public:
//------------------------------

//  uint32_t getNumPlugins() {
//    return MClapFactory->get_plugin_count(MClapFactory);
//  }
//
//  const char* getPluginName(uint32_t index) {
//    const clap_plugin_descriptor_t* descriptor = MClapFactory->get_plugin_descriptor(MClapFactory,index);
//    return descriptor->name;
//  }
//
//  const char* getPluginDescription(uint32_t index) {
//    const clap_plugin_descriptor_t* descriptor = MClapFactory->get_plugin_descriptor(MClapFactory,index);
//    return descriptor->description;
//  }

//------------------------------
private:
//------------------------------

  // these are a bit hacky..

  //----------

  // /home/skei/test.so -> test.so
  // returns ptr to first character after last /

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

  //----------

  // /home/skei/test.so -> /home/skei/
  // makes a copy of dst
  // inserts a 0 after the last /

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
    return NULL;
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

  /*
    TODO:
    should we have a flag/bitfield for which extensions we support?
  */

  const void* get_extension(const char *extension_id) {
    MIP_ClapPrint("extension_id: '%s' -> ",extension_id);
    if (strcmp(extension_id, CLAP_EXT_AUDIO_PORTS         ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostAudioPorts);        return &MClapHostAudioPorts; }
    if (strcmp(extension_id, CLAP_EXT_AUDIO_PORTS_CONFIG  ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostAudioPortsConfig);  return &MClapHostAudioPortsConfig; }
    if (strcmp(extension_id, CLAP_EXT_CHECK_FOR_UPDATE    ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostChekForUpdate);     return &MClapHostChekForUpdate; }
    if (strcmp(extension_id, CLAP_EXT_EVENT_FILTER        ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostEventFilter);       return &MClapHostEventFilter; }
    if (strcmp(extension_id, CLAP_EXT_EVENT_REGISTRY      ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostEventRegistry);     return &MClapHostEventRegistry; }
    if (strcmp(extension_id, CLAP_EXT_POSIX_FD_SUPPORT    ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostPosixFdSupport);    return &MClapHostPosixFdSupport; }
    if (strcmp(extension_id, CLAP_EXT_FILE_REFERENCE      ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostFileReference);     return &MClapHostFileReference; }
    if (strcmp(extension_id, CLAP_EXT_GUI                 ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostGui);               return &MClapHostGui; }
    if (strcmp(extension_id, CLAP_EXT_LATENCY             ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostLatency);           return &MClapHostLatency; }
    if (strcmp(extension_id, CLAP_EXT_LOG                 ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostLog);               return &MClapHostLog; }
    if (strcmp(extension_id, CLAP_EXT_MIDI_MAPPINGS       ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostMidiMappings);      return &MClapHostMidiMappings; }
    if (strcmp(extension_id, CLAP_EXT_NOTE_NAME           ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostNoteName);          return &MClapHostNoteName; }
    if (strcmp(extension_id, CLAP_EXT_NOTE_PORTS          ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostNotePorts);         return &MClapHostNotePorts; }
    if (strcmp(extension_id, CLAP_EXT_PARAMS              ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostParams);            return &MClapHostParams; }
    if (strcmp(extension_id, CLAP_EXT_QUICK_CONTROLS      ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostQuickControls);     return &MClapHostQuickControls; }
    if (strcmp(extension_id, CLAP_EXT_STATE               ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostState);             return &MClapHostState; }
    if (strcmp(extension_id, CLAP_EXT_THREAD_CHECK        ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostThreadCheck);       return &MClapHostThreadCheck; }
    if (strcmp(extension_id, CLAP_EXT_THREAD_POOL         ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostThreadPool);        return &MClapHostThreadPool; }
    if (strcmp(extension_id, CLAP_EXT_TIMER_SUPPORT       ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostTimerSupport);      return &MClapHostTimerSupport; }
    if (strcmp(extension_id, CLAP_EXT_TRACK_INFO          ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostTrackInfo);         return &MClapHostTrackInfo; }
    if (strcmp(extension_id, CLAP_EXT_TUNING              ) == 0) { MIP_ClapDPrint("%p\n",&MClapHostTuning);            return &MClapHostTuning; }
    MIP_ClapDPrint("null\n");
    return nullptr;
  }

  //----------

  void request_restart() {
    MIP_CLAPPRINT;
  }

  //----------

  void request_process() {
    MIP_CLAPPRINT;
  }

  //----------

  void request_callback() {
    MIP_CLAPPRINT;
  }

//------------------------------
public: // extensions
//------------------------------

  //------------------------------
  // ambisonic
  //------------------------------

  void ambisonic_changed() {
    MIP_CLAPPRINT;
  }

  //------------------------------
  // audio-ports
  //------------------------------

  uint32_t audio_ports_get_preferred_sample_size() {
    MIP_ClapPrint("-> 32\n");
    return 32;
  }

  //----------

  void audio_ports_rescan(uint32_t flags) {
    MIP_ClapPrint("flags: %i\n",flags);
  }

  //------------------------------
  // audio-ports-config
  //------------------------------

  void audio_ports_config_rescan() {
    MIP_CLAPPRINT;
  }

  //------------------------------
  // check-for-update.draft/0
  //------------------------------

  void check_for_update_on_new_version(const clap_check_for_update_info *update_info) {
    MIP_CLAPPRINT;
  }

  //------------------------------
  // event-filter
  //------------------------------

  void event_filter_changed() {
    MIP_CLAPPRINT;
  }

  //------------------------------
  // event-registry
  //------------------------------

  bool event_registry_query(const char* space_name, uint16_t* space_id) {
    MIP_ClapPrint("space_name '%s' -> false\n",space_name);
    return false;
  }

  //------------------------------
  // file-reference.draft/0
  //------------------------------

  void file_reference_changed() {
    MIP_CLAPPRINT;
  }

  //----------

  void file_reference_set_dirty(clap_id resource_id) {
    MIP_ClapPrint("resource_id %i\n",resource_id);
  }

  //------------------------------
  // gui
  //------------------------------

  bool gui_resize(uint32_t width, uint32_t height) {
    MIP_ClapPrint("width %i height %i -> true\n",width,height);
    return true;
  }

  //------------------------------
  // latency
  //------------------------------

  void latency_changed() {
    MIP_CLAPPRINT;
  }

  //------------------------------
  // log
  //------------------------------

  void log_log(clap_log_severity severity, const char *msg) {
    MIP_ClapPrint("severity: %i msg: '%s'\n",severity,msg);
  }

  //------------------------------
  // midi-mappings.draft/0
  //------------------------------

  void midi_mappings_changed() {
    MIP_CLAPPRINT;
  }

  //------------------------------
  // note-name
  //------------------------------

  void note_name_changed() {
    MIP_CLAPPRINT;
  }

  //------------------------------
  // note-ports
  //------------------------------

  void note_ports_rescan(uint32_t flags) {
    MIP_ClapPrint("flags %i\n",flags);
  }

  //------------------------------
  // params
  //------------------------------

  void params_rescan(clap_param_rescan_flags flags) {
    MIP_ClapPrint("flags %i\n",flags);
  }

  //----------

  void params_clear(clap_id param_id, clap_param_clear_flags flags) {
    MIP_ClapPrint("param_id %i flags %i\n",param_id,flags);
  }

  //----------

  void params_request_flush() {
    MIP_CLAPPRINT;
  }

  //------------------------------
  // posix-fd-support
  //------------------------------

  bool posix_fd_support_register_fd(int fd, int flags) {
    MIP_CLAPPRINT;
    return false;
  }

  //----------

  bool posix_fd_support_modify_fd(int fd, int flags) {
    MIP_CLAPPRINT;
    return false;
  }

  //----------

  bool posix_fd_support_unregister_fd(int fd) {
    MIP_CLAPPRINT;
    return false;
  }

  //------------------------------
  // quick-controls.draft/0
  //------------------------------

  void quick_controls_changed(clap_quick_controls_changed_flags flags) {
    MIP_ClapPrint("flags %i\n",flags);
  }

  //------------------------------
  // state
  //------------------------------

  void state_mark_dirty() {
    MIP_CLAPPRINT;
  }

  //------------------------------
  // surround
  //------------------------------

  void surround_changed() {
    MIP_CLAPPRINT;
  }

  void surround_get_preferred_channel_map(uint8_t* channel_map, uint32_t channel_map_capacity, uint32_t* channel_count) {
    MIP_CLAPPRINT;
  }


  //------------------------------
  // thread-check
  //------------------------------

  bool thread_check_is_main_thread() {
    //MIP_ClapPrint("-> true\n");
    return true;
  }

  //----------

  bool thread_check_is_audio_thread() {
    //MIP_ClapPrint("-> true\n");
    return true;
  }

  //------------------------------
  // thread-pool.draft/0
  //------------------------------

  bool thread_pool_request_exec(uint32_t num_tasks) {
    MIP_ClapPrint("num_tasks %i -> false\n",num_tasks);
    return false;
  }

  //------------------------------
  // timer-support
  //------------------------------

  bool timer_support_register_timer(uint32_t period_ms, clap_id *timer_id) {
    MIP_ClapPrint("period_ms %i -> false\n",period_ms);
    //MTimer = new MIP_Timer(this);
    //MTimer->start(period_ms);
    return false;
  }

  //----------

  bool timer_support_unregister_timer(clap_id timer_id) {
    MIP_ClapPrint("timer_id %i -> false\n",timer_id);
    //MTimer->stop();
    //delete MTimer;
    //MTimer = nullptr;
    return false;
  }

  //------------------------------
  // track-info.draft/0
  //------------------------------

  bool track_info_get(clap_track_info *info) {
    MIP_ClapPrint("-> false\n");
    return false;
  }

  //------------------------------
  // tuning.draft/0
  //------------------------------

  double tuning_get(int32_t key, int32_t channel) {
    MIP_ClapPrint("-> 0.0\n");
    return 0.0;
  }

//------------------------------
private:
//------------------------------

  static const void* clap_host_get_extension_callback(const struct clap_host *host, const char *extension_id) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    return host_->get_extension(extension_id);
  }

  static void clap_host_request_restart_callback(const struct clap_host *host) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->request_restart();
  }

  static void clap_host_request_process_callback(const struct clap_host *host) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->request_process();
  }

  static void clap_host_request_callback_callback(const struct clap_host *host) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->request_callback();
  }

  //const
  clap_host MClapHost = {
    CLAP_VERSION,
    this,
    "MIP_ClapHost",
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
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    return host_->ambisonic_changed();
  }

  clap_host_ambisonic MClapHostAmbisonic = {
    clap_host_ambisonic_changed_callback
  };

  // audio-ports

  static uint32_t clap_host_audio_ports_get_preferred_sample_size_callback(const clap_host *host) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    return host_->audio_ports_get_preferred_sample_size();
  }

  static void clap_host_audio_ports_rescan_callback(const clap_host *host, uint32_t flags) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->audio_ports_rescan(flags);
  }

  clap_host_audio_ports MClapHostAudioPorts = {
    clap_host_audio_ports_get_preferred_sample_size_callback,
    clap_host_audio_ports_rescan_callback
  };

  // audio-ports-config

  static void clap_host_audio_ports_config_rescan_callback(const struct clap_host *host) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->audio_ports_config_rescan();
  }

  clap_host_audio_ports_config MClapHostAudioPortsConfig = {
   clap_host_audio_ports_config_rescan_callback
  };

  // check-for-update.draft/0

  static void clap_host_check_for_update_on_new_version_callback(const clap_host *host, const clap_check_for_update_info *update_info) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->check_for_update_on_new_version(update_info);
  }

  clap_host_check_for_update MClapHostChekForUpdate = {
    clap_host_check_for_update_on_new_version_callback
  };

  // event-filter

  static void clap_host_event_filter_changed_callback(const clap_host *host) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->event_filter_changed();
  }

  clap_host_event_filter MClapHostEventFilter = {
    clap_host_event_filter_changed_callback
  };

  // event-registry

  static bool clap_host_event_registry_query_callback(const clap_host_t *host, const char *space_name, uint16_t *space_id) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    return host_->event_registry_query(space_name,space_id);
  }

  clap_host_event_registry MClapHostEventRegistry = {
    clap_host_event_registry_query_callback
  };

  // file-reference.draft/0

  static void clap_host_file_reference_changed_callback(const clap_host *host) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->file_reference_changed();
  }

  static void clap_host_file_reference_set_dirty_callback(const clap_host *host, clap_id resource_id) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->file_reference_set_dirty(resource_id);
  }

  clap_host_file_reference MClapHostFileReference = {
    clap_host_file_reference_changed_callback,
    clap_host_file_reference_set_dirty_callback
  };

  // gui

  static bool clap_host_gui_resize_callback(const clap_host *host, uint32_t width, uint32_t height) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    return host_->gui_resize(width,height);
  }

  clap_host_gui MClapHostGui = {
    clap_host_gui_resize_callback
  };

  // latency

  static void clap_host_latency_changed_callback(const clap_host *host) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->latency_changed();
  }

  clap_host_latency MClapHostLatency = {
    clap_host_latency_changed_callback
  };

  // log

  static void clap_host_log_log_callback(const clap_host *host, clap_log_severity severity, const char *msg) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->log_log(severity,msg);
  }

  clap_host_log MClapHostLog = {
    clap_host_log_log_callback
  };

  // midi-mappings.draft/0

  static void clap_host_midi_mappings_changed_callback(const clap_host *host) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->midi_mappings_changed();
  }

  clap_host_midi_mappings MClapHostMidiMappings = {
    clap_host_midi_mappings_changed_callback
  };

  // note-name

  static void clap_host_note_name_changed_callback(const clap_host *host) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->note_name_changed();
  }

  clap_host_note_name MClapHostNoteName = {
    clap_host_note_name_changed_callback
  };

  // note-ports

  static void clap_host_note_ports_rescan_callback(const clap_host *host, uint32_t flags) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->note_ports_rescan(flags);
  }

  clap_host_note_ports MClapHostNotePorts = {
    clap_host_note_ports_rescan_callback
  };

  // params

  static void clap_host_params_rescan_callback(const clap_host *host, clap_param_rescan_flags flags) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->params_rescan(flags);
  }

  static void clap_host_params_clear_callback(const clap_host *host, clap_id param_id, clap_param_clear_flags flags) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->params_clear(param_id,flags);
  }

  static void clap_host_params_request_flush_callback(const clap_host *host) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->params_request_flush();
  }

  clap_host_params MClapHostParams = {
    clap_host_params_rescan_callback,
    clap_host_params_clear_callback,
    clap_host_params_request_flush_callback
  };

  // posix-fd-support

  static bool clap_host_posix_fd_support_register_fd_callback(const clap_host *host, int fd, int flags) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    return host_->posix_fd_support_register_fd(fd,flags);
  }

  static bool clap_host_posix_fd_support_modify_fd_callback(const clap_host *host, int fd, int flags) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    return host_->posix_fd_support_modify_fd(fd,flags);
  }

  static bool clap_host_posix_fd_support_unregister_fd_callback(const clap_host *host, int fd) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    return host_->posix_fd_support_unregister_fd(fd);
  }

  clap_host_posix_fd_support MClapHostPosixFdSupport = {
    clap_host_posix_fd_support_register_fd_callback,
    clap_host_posix_fd_support_modify_fd_callback,
    clap_host_posix_fd_support_unregister_fd_callback
  };

  // quick-controls.draft/0

  static void clap_host_quick_controls_changed_callback(const clap_host *host, clap_quick_controls_changed_flags flags) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->quick_controls_changed(flags);
  }

  clap_host_quick_controls MClapHostQuickControls = {
    clap_host_quick_controls_changed_callback
  };

  // state

  static void clap_host_state_mark_dirty_callback(const clap_host *host) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->state_mark_dirty();
  }

  clap_host_state MClapHostState = {
    clap_host_state_mark_dirty_callback
  };

  // surround

  static void clap_host_surround_changed_callback(const clap_host_t *host) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->surround_changed();
  }

  static void clap_host_surround_get_preferred_channel_map_callback(const clap_host_t *host, uint8_t* channel_map, uint32_t channel_map_capacity, uint32_t* channel_count) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    host_->surround_get_preferred_channel_map(channel_map,channel_map_capacity,channel_count);
  }

  clap_host_surround MClapHostSurround = {
    clap_host_surround_changed_callback,
    clap_host_surround_get_preferred_channel_map_callback
  };

  // thread-check

  static bool clap_host_thread_check_is_main_thread_callback(const clap_host *host) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    return host_->thread_check_is_main_thread();
  }

  static bool clap_host_thread_check_is_audio_thread_callback(const clap_host *host) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    return host_->thread_check_is_audio_thread();
  }

  clap_host_thread_check MClapHostThreadCheck = {
    clap_host_thread_check_is_main_thread_callback,
    clap_host_thread_check_is_audio_thread_callback
  };

  // thread-pool.draft/0

  static bool clap_host_thread_pool_request_exec_callback(const clap_host *host, uint32_t num_tasks) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    return host_->thread_pool_request_exec(num_tasks);
  }

  clap_host_thread_pool MClapHostThreadPool = {
    clap_host_thread_pool_request_exec_callback
  };

  // timer-support

  static bool clap_host_timer_support_register_timer_callback(const clap_host *host, uint32_t period_ms, clap_id *timer_id) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    return host_->timer_support_register_timer(period_ms,timer_id);
  }

  static bool clap_host_timer_support_unregister_timer_callback(const clap_host *host, clap_id timer_id) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    return host_->timer_support_unregister_timer(timer_id);
  }

  clap_host_timer_support MClapHostTimerSupport = {
    clap_host_timer_support_register_timer_callback,
    clap_host_timer_support_unregister_timer_callback
  };

  // track-info.draft/0

  static bool clap_host_track_info_get_callback(const clap_host *host, clap_track_info *info) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    return host_->track_info_get(info);
  }

  clap_host_track_info MClapHostTrackInfo = {
    clap_host_track_info_get_callback
  };

  // tuning.draft/0

  static double clap_host_tuning_get_callback(const clap_host *host, int32_t key, int32_t channel) {
    MIP_ClapHost* host_ = (MIP_ClapHost*)host->host_data;
    return host_->tuning_get(key,channel);
  }

  clap_host_tuning MClapHostTuning = {
    clap_host_tuning_get_callback
  };


};

//----------

#undef MIP_CLAP_VIRTUAL

//----------------------------------------------------------------------
#endif

