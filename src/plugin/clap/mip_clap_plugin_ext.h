#ifndef mip_clap_plugin_ext_included
#define mip_clap_plugin_ext_included
//----------------------------------------------------------------------

//TODO

#include "mip.h"
#include "plugin/clap/mip_clap.h"

//struct MIP_ClapPluginHostExt {
//  // extensions
//  clap_host_audio_ports*        audio_ports         = nullptr;
//  clap_host_audio_ports_config* audio_ports_config  = nullptr;
//  clap_host_event_filter*       event_filter        = nullptr;
//  clap_host_fd_support*         fd_support          = nullptr;
//  clap_host_gui*                gui                 = nullptr;
//  clap_host_latency*            latency             = nullptr;
//  clap_host_log*                log                 = nullptr;
//  clap_host_note_name*          note_name           = nullptr;
//  clap_host_note_ports*         note_ports          = nullptr;
//  clap_host_params*             params              = nullptr;
//  clap_host_state*              state               = nullptr;
//  clap_host_thread_check*       thread_check        = nullptr;
//  clap_host_thread_pool*        thread_pool         = nullptr;
//  clap_host_timer_support*      timer_support       = nullptr;
//
//  // draft
//  clap_host_check_for_update*   check_for_update    = nullptr;
//  clap_host_file_reference*     file_reference      = nullptr;
//  clap_host_midi_mappings*      midi_mappings       = nullptr;
//  clap_host_quick_controls*     quick_controls      = nullptr;
//  clap_host_surround*           surround            = nullptr;
//  clap_host_track_info*         track_info          = nullptr;
//  clap_host_tuning*             tuning              = nullptr;
//};
//
////----------
//
//void init_extensions(const clap_host* host, MIP_ClapPluginHostExt* ext) {
//  ext->audio_ports    = (clap_host_audio_ports*)        host->get_extension(host,CLAP_EXT_AUDIO_PORTS);
//  ext->audio_ports_config  = (clap_host_audio_ports_config*) host->get_extension(host,CLAP_EXT_AUDIO_PORTS_CONFIG);
//  ext->event_filter        = (clap_host_event_filter*)       host->get_extension(host,CLAP_EXT_EVENT_FILTER);
//  ext->fd_support          = (clap_host_fd_support*)         host->get_extension(host,CLAP_EXT_FD_SUPPORT);
//  ext->gui                 = (clap_host_gui*)                host->get_extension(host,CLAP_EXT_GUI);
//  ext->latency             = (clap_host_latency*)            host->get_extension(host,CLAP_EXT_LATENCY);
//  ext->log                 = (clap_host_log*)                host->get_extension(host,CLAP_EXT_LOG);
//  ext->note_name           = (clap_host_note_name*)          host->get_extension(host,CLAP_EXT_NOTE_NAME);
//  ext->note_ports          = (clap_host_note_ports*)         host->get_extension(host,CLAP_EXT_NOTE_PORTS);
//  ext->params              = (clap_host_params*)             host->get_extension(host,CLAP_EXT_PARAMS);
//  ext->state               = (clap_host_state*)              host->get_extension(host,CLAP_EXT_STATE);
//  ext->thread_check        = (clap_host_thread_check*)       host->get_extension(host,CLAP_EXT_THREAD_CHECK);
//  ext->thread_pool         = (clap_host_thread_pool*)        host->get_extension(host,CLAP_EXT_THREAD_POOL);
//  ext->timer_support       = (clap_host_timer_support*)      host->get_extension(host,CLAP_EXT_TIMER_SUPPORT);
//  // draft
//  ext->check_for_update    = (clap_host_check_for_update*)   host->get_extension(host,CLAP_EXT_CHECK_FOR_UPDATE);
//  ext->file_reference      = (clap_host_file_reference*)     host->get_extension(host,CLAP_EXT_FILE_REFERENCE);
//  ext->midi_mappings       = (clap_host_midi_mappings*)      host->get_extension(host,CLAP_EXT_MIDI_MAPPINGS);
//  ext->quick_controls      = (clap_host_quick_controls*)     host->get_extension(host,CLAP_EXT_QUICK_CONTROLS);
//  ext->surround            = (clap_host_surround*)           host->get_extension(host,CLAP_EXT_SURROUND);
//  ext->track_info          = (clap_host_track_info*)         host->get_extension(host,CLAP_EXT_TRACK_INFO);
//  ext->tuning              = (clap_host_tuning*)             host->get_extension(host,CLAP_EXT_TUNING);
//}


//----------------------------------------------------------------------
#endif
