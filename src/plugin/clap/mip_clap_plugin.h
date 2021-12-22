#ifndef mip_clap_plugin_included
#define mip_clap_plugin_included
//----------------------------------------------------------------------

class MIP_ClapPlugin {

//------------------------------
protected:
//------------------------------

  struct MIP_ClapHostExtensions {
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
    // draft
    clap_host_check_for_update*   check_for_update    = nullptr;
    clap_host_file_reference*     file_reference      = nullptr;
    clap_host_midi_mappings*      midi_mappings       = nullptr;
    clap_host_quick_controls*     quick_controls      = nullptr;
    clap_host_surround*           surround            = nullptr;
    clap_host_track_info*         track_info          = nullptr;
    clap_host_tuning*             tuning              = nullptr;
  };

  MIP_ClapHostExtensions  host;

  const clap_host*  MClapHost = nullptr;


//------------------------------
public:
//------------------------------

  MIP_ClapPlugin(const clap_plugin_descriptor* desc, const clap_host_t* host) {
    MClapPlugin.desc = desc;
    MClapHost = host;
    init_host_extensions();
  }

  //----------

  virtual ~MIP_ClapPlugin() {
  }

  //----------

  const clap_plugin_t* getClapPlugin() {
    return &MClapPlugin;
  }

//------------------------------
public:
//------------------------------

  virtual bool                init() { return true; }
  virtual void                destroy() {}
  virtual bool                activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) { return true; }
  virtual void                deactivate() {}
  virtual bool                start_processing() { return true; }
  virtual void                stop_processing() {}
  virtual clap_process_status process(const clap_process_t *process) { return CLAP_PROCESS_SLEEP; }

  const void* get_extension(const char *id) {
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0)         return &MExtAudioPorts;
//    if (strcmp(id,CLAP_EXT_AUDIO_PORTS_CONFIG) == 0)  return &MExtAudioPortsConfig;
//    if (strcmp(id,CLAP_EXT_EVENT_FILTER) == 0)        return &MExtEventFilter;
//    if (strcmp(id,CLAP_EXT_FD_SUPPORT) == 0)          return &MExtFdSupport;
//    if (strcmp(id,CLAP_EXT_GUI) == 0)                 return &MExtGui;
//    if (strcmp(id,CLAP_EXT_GUI_X11) == 0)             return &MExtGuiX11;
//    if (strcmp(id,CLAP_EXT_LATENCY) == 0)             return &MExtLatency;
//    if (strcmp(id,CLAP_EXT_LOG) == 0)                 return &MExtLog;
//    if (strcmp(id,CLAP_EXT_NOTE_NAME) == 0)           return &MExtNoteName;
//    if (strcmp(id,CLAP_EXT_NOTE_PORTS) == 0)          return &MExtNotePorts;
//    if (strcmp(id,CLAP_EXT_PARAMS) == 0)              return &MExtParams;
//    if (strcmp(id,CLAP_EXT_STATE) == 0)               return &MExtState;
//    if (strcmp(id,CLAP_EXT_THREAD_CHECK) == 0)        return &MExtThreadCheck;
//    if (strcmp(id,CLAP_EXT_THREAD_POOL) == 0)         return &MExtThreadPool;
//    if (strcmp(id,CLAP_EXT_TIMER_SUPPORT) == 0)       return &MExtTimerSupport;
//    // draft
//    if (strcmp(id,CLAP_EXT_CHECK_FOR_UPDATE) == 0)    return &MExtCheckForUpdate;
//    if (strcmp(id,CLAP_EXT_FILE_REFERENCE) == 0)      return &MExtFileReference;
//    if (strcmp(id,CLAP_EXT_MIDI_MAPPINGS) == 0)       return &MExtMidiMappings;
//    if (strcmp(id,CLAP_EXT_PRESET_LOAD) == 0)         return &MExtPresetLoad;
//    if (strcmp(id,CLAP_EXT_QUICK_CONTROLS) == 0)      return &MExtQuickControls;
//    if (strcmp(id,CLAP_EXT_SURROUND) == 0)            return &MExtSurround;
//    if (strcmp(id,CLAP_EXT_TRACK_INFO) == 0)          return &MExtTrackInfo;
//    if (strcmp(id,CLAP_EXT_TUNING) == 0)              return &MExtTuning;
    return nullptr;
  }

  void on_main_thread() {
  }

  virtual uint32_t  audio_ports_count(bool is_input) { return 0; }
  virtual bool      audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t* info) { return false; }

//------------------------------
private:
//------------------------------

  void init_host_extensions() {
    MIP_PRINT;
    host.audio_ports        = (clap_host_audio_ports*)        MClapHost->get_extension(MClapHost,CLAP_EXT_AUDIO_PORTS);
    host.audio_ports_config = (clap_host_audio_ports_config*) MClapHost->get_extension(MClapHost,CLAP_EXT_AUDIO_PORTS_CONFIG);
    host.event_filter       = (clap_host_event_filter*)       MClapHost->get_extension(MClapHost,CLAP_EXT_EVENT_FILTER);
    host.fd_support         = (clap_host_fd_support*)         MClapHost->get_extension(MClapHost,CLAP_EXT_FD_SUPPORT);
    host.gui                = (clap_host_gui*)                MClapHost->get_extension(MClapHost,CLAP_EXT_GUI);
    host.latency            = (clap_host_latency*)            MClapHost->get_extension(MClapHost,CLAP_EXT_LATENCY);
    host.log                = (clap_host_log*)                MClapHost->get_extension(MClapHost,CLAP_EXT_LOG);
    host.note_name          = (clap_host_note_name*)          MClapHost->get_extension(MClapHost,CLAP_EXT_NOTE_NAME);
    host.note_ports         = (clap_host_note_ports*)         MClapHost->get_extension(MClapHost,CLAP_EXT_NOTE_PORTS);
    host.params             = (clap_host_params*)             MClapHost->get_extension(MClapHost,CLAP_EXT_PARAMS);
    host.state              = (clap_host_state*)              MClapHost->get_extension(MClapHost,CLAP_EXT_STATE);
    host.thread_check       = (clap_host_thread_check*)       MClapHost->get_extension(MClapHost,CLAP_EXT_THREAD_CHECK);
    host.thread_pool        = (clap_host_thread_pool*)        MClapHost->get_extension(MClapHost,CLAP_EXT_THREAD_POOL);
    host.timer_support      = (clap_host_timer_support*)      MClapHost->get_extension(MClapHost,CLAP_EXT_TIMER_SUPPORT);
    // draft
    host.check_for_update   = (clap_host_check_for_update*)   MClapHost->get_extension(MClapHost,CLAP_EXT_CHECK_FOR_UPDATE);
    host.file_reference     = (clap_host_file_reference*)     MClapHost->get_extension(MClapHost,CLAP_EXT_FILE_REFERENCE);
    host.midi_mappings      = (clap_host_midi_mappings*)      MClapHost->get_extension(MClapHost,CLAP_EXT_MIDI_MAPPINGS);
    host.quick_controls     = (clap_host_quick_controls*)     MClapHost->get_extension(MClapHost,CLAP_EXT_QUICK_CONTROLS);
    host.surround           = (clap_host_surround*)           MClapHost->get_extension(MClapHost,CLAP_EXT_SURROUND);
    host.track_info         = (clap_host_track_info*)         MClapHost->get_extension(MClapHost,CLAP_EXT_TRACK_INFO);
    host.tuning             = (clap_host_tuning*)             MClapHost->get_extension(MClapHost,CLAP_EXT_TUNING);
  }

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

  //----------

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

//------------------------------
private:
//------------------------------

  // clap.audio-ports

  static uint32_t clap_plugin_audio_ports_count_callback(const clap_plugin_t* plugin, bool is_input) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_count(is_input);
  }

  static bool clap_plugin_audio_ports_get_callback(const clap_plugin_t* plugin, uint32_t index, bool is_input, clap_audio_port_info_t* info) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_get(index,is_input,info);
  }

  //----------

  clap_plugin_audio_ports_t MExtAudioPorts = {
    clap_plugin_audio_ports_count_callback,
    clap_plugin_audio_ports_get_callback
  };

//  typedef struct clap_plugin_audio_ports_config {
//    uint32_t (*count)(const clap_plugin_t *plugin);
//    bool (*get)(const clap_plugin_t *plugin, uint32_t index, clap_audio_ports_config_t *config);
//    bool (*select)(const clap_plugin_t *plugin, clap_id config_id);
//  } clap_plugin_audio_ports_config_t;
//
//  typedef struct clap_plugin_event_filter {
//   bool (*accepts)(const clap_plugin_t *plugin, clap_event_type event_type);
//  } clap_plugin_event_filter_t;
//
//  typedef struct clap_plugin_fd_support {
//   void (*on_fd)(const clap_plugin_t *plugin, clap_fd fd, clap_fd_flags flags);
//  } clap_plugin_fd_support_t
//
//  typedef struct clap_plugin_gui {
//   bool (*create)(const clap_plugin_t *plugin);
//   void (*destroy)(const clap_plugin_t *plugin);
//   void (*set_scale)(const clap_plugin_t *plugin, double scale);
//   bool (*get_size)(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height);
//   bool (*can_resize)(const clap_plugin_t *plugin);
//   void (*round_size)(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height);
//   bool (*set_size)(const clap_plugin_t *plugin, uint32_t width, uint32_t height);
//   void (*show)(const clap_plugin_t *plugin);
//   void (*hide)(const clap_plugin_t *plugin);
//  } clap_plugin_gui_t;
//
//  typedef struct clap_plugin_gui_x11 {
//   bool (*attach)(const clap_plugin_t *plugin, const char *display_name, unsigned long window);
//  } clap_plugin_gui_x11_t;
//
//  typedef struct clap_plugin_latency {
//   uint32_t (*get)(const clap_plugin_t *plugin);
//  } clap_plugin_latency_t;
//
//  typedef struct clap_plugin_note_name {
//   uint32_t (*count)(const clap_plugin_t *plugin);
//   bool (*get)(const clap_plugin_t *plugin, uint32_t index, clap_note_name_t *note_name);
//  } clap_plugin_note_name;
//
//  typedef struct clap_plugin_note_ports {
//   uint32_t (*count)(const clap_plugin_t *plugin, bool is_input);
//   bool (*get)(const clap_plugin_t* plugin, uint32_t index, bool is_input, clap_note_port_info_t *info);
//  } clap_plugin_note_ports_t;
//
//  typedef struct clap_plugin_params {
//   uint32_t (*count)(const clap_plugin_t *plugin);
//   bool (*get_info)(const clap_plugin_t *plugin, int32_t param_index, clap_param_info_t* param_info);
//   bool (*get_value)(const clap_plugin_t *plugin, clap_id param_id, double *value);
//   bool (*value_to_text)(const clap_plugin_t *plugin, clap_id param_id, double value, char *display, uint32_t size);
//   bool (*text_to_value)(const clap_plugin_t *plugin, clap_id param_id, const char *display, double *value);
//   void (*flush)(const clap_plugin_t* plugin, const clap_event_list_t *input_parameter_changes, const clap_event_list_t *output_parameter_changes);
//  } clap_plugin_params_t;
//
//  typedef struct clap_plugin_render {
//   void (*set)(const clap_plugin_t *plugin, clap_plugin_render_mode mode);
//  } clap_plugin_render_t;
//
//  typedef struct clap_plugin_state {
//   bool (*save)(const clap_plugin_t *plugin, clap_ostream_t *stream);
//   bool (*load)(const clap_plugin_t *plugin, clap_istream_t *stream);
//  } clap_plugin_state_t;
//
//  typedef struct clap_plugin_thread_pool {
//   void (*exec)(const clap_plugin_t *plugin, uint32_t task_index);
//  } clap_plugin_thread_pool_t;
//
//  typedef struct clap_plugin_timer_support {
//   void (*on_timer)(const clap_plugin_t *plugin, clap_id timer_id);
//  } clap_plugin_timer_support_t;
//
//  //------------------------------
//  // extensions
//  //------------------------------
//
//  typedef struct clap_plugin_check_for_update {
//   void (*check)(const clap_host_t *host, bool include_beta);
//  } clap_plugin_check_for_update;
//
//  typedef struct clap_plugin_file_reference {
//   uint32_t (*count)(const clap_plugin_t *plugin);
//   bool (*get)(const clap_plugin_t *plugin, uint32_t index, clap_file_reference_t *file_reference);
//   bool (*get_hash)(const clap_plugin_t *plugin,
//                    clap_id              resource_id,
//                    clap_hash            hash,
//                    uint8_t             *digest,
//                    uint32_t             digest_size);
//   bool (*update_path)(const clap_plugin_t *plugin, clap_id resource_id, const char *path);
//   bool (*save_resources)(const clap_plugin_t *plugin);
//  } clap_plugin_file_reference_t;
//
//  typedef struct clap_plugin_midi_mappings {
//   uint32_t (*count)(const clap_plugin_t *plugin);
//   bool (*get)(const clap_plugin_t *plugin, uint32_t index, clap_midi_mapping_t *mapping);
//  } clap_plugin_midi_mappings_t;
//
//  typedef struct clap_plugin_preset_load {
//   bool (*from_file)(const clap_plugin_t *plugin, const char *path);
//  } clap_plugin_preset_load_
//
//  typedef struct clap_plugin_quick_controls {
//   uint32_t (*count)(const clap_plugin_t *plugin);
//   bool (*get)(const clap_plugin_t *plugin, uint32_t page_index, clap_quick_controls_page_t *page);
//   void (*select)(const clap_plugin_t *plugin, clap_id page_id);
//   clap_id (*get_selected)(const clap_plugin_t *plugin);
//  } clap_plugin_quick_controls_t;
//
//  typedef struct clap_plugin_surround {
//   uint32_t (*get_channel_type)(const clap_plugin_t *plugin,
//                                bool                 is_input,
//                                uint32_t             port_index,
//                                uint32_t             channel_index);
//  } clap_plugin_surround_t;
//
//  typedef struct clap_plugin_track_info {
//   void (*changed)(const clap_plugin_t *plugin);
//  } clap_plugin_track_info_t;



};

//----------------------------------------------------------------------
#endif

