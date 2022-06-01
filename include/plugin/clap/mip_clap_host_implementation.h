#ifndef mip_clap_host_implementation_included
#define mip_clap_host_implementation_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ClapHostImplementation {

//------------------------------
public:
//------------------------------

  MIP_ClapHostImplementation() {
  }

  //----------

  virtual ~MIP_ClapHostImplementation() {
  }

//------------------------------
public:
//------------------------------

  const clap_host_t* getHost() {
    return &MHost;
  }

//------------------------------
public:
//------------------------------

  virtual const void* get_extension(const char *extension_id) {
    CLAP_Print("id %s\n",extension_id);
//    if (MIP_IsEqual(id,CLAP_EXT_AMBISONIC))           return &MAmbisonic;
//    if (MIP_IsEqual(id,CLAP_EXT_AUDIO_PORTS))         return &MAudioPorts;
//    if (MIP_IsEqual(id,CLAP_EXT_AUDIO_PORTS_CONFIG))  return &MAudioPorts;
//    //if (MIP_IsEqual(id,CLAP_EXT_CHECK_FOR_UPDATE))    return &MCheckForUpdate;
//    if (MIP_IsEqual(id,CLAP_EXT_CV))                  return &MCv;
//    if (MIP_IsEqual(id,CLAP_EXT_FILE_REFERENCE))      return &MFileReference;
//    if (MIP_IsEqual(id,CLAP_EXT_GUI))                 return &MGui;
//    if (MIP_IsEqual(id,CLAP_EXT_LATENCY))             return &MLatency;
//    if (MIP_IsEqual(id,CLAP_EXT_MIDI_MAPPINGS))       return &MMidiMappings;
//    if (MIP_IsEqual(id,CLAP_EXT_NOTE_NAME))           return &MNoteName;
//    if (MIP_IsEqual(id,CLAP_EXT_NOTE_PORTS))          return &MNotePorts;
//    if (MIP_IsEqual(id,CLAP_EXT_PARAMS))              return &MParams;
//    if (MIP_IsEqual(id,CLAP_EXT_POSIX_FD_SUPPORT))    return &MPosixFdSupport;
//    if (MIP_IsEqual(id,CLAP_EXT_PRESET_LOAD))         return &MPresetLoad;
//    if (MIP_IsEqual(id,CLAP_EXT_QUICK_CONTROLS))      return &MQuickControls;
//    if (MIP_IsEqual(id,CLAP_EXT_RENDER))              return &MRender;
//    if (MIP_IsEqual(id,CLAP_EXT_STATE))               return &MState;
//    if (MIP_IsEqual(id,CLAP_EXT_SURROUND))            return &MSurround;
//    if (MIP_IsEqual(id,CLAP_EXT_TAIL))                return &MTail;
//    if (MIP_IsEqual(id,CLAP_EXT_THREAD_POOL))         return &MThreadPool;
//    if (MIP_IsEqual(id,CLAP_EXT_TIMER_SUPPORT))       return &MTimerSupport;
//    if (MIP_IsEqual(id,CLAP_EXT_TRACK_INFO))          return &MTrackInfo;
//    if (MIP_IsEqual(id,CLAP_EXT_TUNING))              return &MTuning;
//    if (MIP_IsEqual(id,CLAP_EXT_VOICE_INFO))          return &MVoiceInfo;
    return nullptr;
  }

  //----------

  virtual void request_restart() {
  }

  //----------

  virtual void request_process() {
  }

  //----------

  virtual void request_callback() {
  }

//------------------------------
private: // host
//------------------------------

  clap_host_t MHost = {
    .clap_version   = CLAP_VERSION,
    .host_data      = this,
    .name           = "MIP_ClapHostImplementation",
    .vendor         = "Tor-Helge Skei",
    .url            = "https://torhelgeskei.com",
    .version        = "1",
    .get_extension    = clap_host_get_extension_callback,
    .request_restart  = clap_host_request_restart_callback,
    .request_process  = clap_host_request_process_callback,
    .request_callback = clap_host_request_callback_callback
  };

  //----------

  static const void* clap_host_get_extension_callback(const struct clap_host *host, const char *extension_id) {
    MIP_ClapHostImplementation* exehost = (MIP_ClapHostImplementation*)host->host_data;
    return exehost->get_extension(extension_id);
  }

  static void clap_host_request_restart_callback(const struct clap_host *host) {
    MIP_ClapHostImplementation* exehost = (MIP_ClapHostImplementation*)host->host_data;
    return exehost->request_restart();
  }

  static void clap_host_request_process_callback(const struct clap_host *host) {
    MIP_ClapHostImplementation* exehost = (MIP_ClapHostImplementation*)host->host_data;
    return exehost->request_process();
  }

  static void clap_host_request_callback_callback(const struct clap_host *host) {
    MIP_ClapHostImplementation* exehost = (MIP_ClapHostImplementation*)host->host_data;
    return exehost->request_callback();
  }

//------------------------------
private: // extensions
//------------------------------

  //--------------------
  // audio ports config
  //--------------------

  //typedef struct clap_host_audio_ports_config {
  //   void (*rescan)(const clap_host_t *host);
  //} clap_host_audio_ports_config_t;

  //--------------------
  // audio ports
  //--------------------

  //typedef struct clap_host_audio_ports {
  //   bool (*is_rescan_flag_supported)(const clap_host_t *host, uint32_t flag);
  //   void (*rescan)(const clap_host_t *host, uint32_t flags);
  //} clap_host_audio_ports_t;

  //--------------------
  // event registry
  //--------------------

  //typedef struct clap_host_event_registry {
  //   bool (*query)(const clap_host_t *host, const char *space_name, uint16_t *space_id);
  //} clap_host_event_registry_t;

  //--------------------
  // gui
  //--------------------

  //typedef struct clap_host_gui {
  //   void (*resize_hints_changed)(const clap_host_t *host);
  //   bool (*request_resize)(const clap_host_t *host, uint32_t width, uint32_t height);
  //   bool (*request_show)(const clap_host_t *host);
  //   bool (*request_hide)(const clap_host_t *host);
  //   void (*closed)(const clap_host_t *host, bool was_destroyed);
  //} clap_host_gui_t;

  //--------------------
  // latency
  //--------------------

  //typedef struct clap_host_latency {
  //   void (*changed)(const clap_host_t *host);
  //} clap_host_latency_t;

  //--------------------
  // log
  //--------------------

  //typedef struct clap_host_log {
  //   void (*log)(const clap_host_t *host, clap_log_severity severity, const char *msg);
  //} clap_host_log_t;

  //--------------------
  // note name
  //--------------------

  //typedef struct clap_host_note_name {
  //   void (*changed)(const clap_host_t *host);
  //} clap_host_note_name_t;

  //--------------------
  // note ports
  //--------------------

  //typedef struct clap_host_note_ports {
  //   uint32_t (*supported_dialects)(const clap_host_t *host);
  //   void (*rescan)(const clap_host_t *host, uint32_t flags);
  //} clap_host_note_ports_t;

  //--------------------
  // params
  //--------------------

  //typedef struct clap_host_params {
  //   void (*rescan)(const clap_host_t *host, clap_param_rescan_flags flags);
  //   void (*clear)(const clap_host_t *host, clap_id param_id, clap_param_clear_flags flags);
  //   void (*request_flush)(const clap_host_t *host);
  //} clap_host_params_t;

  //--------------------
  // posix fd support
  //--------------------

  //typedef struct clap_host_posix_fd_support {
  //   bool (*register_fd)(const clap_host_t *host, int fd, clap_posix_fd_flags_t flags);
  //   bool (*modify_fd)(const clap_host_t *host, int fd, clap_posix_fd_flags_t flags);
  //   bool (*unregister_fd)(const clap_host_t *host, int fd);
  //} clap_host_posix_fd_support_t;

  //--------------------
  // state
  //--------------------

  //typedef struct clap_host_state {
  //   void (*mark_dirty)(const clap_host_t *host);
  //} clap_host_state_t;

  //--------------------
  // tail
  //--------------------

  //typedef struct clap_host_tail {
  //   void (*changed)(const clap_host_t *host);
  //} clap_host_tail_t;

  //--------------------
  // thread check
  //--------------------

  //typedef struct clap_host_thread_check {
  //   bool (*is_main_thread)(const clap_host_t *host);
  //   bool (*is_audio_thread)(const clap_host_t *host);
  //} clap_host_thread_check_t;

  //--------------------
  //thread pool
  //--------------------

  //typedef struct clap_host_thread_pool {
  //   bool (*request_exec)(const clap_host_t *host, uint32_t num_tasks);
  //} clap_host_thread_pool_t;

  //--------------------
  // timer support
  //--------------------

  //typedef struct clap_host_timer_support {
  //   bool (*register_timer)(const clap_host_t *host, uint32_t period_ms, clap_id *timer_id);
  //   bool (*unregister_timer)(const clap_host_t *host, clap_id timer_id);
  //} clap_host_timer_support_t;

//------------------------------
private: // draft
//------------------------------

  //--------------------
  // ambisonic
  //--------------------

  //typedef struct clap_host_ambisonic {
  //   void (*changed)(const clap_host_t *host);
  //} clap_host_ambisonic_t;

  //--------------------
  // check for update
  //--------------------

  //typedef struct clap_host_check_for_update {
  //   void (*on_new_version)(const clap_host_t *host, const clap_check_for_update_info_t *update_info);
  //} clap_host_check_for_update_t;

  //--------------------
  // cv
  //--------------------

  //typedef struct clap_host_cv {
  //   void (*changed)(const clap_host_t *host);
  //} clap_host_cv_t;

  //--------------------
  // file reference
  //--------------------

  //typedef struct clap_host_file_reference {
  //   void (*changed)(const clap_host_t *host);
  //   void (*set_dirty)(const clap_host_t *host, clap_id resource_id);
  //} clap_host_file_reference;

  //--------------------
  // midi mappings
  //--------------------

  //typedef struct clap_host_midi_mappings {
  //   void (*changed)(const clap_host_t *host);
  //} clap_host_midi_mappings_t;

  //--------------------
  // quick controls
  //--------------------

  //typedef struct clap_host_quick_controls {
  //   void (*changed)(const clap_host_t *host);
  //   void (*suggest_page)(const clap_host_t *host, clap_id page_id);
  //} clap_host_quick_controls_t;

  //--------------------
  // surround
  //--------------------

  //typedef struct clap_host_surround {
  //   void (*changed)(const clap_host_t *host);
  //   void (*get_preferred_channel_map)(const clap_host_t *host, uint8_t *channel_map, uint32_t channel_map_capacity, uint32_t *channel_count);
  //} clap_host_surround_t;

  //--------------------
  // track info
  //--------------------

  //typedef struct clap_host_track_info {
  //   bool (*get)(const clap_host_t *host, clap_track_info_t *info);
  //} clap_host_track_info_t;

  //--------------------
  // transport control
  //--------------------

  //typedef struct clap_host_transport_control {
  //   void (*request_start)(const clap_host_t *host);
  //   void (*request_stop)(const clap_host_t *host);
  //   void (*request_continue)(const clap_host_t *host);
  //   void (*request_pause)(const clap_host_t *host);
  //   void (*request_toggle_play)(const clap_host_t *host);
  //   void (*request_jump)(const clap_host_t *host, clap_beattime position);
  //   void (*request_loop_region)(const clap_host_t *host, clap_beattime start, clap_beattime duration);
  //   void (*request_toggle_loop)(const clap_host_t *host);
  //   void (*request_enable_loop)(const clap_host_t *host, bool is_enabled);
  //   void (*request_record)(const clap_host_t *host, bool is_recording);
  //   void (*request_toggle_record)(const clap_host_t *host);
  //} clap_host_transport_control_t;

  //--------------------
  // tuning
  //--------------------

  //typedef struct clap_host_tuning {
  //   double (*get_relative)(const clap_host_t *host, clap_id tuning_id, int32_t channel, int32_t key, uint32_t sample_offset);
  //   bool (*should_play)(const clap_host_t *host, clap_id tuning_id, int32_t channel, int32_t key);
  //   uint32_t (*get_tuning_count)(const clap_host_t *host);
  //   bool (*get_info)(const clap_host_t *host, uint32_t tuning_index, clap_tuning_info_t *info);
  //} clap_host_tuning_t;

  //--------------------
  // voice info
  //--------------------

  //typedef struct clap_host_voice_info {
  //   void (*changed)(const clap_host_t *host);
  //} clap_host_voice_info_t;

};

//----------------------------------------------------------------------
#endif
