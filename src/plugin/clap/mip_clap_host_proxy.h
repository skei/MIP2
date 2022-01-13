#ifndef mip_clap_host_proxy_included
#define mip_clap_host_proxy_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------

//struct MIP_ClapHostExt {
//  clap_host_audio_ports*        audio_ports         = nullptr;
//  clap_host_audio_ports_config* audio_ports_config  = nullptr;
//  clap_host_event_filter*       event_filter        = nullptr;
////  clap_host_fd_support*         fd_support          = nullptr;
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
//  // draft
//  clap_host_check_for_update*   check_for_update    = nullptr;
//  clap_host_file_reference*     file_reference      = nullptr;
//  clap_host_midi_mappings*      midi_mappings       = nullptr;
//  clap_host_quick_controls*     quick_controls      = nullptr;
//  clap_host_surround*           surround            = nullptr;
//  clap_host_track_info*         track_info          = nullptr;
//  clap_host_tuning*             tuning              = nullptr;
//};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ClapHostProxy
: public MIP_HostProxy {

//------------------------------
private:
//------------------------------

  const clap_host*  MHost     = nullptr;
  const char*       MName     = "";
  const char*       MVendor   = "";
  const char*       MUrl      = "";
  const char*       MVersion  = "";

public:

  //MIP_ClapHostExt   ext       = {0};
  clap_host_audio_ports*        audio_ports         = nullptr;
  clap_host_audio_ports_config* audio_ports_config  = nullptr;
  clap_host_event_filter*       event_filter        = nullptr;
//  clap_host_fd_support*         fd_support          = nullptr;
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

//------------------------------
public:
//------------------------------

  // TODO: check clap version

  MIP_ClapHostProxy(const clap_host* host) {
    MHost       = host;
    MName      = host->name;
    MVendor    = host->vendor;
    MUrl       = host->url;
    MVersion   = host->version;
    init_extensions();
  }

  //----------

  ~MIP_ClapHostProxy() {
  }

  //

  uint32_t getPluginFormat() final {
    return MIP_PLUGIN_FORMAT_CLAP;
  }

  const clap_host* getClapHost() {
    return MHost;
  }

//------------------------------
public: // host proxy
//------------------------------

  void updateParameter(uint32_t AIndex, float AValue) final {
    //MIP_PRINT;
  }

  void resizeEditor(uint32_t AWidth, uint32_t AHeight) final {
    //MIP_PRINT;
  }

  void sendMidi(uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) final {
    //MIP_PRINT;
    //queueMidi/flushMidi
  }

//------------------------------
public:
//------------------------------

  const char* getName()             { return MName; }
  const char* getVendor()           { return MVendor; }
  const char* getUrl()              { return MUrl; }
  const char* getVersion()          { return MVersion; }

//  bool has_ext_audio_ports()        { return (ext.audio_ports != nullptr); }
//  bool has_ext_audio_ports_config() { return (ext.audio_ports_config != nullptr); }
//  bool has_ext_event_filter()       { return (ext.event_filter != nullptr); }
////  bool has_ext_fd_support()         { return (ext.fd_support != nullptr); }
//  bool has_ext_gui()                { return (ext.gui != nullptr); }
//  bool has_ext_latency()            { return (ext.latency != nullptr); }
//  bool has_ext_log()                { return (ext.log != nullptr); }
//  bool has_ext_note_name()          { return (ext.note_name != nullptr); }
//  bool has_ext_note_ports()         { return (ext.note_ports != nullptr); }
//  bool has_ext_params()             { return (ext.params != nullptr); }
//  bool has_ext_state()              { return (ext.state != nullptr); }
//  bool has_ext_thread_check()       { return (ext.thread_check != nullptr); }
//  bool has_ext_thread_pool()        { return (ext.thread_pool != nullptr); }
//  bool has_ext_timer_support()      { return (ext.timer_support != nullptr); }
//  // draft
//  bool has_ext_check_for_update()   { return (ext.check_for_update != nullptr); }
//  bool has_ext_file_reference()     { return (ext.file_reference != nullptr); }
//  bool has_ext_midi_mappings()      { return (ext.midi_mappings != nullptr); }
//  bool has_ext_quick_controls()     { return (ext.quick_controls != nullptr); }
//  bool has_ext_surround()           { return (ext.surround != nullptr); }
//  bool has_ext_track_info()         { return (ext.track_info != nullptr); }
//  bool has_ext_tuning()             { return (ext.tuning != nullptr); }

  //----------

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
public: // extensions
//------------------------------

  // audio ports

  uint32_t audio_ports_get_preferred_sample_size() {
    if (audio_ports) return audio_ports->get_preferred_sample_size(MHost);
    return 0;
  }

  void audio_ports_rescan(uint32_t flags) {
    if (audio_ports) audio_ports->rescan(MHost,flags);
  }

  // audio_ports_config

  void audio_ports_config_rescan() {
    if (audio_ports_config) audio_ports_config->rescan(MHost);
  }

  // event_filter

  void event_filter_changed() {
    if (event_filter) event_filter->changed(MHost);
  }

  // fd_support


//  bool fd_support_register_fd(clap_fd fd, clap_fd_flags flags) {
//    if (fd_support) return fd_support->register_fd(MHost,fd,flags);
//    return false;
//  }
//
//  bool fd_support_modify_fd(clap_fd fd, clap_fd_flags flags) {
//    if (fd_support) return fd_support->modify_fd(MHost,fd,flags);
//    return false;
//  }
//
//  bool fd_support_unregister_fd(clap_fd fd) {
//    if (fd_support) return fd_support->unregister_fd(MHost,fd);
//    return false;
//  }

  // gui

//  bool gui_resize(uint32_t width, uint32_t height) {
//    if (gui) return gui->resize(MHost,width,height);
//    return false;
//  }

  // latency

  void latency_changed() {
    if (latency) latency->changed(MHost);
  }

  // log

  void log_log(clap_log_severity severity, const char *msg) {
    if (log) log->log(MHost,severity,msg);
  }

  // note_name

  void note_name_changed() {
    if (note_name) note_name->changed(MHost);
  }

  // note_ports

  void note_ports_changed(uint32_t flags) {
    if (note_ports) note_ports->rescan(MHost,flags);
  }

  // params

  void params_rescan(clap_param_rescan_flags flags) {
    if (params) params->rescan(MHost,flags);
  }

  void params_clear(clap_id param_id, clap_param_clear_flags flags) {
    if (params) params->clear(MHost,param_id,flags);
  }

  void params_request_flush() {
    if (params) params->request_flush(MHost);
  }

  // state

  void state_mark_dirty() {
    if (state) state->mark_dirty(MHost);
  }

  // thread_check

  bool thread_check_is_main_thread() {
    if (thread_check) return thread_check->is_main_thread(MHost);
    return true;
  }

  bool thread_check_is_audio_thread() {
    if (thread_check) return thread_check->is_audio_thread(MHost);
    return true;
  }

  // thread_pool

  void thread_pool_request_flush(uint32_t num_tasks) {
    if (thread_pool) thread_pool->request_exec(MHost,num_tasks);
  }

  // timer_support

  bool timer_support_register_timer(uint32_t period_ms, clap_id *timer_id) {
    if (timer_support) return timer_support->register_timer(MHost,period_ms,timer_id);
    return false;
  }

  bool timer_support_unregister_timer(clap_id timer_id) {
    if (timer_support) return timer_support->unregister_timer(MHost,timer_id);
    return false;
  }

//------------------------------
public: // draft
//------------------------------

  // check_for_update

  void check_for_update_on_new_version(const clap_check_for_update_info_t *update_info) {
    if (check_for_update) check_for_update->on_new_version(MHost,update_info);
  }

  // file_reference


  void file_reference_changed() {
    if (file_reference) file_reference->changed(MHost);
  }

  void file_reference_set_dirty(clap_id resource_id) {
    if (file_reference) file_reference->set_dirty(MHost,resource_id);
  }

  // midi_mappings

  void midi_mappings_changed() {
    if (midi_mappings) midi_mappings->changed(MHost);
  }

  // quick_controls

  void quick_controls_changed(clap_quick_controls_changed_flags flags) {
    if (quick_controls) quick_controls->changed(MHost,flags);
  }

  // surround

  void surround_changed() {
    if (surround) surround->changed(MHost);
  }

  // track_info

  bool track_info_get(clap_track_info_t *info) {
    if (track_info) return track_info->get(MHost,info);
    return false;
  }

  // tuning

  double tuning_get(int32_t key, int32_t channel) {
    if (tuning) return tuning->get(MHost,key,channel);
    return 0.0;
  }

//------------------------------
private:
//------------------------------

  void init_extensions() {
    //MIP_PRINT;
    audio_ports        = (clap_host_audio_ports*)        get_extension(CLAP_EXT_AUDIO_PORTS);
    audio_ports_config = (clap_host_audio_ports_config*) get_extension(CLAP_EXT_AUDIO_PORTS_CONFIG);
    event_filter       = (clap_host_event_filter*)       get_extension(CLAP_EXT_EVENT_FILTER);
//    fd_support         = (clap_host_fd_support*)         get_extension(CLAP_EXT_FD_SUPPORT);
    gui                = (clap_host_gui*)                get_extension(CLAP_EXT_GUI);
    latency            = (clap_host_latency*)            get_extension(CLAP_EXT_LATENCY);
    log                = (clap_host_log*)                get_extension(CLAP_EXT_LOG);
    note_name          = (clap_host_note_name*)          get_extension(CLAP_EXT_NOTE_NAME);
    note_ports         = (clap_host_note_ports*)         get_extension(CLAP_EXT_NOTE_PORTS);
    params             = (clap_host_params*)             get_extension(CLAP_EXT_PARAMS);
    state              = (clap_host_state*)              get_extension(CLAP_EXT_STATE);
    thread_check       = (clap_host_thread_check*)       get_extension(CLAP_EXT_THREAD_CHECK);
    thread_pool        = (clap_host_thread_pool*)        get_extension(CLAP_EXT_THREAD_POOL);
    timer_support      = (clap_host_timer_support*)      get_extension(CLAP_EXT_TIMER_SUPPORT);
    // draft
    check_for_update   = (clap_host_check_for_update*)   get_extension(CLAP_EXT_CHECK_FOR_UPDATE);
    file_reference     = (clap_host_file_reference*)     get_extension(CLAP_EXT_FILE_REFERENCE);
    midi_mappings      = (clap_host_midi_mappings*)      get_extension(CLAP_EXT_MIDI_MAPPINGS);
    quick_controls     = (clap_host_quick_controls*)     get_extension(CLAP_EXT_QUICK_CONTROLS);
    surround           = (clap_host_surround*)           get_extension(CLAP_EXT_SURROUND);
    track_info         = (clap_host_track_info*)         get_extension(CLAP_EXT_TRACK_INFO);
    tuning             = (clap_host_tuning*)             get_extension(CLAP_EXT_TUNING);
  }

};

//----------------------------------------------------------------------
#endif

