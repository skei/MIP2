#ifndef mip_clap_hosted_plugin_included
#define mip_clap_hosted_plugin_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"

#include <dlfcn.h>

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ClapHostedPlugin {

//------------------------------
private:
//------------------------------

  void*                           MLibHandle      = nullptr;
  const clap_plugin_entry_t*      MClapEntry      = nullptr;
  const clap_plugin_factory_t*    MClapFactory    = nullptr;
  const clap_plugin_descriptor_t* MClapDescriptor = nullptr;
  const clap_plugin_t*            MPlugin         = nullptr;

//------------------------------
public:
//------------------------------

  clap_plugin_ambisonic_t*          ambisonic           = nullptr;
  clap_plugin_audio_ports_config_t* audio_ports_config  = nullptr;
  clap_plugin_audio_ports_t*        audio_ports         = nullptr;
  clap_plugin_cv_t*                 cv                  = nullptr;
  clap_plugin_event_filter_t*       event_filter        = nullptr;
  clap_plugin_file_reference*       file_reference      = nullptr;
  clap_plugin_gui_t*                gui                 = nullptr;
  clap_plugin_gui_x11_t*            gui_x11             = nullptr;
  clap_plugin_latency_t*            latency             = nullptr;
  clap_plugin_midi_mappings_t*      midi_mapping        = nullptr;
  clap_plugin_note_ports_t*         note_ports          = nullptr;
  clap_plugin_params_t*             params              = nullptr;
  clap_plugin_preset_load_t*        preset_load         = nullptr;
  clap_plugin_quick_controls_t*     quick_controls      = nullptr;
  clap_plugin_render_t*             render              = nullptr;
  clap_plugin_state_t*              state               = nullptr;
  clap_plugin_surround_t*           surround            = nullptr;
  clap_plugin_thread_pool_t*        thread_pool         = nullptr;
  clap_plugin_timer_support_t*      timer_support       = nullptr;
  clap_plugin_track_info_t*         track_info          = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ClapHostedPlugin(const char* APath) {
    MPlugin = load_plugin(APath);
    initExtensions();
  }

  MIP_ClapHostedPlugin(const clap_plugin_t* APlugin) {
    MPlugin = APlugin;
    initExtensions();
  }

  virtual ~MIP_ClapHostedPlugin() {
    unload_plugin();
  }

//------------------------------
public:
//------------------------------

  bool load_plugin(const char* APath, uint32_t AIndex=0) {
    MLibhandle = dlopen(APath,RTLD_LAZY | RTLD_LOCAL);
    if (MLibHandle) {
      MClapEntry = (const clap_plugin_entry_t*)dlsym(MLibHandle,"clap_entry");
      if (MClapEntry) {
        MClapEntry->init();
        MClapFactory = MClapEntry->get_factory(CLAP_PLUGIN_FACTORY_ID);
        if (MClapFactory) {
          uint32_t count = MClapFactory->get_plugin_count();
          if (AIndex < count) {
            MClapDescriptor = factory->get_plugin_descriptor(factory,AIndex);
            if (MClapDescriptor) {
              MPlugin = factory->create_plugin(factory,host,descriptor->id);
              //MPlugin->init(MPlugin);
              return true;
            }
          } // count
        } // factory
      } //entry
    } // lib
    return false;
  }

  //----------

  void unload_plugin() {
    if (MPlugin)      MPlugin->destroy(MPlugin);
    //if (MClapFactory) MClapFactory->
    if (MClapEntry)   MClapEntry->deinit();
    if (MLibHandle)   dlclose(MLibHandle);
    MPlugin       = nullptr;
    MClapEntry    = nullptr;
    MClapFactory  = nullptr;
    MLibHandle    = nullptr;
  }

  //----------

  void initExtensions() {
    ambisonic           = (clap_plugin_ambisonic_t*)APlugin->get_extension(APlugin,CLAP_EXT_AMBISONIC);
    audio_ports_config  = (clap_plugin_audio_ports_config_t*)APlugin->get_extension(APlugin,CLAP_EXT_AUDIO_PORTS_CONFIG);
    audio_ports         = (clap_plugin_audio_ports_t*)APlugin->get_extension(APlugin,CLAP_EXT_AUDIO_PORTS);
    cv                  = (clap_plugin_cv_t*)APlugin->get_extension(APlugin,CLAP_EXT_CV);
    event_filter        = (clap_plugin_event_filter_t*)APlugin->get_extension(APlugin,CLAP_EXT_EVENT_FILTER);
    file_reference      = (clap_plugin_file_reference*)APlugin->get_extension(APlugin,CLAP_EXT_FILE_REFERENCE);
    gui                 = (clap_plugin_gui_t*)APlugin->get_extension(APlugin,CLAP_EXT_GUI);
    gui_x11             = (clap_plugin_gui_x11_t*)APlugin->get_extension(APlugin,CLAP_EXT_GUI_X11);
    latency             = (clap_plugin_latency_t*)APlugin->get_extension(APlugin,CLAP_EXT_LATENCY);
    midi_mapping        = (clap_plugin_midi_mappings_t*)APlugin->get_extension(APlugin,CLAP_EXT_MIDI_MAPPINGS);
    note_ports          = (clap_plugin_note_ports_t*)APlugin->get_extension(APlugin,CLAP_EXT_NOTE_PORTS);
    params              = (clap_plugin_params_t*)APlugin->get_extension(APlugin,CLAP_EXT_PARAMS);
    preset_load         = (clap_plugin_preset_load_t*)APlugin->get_extension(APlugin,CLAP_EXT_PRESET_LOAD);
    quick_controls      = (clap_plugin_quick_controls_t*)APlugin->get_extension(APlugin,CLAP_EXT_QUICK_CONTROLS);
    render              = (clap_plugin_render_t*)APlugin->get_extension(APlugin,CLAP_EXT_RENDER);
    state               = (clap_plugin_state_t*)APlugin->get_extension(APlugin,CLAP_EXT_STATE);
    surround            = (clap_plugin_surround_t*)APlugin->get_extension(APlugin,CLAP_EXT_SURROUND);
    thread_pool         = (clap_plugin_thread_pool_t*)APlugin->get_extension(APlugin,CLAP_EXT_THREAD_POOL);
    timer_support       = (clap_plugin_timer_support_t*)APlugin->get_extension(APlugin,CLAP_EXT_TIMER_SUPPORT);
    track_info          = (clap_plugin_track_info_t*)APlugin->get_extension(APlugin,CLAP_EXT_TRACK_INFO);
  }

//------------------------------
public:
//------------------------------

  bool init() {
    return MPlugin->init(MPlugin);
  }

  void destroy() {
    MPlugin->destroy(MPlugin);
    MPlugin = nullptr;
  }

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    return MPlugin->activate(MPlugin,sample_rate,min_frames_count,max_frames_count);
  }

  void deactivate() {
    MPlugin->deactivate(MPlugin);
  }

  bool start_processing() {
    MPlugin->start_processing(MPlugin);
    return true;
  }

  void stop_processing() {
    MPlugin->stop_processing(MPlugin);
  }

  clap_process_status process(const clap_process_t *process) {
    return MPlugin->process(MPlugin,process);
  }

  const void* get_extension(const char *id) {
    return MPlugin->get_extension(MPlugin,id);
  }

  void on_main_thread() {
    MPlugin->on_main_thread(MPlugin);
  }

};

//----------------------------------------------------------------------
#endif
