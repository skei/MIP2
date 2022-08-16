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

  //const clap_host_t* MHost = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ClapPlugin(const clap_plugin_descriptor_t* descriptor/*, const clap_host_t* host*/) {
    //MHost = host;
    MPlugin.desc = descriptor;
  }

  //----------

  virtual ~MIP_ClapPlugin() {
  }

  //----------

  const clap_plugin_t* getPlugin() {
    return &MPlugin;
  }

  const clap_plugin_descriptor_t* getDescriptor() {
    return MPlugin.desc;
  }

//------------------------------
public: // plugin
//------------------------------

  virtual bool                init() { return true; }
  virtual void                destroy() { }
  virtual bool                activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) { return true; }
  virtual void                deactivate() {  }
  virtual bool                start_processing() { return true; }
  virtual void                stop_processing() { }
  virtual void                reset() { }
  virtual clap_process_status process(const clap_process_t *process) { return CLAP_PROCESS_CONTINUE; }
  virtual const void*         get_extension(const char *id) { return nullptr; }
  virtual void                on_main_thread() { }

//------------------------------
public: // extensions
//------------------------------

  virtual uint32_t  audio_ports_config_count() { return 0; }
  virtual bool      audio_ports_config_get(uint32_t index, clap_audio_ports_config_t *config) { return false; }
  virtual bool      audio_ports_config_select(clap_id config_id) { return false; }
  virtual uint32_t  audio_ports_count(bool is_input) { return 0; }
  virtual bool      audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t *info) { return false; }
  virtual bool      gui_is_api_supported(const char *api, bool is_floating) { return false; }
  virtual bool      gui_get_preferred_api(const char **api, bool *is_floating) { return false; }
  virtual bool      gui_create(const char *api, bool is_floating) { return false; }
  virtual void      gui_destroy() {}
  virtual bool      gui_set_scale(double scale) { return false; }
  virtual bool      gui_get_size(uint32_t *width, uint32_t *height) { return false; }
  virtual bool      gui_can_resize() { return false; }
  virtual bool      gui_get_resize_hints(clap_gui_resize_hints_t *hints) { return false; }
  virtual bool      gui_adjust_size(uint32_t *width, uint32_t *height) { return false; }
  virtual bool      gui_set_size(uint32_t width, uint32_t height) { return false; }
  virtual bool      gui_set_parent(const clap_window_t *window) { return false; }
  virtual bool      gui_set_transient(const clap_window_t *window) { return false; }
  virtual void      gui_suggest_title(const char *title) {}
  virtual bool      gui_show() { return false; }
  virtual bool      gui_hide() { return false; }
  virtual uint32_t  latency_get() { return 0; }
  virtual uint32_t  note_names_count() { return 0; }
  virtual bool      note_names_get(uint32_t index, clap_note_name_t *note_name) { return false; }
  virtual uint32_t  note_ports_count(bool is_input) { return 0; }
  virtual bool      note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t* info) { return false; }
  virtual uint32_t  params_count() { return 0; }
  virtual bool      params_get_info(uint32_t param_index, clap_param_info_t *param_info) { return false; }
  virtual bool      params_get_value(clap_id param_id, double *value) { return false; }
  virtual bool      params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) { return false; }
  virtual bool      params_text_to_value(clap_id param_id, const char *display, double *value) { return false; }
  virtual void      params_flush(const clap_input_events_t *in, const clap_output_events_t *out) {}
  virtual void      posix_fd_support_on_fd(int fd, clap_posix_fd_flags_t flags) {}
  virtual bool      render_has_hard_realtime_requirement() { return false; }
  virtual bool      render_set(clap_plugin_render_mode mode) { return false; }
  virtual bool      state_save(const clap_ostream_t *stream) { return false; }
  virtual bool      state_load(const clap_istream_t *stream) { return false; }
  virtual uint32_t  tail_get() { return 0; }
  virtual void      thread_pool_exec(uint32_t task_index) {}
  virtual void      timer_support_on_timer(clap_id timer_id) {}

//------------------------------
public: // draft
//------------------------------

  virtual bool      ambisonic_get_info(bool is_input,  uint32_t port_index, clap_ambisonic_info_t *info) { return false; }
  virtual void      check_for_updates_check(bool include_preview) {}
  virtual bool      cv_get_channel_type(bool is_input, uint32_t port_index, uint32_t channel_index, uint32_t *channel_type) { return false; }
  virtual uint32_t  file_reference_count() { return 0; }
  virtual bool      file_reference_get(uint32_t index, clap_file_reference_t *file_reference) { return false; }
  virtual bool      file_reference_get_blake3_digest(clap_id resource_id, uint8_t *digest) { return false; }
  virtual bool      file_reference_get_file_size(clap_id resource_id, uint64_t *size) { return false; }
  virtual bool      file_reference_update_path(clap_id resource_id, const char *path) { return false; }
  virtual bool      file_reference_save_resources() { return false; }
  virtual uint32_t  midi_mappings_count() { return 0; }
  virtual bool      midi_mappings_get(uint32_t index, clap_midi_mapping_t *mapping) { return false; }
  virtual bool      preset_load_from_file(const char *path) { return false; }
  virtual uint32_t  quick_controls_count() { return 0; }
  virtual bool      quick_controls_get(uint32_t page_index, clap_quick_controls_page_t *page) { return false; }
  virtual uint32_t  surround_get_channel_map(bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) { return 0; }
  virtual void      surround_changed() {}
  virtual void      track_info_changed() {}
  virtual void      tuning_changed() {}
  virtual bool      voice_info_get(clap_voice_info_t *info) { return false; }

//--------------------------------------------------
private: // plugin
//--------------------------------------------------

  static
  bool clap_plugin_init_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->init();
  }

  static
  void clap_plugin_destroy_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->destroy();
  }

  static
  bool clap_plugin_activate_callback(const struct clap_plugin *plugin, double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->activate(sample_rate,min_frames_count,max_frames_count);
  }

  static
  void clap_plugin_deactivate_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->deactivate();
  }

  static
  bool clap_plugin_start_processing_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->start_processing();
  }

  static
  void clap_plugin_stop_processing_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->stop_processing();
  }

  static
  void clap_plugin_reset_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->reset();
  }

  static
  clap_process_status clap_plugin_process_callback(const struct clap_plugin *plugin, const clap_process_t *process) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->process(process);
  }

  static
  const void* clap_plugin_get_extension_callback(const struct clap_plugin *plugin, const char *id) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->get_extension(id);
  }

  static
  void clap_plugin_on_main_thread_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->on_main_thread();
  }

protected:

  clap_plugin_t MPlugin = {
    .desc             = nullptr,
    .plugin_data      = this,
    .init             = clap_plugin_init_callback,
    .destroy          = clap_plugin_destroy_callback,
    .activate         = clap_plugin_activate_callback,
    .deactivate       = clap_plugin_deactivate_callback,
    .start_processing = clap_plugin_start_processing_callback,
    .stop_processing  = clap_plugin_stop_processing_callback,
    .reset            = clap_plugin_reset_callback,
    .process          = clap_plugin_process_callback,
    .get_extension    = clap_plugin_get_extension_callback,
    .on_main_thread   = clap_plugin_on_main_thread_callback
  };

//------------------------------
private: // ext: audio ports config
//------------------------------

  static
  uint32_t clap_plugin_audio_ports_config_count_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_config_count();
  }

  static
  bool clap_plugin_audio_ports_config_get_callback(const clap_plugin_t *plugin, uint32_t index, clap_audio_ports_config_t *config) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_config_get(index,config);
  }

  static
  bool clap_plugin_audio_ports_config_select_callback(const clap_plugin_t *plugin, clap_id config_id) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_config_select(config_id);
  }

protected:

  const clap_plugin_audio_ports_config_t MAudioPortsConfig {
    .count  = clap_plugin_audio_ports_config_count_callback,
    .get    = clap_plugin_audio_ports_config_get_callback,
    .select = clap_plugin_audio_ports_config_select_callback
  };

//------------------------------
private: // ext: audio ports
//------------------------------

  static
  uint32_t clap_plugin_audio_ports_count_callback(const clap_plugin_t* plugin, bool is_input) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_count(is_input);
  }

  static
  bool clap_plugin_audio_ports_get_callback(const clap_plugin_t* plugin, uint32_t index, bool is_input, clap_audio_port_info_t *info) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_get(index,is_input,info);
  }

protected:

  const clap_plugin_audio_ports_t MAudioPorts {
    .count  = clap_plugin_audio_ports_count_callback,
    .get    = clap_plugin_audio_ports_get_callback
  };

//------------------------------
private: // ext: gui
//------------------------------

  static
  bool clap_plugin_gui_is_api_supported_callback(const clap_plugin_t *plugin, const char *api, bool is_floating) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_is_api_supported(api,is_floating);
  }

  static
  bool clap_plugin_gui_get_preferred_api_callback(const clap_plugin_t *plugin, const char **api, bool *is_floating) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_get_preferred_api(api,is_floating);
  }

  static
  bool clap_plugin_gui_create_callback(const clap_plugin_t *plugin, const char *api, bool is_floating) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_create(api,is_floating);
  }

  static
  void clap_plugin_gui_destroy_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->gui_destroy();
  }

  static
  bool clap_plugin_gui_set_scale_callback(const clap_plugin_t *plugin, double scale) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_set_scale(scale);
  }

  static
  bool clap_plugin_gui_get_size_callback(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_get_size(width,height);
  }

  static
  bool clap_plugin_gui_can_resize_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_can_resize();
  }

  static
  bool clap_plugin_gui_get_resize_hints_callback(const clap_plugin_t *plugin, clap_gui_resize_hints_t *hints) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_get_resize_hints(hints);
  }

  static
  bool clap_plugin_gui_adjust_size_callback(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_adjust_size(width,height);
  }

  static
  bool clap_plugin_gui_set_size_callback(const clap_plugin_t *plugin, uint32_t width, uint32_t height) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_set_size(width,height);
  }

  static
  bool clap_plugin_gui_set_parent_callback(const clap_plugin_t *plugin, const clap_window_t *window) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_set_parent(window);
  }

  static
  bool clap_plugin_gui_set_transient_callback(const clap_plugin_t *plugin, const clap_window_t *window) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_set_transient(window);
  }

  static
  void clap_plugin_gui_suggest_title_callback(const clap_plugin_t *plugin, const char *title) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->gui_suggest_title(title);
  }

  static
  bool clap_plugin_gui_show_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_show();
  }

  static
  bool clap_plugin_gui_hide_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_hide();
  }

protected:

  const clap_plugin_gui_t MGui {
    .is_api_supported   = clap_plugin_gui_is_api_supported_callback,
    .get_preferred_api  = clap_plugin_gui_get_preferred_api_callback,
    .create             = clap_plugin_gui_create_callback,
    .destroy            = clap_plugin_gui_destroy_callback,
    .set_scale          = clap_plugin_gui_set_scale_callback,
    .get_size           = clap_plugin_gui_get_size_callback,
    .can_resize         = clap_plugin_gui_can_resize_callback,
    .get_resize_hints   = clap_plugin_gui_get_resize_hints_callback,
    .adjust_size        = clap_plugin_gui_adjust_size_callback,
    .set_size           = clap_plugin_gui_set_size_callback,
    .set_parent         = clap_plugin_gui_set_parent_callback,
    .set_transient      = clap_plugin_gui_set_transient_callback,
    .suggest_title      = clap_plugin_gui_suggest_title_callback,
    .show               = clap_plugin_gui_show_callback,
    .hide               = clap_plugin_gui_hide_callback
  };

//------------------------------
private: // ext: latency
//------------------------------

  static
  uint32_t clap_plugin_latency_get_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->latency_get();
  }

protected:

  const clap_plugin_latency_t MLatency = {
    .get = clap_plugin_latency_get_callback
  };

//------------------------------
private: // ext: note names
//------------------------------

  static
  uint32_t clap_plugin_note_names_count_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->note_names_count();
  }

  static
  bool clap_plugin_note_names_get_callback(const clap_plugin_t *plugin, uint32_t index, clap_note_name_t *note_name) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->note_names_get(index,note_name);
  }

protected:

  const clap_plugin_note_name MNoteName = {
    .count  = clap_plugin_note_names_count_callback,
    .get    = clap_plugin_note_names_get_callback
  };

//------------------------------
private: // ext: note ports
//------------------------------

  static
  uint32_t clap_plugin_note_ports_count_callback(const clap_plugin_t *plugin, bool is_input) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->note_ports_count(is_input);
  }

  static
  bool clap_plugin_note_ports_get_callback(const clap_plugin_t* plugin, uint32_t index, bool is_input, clap_note_port_info_t* info) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->note_ports_get(index,is_input,info);
  }

protected:

  const clap_plugin_note_ports_t MNotePorts {
    .count  = clap_plugin_note_ports_count_callback,
    .get    = clap_plugin_note_ports_get_callback
  };



//------------------------------
private: // ext: params
//------------------------------

  static
  uint32_t clap_plugin_params_count_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->params_count();
  }

  static
  bool clap_plugin_params_get_info_callback(const clap_plugin_t *plugin, uint32_t param_index, clap_param_info_t *param_info) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->params_get_info(param_index,param_info);
  }

  static
  bool clap_plugin_params_get_value_callback(const clap_plugin_t *plugin, clap_id param_id, double *value) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->params_get_value(param_id,value);
  }

  static
  bool clap_plugin_params_value_to_text_callback( const clap_plugin_t *plugin, clap_id param_id, double value, char *display, uint32_t size) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->params_value_to_text(param_id,value,display,size);
  }

  static
  bool clap_plugin_params_text_to_value_callback(const clap_plugin_t *plugin, clap_id param_id, const char *display, double *value) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->params_text_to_value(param_id,display,value);
  }

  static
  void clap_plugin_params_flush_callback(const clap_plugin_t *plugin, const clap_input_events_t *in, const clap_output_events_t *out) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->params_flush(in,out);
  }

protected:

  const clap_plugin_params_t MParams = {
    .count          = clap_plugin_params_count_callback,
    .get_info       = clap_plugin_params_get_info_callback,
    .get_value      = clap_plugin_params_get_value_callback,
    .value_to_text  = clap_plugin_params_value_to_text_callback,
    .text_to_value  = clap_plugin_params_text_to_value_callback,
    .flush          = clap_plugin_params_flush_callback
  };

//------------------------------
private: // ext: posix fd support
//------------------------------

  static
  void clap_plugin_posix_fd_support_on_fd_callback(const clap_plugin_t *plugin, int fd, clap_posix_fd_flags_t flags) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->posix_fd_support_on_fd(fd,flags);
  }

protected:

  const clap_plugin_posix_fd_support_t MPosixFdSupport = {
    .on_fd = clap_plugin_posix_fd_support_on_fd_callback
  };

//------------------------------
private: // ext: render
//------------------------------

  static
  bool clap_plugin_render_has_hard_realtime_requirement_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->render_has_hard_realtime_requirement();
  }

  static
  bool clap_plugin_render_set_callback(const clap_plugin_t *plugin, clap_plugin_render_mode mode) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->render_set(mode);
  }

protected:

  const clap_plugin_render_t MRender = {
    .has_hard_realtime_requirement  = clap_plugin_render_has_hard_realtime_requirement_callback,
    .set                            = clap_plugin_render_set_callback
  };

//------------------------------
private: // ext: state
//------------------------------

  static
  bool clap_plugin_state_save_callback(const clap_plugin_t *plugin, const clap_ostream_t *stream) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->state_save(stream);
  }

  static
  bool clap_plugin_state_load_callback(const clap_plugin_t *plugin, const clap_istream_t *stream) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->state_load(stream);
  }

protected:

  const clap_plugin_state_t MState = {
    .save = clap_plugin_state_save_callback,
    .load = clap_plugin_state_load_callback
  };

//------------------------------
private: // ext: tail
//------------------------------

  static
  uint32_t clap_plugin_tail_get_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->tail_get();
  }

protected:

  const clap_plugin_tail_t MTail = {
    .get = clap_plugin_tail_get_callback
  };

//------------------------------
private: // ext: thread pool
//------------------------------

  static
  void clap_plugin_thread_pool_exec_callback(const clap_plugin_t *plugin, uint32_t task_index) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->thread_pool_exec(task_index);
  }

protected:

  const clap_plugin_thread_pool_t MThreadPool = {
    .exec = clap_plugin_thread_pool_exec_callback
  };

//------------------------------
private: // ext: timer support
//------------------------------

  static
  void clap_plugin_timer_support_on_timer_callback(const clap_plugin_t *plugin, clap_id timer_id) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->timer_support_on_timer(timer_id);
  }

protected:

  const clap_plugin_timer_support_t MTimerSupport = {
    .on_timer = clap_plugin_timer_support_on_timer_callback
  };

//--------------------------------------------------
//
//--------------------------------------------------

//------------------------------
private: // draft: ambisonic
//------------------------------

  static
  bool clap_plugin_ambisonic_get_info_callback(const clap_plugin_t *plugin, bool is_input,  uint32_t port_index, clap_ambisonic_info_t *info) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->ambisonic_get_info(is_input,port_index,info);
  }

protected:

  const clap_plugin_ambisonic_t MAmbisonic = {
    .get_info = clap_plugin_ambisonic_get_info_callback
  };

//------------------------------
private: // draft: check for update
//------------------------------

  static
  void clap_plugin_check_for_update_check(const clap_plugin_t *plugin, bool include_preview) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->check_for_updates_check(include_preview);
  }

protected:

  const clap_plugin_check_for_update MCheckForUpdate = {
    .check = clap_plugin_check_for_update_check
  };

//------------------------------
private: // draft: cv
//------------------------------

  static
  bool clap_plugin_cv_get_channel_type_callback(const clap_plugin_t *plugin, bool is_input, uint32_t port_index, uint32_t channel_index, uint32_t *channel_type) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->cv_get_channel_type(is_input,port_index,channel_index,channel_type);
  }

protected:

  const clap_plugin_cv_t MCv = {
    .get_channel_type = clap_plugin_cv_get_channel_type_callback
  };

//------------------------------
private: // draft: file reference
//------------------------------

  static
  uint32_t clap_plugin_file_reference_count_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->file_reference_count();
  }

  static
  bool clap_plugin_file_reference_get_callback(const clap_plugin_t *plugin, uint32_t index, clap_file_reference_t *file_reference) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->file_reference_get(index,file_reference);
  }

  static
  bool clap_plugin_file_reference_get_blake3_digest_callback(const clap_plugin_t *plugin, clap_id resource_id, uint8_t *digest) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->file_reference_get_blake3_digest(resource_id,digest);
  }

  static
  bool clap_plugin_file_reference_get_file_size_callback(const clap_plugin_t *plugin, clap_id resource_id, uint64_t *size) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->file_reference_get_file_size(resource_id,size);
  }

  static
  bool clap_plugin_file_reference_update_path_callback(const clap_plugin_t *plugin, clap_id resource_id, const char *path) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->file_reference_update_path(resource_id,path);
  }

  static
  bool clap_plugin_file_reference_save_resources_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->file_reference_save_resources();
  }

protected:

  const clap_plugin_file_reference_t MFileReference = {
    .count              = clap_plugin_file_reference_count_callback,
    .get                = clap_plugin_file_reference_get_callback,
    .get_blake3_digest  = clap_plugin_file_reference_get_blake3_digest_callback,
    .get_file_size      = clap_plugin_file_reference_get_file_size_callback,
    .update_path        = clap_plugin_file_reference_update_path_callback,
    .save_resources     = clap_plugin_file_reference_save_resources_callback
  };

//------------------------------
private: // draft: midi mappings
//------------------------------

  static
  uint32_t clap_plugin_midi_mappings_count_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->midi_mappings_count();
  }

  static
  bool clap_plugin_midi_mappings_get_callback(const clap_plugin_t *plugin, uint32_t index, clap_midi_mapping_t *mapping) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->midi_mappings_get(index,mapping);
  }

protected:

  const clap_plugin_midi_mappings_t MMidiMappings = {
    .count  = clap_plugin_midi_mappings_count_callback,
    .get    = clap_plugin_midi_mappings_get_callback
  };

//------------------------------
private: // draft: preset load
//------------------------------

  static
  bool clap_plugin_preset_load_from_file_callback(const clap_plugin_t *plugin, const char *path) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->preset_load_from_file(path);
  }

protected:

  const clap_plugin_preset_load_t MPresetLoad = {
    .from_file = clap_plugin_preset_load_from_file_callback
  };

//------------------------------
private: // draft: quick controls
//------------------------------

  static
  uint32_t clap_plugin_quick_controls_count_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->quick_controls_count();
  }

  static
  bool clap_plugin_quick_controls_get_callback(const clap_plugin_t *plugin, uint32_t page_index, clap_quick_controls_page_t *page) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->quick_controls_get(page_index,page);
  }

protected:

  const clap_plugin_quick_controls_t MQuickControls = {
    .count  = clap_plugin_quick_controls_count_callback,
    .get    = clap_plugin_quick_controls_get_callback
  };

//------------------------------
private: // draft: surround
//------------------------------

  static
  uint32_t clap_plugin_surround_get_channel_map_callback(const clap_plugin_t *plugin, bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->surround_get_channel_map(is_input,port_index,channel_map,channel_map_capacity);
  }

  static
  void clap_plugin_surround_changed_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->surround_changed();
  }

protected:

  const clap_plugin_surround_t MSurround = {
    .get_channel_map  = clap_plugin_surround_get_channel_map_callback,
    .changed          = clap_plugin_surround_changed_callback
  };

//------------------------------
private: // draft: track info
//------------------------------

  static
  void clap_plugin_track_info_changed_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->track_info_changed();
  }

protected:

  const clap_plugin_track_info_t MTrackInfo = {
    .changed = clap_plugin_track_info_changed_callback
  };

//------------------------------
private: // draft: tuning
//------------------------------

  static
  void clap_plugin_tuning_changed_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->tuning_changed();
  }

protected:

  const clap_plugin_tuning_t MTuning = {
    .changed = clap_plugin_tuning_changed_callback
  };

//------------------------------
private: // draft: voice info
//------------------------------

  static
  bool clap_plugin_voice_info_get_callback(const clap_plugin_t *plugin, clap_voice_info_t *info) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->voice_info_get(info);
  }

protected:

  const clap_plugin_voice_info_t MVoiceInfo = {
    .get = clap_plugin_voice_info_get_callback
  };

};

//----------------------------------------------------------------------
#endif
