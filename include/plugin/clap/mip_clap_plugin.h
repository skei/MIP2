#ifndef mip_clap_plugin_included
#define mip_clap_plugin_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ClapPlugin {

//------------------------------
protected:
//------------------------------

  const clap_plugin_descriptor_t* MDescriptor = nullptr;
  const clap_host_t*              MHost       = nullptr;
  //MIP_ClapHostProxy*            MHostProxy  = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ClapPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
    MDescriptor   = ADescriptor;
    MHost         = AHost;
    //MHostProxy  = new MIP_ClapHostProxy(AHost);
    MPlugin.desc  = ADescriptor;
  }

  //----------

  virtual ~MIP_ClapPlugin() {
    //delete MHostProxy;
  }

  //----------

  const clap_plugin_t* getPlugin() {
    return &MPlugin;
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
  virtual void on_main_thread() {}

  virtual const void* get_extension(const char *id) {
    //if (strcmp(id,CLAP_EXT_AMBISONIC) == 0)           return &MAmbisonic;
    //if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0)         return &MAudioPorts;
    //if (strcmp(id,CLAP_EXT_AUDIO_PORTS_CONFIG) == 0)  return &MAudioPortsConfig;
    //if (strcmp(id,CLAP_EXT_CHECK_FOR_UPDATE) == 0)    return &MCheckForUpdate;
    if (strcmp(id,CLAP_EXT_EVENT_FILTER) == 0)        return &MEventFilter;
    //if (strcmp(id,CLAP_EXT_FILE_REFERENCE) == 0)      return &MFileReference;
    //if (strcmp(id,CLAP_EXT_GUI) == 0)                 return &MGui;
    //if (strcmp(id,CLAP_EXT_GUI_X11) == 0)             return &MGuiX11;
    if (strcmp(id,CLAP_EXT_LATENCY) == 0)             return &MLatency;
    //if (strcmp(id,CLAP_EXT_MIDI_MAPPINGS) == 0)       return &MMidiMappings;
    //if (strcmp(id,CLAP_EXT_NOTE_NAME) == 0)           return &MNoteName;
    //if (strcmp(id,CLAP_EXT_NOTE_PORTS) == 0)          return &MNotePorts;
    if (strcmp(id,CLAP_EXT_PARAMS) == 0)              return &MParams;
    //if (strcmp(id,CLAP_EXT_POSIX_FD_SUPPORT) == 0)    return &MPosixFdSupport;
    //if (strcmp(id,CLAP_EXT_PRESET_LOAD) == 0)         return &MPresetLoad;
    //if (strcmp(id,CLAP_EXT_QUICK_CONTROLS) == 0)      return &MQuickControls;
    if (strcmp(id,CLAP_EXT_RENDER) == 0)              return &MRender;
    if (strcmp(id,CLAP_EXT_STATE) == 0)               return &MState;
    //if (strcmp(id,CLAP_EXT_SURROUND) == 0)            return &MSurround;
    //if (strcmp(id,CLAP_EXT_THREAD_POOL) == 0)         return &MThreadPool;
    //if (strcmp(id,CLAP_EXT_TIMER_SUPPORT) == 0)       return &MTimerSupport;
    //if (strcmp(id,CLAP_EXT_TRACK_INFO) == 0)          return &MTrackInfo;
    return nullptr;
  }

//------------------------------
public: // extensions
//------------------------------

  virtual uint32_t  audio_ports_count(bool is_input) { return 0; }
  virtual bool      audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t* info) { return false; }
  virtual uint32_t  audio_ports_config_count() { return 0; }
  virtual bool      audio_ports_config_get(uint32_t index, clap_audio_ports_config_t *config) { return false; }
  virtual bool      audio_ports_config_select(clap_id config_id) { return false; }
  virtual bool      event_filter_accepts(uint16_t space_id, uint16_t event_type) { return false; }
  virtual bool      gui_create() { return false; }
  virtual void      gui_destroy() {}
  virtual bool      gui_set_scale(double scale) { return false; }
  virtual bool      gui_get_size(uint32_t *width, uint32_t *height) { return false; }
  virtual bool      gui_can_resize() { return false; }
  virtual void      gui_round_size(uint32_t *width, uint32_t *height) {}
  virtual bool      gui_set_size(uint32_t width, uint32_t height) { return false; }
  virtual void      gui_show() {}
  virtual void      gui_hide() {}
  virtual bool      gui_x11_attach(const char *display_name, unsigned long window) { return false; }
  virtual uint32_t  latency_get() { return 0; }
  virtual uint32_t  note_name_count() { return 0; }
  virtual bool      note_name_get(uint32_t index, clap_note_name_t *note_name) { return false; }
  virtual uint32_t  note_ports_count(bool is_input) { return 0; }
  virtual bool      note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t *info) { return false; }
  virtual uint32_t  params_count() { return 0; }
  virtual bool      params_get_info(int32_t param_index, clap_param_info_t* param_info) { return false; }
  virtual bool      params_get_value(clap_id param_id, double *value) { return false; }
  virtual bool      params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) { return false; }
  virtual bool      params_text_to_value(clap_id param_id, const char *display, double *value) { return false; }
  virtual void      params_flush(const clap_input_events_t* in, const clap_output_events_t* out) {}
  virtual void      posix_fd_support_on_fd(int fd, int flags) {}
  virtual void      render_set(clap_plugin_render_mode mode) {}
  virtual bool      state_save(clap_ostream_t *stream) { return true; }
  virtual bool      state_load(clap_istream_t *stream) { return true; }
  virtual void      thread_pool_exec(uint32_t task_index) {}
  virtual void      timer_support_on_timer(clap_id timer_id) {}

//------------------------------
public: // drafts
//------------------------------

  virtual bool      ambisonic_get_info(bool is_input, uint32_t port_index, clap_ambisonic_info_t* info) { return false; }
  virtual void      check_for_update_check(bool include_beta) {}
  virtual uint32_t  file_reference_count() { return 0; }
  virtual bool      file_reference_get(uint32_t index, clap_file_reference_t *file_reference) { return false; }
  virtual bool      file_reference_get_hash(clap_id resource_id, clap_hash hash, uint8_t* digest, uint32_t digest_size) { return false; }
  virtual bool      file_reference_update_path(clap_id resource_id, const char *path) { return false; }
  virtual bool      file_reference_save_resources() { return false; }
  virtual uint32_t  midi_mappings_count() { return 0; }
  virtual bool      midi_mappings_get(uint32_t index, clap_midi_mapping_t *mapping) { return false; }
  virtual bool      preset_load_from_file(const char *path) { return false; }
  virtual uint32_t  quick_controls_count() { return 0; }
  virtual bool      quick_controls_get(uint32_t page_index, clap_quick_controls_page_t *page) { return false; }
  virtual void      quick_controls_select(clap_id page_id) {}
  virtual clap_id   quick_controls_get_selected() { return 0; }
  //virtual uint32_t  surround_get_channel_type(bool is_input, uint32_t port_index, uint32_t channel_index) { return 0; }
  virtual uint32_t  surround_get_channel_map(bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) { return 0; }
  virtual void      surround_changed() {}
  virtual void      track_info_changed() {}

//------------------------------
private: // callbacks
//------------------------------

  static bool clap_plugin_init_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->init();
  }

  static void clap_plugin_destroy_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->destroy();
    delete plug;
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

  //const
  clap_plugin_t MPlugin = {
    nullptr, //descriptor
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
private: // extensions
//------------------------------

  //--------------------
  // clap.audio-ports
  //--------------------

  static uint32_t clap_plugin_audio_ports_count_callback(const clap_plugin_t* plugin, bool is_input) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_count(is_input);
  }

  static bool clap_plugin_audio_ports_get_callback(const clap_plugin_t* plugin, uint32_t index, bool is_input, clap_audio_port_info_t* info) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_get(index,is_input,info);
  }

  clap_plugin_audio_ports_t MAudioPorts = {
    clap_plugin_audio_ports_count_callback,
    clap_plugin_audio_ports_get_callback
  };

  //--------------------
  // clap.audio-ports-config
  //--------------------

  static uint32_t clap_plugin_audio_ports_config_count_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_config_count();
  }

  static bool clap_plugin_audio_ports_config_get_callback(const clap_plugin_t *plugin, uint32_t index, clap_audio_ports_config_t *config) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_config_get(index,config);
  }

  static bool clap_plugin_audio_ports_config_select_callback(const clap_plugin_t *plugin, clap_id config_id) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_config_select(config_id);
  }

  clap_plugin_audio_ports_config_t MAudioPortsConfig = {
    clap_plugin_audio_ports_config_count_callback,
    clap_plugin_audio_ports_config_get_callback,
    clap_plugin_audio_ports_config_select_callback
  };

  //--------------------
  // clap.event-filter
  //--------------------

  static bool clap_plugin_event_filter_accepts_callback(const clap_plugin_t *plugin, uint16_t space_id, uint16_t event_type) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->event_filter_accepts(space_id,event_type);
  }

  clap_plugin_event_filter_t MEventFilter = {
    clap_plugin_event_filter_accepts_callback
  };

  //--------------------
  // clap.gui
  //--------------------

  static bool clap_plugin_gui_create_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_create();
  }

  static void clap_plugin_gui_destroy_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_destroy();
  }

  static bool clap_plugin_gui_set_scale_callback(const clap_plugin_t *plugin, double scale) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_set_scale(scale);
  }

  static bool clap_plugin_gui_get_size_callback(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_get_size(width,height);
  }

  static bool clap_plugin_gui_can_resize_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_can_resize();
  }

  static void clap_plugin_gui_round_size_callback(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_round_size(width,height);
  }

  static bool clap_plugin_gui_set_size_callback(const clap_plugin_t *plugin, uint32_t width, uint32_t height) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_set_size(width,height);
  }

  static void clap_plugin_gui_show_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_show();
  }

  static void clap_plugin_gui_hide_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_hide();
  }

  clap_plugin_gui_t MGui = {
    clap_plugin_gui_create_callback,
    clap_plugin_gui_destroy_callback,
    clap_plugin_gui_set_scale_callback,
    clap_plugin_gui_get_size_callback,
    clap_plugin_gui_can_resize_callback,
    clap_plugin_gui_round_size_callback,
    clap_plugin_gui_set_size_callback,
    clap_plugin_gui_show_callback,
    clap_plugin_gui_hide_callback
  };

  //--------------------
  // clap.gui-x11
  //--------------------

  static bool clap_plugin_gui_x11_attach_callback(const clap_plugin_t *plugin, const char *display_name, unsigned long window) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_x11_attach(display_name,window);
  }

  clap_plugin_gui_x11_t MGuiX11 = {
    clap_plugin_gui_x11_attach_callback
  };

  //--------------------
  // clap.latency
  //--------------------

  static uint32_t clap_plugin_latency_get_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->latency_get();
  }

  clap_plugin_latency_t MLatency = {
    clap_plugin_latency_get_callback
  };

  //--------------------
  // clap.note_name
  //--------------------

  static uint32_t clap_plugin_note_name_count_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->note_name_count();
  }

  static bool clap_plugin_note_name_get_callback(const clap_plugin_t *plugin, uint32_t index, clap_note_name_t *note_name) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->note_name_get(index,note_name);
  }

  clap_plugin_note_name MNoteName = {
    clap_plugin_note_name_count_callback,
    clap_plugin_note_name_get_callback
  };

  //--------------------
  // clap.note_ports
  //--------------------

  static uint32_t clap_plugin_note_ports_count_callback(const clap_plugin_t *plugin, bool is_input) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->note_ports_count(is_input);
  }

  static bool clap_plugin_note_ports_get_callback(const clap_plugin_t* plugin, uint32_t index, bool is_input, clap_note_port_info_t *info) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->note_ports_get(index,is_input,info);
  }

  clap_plugin_note_ports_t MNotePorts = {
    clap_plugin_note_ports_count_callback,
    clap_plugin_note_ports_get_callback
  };

  //--------------------
  // clap.params
  //--------------------

  static uint32_t clap_plugin_params_count_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->params_count();
  }

  static bool clap_plugin_params_get_info_callback(const clap_plugin_t *plugin, int32_t param_index, clap_param_info_t* param_info) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->params_get_info(param_index,param_info);
  }

  static bool clap_plugin_params_get_value_callback(const clap_plugin_t *plugin, clap_id param_id, double *value) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->params_get_value(param_id,value);
  }

  static bool clap_plugin_params_value_to_text_callback(const clap_plugin_t *plugin, clap_id param_id, double value, char *display, uint32_t size) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->params_value_to_text(param_id,value,display,size);
  }

  static bool clap_plugin_params_text_to_value_callback(const clap_plugin_t *plugin, clap_id param_id, const char *display, double *value) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->params_text_to_value(param_id,display,value);
  }

  static void clap_plugin_params_flush_callback(const clap_plugin_t* plugin, const clap_input_events_t* in, const clap_output_events_t* out) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->params_flush(in,out);
  }

  clap_plugin_params_t MParams = {
    clap_plugin_params_count_callback,
    clap_plugin_params_get_info_callback,
    clap_plugin_params_get_value_callback,
    clap_plugin_params_value_to_text_callback,
    clap_plugin_params_text_to_value_callback,
    clap_plugin_params_flush_callback
  };

  //--------------------
  // clap.posix-fd-support
  //--------------------

  static void clap_plugin_posix_fd_support_on_fd_callback(const clap_plugin_t *plugin, int fd, int flags) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->posix_fd_support_on_fd(fd,flags);
  }

  clap_plugin_posix_fd_support MPosixFdSupport = {
    clap_plugin_posix_fd_support_on_fd_callback
  };

  //--------------------
  // clap.render
  //--------------------

  static void clap_plugin_render_set_callback(const clap_plugin_t *plugin, clap_plugin_render_mode mode) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->render_set(mode);
  }

  clap_plugin_render_t MRender = {
    clap_plugin_render_set_callback
  };

  //--------------------
  // clap.state
  //--------------------

  static bool clap_plugin_state_save_callback(const clap_plugin_t *plugin, clap_ostream_t *stream) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->state_save(stream);
  }

  static bool clap_plugin_state_load_callback(const clap_plugin_t *plugin, clap_istream_t *stream) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->state_load(stream);
  }

  clap_plugin_state_t MState {
    clap_plugin_state_save_callback,
    clap_plugin_state_load_callback
  };

  //--------------------
  // clap.thread-pool
  //--------------------

  static void clap_plugin_thread_pool_exec_callback(const clap_plugin_t *plugin, uint32_t task_index) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->thread_pool_exec(task_index);
  }

  clap_plugin_thread_pool_t MThreadPool = {
    clap_plugin_thread_pool_exec_callback
  };

  //--------------------
  // clap.timer-support
  //--------------------

  static void clap_plugin_timer_support_on_timer_callback(const clap_plugin_t *plugin, clap_id timer_id) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->timer_support_on_timer(timer_id);
  }

  clap_plugin_timer_support_t MTimerSupport = {
    clap_plugin_timer_support_on_timer_callback
  };

//------------------------------
// drafts
//------------------------------

  //--------------------
  // clap.ambisonic
  //--------------------

  static bool clap_plugin_ambisonic_get_info_callback(const clap_plugin_t* plugin, bool is_input, uint32_t port_index, clap_ambisonic_info_t* info) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->ambisonic_get_info(is_input,port_index,info);
  }

  clap_plugin_ambisonic_t MAmbisonic = {
    clap_plugin_ambisonic_get_info_callback
  };

  //--------------------
  // clap.check-for-update.draft/0
  //--------------------

  //static void clap_plugin_check_for_update_check_callback(const clap_plugin_t *plugin, bool include_beta) {
  //  MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
  //  return plug->check_for_update_check(bool include_beta);
  //}

  static void clap_plugin_check_for_update_check_callback(const clap_host_t *host, bool include_beta) {
  }

  clap_plugin_check_for_update MCheckForUpdate = {
    clap_plugin_check_for_update_check_callback
  };

  //--------------------
  // clap.file-reference.draft/0
  //--------------------

  static uint32_t clap_plugin_file_reference_count_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->file_reference_count();
  }

  static bool clap_plugin_file_reference_get_callback(const clap_plugin_t *plugin, uint32_t index, clap_file_reference_t *file_reference) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->file_reference_get(index,file_reference);
  }

  static bool clap_plugin_file_reference_get_hash_callback(const clap_plugin_t *plugin, clap_id resource_id, clap_hash hash, uint8_t* digest, uint32_t digest_size) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->file_reference_get_hash(resource_id,hash,digest,digest_size);
  }

  static bool clap_plugin_file_reference_update_path_callback(const clap_plugin_t *plugin, clap_id resource_id, const char *path) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->file_reference_update_path(resource_id,path);
  }

  static bool clap_plugin_file_reference_save_resources_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->file_reference_save_resources();
  }

  clap_plugin_file_reference_t MFileReference = {
    clap_plugin_file_reference_count_callback,
    clap_plugin_file_reference_get_callback,
    clap_plugin_file_reference_get_hash_callback,
    clap_plugin_file_reference_update_path_callback,
    clap_plugin_file_reference_save_resources_callback
  };

  //--------------------
  // clap.midi-mappings.draft/0
  //--------------------

  static uint32_t clap_plugin_midi_mappings_count_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->midi_mappings_count();
  }
  static bool clap_plugin_midi_mappings_get_callback(const clap_plugin_t *plugin, uint32_t index, clap_midi_mapping_t *mapping) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->midi_mappings_get(index,mapping);
  }

  clap_plugin_midi_mappings_t MMidiMappings = {
    clap_plugin_midi_mappings_count_callback,
    clap_plugin_midi_mappings_get_callback
  };

  //--------------------
  // clap.preset-load.draft/0
  //--------------------

  static bool clap_plugin_preset_load_from_file_callback(const clap_plugin_t *plugin, const char *path) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->preset_load_from_file(path);
  }

  clap_plugin_preset_load_t MPresetLoad = {
    clap_plugin_preset_load_from_file_callback
  };

  //--------------------
  // clap.quick-controls.draft/0
  //--------------------

  static uint32_t clap_plugin_quick_controls_count_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->quick_controls_count();
  }

  static bool clap_plugin_quick_controls_get_callback(const clap_plugin_t *plugin, uint32_t page_index, clap_quick_controls_page_t *page) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->quick_controls_get(page_index,page);
  }

  static void clap_plugin_quick_controls_select_callback(const clap_plugin_t *plugin, clap_id page_id) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->quick_controls_select(page_id);
  }

  static clap_id clap_plugin_quick_controls_get_selected_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->quick_controls_get_selected();
  }

  clap_plugin_quick_controls_t MQuickControls = {
    clap_plugin_quick_controls_count_callback,
    clap_plugin_quick_controls_get_callback,
    clap_plugin_quick_controls_select_callback,
    clap_plugin_quick_controls_get_selected_callback
  };

  //--------------------
  // clap.surround.draft/0
  //--------------------

  static uint32_t clap_plugin_surround_get_channel_map_callback(const clap_plugin_t *plugin, bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->surround_get_channel_map(is_input,port_index,channel_map,channel_map_capacity);
  }

  static void clap_plugin_surround_changed_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->surround_changed();
  }

  clap_plugin_surround_t MSurround = {
    clap_plugin_surround_get_channel_map_callback,
    clap_plugin_surround_changed_callback
  };

  //--------------------
  // clap.track-info.draft/0
  //--------------------

  static void clap_plugin_track_info_changed_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->track_info_changed();
  }

  clap_plugin_track_info_t MTrackInfo = {
    clap_plugin_track_info_changed_callback
  };

};

//----------------------------------------------------------------------
#endif

