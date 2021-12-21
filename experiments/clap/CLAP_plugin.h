#ifndef CLAP_plugin_included
#define CLAP_plugin_included
//----------------------------------------------------------------------

//// nc -U -l -k /tmp/mip.socket
////#define MIP_DEBUG_PRINT_SOCKET
//#define MIP_NO_PLUGIN
//#define MIP_NO_GUI
//#include "mip.h"

#include "extern/clap/all.h"
#include "extern/clap/ext/draft/check-for-update.h"
#include "extern/clap/ext/draft/midi-mappings.h"

#include "editor.h"

//----------------------------------------------------------------------

class CLAP_Plugin {

//------------------------------
protected:
//------------------------------

  const clap_host*              MHost                 = nullptr;
  clap_plugin*                  MPlugin               = nullptr;
  bool                          MAllocatedPlugin      = false;
  Editor*                       MEditor               = nullptr;

  // extensions

  clap_host_audio_ports*        MHostAudioPorts       = nullptr;
  clap_host_audio_ports_config* MHostAudioPortsConfig = nullptr;
//clap_host_check_for_update*   MHostCheckForUpdate   = nullptr;  // draft
  clap_host_event_filter*       MHostEventFilter      = nullptr;
  clap_host_fd_support*         MHostFdSupport        = nullptr;
  clap_host_file_reference*     MHostFileReference    = nullptr;  // draft
  clap_host_gui*                MHostGui              = nullptr;
  clap_host_latency*            MHostLatency          = nullptr;
  clap_host_log*                MHostLog              = nullptr;
  clap_host_midi_mappings*      MHostMidiMappings     = nullptr;  // draft
  clap_host_note_name*          MHostNoteName         = nullptr;
  clap_host_note_ports*         MHostNotePorts        = nullptr;  //
  clap_host_params*             MHostParams           = nullptr;
  clap_host_quick_controls*     MHostQuickControls    = nullptr;  // draft
  clap_host_state*              MHostState            = nullptr;
  clap_host_thread_check*       MHostThreadCheck      = nullptr;
  clap_host_thread_pool*        MHostThreadPool       = nullptr;  // draft
  clap_host_timer_support*      MHostTimerSupport     = nullptr;
  clap_host_track_info*         MHostTrackInfo        = nullptr;  // draft
  clap_host_tuning*             MHostTuning           = nullptr;  // draft

//------------------------------
protected:
//------------------------------

  float     MParameterValue     = 0.0;
  uint32_t  MSelectedPortConfig = 0;;

//------------------------------
public:
//------------------------------

  //CLAP_Plugin(const clap_plugin* plugin) {
  //  MPlugin = (clap_plugin*)plugin;
  //  MAllocatedPlugin = false;
  //}

  //----------

  CLAP_Plugin(const clap_plugin_descriptor* desc, const clap_host *host) {

    MHost = host;
    MPlugin           = (clap_plugin*)malloc(sizeof(clap_plugin));
    MAllocatedPlugin  = true;

    MPlugin->desc              = desc;
    MPlugin->plugin_data       = this;
    MPlugin->init              = clap_plugin_init_callback;
    MPlugin->destroy           = clap_plugin_destroy_callback;
    MPlugin->activate          = clap_plugin_activate_callback;
    MPlugin->deactivate        = clap_plugin_deactivate_callback;
    MPlugin->start_processing  = clap_plugin_start_processing_callback;
    MPlugin->stop_processing   = clap_plugin_stop_processing_callback;
    MPlugin->process           = clap_plugin_process_callback;
    MPlugin->get_extension     = clap_plugin_get_extension_callback;
    MPlugin->on_main_thread    = clap_plugin_on_main_thread_callback;

    // extensions

    MHostAudioPorts       = (clap_host_audio_ports*)host->get_extension(host,CLAP_EXT_AUDIO_PORTS);
    MHostAudioPortsConfig = (clap_host_audio_ports_config*)host->get_extension(host,CLAP_EXT_AUDIO_PORTS_CONFIG);
  //MHostCheckForUpdate   = (clap_host_check_for_update*)host->get_extension(host,CLAP_EXT_CHECK_FOR_UPDATE);
    MHostEventFilter      = (clap_host_event_filter*)host->get_extension(host,CLAP_EXT_EVENT_FILTER);
    MHostFdSupport        = (clap_host_fd_support*)host->get_extension(host,CLAP_EXT_FD_SUPPORT);
    MHostFileReference    = (clap_host_file_reference*)host->get_extension(host,CLAP_EXT_FILE_REFERENCE);
    MHostGui              = (clap_host_gui*)host->get_extension(host,CLAP_EXT_GUI);
    MHostLatency          = (clap_host_latency*)host->get_extension(host,CLAP_EXT_LATENCY);
    MHostLog              = (clap_host_log*)host->get_extension(host,CLAP_EXT_LOG);
    MHostMidiMappings     = (clap_host_midi_mappings*)host->get_extension(host,CLAP_EXT_MIDI_MAPPINGS);
    MHostNoteName         = (clap_host_note_name*)host->get_extension(host,CLAP_EXT_NOTE_NAME);
    MHostNotePorts        = (clap_host_note_ports*)host->get_extension(host,CLAP_EXT_NOTE_PORTS);
    MHostParams           = (clap_host_params*)host->get_extension(host,CLAP_EXT_PARAMS);
    MHostQuickControls    = (clap_host_quick_controls*)host->get_extension(host,CLAP_EXT_QUICK_CONTROLS);
    MHostState            = (clap_host_state*)host->get_extension(host,CLAP_EXT_STATE);
    MHostThreadCheck      = (clap_host_thread_check*)host->get_extension(host,CLAP_EXT_THREAD_CHECK);
    MHostThreadPool       = (clap_host_thread_pool*)host->get_extension(host,CLAP_EXT_THREAD_POOL);
    MHostTimerSupport     = (clap_host_timer_support*)host->get_extension(host,CLAP_EXT_TIMER_SUPPORT);
    MHostTrackInfo        = (clap_host_track_info*)host->get_extension(host,CLAP_EXT_TRACK_INFO);
    MHostTuning           = (clap_host_tuning*)host->get_extension(host,CLAP_EXT_TUNING);
  }

  //----------

  virtual ~CLAP_Plugin() {
    if (MAllocatedPlugin && MPlugin) free(MPlugin);
  }

//------------------------------
public:
//------------------------------

  const clap_plugin* getClapPlugin() { return MPlugin; }

//------------------------------
public: // plugin
//------------------------------

  virtual bool init() { return true; }
  virtual void destroy() {}
  virtual bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) { return false; }
  virtual void deactivate() {}
  virtual bool start_processing() { return false; }
  virtual void stop_processing() {}
  virtual clap_process_status process(const clap_process *process) { return 0; }
  virtual const void* get_extension(const char *id) { return nullptr; }
  virtual void on_main_thread() {}

//------------------------------
public: // extensions
//------------------------------

  // audio ports
  virtual uint32_t audio_ports_count(bool is_input) { return 0; }
  virtual bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info *info) { return false; }

  // audio ports config
  virtual uint32_t audio_ports_config_count() { return 0; }
  virtual bool audio_ports_config_get(uint32_t index, clap_audio_ports_config *config) { return false; }
  virtual bool audio_ports_config_select(clap_id config_id) { return false; }

  // check for update
  virtual void check_for_update_check(bool include_beta) {}

  // event filter
  virtual bool event_filter_accepts(clap_event_type event_type) { return false; }

  // fd support
  virtual void fd_support_on_fd(clap_fd fd, clap_fd_flags flags) {}

  // file reference
  virtual uint32_t file_reference_count() { return 0; }
  virtual bool file_reference_get(uint32_t index, clap_file_reference *file_reference) { return false; }
  virtual bool file_reference_get_hash(clap_id resource_id, clap_hash hash, uint8_t* digest, uint32_t digest_size) { return false; }
  virtual bool file_reference_update_path(clap_id resource_id, const char *path) { return false; }
  virtual bool file_reference_save_resources() { return false; }

  // gui
  virtual bool gui_create() { return true; }
  virtual void gui_destroy() {}
  virtual void gui_set_scale(double scale) {}
  virtual bool gui_get_size(uint32_t* width, uint32_t* height) { return false; }
  virtual bool gui_can_resize() { return false; }
  virtual void gui_round_size(uint32_t* width, uint32_t* height) {}
  virtual bool gui_set_size(uint32_t width, uint32_t height) { return false; }
  virtual void gui_show() {}
  virtual void gui_hide() {}

  // gui x11
  virtual bool gui_x11_attach(const char* display_name, unsigned long window) { return false; }

  // latency
  virtual uint32_t latency_get() { return 0; }

  // midi mappings
  virtual uint32_t midi_mappings_count() { return 0; }
  virtual bool midi_mappings_get(uint32_t index, clap_midi_mapping* mapping) { return false; }

  // note name
  virtual uint32_t note_name_count() { return 0; }
  virtual bool note_name_get(uint32_t index, clap_note_name* note_name) { return false; }

  // note-ports
  virtual uint32_t note_ports_count(bool is_input) { return 0; }
  virtual bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info* info) { return false; }

  // params
  virtual uint32_t params_count() { return 1; }
  virtual bool params_get_info(int32_t param_index, clap_param_info *param_info) { return false; }
  virtual bool params_get_value(clap_id param_id, double* value) { return false; }
  virtual bool params_value_to_text(clap_id param_id, double value, char* display, uint32_t size) { return false; }
  virtual bool params_text_to_value(clap_id param_id, const char* display, double* value) { return false; }
  virtual void params_flush(const clap_event_list *input_parameter_changes, const clap_event_list *output_parameter_changes) {}

  // preset load
  virtual bool preset_load_from_file(const char *path) { return false; }

  // quick controls
  virtual uint32_t quick_controls_count() { return 0; }
  virtual bool quick_controls_get(uint32_t page_index, clap_quick_controls_page *page) { return false; }
  virtual void quick_controls_select(clap_id page_id) {}
  virtual clap_id quick_controls_get_selected() { return 0; }

  // render
  virtual void render_set(clap_plugin_render_mode mode) {}

  // state
  virtual bool state_save(clap_ostream *stream) { return true; }
  virtual bool state_load(clap_istream *stream) { return true; }

  // thread pool
  virtual void thread_pool_exec(uint32_t task_index) {}

  // timer support
  virtual void timer_support_on_timer(clap_id timer_id) {}

  // track info
  virtual void track_info_changed() {}

  // vst2 convert
  virtual int32_t vst2_convert_get_vst2_plugin_id(uint32_t* vst2_plugin_id, char* name, uint32_t name_size) { return 0; }
  virtual bool vst2_convert_restore_vst2_state(clap_istream *stream) { return false; }
  virtual bool vst2_convert_convert_normalized_value(uint32_t vst2_param_id, double vst2_normalized_value, clap_id* clap_param_id, double* clap_plain_value) { return false; }
  virtual bool vst2_convert_convert_plain_value(uint32_t vst2_param_id, double vst2_plain_value, clap_id* clap_param_id, double* clap_plain_value) { return false; }

  // vst3 convert
  virtual void vst3_convert_get_vst3_plugin_id(const clap_plugin *plugin, uint8_t *vst3_plugin_uuid) {}
  virtual bool vst3_convert_restore_vst3_state(const clap_plugin *plugin, clap_istream *stream) { return false; }
  virtual bool vst3_convert_convert_normalized_value(const clap_plugin *plugin, uint32_t vst3_param_id, double vst3_normalized_value, clap_id* clap_param_id, double* clap_plain_value) { return false; }
  virtual bool vst3_convert_convert_plain_value(const clap_plugin *plugin, uint32_t vst3_param_id, double vst3_plain_value, clap_id* clap_param_id, double* clap_plain_value) { return false; }

//------------------------------
private: // callbacks
//------------------------------

  static bool clap_plugin_init_callback(const struct clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->init();
  }

  static void clap_plugin_destroy_callback(const struct clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->destroy();
  }

  static bool clap_plugin_activate_callback(const struct clap_plugin *plugin, double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->activate(sample_rate,min_frames_count,max_frames_count);
  }

  static void clap_plugin_deactivate_callback(const struct clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->deactivate();
  }

  static bool clap_plugin_start_processing_callback(const struct clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->start_processing();
  }

  static void clap_plugin_stop_processing_callback(const struct clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->stop_processing();
  }

  static clap_process_status clap_plugin_process_callback(const struct clap_plugin *plugin, const clap_process *process) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->process(process);
  }

  static const void* clap_plugin_get_extension_callback(const struct clap_plugin *plugin, const char *id) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->get_extension(id);
  }

  static void clap_plugin_on_main_thread_callback(const struct clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->on_main_thread();
  }

//------------------------------
protected: // extension callbacks
//------------------------------

  // clap.audio-ports-config

  struct clap_plugin_audio_ports_config MClapAudioPortsConfig = {
    clap_audio_ports_config_count_callback,
    clap_audio_ports_config_get_callback,
    clap_audio_ports_config_select_callback
  };

  static uint32_t clap_audio_ports_config_count_callback(const clap_plugin* plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->audio_ports_config_count();
  }

  static bool clap_audio_ports_config_get_callback(const clap_plugin* plugin, uint32_t index, clap_audio_ports_config *config) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->audio_ports_config_get(index,config);
  }

  static bool clap_audio_ports_config_select_callback(const clap_plugin* plugin, clap_id config_id) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->audio_ports_config_select(config_id);
  }

  // clap.audio-ports

  struct clap_plugin_audio_ports MClapAudioPorts =  {
    clap_audio_ports_count_callback,
    clap_audio_ports_get_callback
  };

  static uint32_t clap_audio_ports_count_callback(const clap_plugin *plugin, bool is_input) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->audio_ports_count(is_input);

  }

  static bool clap_audio_ports_get_callback(const clap_plugin* plugin, uint32_t index, bool is_input, clap_audio_port_info *info) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->audio_ports_get(index,is_input,info);
  };

  // clap.check-for-updates

  //struct clap_plugin_check_for_update MClapCheckForUpdate = {
  //  clap_check_for_update_check_callback
  //};
  //
  //static void clap_check_for_update_check_callback(const clap_host *host, bool include_beta) {
  //  CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
  //}

  // clap.event-filter

  struct clap_plugin_event_filter MClapEventFilter = {
    clap_event_filter_accepts_callback
  };

  static bool clap_event_filter_accepts_callback(const clap_plugin *plugin, clap_event_type event_type) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->event_filter_accepts(event_type);
  }

  // clap.fd-support

  struct clap_plugin_fd_support MClapFdSupport = {
    clap_fd_support_on_fd_callback
  };

  static void clap_fd_support_on_fd_callback(const clap_plugin *plugin, clap_fd fd, clap_fd_flags flags) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->fd_support_on_fd(fd,flags);
  }

  // clap.file-reference

  struct clap_plugin_file_reference MClapFileReference = {
    clap_file_reference_count_callback,
    clap_file_reference_get_callback,
    clap_file_reference_get_hash_callback,
    clap_file_reference_update_path_callback,
    clap_file_reference_save_resources_callback
  };

  static uint32_t clap_file_reference_count_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->file_reference_count();
  }

  static bool clap_file_reference_get_callback(const clap_plugin *plugin, uint32_t index, clap_file_reference *file_reference) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->file_reference_get(index,file_reference);
  }

  static bool clap_file_reference_get_hash_callback(const clap_plugin *plugin, clap_id resource_id, clap_hash hash, uint8_t* digest, uint32_t digest_size) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->file_reference_get_hash(resource_id,hash,digest,digest_size);
  }

  static bool clap_file_reference_update_path_callback(const clap_plugin *plugin,  clap_id resource_id, const char *path) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->file_reference_update_path(resource_id,path);
  }

  static bool clap_file_reference_save_resources_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->file_reference_save_resources();
  }

  // clap.gui

  struct clap_plugin_gui MClapGui = {
    clap_gui_create_callback,
    clap_gui_destroy_callback,
    clap_gui_set_scale_callback,
    clap_gui_get_size_callback,
    clap_gui_can_resize_callback,
    clap_gui_round_size_callback,
    clap_gui_set_size_callback,
    clap_gui_show_callback,
    clap_gui_hide_callback
  };

  static bool clap_gui_create_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->gui_create();
  }

  static void clap_gui_destroy_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->gui_destroy();
  }

  static void clap_gui_set_scale_callback(const clap_plugin *plugin, double scale) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->gui_set_scale(scale);
  }

  static bool clap_gui_get_size_callback(const clap_plugin *plugin, uint32_t *width, uint32_t *height) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->gui_get_size(width,height);
  }

  static bool clap_gui_can_resize_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->gui_can_resize();
  }

  static void clap_gui_round_size_callback(const clap_plugin *plugin, uint32_t *width, uint32_t *height) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->gui_round_size(width,height);
  }

  static bool clap_gui_set_size_callback(const clap_plugin *plugin, uint32_t width, uint32_t height) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->gui_set_size(width,height);
  }

  static void clap_gui_show_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->gui_show();
  }

  static void clap_gui_hide_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->gui_hide();
  }

  // clap.gui-x11

  struct clap_plugin_gui_x11 MClapGuiX11 = {
    clap_gui_x11_attach_callback
  };

  static bool clap_gui_x11_attach_callback(const clap_plugin * plugin, const char* display_name, unsigned long window) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->gui_x11_attach(display_name,window);
  }

  // clap.latency

  struct clap_plugin_latency MClapLatency = {
    clap_latency_get_callback
  };

  static uint32_t clap_latency_get_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->latency_get();
  }

  // clap.midi-mappings

  struct clap_plugin_midi_mappings MClapMidiMappings = {
    clap_midi_mappings_count_callback,
    clap_midi_mappings_get_callback
  };

  static uint32_t clap_midi_mappings_count_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->midi_mappings_count();
  }

  static bool clap_midi_mappings_get_callback(const clap_plugin *plugin, uint32_t index, clap_midi_mapping *mapping) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->midi_mappings_get(index,mapping);
  }

  // clap.note-name

  struct clap_plugin_note_name MClapNoteName = {
    clap_note_name_count_callback,
    clap_note_name_get_callback
  };

  static uint32_t clap_note_name_count_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->note_name_count();
  }

  static bool clap_note_name_get_callback(const clap_plugin *plugin, uint32_t index, clap_note_name *note_name) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->note_name_get(index,note_name);
  }

  // clap.note-ports

  struct clap_plugin_note_ports MClapNotePorts = {
    clap_note_ports_count_callback,
    clap_note_ports_get_callback
  };

  static uint32_t clap_note_ports_count_callback(const clap_plugin *plugin, bool is_input) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->note_ports_count(is_input);
  }

  static bool clap_note_ports_get_callback(const clap_plugin* plugin, uint32_t index, bool is_input, clap_note_port_info *info) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->note_ports_get(index,is_input,info);
  }

  // clap.params

  struct clap_plugin_params MClapParams = {
    clap_params_count_callback,
    clap_params_get_info_callback,
    clap_params_get_value_callback,
    clap_params_value_to_text_callback,
    clap_params_text_to_value_callback,
    clap_params_flush_callback
  };

  static uint32_t clap_params_count_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->params_count();
  }

  static bool clap_params_get_info_callback(const clap_plugin *plugin, int32_t param_index, clap_param_info *param_info) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->params_get_info(param_index,param_info);
  }

  static bool clap_params_get_value_callback(const clap_plugin *plugin, clap_id param_id, double *value) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->params_get_value(param_id,value);
  }

  static bool clap_params_value_to_text_callback(const clap_plugin *plugin, clap_id param_id, double value, char *display, uint32_t size) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->params_value_to_text(param_id,value,display,size);
  }

  static bool clap_params_text_to_value_callback(const clap_plugin *plugin, clap_id param_id, const char* display, double* value) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->params_text_to_value(param_id,display,value);
  }

  static void clap_params_flush_callback(const clap_plugin* plugin, const clap_event_list *input_parameter_changes, const clap_event_list *output_parameter_changes) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->params_flush(input_parameter_changes,output_parameter_changes);
  }

  // clap.preset-load

  struct clap_plugin_preset_load MClapPresetLoad = {
    clap_preset_load_from_file_callback
  };

  static bool clap_preset_load_from_file_callback(const clap_plugin *plugin, const char *path) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->preset_load_from_file(path);
  }

  // clap.quick-controls

  struct clap_plugin_quick_controls MClapQuickControls = {
    clap_quick_controls_count_callback,
    clap_quick_controls_get_callback,
    clap_quick_controls_select_callback,
    clap_quick_controls_get_selected_callback
  };

  static uint32_t clap_quick_controls_count_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->quick_controls_count();
  }

  static bool clap_quick_controls_get_callback(const clap_plugin *plugin, uint32_t page_index, clap_quick_controls_page *page) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->quick_controls_get(page_index,page);
  }

  static void clap_quick_controls_select_callback(const clap_plugin *plugin, clap_id page_id) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->quick_controls_select(page_id);
  }

  static clap_id clap_quick_controls_get_selected_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->quick_controls_get_selected();
  }

  // clap.render

  struct clap_plugin_render MClapRender = {
    clap_render_set_callback
  };

  static void clap_render_set_callback(const clap_plugin *plugin, clap_plugin_render_mode mode) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->render_set(mode);
  }

  // clap.state

  struct clap_plugin_state MClapState = {
    clap_state_save_callback,
    clap_state_load_callback
  };

  static bool clap_state_save_callback(const clap_plugin *plugin, clap_ostream *stream) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->state_save(stream);
  }

  static bool clap_state_load_callback(const clap_plugin *plugin, clap_istream *stream) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->state_load(stream);
  }

  // clap.thread-pool

  struct clap_plugin_thread_pool MClapThreadPool = {
    clap_thread_pool_exec_callback
  };

  static void clap_thread_pool_exec_callback(const clap_plugin *plugin, uint32_t task_index) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->thread_pool_exec(task_index);
  }

  // clap.timer-support

  struct clap_plugin_timer_support MClapTimerSupport = {
    clap_timer_support_on_timer_callback
  };

  static void clap_timer_support_on_timer_callback(const clap_plugin *plugin, clap_id timer_id) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->timer_support_on_timer(timer_id);
  }

  // clap.track-info

  struct clap_plugin_track_info MClapTrackInfo = {
    clap_track_info_changed_callback
  };

  static void clap_track_info_changed_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->track_info_changed();
  }

  // clap.vst2-convert

  struct clap_plugin_vst2_convert MClapVst2Convert = {
    clap_vst2_convert_get_vst2_plugin_id_callback,
    clap_vst2_convert_restore_vst2_state_callback,
    clap_vst2_convert_convert_normalized_value_callback,
    clap_vst2_convert_convert_plain_value_callback
  };

  static int32_t clap_vst2_convert_get_vst2_plugin_id_callback(const clap_plugin *plugin, uint32_t* vst2_plugin_id, char* name, uint32_t name_size) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->vst2_convert_get_vst2_plugin_id(vst2_plugin_id,name,name_size);
  }

  static bool clap_vst2_convert_restore_vst2_state_callback(const clap_plugin *plugin, clap_istream *stream) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->vst2_convert_restore_vst2_state(stream);
  }

  static bool clap_vst2_convert_convert_normalized_value_callback(const clap_plugin *plugin, uint32_t vst2_param_id, double vst2_normalized_value, clap_id* clap_param_id, double* clap_plain_value) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->vst2_convert_convert_normalized_value(vst2_param_id,vst2_normalized_value,clap_param_id,clap_plain_value);
  }

  static bool clap_vst2_convert_convert_plain_value_callback(const clap_plugin *plugin, uint32_t vst2_param_id, double vst2_plain_value, clap_id* clap_param_id, double* clap_plain_value) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->vst2_convert_convert_plain_value(vst2_param_id,vst2_plain_value,clap_param_id,clap_plain_value);
  }

  // clap.vst3-convert

  struct clap_plugin_vst3_convert MClapVst3Convert = {
    clap_vst3_convert_get_vst3_plugin_id_callback,
    clap_vst3_convert_restore_vst3_state_callback,
    clap_vst3_convert_convert_normalized_value_callback,
    clap_vst3_convert_convert_plain_value_callback
  };

  static void clap_vst3_convert_get_vst3_plugin_id_callback(const clap_plugin *plugin, uint8_t *vst3_plugin_uuid) {
    //CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    //plug->vst3_convert_get_vst3_plugin_id(vst3_plugin_uuid);
  }

  static bool clap_vst3_convert_restore_vst3_state_callback(const clap_plugin *plugin, clap_istream *stream) {
    //CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    //return plug->vst3_convert_restore_vst3_state(stream);
    return false;
  }

  static bool clap_vst3_convert_convert_normalized_value_callback(const clap_plugin *plugin, uint32_t vst3_param_id, double vst3_normalized_value, clap_id* clap_param_id, double* clap_plain_value) {
    //CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    //return plug->vst3_convert_convert_normalized_value_callback(vst3_param_id,vst3_normalized_value,clap_param_id,clap_plain_value);
    return false;
  }

  static bool clap_vst3_convert_convert_plain_value_callback(const clap_plugin *plugin, uint32_t vst3_param_id, double vst3_plain_value, clap_id* clap_param_id, double* clap_plain_value) {
    //CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    //return->vst3_convert_convert_plain_value(vst3_param_id,vst3_plain_value,clap_param_id,clap_plain_value);
    return false;
  }

};

//----------------------------------------------------------------------
#endif
