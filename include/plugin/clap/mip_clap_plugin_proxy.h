#ifndef mip_clap_plugin_proxy_included
#define mip_clap_plugin_proxy_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ClapPluginProxy {

//------------------------------
public:
//------------------------------

  const clap_plugin_t*              plugin              = nullptr;
  clap_plugin_audio_ports_config_t* audio_ports_config  = nullptr;
  clap_plugin_audio_ports_t*        audio_ports         = nullptr;
  clap_plugin_event_filter_t*       event_filter        = nullptr;
  clap_plugin_gui_t*                gui                 = nullptr;
  clap_plugin_gui_x11_t*            gui_x11             = nullptr;
  clap_plugin_latency_t*            latency             = nullptr;
  clap_plugin_note_ports_t*         note_ports          = nullptr;
  clap_plugin_params_t*             params              = nullptr;
  clap_plugin_render_t*             render              = nullptr;
  clap_plugin_state_t*              state               = nullptr;
  clap_plugin_thread_pool_t*        thread_pool         = nullptr;
  clap_plugin_timer_support_t*      timer_support       = nullptr;
  clap_plugin_ambisonic_t*          ambisonic           = nullptr;
  clap_plugin_file_reference*       file_reference      = nullptr;
  clap_plugin_midi_mappings_t*      midi_mapping        = nullptr;
  clap_plugin_preset_load_t*        preset_load         = nullptr;
  clap_plugin_quick_controls_t*     quick_controls      = nullptr;
  clap_plugin_surround_t*           surround            = nullptr;
  clap_plugin_track_info_t*         track_info          = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ClapPluginProxy(const clap_plugin_t* APlugin) {
    plugin              = APlugin;
    audio_ports_config  = (clap_plugin_audio_ports_config_t*)APlugin->get_extension(APlugin,CLAP_EXT_AUDIO_PORTS_CONFIG);
    audio_ports         = (clap_plugin_audio_ports_t*)APlugin->get_extension(APlugin,CLAP_EXT_AUDIO_PORTS);
    event_filter        = (clap_plugin_event_filter_t*)APlugin->get_extension(APlugin,CLAP_EXT_EVENT_FILTER);
    gui                 = (clap_plugin_gui_t*)APlugin->get_extension(APlugin,CLAP_EXT_GUI);
    gui_x11             = (clap_plugin_gui_x11_t*)APlugin->get_extension(APlugin,CLAP_EXT_GUI_X11);
    latency             = (clap_plugin_latency_t*)APlugin->get_extension(APlugin,CLAP_EXT_LATENCY);
    note_ports          = (clap_plugin_note_ports_t*)APlugin->get_extension(APlugin,CLAP_EXT_NOTE_PORTS);
    params              = (clap_plugin_params_t*)APlugin->get_extension(APlugin,CLAP_EXT_PARAMS);
    state               = (clap_plugin_state_t*)APlugin->get_extension(APlugin,CLAP_EXT_STATE);
    thread_pool         = (clap_plugin_thread_pool_t*)APlugin->get_extension(APlugin,CLAP_EXT_THREAD_POOL);
    timer_support       = (clap_plugin_timer_support_t*)APlugin->get_extension(APlugin,CLAP_EXT_TIMER_SUPPORT);
    ambisonic           = (clap_plugin_ambisonic_t*)APlugin->get_extension(APlugin,CLAP_EXT_AMBISONIC);
    file_reference      = (clap_plugin_file_reference*)APlugin->get_extension(APlugin,CLAP_EXT_FILE_REFERENCE);
    midi_mapping        = (clap_plugin_midi_mappings_t*)APlugin->get_extension(APlugin,CLAP_EXT_MIDI_MAPPINGS);
    quick_controls      = (clap_plugin_quick_controls_t*)APlugin->get_extension(APlugin,CLAP_EXT_QUICK_CONTROLS);
    surround            = (clap_plugin_surround_t*)APlugin->get_extension(APlugin,CLAP_EXT_SURROUND);
    track_info          = (clap_plugin_track_info_t*)APlugin->get_extension(APlugin,CLAP_EXT_TRACK_INFO);
  }

  virtual ~MIP_ClapPluginProxy() {
  }

//------------------------------
public:
//------------------------------

  bool init() {
    return plugin->init(plugin);
  }

  void destroy() {
    plugin->destroy(plugin);
  }

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    return plugin->activate(plugin,sample_rate,min_frames_count,max_frames_count);
  }

  void deactivate() {
    plugin->deactivate(plugin);
  }

  bool start_processing() {
    plugin->start_processing(plugin);
    return true;
  }

  void stop_processing() {
    plugin->stop_processing(plugin);
  }

  clap_process_status process(const clap_process_t *process) {
    return plugin->process(plugin,process);
  }

  const void* get_extension(const char *id) {
    return plugin->get_extension(plugin,id);
  }

  void on_main_thread() {
    plugin->on_main_thread(plugin);
  }

};

//----------------------------------------------------------------------
#endif
