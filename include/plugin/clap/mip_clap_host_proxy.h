#ifndef mip_clap_host_proxy_included
#define mip_clap_host_proxy_included
//----------------------------------------------------------------------

//#define MIP_NO_PLUGIN_LOADING


#include "mip.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ClapHostProxy {

//------------------------------
public:
//------------------------------

  const clap_host_t*              host                = nullptr;
  clap_host_audio_ports_config_t* audio_ports_config  = nullptr;
  clap_host_audio_ports_t*        audio_ports         = nullptr;
  clap_host_event_filter_t*       event_filter        = nullptr;
  clap_host_event_registry_t*     event_registry      = nullptr;
  clap_host_gui_t*                gui                 = nullptr;
  clap_host_latency_t*            latency             = nullptr;
  clap_host_log_t*                log                 = nullptr;
  clap_host_note_name_t*          note_name           = nullptr;
  clap_host_note_ports_t*         note_ports          = nullptr;
  clap_host_params_t*             params              = nullptr;
  clap_host_posix_fd_support_t*   posix_fd_support    = nullptr;
  clap_host_state_t*              state               = nullptr;
  clap_host_thread_check_t*       thread_check        = nullptr;
  clap_host_thread_pool_t*        thread_pool         = nullptr;
  clap_host_timer_support_t*      timer_support       = nullptr;
  clap_host_ambisonic_t*          ambisonic           = nullptr;
  clap_host_check_for_update_t*   check_for_update    = nullptr;
  clap_host_file_reference*       file_reference      = nullptr;
  clap_host_midi_mappings_t*      midi_mapping        = nullptr;
  clap_host_quick_controls_t*     quick_controls      = nullptr;
  clap_host_surround_t*           surround            = nullptr;
  clap_host_track_info_t*         track_info          = nullptr;
  clap_host_tuning_t*             tuning              = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ClapHostProxy(const clap_host_t* AHost) {
    host                = AHost;
    audio_ports_config  = (clap_host_audio_ports_config_t*)AHost->get_extension(AHost,CLAP_EXT_AUDIO_PORTS_CONFIG);
    audio_ports         = (clap_host_audio_ports_t*)AHost->get_extension(AHost,CLAP_EXT_AUDIO_PORTS);
    event_filter        = (clap_host_event_filter_t*)AHost->get_extension(AHost,CLAP_EXT_EVENT_FILTER);
    event_registry      = (clap_host_event_registry_t*)AHost->get_extension(AHost,CLAP_EXT_EVENT_REGISTRY);
    gui                 = (clap_host_gui_t*)AHost->get_extension(AHost,CLAP_EXT_GUI);
    latency             = (clap_host_latency_t*)AHost->get_extension(AHost,CLAP_EXT_LATENCY);
    log                 = (clap_host_log_t*)AHost->get_extension(AHost,CLAP_EXT_LOG);
    note_name           = (clap_host_note_name_t*)AHost->get_extension(AHost,CLAP_EXT_NOTE_NAME);
    note_ports          = (clap_host_note_ports_t*)AHost->get_extension(AHost,CLAP_EXT_NOTE_PORTS);
    params              = (clap_host_params_t*)AHost->get_extension(AHost,CLAP_EXT_PARAMS);
    posix_fd_support    = (clap_host_posix_fd_support_t*)AHost->get_extension(AHost,CLAP_EXT_POSIX_FD_SUPPORT);
    state               = (clap_host_state_t*)AHost->get_extension(AHost,CLAP_EXT_STATE);
    thread_check        = (clap_host_thread_check_t*)AHost->get_extension(AHost,CLAP_EXT_THREAD_CHECK);
    thread_pool         = (clap_host_thread_pool_t*)AHost->get_extension(AHost,CLAP_EXT_THREAD_POOL);
    timer_support       = (clap_host_timer_support_t*)AHost->get_extension(AHost,CLAP_EXT_TIMER_SUPPORT);
    ambisonic           = (clap_host_ambisonic_t*)AHost->get_extension(AHost,CLAP_EXT_AMBISONIC);
    check_for_update    = (clap_host_check_for_update_t*)AHost->get_extension(AHost,CLAP_EXT_CHECK_FOR_UPDATE);
    file_reference      = (clap_host_file_reference*)AHost->get_extension(AHost,CLAP_EXT_FILE_REFERENCE);
    midi_mapping        = (clap_host_midi_mappings_t*)AHost->get_extension(AHost,CLAP_EXT_MIDI_MAPPINGS);
    quick_controls      = (clap_host_quick_controls_t*)AHost->get_extension(AHost,CLAP_EXT_QUICK_CONTROLS);
    surround            = (clap_host_surround_t*)AHost->get_extension(AHost,CLAP_EXT_SURROUND);
    track_info          = (clap_host_track_info_t*)AHost->get_extension(AHost,CLAP_EXT_TRACK_INFO);
    tuning              = (clap_host_tuning_t*)AHost->get_extension(AHost,CLAP_EXT_TUNING);
  }

  virtual ~MIP_ClapHostProxy() {
  }

//------------------------------
public:
//------------------------------

   const void* get_extension(const char *extension_id) {
     return host->get_extension(host,extension_id);
   }

   void request_restart() {
     host->request_restart(host);
   }

   void request_process() {
     host->request_process(host);
   }

   void request_callback() {
     host->request_callback(host);
   }


};

//----------------------------------------------------------------------
#endif
