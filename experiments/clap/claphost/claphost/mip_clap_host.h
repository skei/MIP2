#ifndef mip_clap_host_included
#define mip_clap_host_included
//----------------------------------------------------------------------

// this is what the plugin sees

#include "plugin/clap/mip_clap.h"

#define HOST_NAME     "clap-test"
#define HOST_AUTHOR   "Tor-Helge Skei"
#define HOST_URL      "https://torhelgeskei.com"
#define HOST_VERSION  "0.0.1"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ClapHost {

//------------------------------
public:
//------------------------------

  MIP_ClapHost() {
  }

  //----------

  ~MIP_ClapHost() {
  }

  //----------

  clap_host* getClapHost() {
    return &MClapHost;
  }

//------------------------------------------------------------
private: // extensions
//------------------------------------------------------------

/*
  TODO: implement (fill in) empty methods...
*/

//------------------------------------------------------------

  //--------------------
  // clap_host
  //--------------------

  const void* get_extension(const char *extension_id) {
    printf("from plugin > get_extension(%s) -> NULL\n",extension_id);
    if (strcmp(extension_id,CLAP_EXT_AUDIO_PORTS) == 0)     { return NULL; } //&MClapHostAudioPorts; }
    if (strcmp(extension_id,CLAP_EXT_EVENT_FILTER) == 0)    { return NULL; } //&MClapHostEventFilter; }
    if (strcmp(extension_id,CLAP_EXT_FD_SUPPORT) == 0)      { return NULL; } //&MClapHostFdSupport; }
    if (strcmp(extension_id,CLAP_EXT_FILE_REFERENCE) == 0)  { return NULL; } //&MClapHostFileReference; }
    if (strcmp(extension_id,CLAP_EXT_GUI) == 0)             { return NULL; } //&MClapHostGui; }
    if (strcmp(extension_id,CLAP_EXT_LATENCY) == 0)         { return NULL; } //&MClapHostLatency; }
    if (strcmp(extension_id,CLAP_EXT_LOG) == 0)             { return NULL; } //&MClapHostLog; }
    if (strcmp(extension_id,CLAP_EXT_NOTE_NAME) == 0)       { return NULL; } //&MClapHostNoteName; }
    if (strcmp(extension_id,CLAP_EXT_PARAMS) == 0)          { return NULL; } //&MClapHostParams; }
    if (strcmp(extension_id,CLAP_EXT_QUICK_CONTROLS) == 0)  { return NULL; } //&MClapHostQuickControls; }
    if (strcmp(extension_id,CLAP_EXT_STATE) == 0)           { return NULL; } //&MClapHostState; }
    if (strcmp(extension_id,CLAP_EXT_THREAD_CHECK) == 0)    { return NULL; } //&MClapHostThreadCheck; }
    if (strcmp(extension_id,CLAP_EXT_THREAD_POOL) == 0)     { return NULL; } //&MClapHostThreadPool; }
    if (strcmp(extension_id,CLAP_EXT_TIMER_SUPPORT) == 0)   { return NULL; } //&MClapHostTimerSupport; }
    if (strcmp(extension_id,CLAP_EXT_TRACK_INFO) == 0)      { return NULL; } //&MClapHostTrackInfo; }
    return NULL;
  }

  //----------

  void request_restart() {
    printf("from plugin > request_restart()\n");
  }

  //----------

  void request_process() {
    printf("from plugin > request_process()\n");
  }

  //----------

  void request_callback() {
    printf("from plugin > request_callback()\n");
  }

  //--------------------
  // clap.audio-ports
  //--------------------

  uint32_t audio_ports_get_preferred_sample_size() {
    printf("from plugin > audio-ports.get_preferred_sample_size() -> 32\n");
    return 32;
  }

  void audio_ports_rescan(uint32_t flags) {
    printf("from plugin > audio-ports.rescan(%i)\n",flags);
  }

  //--------------------
  // clap.event_filter
  //--------------------

  void event_filter_changed() {
    printf("from plugin > event-filter.changed()\n");
  }

  //--------------------
  // clap.fd-support
  //--------------------

  bool fd_support_register_fd(clap_fd fd, clap_fd_flags flags) {
    printf("from plugin > fd-support.register_fd(%i)\n",flags);
    return false;
  }

  bool fd_support_modify_fd(clap_fd fd, clap_fd_flags flags) {
    printf("from plugin > fd-support.modify(%i)\n",flags);
    return false;
  }

  bool fd_support_unregister_fd(clap_fd fd) {
    printf("from plugin > fd-support.unregister_fd()\n");
    return false;
  }

  //--------------------
  // clap.file-reference.draft/0
  //--------------------

  void file_reference_changed() {
    printf("from plugin > file-reference.changed()\n");
  }

  void file_reference_set_dirty(clap_id resource_id) {
    printf("from plugin > file-reference.set_dirty(%i)\n",resource_id);
  }

  //--------------------
  // clap.gui
  //--------------------

  bool gui_resize(uint32_t width, uint32_t height) {
    printf("from plugin > gui.resize(%i,%i)\n",width,height);
    return false;
  }

  //--------------------
  // clap.latency
  //--------------------

  void latency_changed() {
    printf("from plugin > latency.changed()\n");
  }

  //--------------------
  // clap.log
  //--------------------

  void log_log(clap_log_severity severity, const char *msg) {
    printf("from plugin > log.log(%i,%s)\n",severity,msg);
  }

  //--------------------
  // clap.note-name
  //--------------------

  void note_name_changed() {
    printf("from plugin > note-name.changed()\n");
  }

  //--------------------
  // clap.params
  //--------------------

  void params_rescan(clap_param_rescan_flags flags) {
    printf("from plugin > params.rescan(%i)\n",flags);
  }

  void params_clear(clap_id param_id, clap_param_clear_flags flags) {
    printf("from plugin > params.clear(%i,%i)\n",param_id,flags);
  }

  void params_request_flush() {
    printf("from plugin > params.request_flush()\n");
  }

  //--------------------
  // clap.quick-controls.draft/0
  //--------------------

  void quick_controls_changed(clap_quick_controls_changed_flags flags) {
    printf("from plugin > quick-controls.changed(%i)\n",flags);
  }

  //--------------------
  // clap.state
  //--------------------

  void state_mark_dirty() {
    printf("from plugin > state.markDirty()\n");
  }

  //--------------------
  // clap.thread-check
  //--------------------

  bool thread_check_is_main_thread() {
    //printf("> thread-check.isMainThread() -> true\n");
    return true;
  }

  bool thread_check_is_audio_thread() {
    //printf("> thread-check.isAudioThread() -> true\n");
    return true;
  }

  //--------------------
  // clap.thread-pool.draft/0
  //--------------------

  bool thread_pool_request_exec(uint32_t num_tasks) {
    printf("from plugin > thread-pool.request_exec(%i)\n",num_tasks);
    return false;
  }

  //--------------------
  // clap.timer-support
  //--------------------

  // return true for testing (ace)

  bool timer_support_register_timer(uint32_t period_ms, clap_id *timer_id) {
    printf("from plugin > timer-support.register_timer(%i)\n",period_ms);
    *timer_id = 0;
    return true;//false;
  }

  bool timer_support_unregister_timer(clap_id timer_id) {
    printf("from plugin > timer-support.unregister_timer(%i)\n",timer_id);
    return false;
  }

  //--------------------
  // clap.track-info.draft/0
  //--------------------

  bool track_info_get(clap_track_info *info) {
    printf("from plugin > track-info.get()\n");
    return false;
  }

//------------------------------------------------------------
private: // clap host
//------------------------------------------------------------

  static const void* get_extension_callback(const struct clap_host* host, const char* extension_id) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    return claphost->get_extension(extension_id);
  }

  static void request_restart_callback(const struct clap_host* host) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    claphost->request_restart();
  }

  static void request_process_callback(const struct clap_host* host) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    claphost->request_process();
  }

  static void request_callback_callback(const struct clap_host* host) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    claphost->request_callback();
  }

  //----------

  struct clap_host MClapHost = {
    CLAP_VERSION,   // initialized to CLAP_VERSION
    this,           // reserved pointer for the host
    HOST_NAME,      // "clap-test",
    HOST_AUTHOR,    // "Tor-Helge Skei",
    HOST_URL,       // "https://torhelgeskei.com",
    HOST_VERSION,   //" 0.0.1",
    get_extension_callback,
    request_restart_callback,
    request_process_callback,
    request_callback_callback
  };

//------------------------------------------------------------
private: // extensions
//------------------------------------------------------------

  // clap.audio-ports

  static uint32_t audio_ports_get_preferred_sample_size_callback(const clap_host* host) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    return claphost->audio_ports_get_preferred_sample_size();
  }

  static void audio_ports_rescan_callback(const clap_host* host, uint32_t flags) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    claphost->audio_ports_rescan(flags);
  }

  struct clap_host_audio_ports MClapHostAudioPorts = {
    audio_ports_get_preferred_sample_size_callback,
    audio_ports_rescan_callback
  };

  // clap.event_filter

  static void event_filter_changed_callback(const clap_host* host) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    claphost->event_filter_changed();
  }

  struct clap_host_event_filter MClapHostEventFilter = {
    event_filter_changed_callback
  };

  // clap.fd-support

  static bool fd_support_register_fd_callback(const clap_host* host, clap_fd fd, clap_fd_flags flags) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    return claphost->fd_support_register_fd(fd,flags);
  }

  static bool fd_support_modify_fd_callback(const clap_host* host, clap_fd fd, clap_fd_flags flags) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    return claphost->fd_support_modify_fd(fd,flags);
  }

  static bool fd_support_unregister_fd_callback(const clap_host* host, clap_fd fd) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    return claphost->fd_support_unregister_fd(fd);
  }

  struct clap_host_fd_support MClapHostFdSupport = {
    fd_support_register_fd_callback,
    fd_support_modify_fd_callback,
    fd_support_unregister_fd_callback
  };

  // clap.file-reference.draft/0

  static void file_reference_changed_callback(const clap_host* host) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    claphost->file_reference_changed();
  }

  static void file_reference_set_dirty_callback(const clap_host* host, clap_id resource_id) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    claphost->file_reference_set_dirty(resource_id);
  }

  struct clap_host_file_reference MClapHostFileReference = {
    file_reference_changed_callback,
    file_reference_set_dirty_callback
  };

  // clap.gui

  static bool gui_resize_callback(const clap_host* host, uint32_t width, uint32_t height) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    return claphost->gui_resize(width,height);
  }

  struct clap_host_gui MClapHostGui = {
    gui_resize_callback
  };

  // clap.latency

  static void latency_changed_callback(const clap_host* host) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    claphost->latency_changed();
  }

  struct clap_host_latency MClapHostLatency = {
    latency_changed_callback
  };

  // clap.log

  static void log_log_callback(const clap_host* host, clap_log_severity severity, const char *msg) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    claphost->log_log(severity,msg);
  }

  struct clap_host_log MClapHostLog = {
    log_log_callback
  };

  // clap.note-name

  static void note_name_changed_callback(const clap_host* host) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    claphost->note_name_changed();
  }

  struct clap_host_note_name MClapHostNoteName = {
    note_name_changed_callback
  };

  // clap.params

  static void params_rescan_callback(const clap_host* host, clap_param_rescan_flags flags) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    claphost->params_rescan(flags);
  }

  static void params_clear_callback(const clap_host* host, clap_id param_id, clap_param_clear_flags flags) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    claphost->params_clear(param_id,flags);
  }

  static void params_request_flush_callback(const clap_host* host) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    claphost->params_request_flush();
  }

  struct clap_host_params MClapHostParams = {
    params_rescan_callback,
    params_clear_callback,
    params_request_flush_callback
  };

  // clap.quick-controls.draft/0

  static void quick_controls_changed_callback(const clap_host* host, clap_quick_controls_changed_flags flags) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    claphost->quick_controls_changed(flags);
  }

  struct clap_host_quick_controls MClapHostQuickControls = {
    quick_controls_changed_callback
  };

  // clap.state

  static void state_mark_dirty_callback(const clap_host* host) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    claphost->state_mark_dirty();
  }

  struct clap_host_state MClapHostState = {
    state_mark_dirty_callback
  };

  // clap.thread-check

  static bool thread_check_is_main_thread_callback(const clap_host* host) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    return claphost->thread_check_is_main_thread();
  }

  static bool thread_check_is_audio_thread_callback(const clap_host* host) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    return claphost->thread_check_is_audio_thread();
  }

  struct clap_host_thread_check MClapHostThreadCheck = {
    thread_check_is_main_thread_callback,
    thread_check_is_audio_thread_callback
  };

  // clap.thread-pool.draft/0

  static bool thread_pool_request_exec_callback(const clap_host* host, uint32_t num_tasks) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    return claphost->thread_pool_request_exec(num_tasks);
  }

  struct clap_host_thread_pool MClapHostThreadPool = {
    thread_pool_request_exec_callback
  };

  // clap.timer-support

  static bool timer_support_register_timer_callback(const clap_host* host, uint32_t period_ms, clap_id* timer_id) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    return claphost->timer_support_register_timer(period_ms,timer_id);
  }

  static bool timer_support_unregister_timer_callback(const clap_host* host, clap_id timer_id) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    return claphost->timer_support_unregister_timer(timer_id);
  }

  struct clap_host_timer_support MClapHostTimerSupport = {
    timer_support_register_timer_callback,
    timer_support_unregister_timer_callback
  };

  // clap.track-info.draft/0

  static bool track_info_get_callback(const clap_host* host, clap_track_info* info) {
    MIP_ClapHost* claphost = (MIP_ClapHost*)host->host_data;
    return claphost->track_info_get(info);
  }

  struct clap_host_track_info MClapHostTrackInfo = {
    track_info_get_callback
  };

};

//----------------------------------------------------------------------
#endif
