#ifndef mip_clap_host_included
#define mip_clap_host_included
//----------------------------------------------------------------------

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

  const clap_host_t*              MHost             = nullptr;

  clap_host_ambisonic_t*          MAmbisonic        = nullptr;
  clap_host_audio_ports_config_t* MAudioPortsConfig = nullptr;
  clap_host_audio_ports_t*        MAudioPorts       = nullptr;
  clap_host_check_for_update_t*   MCheckForUpdate   = nullptr;
  clap_host_cv_t*                 MCv               = nullptr;
  clap_host_event_registry_t*     MEventRegistry    = nullptr;
  clap_host_file_reference*       MFileReference    = nullptr;
  clap_host_gui_t*                MGui              = nullptr;
  clap_host_latency_t*            MLatency          = nullptr;
  clap_host_log_t*                MLog              = nullptr;
  clap_host_midi_mappings_t*      MMidiMapping      = nullptr;
  clap_host_note_name_t*          MNoteName         = nullptr;
  clap_host_note_ports_t*         MNotePorts        = nullptr;
  clap_host_params_t*             MParams           = nullptr;
  clap_host_posix_fd_support_t*   MPosixFdSupport   = nullptr;
  clap_host_quick_controls_t*     MQuickControls    = nullptr;
  clap_host_state_t*              MState            = nullptr;
  clap_host_surround_t*           MSurround         = nullptr;
  clap_host_tail_t*               MTail             = nullptr;
  clap_host_thread_check_t*       MThreadCheck      = nullptr;
  clap_host_thread_pool_t*        MThreadPool       = nullptr;
  clap_host_timer_support_t*      MTimerSupport     = nullptr;
  clap_host_track_info_t*         MTrackInfo        = nullptr;
  clap_host_transport_control_t*  MTransportControl = nullptr;
  clap_host_tuning_t*             MTuning           = nullptr;
  clap_host_voice_info_t*         MVoiceInfo        = nullptr;

//------------------------------
public:
//------------------------------

  /*
    clap-host says:
    The plugin can't query for extensions during the create method. Wait for clap_plugin.init() call.
  */

  MIP_ClapHost(const clap_host_t* AHost) {
    MIP_PRINT;
    MHost = AHost;
  }

  virtual ~MIP_ClapHost() {
    MIP_PRINT;
  }

  //const clap_host_t* getHost() {
  //  return MHost;
  //}


//------------------------------
public:
//------------------------------

   const void* get_extension(const char *extension_id) {
     return MHost->get_extension(MHost,extension_id);
   }

   void request_restart() {
     MHost->request_restart(MHost);
   }

   void request_process() {
     MHost->request_process(MHost);
   }

   void request_callback() {
     MHost->request_callback(MHost);
   }

//------------------------------
public:
//------------------------------

  void initExtensions() {
    MIP_PRINT;
    MAmbisonic        = (clap_host_ambisonic_t*)MHost->get_extension(MHost,CLAP_EXT_AMBISONIC);
    MAudioPortsConfig = (clap_host_audio_ports_config_t*)MHost->get_extension(MHost,CLAP_EXT_AUDIO_PORTS_CONFIG);
    MAudioPorts       = (clap_host_audio_ports_t*)MHost->get_extension(MHost,CLAP_EXT_AUDIO_PORTS);
    MCheckForUpdate   = (clap_host_check_for_update_t*)MHost->get_extension(MHost,CLAP_EXT_CHECK_FOR_UPDATE);
    MCv               = (clap_host_cv_t*)MHost->get_extension(MHost,CLAP_EXT_CV);
    MEventRegistry    = (clap_host_event_registry_t*)MHost->get_extension(MHost,CLAP_EXT_EVENT_REGISTRY);
    MFileReference    = (clap_host_file_reference*)MHost->get_extension(MHost,CLAP_EXT_FILE_REFERENCE);
    MGui              = (clap_host_gui_t*)MHost->get_extension(MHost,CLAP_EXT_GUI);
    MLatency          = (clap_host_latency_t*)MHost->get_extension(MHost,CLAP_EXT_LATENCY);
    MLog              = (clap_host_log_t*)MHost->get_extension(MHost,CLAP_EXT_LOG);
    MMidiMapping      = (clap_host_midi_mappings_t*)MHost->get_extension(MHost,CLAP_EXT_MIDI_MAPPINGS);
    MNoteName         = (clap_host_note_name_t*)MHost->get_extension(MHost,CLAP_EXT_NOTE_NAME);
    MNotePorts        = (clap_host_note_ports_t*)MHost->get_extension(MHost,CLAP_EXT_NOTE_PORTS);
    MParams           = (clap_host_params_t*)MHost->get_extension(MHost,CLAP_EXT_PARAMS);
    MPosixFdSupport   = (clap_host_posix_fd_support_t*)MHost->get_extension(MHost,CLAP_EXT_POSIX_FD_SUPPORT);
    MQuickControls    = (clap_host_quick_controls_t*)MHost->get_extension(MHost,CLAP_EXT_QUICK_CONTROLS);
    MState            = (clap_host_state_t*)MHost->get_extension(MHost,CLAP_EXT_STATE);
    MSurround         = (clap_host_surround_t*)MHost->get_extension(MHost,CLAP_EXT_SURROUND);
    MTail             = (clap_host_tail_t*)MHost->get_extension(MHost,CLAP_EXT_TAIL);
    MThreadCheck      = (clap_host_thread_check_t*)MHost->get_extension(MHost,CLAP_EXT_THREAD_CHECK);
    MThreadPool       = (clap_host_thread_pool_t*)MHost->get_extension(MHost,CLAP_EXT_THREAD_POOL);
    MTimerSupport     = (clap_host_timer_support_t*)MHost->get_extension(MHost,CLAP_EXT_TIMER_SUPPORT);
    MTrackInfo        = (clap_host_track_info_t*)MHost->get_extension(MHost,CLAP_EXT_TRACK_INFO);
    MTransportControl = (clap_host_transport_control_t*)MHost->get_extension(MHost,CLAP_EXT_TRANSPORT_CONTROL);
    MTuning           = (clap_host_tuning_t*)MHost->get_extension(MHost,CLAP_EXT_TUNING);
    MVoiceInfo        = (clap_host_voice_info_t*)MHost->get_extension(MHost,CLAP_EXT_VOICE_INFO);
  }

};

//----------------------------------------------------------------------
#endif
