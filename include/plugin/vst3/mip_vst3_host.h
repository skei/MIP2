#ifndef mip_vst3_host_included
#define mip_vst3_host_included
//----------------------------------------------------------------------

//#include "plugin/clap/mip_clap_host.h"
#include "plugin/clap/mip_clap_host_implementation.h"

class MIP_Vst3Host
: public MIP_ClapHostImplementation {

//------------------------------
private:
//------------------------------

  //audioMasterCallback   MAudioMaster  = nullptr;
  //AEffect*              MAEffect      = nullptr;


//------------------------------
public:
//------------------------------

  //MIP_Vst2Host(audioMasterCallback audioMaster) {
  MIP_Vst3Host()
  : MIP_ClapHostImplementation() {
    //MAudioMaster = audioMaster;
  }

  //----------

  virtual ~MIP_Vst3Host() {
  }

//------------------------------
public:
//------------------------------

  //void setAEffect(AEffect* effect) {
  //  MAEffect = effect;
  //}

//------------------------------
public: // clap plugin
//------------------------------

  void request_restart() override {}
  void request_process() override {}
  void request_callback() override {}

  //const void* get_extension(const char *extension_id) override {
    //if (strcmp(extension_id,CLAP_EXT_AMBISONIC) == 0)           return &MAmbisonic;
    //if (strcmp(extension_id,CLAP_EXT_AUDIO_PORTS) == 0)         return &MAudioPorts;
    //if (strcmp(extension_id,CLAP_EXT_AUDIO_PORTS_CONFIG) == 0)  return &MAudioPortsConfig;
    //if (strcmp(extension_id,CLAP_EXT_CHECK_FOR_UPDATE) == 0)    return &MChekForUpdate;
    //if (strcmp(extension_id,CLAP_EXT_EVENT_FILTER) == 0)        return &MEventFilter;
    //if (strcmp(extension_id,CLAP_EXT_EVENT_REGISTRY) == 0)      return &MEventRegistry;
    //if (strcmp(extension_id,CLAP_EXT_FILE_REFERENCE) == 0)      return &MFileReference;
    //if (strcmp(extension_id,CLAP_EXT_GUI) == 0)                 return &MGui;
    //if (strcmp(extension_id,CLAP_EXT_LATENCY) == 0)             return &MLatency;
    //if (strcmp(extension_id,CLAP_EXT_LOG) == 0)                 return &MLog;
    //if (strcmp(extension_id,CLAP_EXT_MIDI_MAPPINGS) == 0)       return &MMidiMappings;
    //if (strcmp(extension_id,CLAP_EXT_NOTE_NAME) == 0)           return &MNoteName;
    //if (strcmp(extension_id,CLAP_EXT_NOTE_PORTS) == 0)          return &MNotePorts;
    //if (strcmp(extension_id,CLAP_EXT_PARAMS) == 0)              return &MParams;
    //if (strcmp(extension_id,CLAP_EXT_POSIX_FD_SUPPORT) == 0)    return &MPosixFdSupport;
    //if (strcmp(extension_id,CLAP_EXT_QUICK_CONTROLS) == 0)      return &MQuickControls;
    //if (strcmp(extension_id,CLAP_EXT_STATE) == 0)               return &MState;
    //if (strcmp(extension_id,CLAP_EXT_SURROUND) == 0)            return &MSurround;
    //if (strcmp(extension_id,CLAP_EXT_THREAD_CHECK) == 0)        return &MThreadCheck;
    //if (strcmp(extension_id,CLAP_EXT_THREAD_POOL) == 0)         return &MThreadPool;
    //if (strcmp(extension_id,CLAP_EXT_TIMER_SUPPORT) == 0)       return &MTimerSupport;
    //if (strcmp(extension_id,CLAP_EXT_TRACK_INFO) == 0)          return &MTrackInfo;
    //if (strcmp(extension_id,CLAP_EXT_TUNING) == 0)              return &MTuning;
    //if (strcmp(extension_id,CLAP_EXT_TUNING) == 0)              return &MTuning;
    //return nullptr;
  //}

//------------------------------
public: // extensions
//------------------------------

//  void ambisonic_changed() override {}
//  uint32_t audio_ports_get_preferred_sample_size() override { return 0; }
//  void audio_ports_rescan(uint32_t flags) override {}
//  void audio_ports_config_rescan() override {}
//  //void check_for_update_on_new_version(const clap_check_for_update_info *update_info) override {}
//  void event_filter_changed() override {}
//  bool event_registry_query(const char* space_name, uint16_t* space_id) override { return false; }
//  void file_reference_changed() override {}
//  void file_reference_set_dirty(clap_id resource_id) override {}
//  bool gui_resize(uint32_t width, uint32_t height) override { return false; }
//  void latency_changed() override {}
//  void log_log(clap_log_severity severity, const char *msg) override {}
//  void midi_mappings_changed() override {}
//  void note_name_changed() override {}
//  uint32_t note_ports_supported_dialects() override { return 0; }
//  void note_ports_rescan(uint32_t flags) override {}
//  void params_rescan(clap_param_rescan_flags flags) override {}
//  void params_clear(clap_id param_id, clap_param_clear_flags flags) override {}
//  void params_request_flush() override {}
//  bool posix_fd_support_register_fd(int fd, int flags) override { return false; }
//  bool posix_fd_support_modify_fd(int fd, int flags) override { return false; }
//  bool posix_fd_support_unregister_fd(int fd) override { return false; }
//  //void quick_controls_changed(clap_quick_controls_changed_flags flags) override {}
//  void state_mark_dirty() override {}
//  void surround_changed() override {}
//  void surround_get_preferred_channel_map(uint8_t* channel_map, uint32_t channel_map_capacity, uint32_t* channel_count) override {}
//  bool thread_check_is_main_thread() override { return false; }
//  bool thread_check_is_audio_thread() override { return false; }
//  bool thread_pool_request_exec(uint32_t num_tasks) override { return false; }
//  bool timer_support_register_timer(uint32_t period_ms, clap_id *timer_id) override { return false; }
//  bool timer_support_unregister_timer(clap_id timer_id) override { return false; }
//  bool track_info_get(clap_track_info *info) override { return false; }
//  double tuning_get(int32_t key, int32_t channel) override { return 0.0; }

//------------------------------
public:
//------------------------------

  //void addProcessParameter(uint32_t AOffset, double AValue) {
  //  MIP_Print("adding parameter: %i = %f\n",AOffset,AValue);
  //}


};

//----------------------------------------------------------------------
#endif


