#ifndef mip_clap_host_included
#define mip_clap_host_included
//----------------------------------------------------------------------

//#define MIP_NO_PLUGIN_LOADING


#include "mip.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ClapHost {

//------------------------------
public:
//------------------------------

  const clap_host_t*              host                = nullptr;
  clap_host_ambisonic_t*          ambisonic           = nullptr;
  clap_host_audio_ports_config_t* audio_ports_config  = nullptr;
  clap_host_audio_ports_t*        audio_ports         = nullptr;
  //clap_host_check_for_update_t*   check_for_update    = nullptr;
  clap_host_cv_t*                 cv                  = nullptr;
  clap_host_event_registry_t*     event_registry      = nullptr;
  clap_host_file_reference*       file_reference      = nullptr;
  clap_host_gui_t*                gui                 = nullptr;
  clap_host_latency_t*            latency             = nullptr;
  clap_host_log_t*                log                 = nullptr;
  clap_host_midi_mappings_t*      midi_mapping        = nullptr;
  clap_host_note_name_t*          note_name           = nullptr;
  clap_host_note_ports_t*         note_ports          = nullptr;
  clap_host_params_t*             params              = nullptr;
  clap_host_posix_fd_support_t*   posix_fd_support    = nullptr;
  clap_host_quick_controls_t*     quick_controls      = nullptr;
  clap_host_state_t*              state               = nullptr;
  clap_host_surround_t*           surround            = nullptr;
  clap_host_tail_t*               tail                = nullptr;
  clap_host_thread_check_t*       thread_check        = nullptr;
  clap_host_thread_pool_t*        thread_pool         = nullptr;
  clap_host_timer_support_t*      timer_support       = nullptr;
  clap_host_track_info_t*         track_info          = nullptr;
  //clap_host_transport_control_t*  transport_control   = nullptr;
  clap_host_tuning_t*             tuning              = nullptr;
  clap_host_voice_info_t*         voice_info          = nullptr;

//------------------------------
public:
//------------------------------

  /*
    clap-host says:
    The plugin can't query for extensions during the create method. Wait for clap_plugin.init() call.
  */

  MIP_ClapHost(const clap_host_t* AHost) {
    host                = AHost;
/*
    moved to MIP_Plugin.initExtensions
    called from MIP_Plugin.init

    ambisonic           = (clap_host_ambisonic_t*)AHost->get_extension(AHost,CLAP_EXT_AMBISONIC);
    audio_ports_config  = (clap_host_audio_ports_config_t*)AHost->get_extension(AHost,CLAP_EXT_AUDIO_PORTS_CONFIG);
    audio_ports         = (clap_host_audio_ports_t*)AHost->get_extension(AHost,CLAP_EXT_AUDIO_PORTS);
    //check_for_update    = (clap_host_check_for_update_t*)AHost->get_extension(AHost,CLAP_EXT_CHECK_FOR_UPDATE);
    cv                  = (clap_host_cv_t*)AHost->get_extension(AHost,CLAP_EXT_CV);
    event_registry      = (clap_host_event_registry_t*)AHost->get_extension(AHost,CLAP_EXT_EVENT_REGISTRY);
    file_reference      = (clap_host_file_reference*)AHost->get_extension(AHost,CLAP_EXT_FILE_REFERENCE);
    gui                 = (clap_host_gui_t*)AHost->get_extension(AHost,CLAP_EXT_GUI);
    latency             = (clap_host_latency_t*)AHost->get_extension(AHost,CLAP_EXT_LATENCY);
    log                 = (clap_host_log_t*)AHost->get_extension(AHost,CLAP_EXT_LOG);
    midi_mapping        = (clap_host_midi_mappings_t*)AHost->get_extension(AHost,CLAP_EXT_MIDI_MAPPINGS);
    note_name           = (clap_host_note_name_t*)AHost->get_extension(AHost,CLAP_EXT_NOTE_NAME);
    note_ports          = (clap_host_note_ports_t*)AHost->get_extension(AHost,CLAP_EXT_NOTE_PORTS);
    params              = (clap_host_params_t*)AHost->get_extension(AHost,CLAP_EXT_PARAMS);
    posix_fd_support    = (clap_host_posix_fd_support_t*)AHost->get_extension(AHost,CLAP_EXT_POSIX_FD_SUPPORT);
    quick_controls      = (clap_host_quick_controls_t*)AHost->get_extension(AHost,CLAP_EXT_QUICK_CONTROLS);
    state               = (clap_host_state_t*)AHost->get_extension(AHost,CLAP_EXT_STATE);
    surround            = (clap_host_surround_t*)AHost->get_extension(AHost,CLAP_EXT_SURROUND);
    tail                = (clap_host_tail_t*)AHost->get_extension(AHost,CLAP_EXT_TAIL);
    thread_check        = (clap_host_thread_check_t*)AHost->get_extension(AHost,CLAP_EXT_THREAD_CHECK);
    thread_pool         = (clap_host_thread_pool_t*)AHost->get_extension(AHost,CLAP_EXT_THREAD_POOL);
    timer_support       = (clap_host_timer_support_t*)AHost->get_extension(AHost,CLAP_EXT_TIMER_SUPPORT);
    track_info          = (clap_host_track_info_t*)AHost->get_extension(AHost,CLAP_EXT_TRACK_INFO);
    //transport_control   = (clap_host_transport_control_t*)AHost->get_extension(AHost,CLAP_EXT_TRANSPORT_CONTROL);
    tuning              = (clap_host_tuning_t*)AHost->get_extension(AHost,CLAP_EXT_TUNING);
    voice_info          = (clap_host_voice_info_t*)AHost->get_extension(AHost,CLAP_EXT_VOICE_INFO);
*/
  }

  virtual ~MIP_ClapHost() {
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

//------------------------------
public:
//------------------------------

  void initExtensions() {
    ambisonic           = (clap_host_ambisonic_t*)host->get_extension(host,CLAP_EXT_AMBISONIC);
    audio_ports_config  = (clap_host_audio_ports_config_t*)host->get_extension(host,CLAP_EXT_AUDIO_PORTS_CONFIG);
    audio_ports         = (clap_host_audio_ports_t*)host->get_extension(host,CLAP_EXT_AUDIO_PORTS);
    //check_for_update    = (clap_host_check_for_update_t*)host->get_extension(host,CLAP_EXT_CHECK_FOR_UPDATE);
    cv                  = (clap_host_cv_t*)host->get_extension(host,CLAP_EXT_CV);
    event_registry      = (clap_host_event_registry_t*)host->get_extension(host,CLAP_EXT_EVENT_REGISTRY);
    file_reference      = (clap_host_file_reference*)host->get_extension(host,CLAP_EXT_FILE_REFERENCE);
    gui                 = (clap_host_gui_t*)host->get_extension(host,CLAP_EXT_GUI);
    latency             = (clap_host_latency_t*)host->get_extension(host,CLAP_EXT_LATENCY);
    log                 = (clap_host_log_t*)host->get_extension(host,CLAP_EXT_LOG);
    midi_mapping        = (clap_host_midi_mappings_t*)host->get_extension(host,CLAP_EXT_MIDI_MAPPINGS);
    note_name           = (clap_host_note_name_t*)host->get_extension(host,CLAP_EXT_NOTE_NAME);
    note_ports          = (clap_host_note_ports_t*)host->get_extension(host,CLAP_EXT_NOTE_PORTS);
    params              = (clap_host_params_t*)host->get_extension(host,CLAP_EXT_PARAMS);
    posix_fd_support    = (clap_host_posix_fd_support_t*)host->get_extension(host,CLAP_EXT_POSIX_FD_SUPPORT);
    quick_controls      = (clap_host_quick_controls_t*)host->get_extension(host,CLAP_EXT_QUICK_CONTROLS);
    state               = (clap_host_state_t*)host->get_extension(host,CLAP_EXT_STATE);
    surround            = (clap_host_surround_t*)host->get_extension(host,CLAP_EXT_SURROUND);
    tail                = (clap_host_tail_t*)host->get_extension(host,CLAP_EXT_TAIL);
    thread_check        = (clap_host_thread_check_t*)host->get_extension(host,CLAP_EXT_THREAD_CHECK);
    thread_pool         = (clap_host_thread_pool_t*)host->get_extension(host,CLAP_EXT_THREAD_POOL);
    timer_support       = (clap_host_timer_support_t*)host->get_extension(host,CLAP_EXT_TIMER_SUPPORT);
    track_info          = (clap_host_track_info_t*)host->get_extension(host,CLAP_EXT_TRACK_INFO);
    //transport_control   = (clap_host_transport_control_t*)host->get_extension(host,CLAP_EXT_TRANSPORT_CONTROL);
    tuning              = (clap_host_tuning_t*)host->get_extension(host,CLAP_EXT_TUNING);
    voice_info          = (clap_host_voice_info_t*)host->get_extension(host,CLAP_EXT_VOICE_INFO);
  }


  void printSupportedExtensions() {
    if (ambisonic)          { MIP_Print("host supports: %s\n",CLAP_EXT_AMBISONIC); }
    if (audio_ports_config) { MIP_Print("host supports: %s\n",CLAP_EXT_AUDIO_PORTS_CONFIG); }
    if (audio_ports)        { MIP_Print("host supports: %s\n",CLAP_EXT_AUDIO_PORTS); }
  //if (check_for_update)   { MIP_Print("host supports: %s\n",CLAP_EXT_CHECK_FOR_UPDATE); }
    if (cv)                 { MIP_Print("host supports: %s\n",CLAP_EXT_CV); }
    if (event_registry)     { MIP_Print("host supports: %s\n",CLAP_EXT_EVENT_REGISTRY); }
    if (file_reference)     { MIP_Print("host supports: %s\n",CLAP_EXT_FILE_REFERENCE); }
    if (gui)                { MIP_Print("host supports: %s\n",CLAP_EXT_GUI); }
    if (latency)            { MIP_Print("host supports: %s\n",CLAP_EXT_LATENCY); }
    if (log)                { MIP_Print("host supports: %s\n",CLAP_EXT_LOG); }
    if (midi_mapping)       { MIP_Print("host supports: %s\n",CLAP_EXT_MIDI_MAPPINGS); }
    if (note_name)          { MIP_Print("host supports: %s\n",CLAP_EXT_NOTE_NAME); }
    if (note_ports)         { MIP_Print("host supports: %s\n",CLAP_EXT_NOTE_PORTS); }
    if (params)             { MIP_Print("host supports: %s\n",CLAP_EXT_PARAMS); }
    if (posix_fd_support)   { MIP_Print("host supports: %s\n",CLAP_EXT_POSIX_FD_SUPPORT); }
    if (quick_controls)     { MIP_Print("host supports: %s\n",CLAP_EXT_QUICK_CONTROLS); }
    if (state)              { MIP_Print("host supports: %s\n",CLAP_EXT_STATE); }
    if (surround)           { MIP_Print("host supports: %s\n",CLAP_EXT_SURROUND); }
    if (tail)               { MIP_Print("host supports: %s\n",CLAP_EXT_TAIL); }
    if (thread_check)       { MIP_Print("host supports: %s\n",CLAP_EXT_THREAD_CHECK); }
    if (thread_pool)        { MIP_Print("host supports: %s\n",CLAP_EXT_THREAD_POOL); }
    if (timer_support)      { MIP_Print("host supports: %s\n",CLAP_EXT_TIMER_SUPPORT); }
    if (track_info)         { MIP_Print("host supports: %s\n",CLAP_EXT_TRACK_INFO); }
  //if (transport_control)  { MIP_Print("host supports: %s\n",CLAP_EXT_TRANSPORT_CONTROL); }
    if (tuning)             { MIP_Print("host supports: %s\n",CLAP_EXT_TUNING); }
    if (voice_info)         { MIP_Print("host supports: %s\n",CLAP_EXT_VOICE_INFO); }
  }

};

//----------------------------------------------------------------------
#endif
