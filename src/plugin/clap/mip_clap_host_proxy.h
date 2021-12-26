#ifndef mip_clap_host_proxy_included
#define mip_clap_host_proxy_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------

struct MIP_ClapPluginHostExt {
  // extensions
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

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ClapHostProxy {

//------------------------------
private:
//------------------------------

  const clap_host*      MHost     = nullptr;
  MIP_ClapPluginHostExt MExt      = {0};
  const char*           MName      = "";
  const char*           MVendor    = "";
  const char*           MUrl       = "";
  const char*           MVersion   = "";

//------------------------------
public:
//------------------------------

  MIP_ClapHostProxy(const clap_host* host) {
    MHost = host;
    //if (clap_version) ...
    init_extensions();
    //MHostData  = host->host_data;  // reserved pointer for the host
    MName      = host->name;
    MVendor    = host->vendor;
    MUrl       = host->url;
    MVersion   = host->version;
  }

  //----------

  ~MIP_ClapHostProxy() {
  }


//------------------------------
public:
//------------------------------

  const void* get_extension(const char *extension_id) {
    return MHost->get_extension(MHost,extension_id);
  }

  //----------

  void request_restart() {
    MHost->request_restart(MHost);
  }

  //----------

  void request_process() {
    MHost->request_process(MHost);
  }

  //----------

  void request_callback() {
    MHost->request_callback(MHost);
  }

//------------------------------
private:
//------------------------------

  void init_extensions() {
    MExt.audio_ports        = (clap_host_audio_ports*)        get_extension(CLAP_EXT_AUDIO_PORTS);
    MExt.audio_ports_config = (clap_host_audio_ports_config*) get_extension(CLAP_EXT_AUDIO_PORTS_CONFIG);
    MExt.event_filter       = (clap_host_event_filter*)       get_extension(CLAP_EXT_EVENT_FILTER);
    MExt.fd_support         = (clap_host_fd_support*)         get_extension(CLAP_EXT_FD_SUPPORT);
    MExt.gui                = (clap_host_gui*)                get_extension(CLAP_EXT_GUI);
    MExt.latency            = (clap_host_latency*)            get_extension(CLAP_EXT_LATENCY);
    MExt.log                = (clap_host_log*)                get_extension(CLAP_EXT_LOG);
    MExt.note_name          = (clap_host_note_name*)          get_extension(CLAP_EXT_NOTE_NAME);
    MExt.note_ports         = (clap_host_note_ports*)         get_extension(CLAP_EXT_NOTE_PORTS);
    MExt.params             = (clap_host_params*)             get_extension(CLAP_EXT_PARAMS);
    MExt.state              = (clap_host_state*)              get_extension(CLAP_EXT_STATE);
    MExt.thread_check       = (clap_host_thread_check*)       get_extension(CLAP_EXT_THREAD_CHECK);
    MExt.thread_pool        = (clap_host_thread_pool*)        get_extension(CLAP_EXT_THREAD_POOL);
    MExt.timer_support      = (clap_host_timer_support*)      get_extension(CLAP_EXT_TIMER_SUPPORT);
    // draft
    MExt.check_for_update   = (clap_host_check_for_update*)   get_extension(CLAP_EXT_CHECK_FOR_UPDATE);
    MExt.file_reference     = (clap_host_file_reference*)     get_extension(CLAP_EXT_FILE_REFERENCE);
    MExt.midi_mappings      = (clap_host_midi_mappings*)      get_extension(CLAP_EXT_MIDI_MAPPINGS);
    MExt.quick_controls     = (clap_host_quick_controls*)     get_extension(CLAP_EXT_QUICK_CONTROLS);
    MExt.surround           = (clap_host_surround*)           get_extension(CLAP_EXT_SURROUND);
    MExt.track_info         = (clap_host_track_info*)         get_extension(CLAP_EXT_TRACK_INFO);
    MExt.tuning             = (clap_host_tuning*)             get_extension(CLAP_EXT_TUNING);
  }

};

//----------------------------------------------------------------------
#endif

