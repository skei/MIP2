#ifndef mip_clap_plugin_included
#define mip_clap_plugin_included
//----------------------------------------------------------------------

/*
  TODO: extreact MIP_Plugin to a class 'above' this,
        so that this stays completely clap-focused
*/

//----------------------------------------------------------------------

#include "mip.h"
#include "base/utils/mip_convert.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_process_context.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_host_proxy.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ClapPlugin {

//------------------------------
private:
//------------------------------

  MIP_Plugin*                     MPlugin           = nullptr;
  MIP_Descriptor*                 MDescriptor       = nullptr;
  MIP_ClapHostProxy*              MClapHostProxy    = nullptr;
  //const clap_host_t*              MClapHost         = nullptr;
  const clap_plugin_descriptor_t* MClapDescriptor   = nullptr;
  clap_plugin_render_mode         MClapRenderMode   = 0;//CLAP_RENDER_REALTIME;
  MIP_ProcessContext              MProcessContext   = {};
  float*                          MParameterValues  = nullptr;

//------------------------------
public:
//------------------------------

  // we must delete APlugin & clap host proxy

  //MIP_ClapPlugin(MIP_Plugin* APlugin, const clap_plugin_descriptor_t* descriptor, const clap_host_t* host) {
  MIP_ClapPlugin(MIP_Plugin* APlugin, const clap_plugin_descriptor_t* descriptor, MIP_ClapHostProxy* hostproxy) {
    MPlugin           = APlugin; // delete this!
    MDescriptor       = APlugin->getDescriptor();
    MClapDescriptor   = descriptor;
    MClapHostProxy    = hostproxy;
    //MClapHost         = hostproxy->getClapHost();
    MParameterValues  = (float*)malloc(MDescriptor->getNumParameters() * sizeof(float));
    MClapPlugin.desc  = descriptor;
  }

  //----------

  ~MIP_ClapPlugin() {
    if (MPlugin) delete MPlugin;
    if (MClapHostProxy) delete MClapHostProxy;
    free(MParameterValues);
  }

//------------------------------
public:
//------------------------------

  const clap_plugin_t* getClapPlugin() {
    return &MClapPlugin;
  }

//------------------------------
public:
//------------------------------

  void process_input_events(const clap_input_events_t* events) {
    uint32_t num_events = events->size(events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t*      event       = events->get(events,i);
      switch (event->type) {
        case CLAP_EVENT_NOTE_ON: {
          const clap_event_note_t* note_event = (clap_event_note_t*)event;
          //int32_t port = event->note.port_index;
          int32_t key = note_event->key;
          int32_t chan = note_event->channel;
          float value = note_event->velocity;
          MPlugin->on_plugin_midi(MIP_MIDI_NOTE_ON + chan,key,value * 127.0);
          break;
        }
        case CLAP_EVENT_NOTE_OFF: {
          const clap_event_note_t* note_event = (clap_event_note_t*)event;
          //int32_t port = event->note.port_index;
          int32_t key = note_event->key;
          int32_t chan = note_event->channel;
          float value = note_event->velocity;
          MPlugin->on_plugin_midi(MIP_MIDI_NOTE_OFF + chan,key,value * 127.0);
          break;
        }
        case CLAP_EVENT_NOTE_END: {
          //const clap_event_note_t* note_event = (clap_event_note_t*)event;
          break;
        }
        case CLAP_EVENT_NOTE_CHOKE: {
          //const clap_event_note_t* note_event = (clap_event_note_t*)event;
          break;
        }
        case CLAP_EVENT_NOTE_EXPRESSION: {
          //const clap_event_note_expression_t* expression_event = (clap_event_note_expression_t*)event;
          break;
        }
        //case CLAP_EVENT_NOTE_MASK: {
        //  break;
        //}
        case CLAP_EVENT_PARAM_VALUE: {
          const clap_event_param_value_t* param_event = (clap_event_param_value_t*)event;
          MParameterValues[param_event->param_id] = param_event->value;
          MPlugin->on_plugin_parameter(param_event->param_id,param_event->value);
          break;
        }
        case CLAP_EVENT_PARAM_MOD: {
          const clap_event_param_mod_t* mod_event = (clap_event_param_mod_t*)event;
          MPlugin->on_plugin_modulation(mod_event->param_id,mod_event->amount);
          break;
        }
        case CLAP_EVENT_TRANSPORT: {
          break;
        }
        case CLAP_EVENT_MIDI: {
          const clap_event_midi_t* midi_event = (clap_event_midi_t*)event;
          MPlugin->on_plugin_midi(midi_event->data[0],midi_event->data[1],midi_event->data[2]);
          break;
        }
        case CLAP_EVENT_MIDI_SYSEX: {
          //const clap_event_midi_sysex_t* sysex_event = (clap_event_midi_sysex_t*)event;
          break;
        }
      }
    }
  }

  //----------

  void process_output_events(const clap_output_events_t* events) {
  }

//------------------------------
public:
//------------------------------

  bool init() {
    return MPlugin->on_plugin_init();
  }

  //----------

  void destroy() {
    MPlugin->on_plugin_deinit();
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    MProcessContext.samplerate = sample_rate;
    return MPlugin->on_plugin_activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  void deactivate() {
    MPlugin->on_plugin_deactivate();
  }

  //----------

  bool start_processing() {
    return MPlugin->on_plugin_start_processing();
  }

  //----------

  void stop_processing() {
    return MPlugin->on_plugin_stop_processing();
  }

  //----------

  //TODO:
  // this is ugly!! fix this!! don't hardcode stuff!!

  clap_process_status process(const clap_process_t *process) {
    process_input_events(process->in_events);
    MProcessContext.num_inputs  = 2;  // MDescriptor->getNumAudioInputs();
    MProcessContext.num_outputs = 2;  // MDescriptor->getNumAudioOutputs();
    MProcessContext.num_samples = process->frames_count;
    MProcessContext.inputs      = process->audio_inputs[0].data32;
    MProcessContext.outputs     = process->audio_outputs[0].data32;
    MPlugin->on_plugin_process(&MProcessContext);

    //post-process: fluahMidi to host?

    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  const void* get_extension(const char *id) {
    //if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0)         { return &MExtAudioPorts; }
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS_CONFIG) == 0)  { return &MExtAudioPortsConfig; }
    //if (strcmp(id,CLAP_EXT_CHECK_FOR_UPDATE) == 0)    { return &MExtCheckForUpdate; }
    if (strcmp(id,CLAP_EXT_EVENT_FILTER) == 0)        { return &MExtEventFilter; }
    //if (strcmp(id,CLAP_EXT_FD_SUPPORT) == 0)          { return &MExtFdSupport; }
    //if (strcmp(id,CLAP_EXT_FILE_REFERENCE) == 0)      { return &MExtFileReference; }
    //if (strcmp(id,CLAP_EXT_GUI) == 0)                 { return &MExtGui; }
    //if (strcmp(id,CLAP_EXT_LATENCY) == 0)             { return &MExtLatency; }
//    if (strcmp(id,CLAP_EXT_LOG) == 0)                 { return &MExtLog; }
    //if (strcmp(id,CLAP_EXT_MIDI_MAPPINGS) == 0)       { return &MExtMidiMappings; }
    //if (strcmp(id,CLAP_EXT_NOTE_NAME) == 0)           { return &MExtNoteName; }
    //if (strcmp(id,CLAP_EXT_NOTE_PORTS) == 0)          { return &MExtNotePorts; }
    if (strcmp(id,CLAP_EXT_PARAMS) == 0)              { return &MExtParams; }
    //if (strcmp(id,CLAP_EXT_QUICK_CONTROLS) == 0)      { return &MExtQuickControls; }
    if (strcmp(id,CLAP_EXT_STATE) == 0)               { return &MExtState; }
//    if (strcmp(id,CLAP_EXT_THREAD_CHECK) == 0)        { return &MExtThreadCheck; }
    //if (strcmp(id,CLAP_EXT_THREAD_POOL) == 0)         { return &MExtThreadPool; }
    //if (strcmp(id,CLAP_EXT_TIMER_SUPPORT) == 0)       { return &MExtTimerSupport; }
    //if (strcmp(id,CLAP_EXT_TRACK_INFO) == 0)          { return &MExtTrackInfo; }
//    if (strcmp(id,CLAP_EXT_TUNING) == 0)              { return &MExtTuning; }
    return nullptr;
  }

  //----------

  void on_main_thread() {
  }

//------------------------------
public: // extensions
//------------------------------

  uint32_t audio_ports_count(bool is_input) {
    if (is_input) return MDescriptor->getNumAudioInputs();
    else return MDescriptor->getNumAudioOutputs();
    return 0;
  }

  //----------

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t* info) {
    if (is_input) {
      MIP_AudioPort* port = MDescriptor->getAudioInput(index);
      if (port) {
        strncpy(info->name,port->name,CLAP_NAME_SIZE-1);
        info->id            = index;
        info->channel_count = port->num_channels;
        info->channel_map   = CLAP_CHMAP_STEREO;
        info->sample_size   = 32;
        info->is_main       = true;
        info->is_cv         = false;
        info->in_place      = false;
        return true;
      }
    }
    else {
      MIP_AudioPort* port = MDescriptor->getAudioOutput(index);
      if (port) {
        strncpy(info->name,port->name,CLAP_NAME_SIZE-1);
        info->id            = 0;
        info->channel_count = port->num_channels;
        info->channel_map   = CLAP_CHMAP_STEREO;
        info->sample_size   = 32;
        info->is_main       = true;
        info->is_cv         = false;
        info->in_place      = false;
        return true;
      }
    }
    return false;
  }

  //----------

  uint32_t audio_ports_config_count() {
    return 0;
  }

  //----------

  bool audio_ports_config_get(uint32_t index, clap_audio_ports_config_t *config) {
    return false;
  }

  //----------

  bool audio_ports_config_select(clap_id config_id) {
    return false;
  }

  //----------

  bool event_filter_accepts(uint16_t space_id, uint16_t event_type) {
    switch (event_type) {
      case CLAP_EVENT_NOTE_ON:          return true;
      case CLAP_EVENT_NOTE_OFF:         return true;
      case CLAP_EVENT_NOTE_END:         return true;
      case CLAP_EVENT_NOTE_CHOKE:       return true;
      case CLAP_EVENT_NOTE_EXPRESSION:  return true;
      //case CLAP_EVENT_NOTE_MASK:        return true;
      case CLAP_EVENT_PARAM_VALUE:      return true;
      case CLAP_EVENT_PARAM_MOD:        return true;
      case CLAP_EVENT_TRANSPORT:        return true;
      case CLAP_EVENT_MIDI:             return true;
      case CLAP_EVENT_MIDI_SYSEX:       return true;
    }
    return false;
  }

  //----------

//  void fd_support_on_fd(clap_fd fd, clap_fd_flags flags) {
//  }

  //----------

  bool gui_create() {
    return false;
  }

  //----------

  void gui_destroy() {
  }

  //----------

  bool gui_set_scale(double scale) {
    return false;
  }

  //----------

  bool gui_get_size(uint32_t *width, uint32_t *height) {
    return false;
  }

  //----------

  bool gui_can_resize() {
    return false;
  }

  //----------

  void gui_round_size(uint32_t *width, uint32_t *height) {
  }

  //----------

  bool gui_set_size(uint32_t width, uint32_t height) {
    return false;
  }

  //----------

  void gui_show() {
  }

  //----------

  void gui_hide() {
  }

  //----------

  bool gui_x11_attach(const char *display_name, unsigned long window) {
    return false;
  }

  //----------

  uint32_t latency_get() {
    return 0;
  }

  //----------

  uint32_t note_name_count() {
    return 0;
  }

  //----------

  bool note_name_get(uint32_t index, clap_note_name_t *note_name) {
    return false;
  }

  //----------

  uint32_t note_ports_count(bool is_input) {
    return 0;
  }

  //----------

  bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t *info) {
    return false;
  }

  //----------

  uint32_t params_count() {
    return MDescriptor->getNumParameters();
  }

  //----------

  bool params_get_info(int32_t param_index, clap_param_info_t* param_info) {
    MIP_Parameter* param = MDescriptor->getParameter(param_index);
    if (param) {
      strncpy(param_info->name,param->getName(),CLAP_NAME_SIZE-1);
      strncpy(param_info->module,"Module?",CLAP_MODULE_SIZE-1);
      param_info->id            = param_index;
      param_info->flags         = 0;
      if (param->canModulate()) param_info->flags |= CLAP_PARAM_IS_MODULATABLE;
      param_info->cookie        = nullptr; // param?
      param_info->min_value     = param->getMinValue();
      param_info->max_value     = param->getMaxValue();
      param_info->default_value = param->getDefValue();
      return true;
    }
    return false;
  }

  //----------

  bool params_get_value(clap_id param_id, double *value) {
    *value = MParameterValues[param_id];
    return true;
  }

  //----------

  bool params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) {
    MIP_FloatToString(display,value,3);
    return true;
  }

  //----------

  bool params_text_to_value(clap_id param_id, const char *display, double *value) {
    *value = MIP_StringToFloat((char*)display);
    return true;
  }

  //----------

  void params_flush(const clap_input_events_t* in, const clap_output_events_t* out) {
    process_input_events(in);
    //process_output_events(out);
  }

  //----------

  void render_set(clap_plugin_render_mode mode) {
    MClapRenderMode = mode;
  }

  //----------

  bool state_save(clap_ostream_t *stream) {
    //stream->write(stream,version,sizeof(float));
    uint32_t num = MDescriptor->getNumParameters();
    stream->write(stream, &num ,sizeof(uint32_t));
    for (uint32_t i=0; i<num; i++) {
      stream->write(stream,&MParameterValues[i],sizeof(float));
    }
    return true;
  }

  //----------

  bool state_load(clap_istream_t *stream) {
    //stream->read(stream, version ,sizeof(float));
    uint32_t num;
    stream->read(stream,&num,sizeof(uint32_t));
    for (uint32_t i=0; i<num; i++) {
      stream->read(stream,&MParameterValues[i],sizeof(float));
    }
    return true;
  }

  //----------

  void thread_pool_exec(uint32_t task_index) {
  }

  //----------

  void timer_support_on_timer(clap_id timer_id) {
    MPlugin->on_plugin_update_editor();
  }

//------------------------------
public: // drafts
//------------------------------

  void check_for_update_check(const clap_host_t *host, bool include_beta) {
  }

  //----------

  uint32_t file_reference_count() {
    return 0;
  }

  //----------

  bool file_reference_get(uint32_t index, clap_file_reference_t *file_reference) {
    return false;
  }

  //----------

  bool file_reference_get_hash(clap_id resource_id, clap_hash hash, uint8_t* digest, uint32_t digest_size) {
    return false;
  }

  //----------

  bool file_reference_update_path(clap_id resource_id, const char *path) {
    return false;
  }

  //----------

  bool file_reference_save_resources() {
    return false;
  }

  //----------

  uint32_t midi_mappings_count() {
    return 0;
  }

  //----------

  bool midi_mappings_get(uint32_t index, clap_midi_mapping_t *mapping) {
    return false;
  }

  //----------

  bool preset_load_from_file(const char *path) {
    return false;
  }

  //----------

  uint32_t quick_controls_count() {
    return 0;
  }

  //----------

  bool quick_controls_get(uint32_t page_index, clap_quick_controls_page_t *page) {
    return false;
  }

  //----------

  void quick_controls_select(clap_id page_id) {
  }

  //----------

  clap_id quick_controls_get_selected() {
    return 0;
  }

  //----------

  uint32_t surround_get_channel_type(bool is_input, uint32_t port_index, uint32_t channel_index) {
    return 0;
  }

  //----------

  void track_info_changed() {
  }

//------------------------------
private: // callbacks
//------------------------------

  static bool clap_plugin_init_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->init();
  }

  static void clap_plugin_destroy_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->destroy();
  }

  static bool clap_plugin_activate_callback(const struct clap_plugin *plugin, double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->activate(sample_rate,min_frames_count,max_frames_count);
  }

  static void clap_plugin_deactivate_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->deactivate();
  }

  static bool clap_plugin_start_processing_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->start_processing();
  }

  static void clap_plugin_stop_processing_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    plug->stop_processing();
  }

  static clap_process_status clap_plugin_process_callback(const struct clap_plugin *plugin, const clap_process_t *process) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->process(process);
  }

  static const void* clap_plugin_get_extension_callback(const struct clap_plugin *plugin, const char *id) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->get_extension(id);
  }

  static void clap_plugin_on_main_thread_callback(const struct clap_plugin *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->on_main_thread();
  }

  //----------

  //const
  clap_plugin_t MClapPlugin = {
    nullptr,
    this,
    clap_plugin_init_callback,
    clap_plugin_destroy_callback,
    clap_plugin_activate_callback,
    clap_plugin_deactivate_callback,
    clap_plugin_start_processing_callback,
    clap_plugin_stop_processing_callback,
    clap_plugin_process_callback,
    clap_plugin_get_extension_callback,
    clap_plugin_on_main_thread_callback
  };

//------------------------------
private: // extensions
//------------------------------

  //--------------------
  // clap.audio-ports
  //--------------------

  static uint32_t clap_plugin_audio_ports_count_callback(const clap_plugin_t* plugin, bool is_input) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_count(is_input);
  }

  static bool clap_plugin_audio_ports_get_callback(const clap_plugin_t* plugin, uint32_t index, bool is_input, clap_audio_port_info_t* info) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_get(index,is_input,info);
  }

  clap_plugin_audio_ports_t MExtAudioPorts = {
    clap_plugin_audio_ports_count_callback,
    clap_plugin_audio_ports_get_callback
  };

  //--------------------
  // clap.audio-ports-config
  //--------------------

  static uint32_t clap_plugin_audio_ports_config_count_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_config_count();
  }

  static bool clap_plugin_audio_ports_config_get_callback(const clap_plugin_t *plugin, uint32_t index, clap_audio_ports_config_t *config) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_config_get(index,config);
  }

  static bool clap_plugin_audio_ports_config_select_callback(const clap_plugin_t *plugin, clap_id config_id) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_config_select(config_id);
  }

  clap_plugin_audio_ports_config_t MExtAudioPortsConfig = {
    clap_plugin_audio_ports_config_count_callback,
    clap_plugin_audio_ports_config_get_callback,
    clap_plugin_audio_ports_config_select_callback
  };

  //--------------------
  // clap.event-filter
  //--------------------

  static bool clap_plugin_event_filter_accepts_callback(const clap_plugin_t *plugin, uint16_t space_id, uint16_t event_type) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->event_filter_accepts(space_id,event_type);
  }

  clap_plugin_event_filter_t MExtEventFilter = {
    clap_plugin_event_filter_accepts_callback
  };

  //--------------------
  // clap.fd-support
  //--------------------

//  static void clap_plugin_fd_support_on_fd_callback(const clap_plugin_t *plugin, clap_fd fd, clap_fd_flags flags) {
//    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
//    return plug->fd_support_on_fd(fd,flags);
//  }
//
//  clap_plugin_fd_support_t MExtFdSupport = {
//    clap_plugin_fd_support_on_fd_callback
//  };

  //--------------------
  // clap.gui
  //--------------------

  static bool clap_plugin_gui_create_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_create();
  }

  static void clap_plugin_gui_destroy_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_destroy();
  }

  static bool clap_plugin_gui_set_scale_callback(const clap_plugin_t *plugin, double scale) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_set_scale(scale);
  }

  static bool clap_plugin_gui_get_size_callback(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_get_size(width,height);
  }

  static bool clap_plugin_gui_can_resize_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_can_resize();
  }

  static void clap_plugin_gui_round_size_callback(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_round_size(width,height);
  }

  static bool clap_plugin_gui_set_size_callback(const clap_plugin_t *plugin, uint32_t width, uint32_t height) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_set_size(width,height);
  }

  static void clap_plugin_gui_show_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_show();
  }

  static void clap_plugin_gui_hide_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_hide();
  }

  clap_plugin_gui_t MExtGui = {
    clap_plugin_gui_create_callback,
    clap_plugin_gui_destroy_callback,
    clap_plugin_gui_set_scale_callback,
    clap_plugin_gui_get_size_callback,
    clap_plugin_gui_can_resize_callback,
    clap_plugin_gui_round_size_callback,
    clap_plugin_gui_set_size_callback,
    clap_plugin_gui_show_callback,
    clap_plugin_gui_hide_callback
  };

  //--------------------
  // clap.gui-x11
  //--------------------

  static bool clap_plugin_gui_x11_attach_callback(const clap_plugin_t *plugin, const char *display_name, unsigned long window) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->gui_x11_attach(display_name,window);
  }

  clap_plugin_gui_x11_t MExtGuiX11 = {
    clap_plugin_gui_x11_attach_callback
  };

  //--------------------
  // clap.latency
  //--------------------

  static uint32_t clap_plugin_latency_get_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->latency_get();
  }

  clap_plugin_latency_t MExtLatency = {
    clap_plugin_latency_get_callback
  };

  //--------------------
  // clap.note_name
  //--------------------

  static uint32_t clap_plugin_note_name_count_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->note_name_count();
  }

  static bool clap_plugin_note_name_get_callback(const clap_plugin_t *plugin, uint32_t index, clap_note_name_t *note_name) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->note_name_get(index,note_name);
  }

  clap_plugin_note_name MExtNoteName = {
    clap_plugin_note_name_count_callback,
    clap_plugin_note_name_get_callback
  };

  //--------------------
  // clap.note_ports
  //--------------------

  static uint32_t clap_plugin_note_ports_count_callback(const clap_plugin_t *plugin, bool is_input) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->note_ports_count(is_input);
  }

  static bool clap_plugin_note_ports_get_callback(const clap_plugin_t* plugin, uint32_t index, bool is_input, clap_note_port_info_t *info) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->note_ports_get(index,is_input,info);
  }

  clap_plugin_note_ports_t MExtNotePorts = {
    clap_plugin_note_ports_count_callback,
    clap_plugin_note_ports_get_callback
  };

  //--------------------
  // clap.params
  //--------------------

  static uint32_t clap_plugin_params_count_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->params_count();
  }

  static bool clap_plugin_params_get_info_callback(const clap_plugin_t *plugin, int32_t param_index, clap_param_info_t* param_info) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->params_get_info(param_index,param_info);
  }

  static bool clap_plugin_params_get_value_callback(const clap_plugin_t *plugin, clap_id param_id, double *value) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->params_get_value(param_id,value);
  }

  static bool clap_plugin_params_value_to_text_callback(const clap_plugin_t *plugin, clap_id param_id, double value, char *display, uint32_t size) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->params_value_to_text(param_id,value,display,size);
  }

  static bool clap_plugin_params_text_to_value_callback(const clap_plugin_t *plugin, clap_id param_id, const char *display, double *value) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->params_text_to_value(param_id,display,value);
  }

  static void clap_plugin_params_flush_callback(const clap_plugin_t* plugin, const clap_input_events_t* in, const clap_output_events_t* out) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->params_flush(in,out);
  }

  clap_plugin_params_t MExtParams = {
    clap_plugin_params_count_callback,
    clap_plugin_params_get_info_callback,
    clap_plugin_params_get_value_callback,
    clap_plugin_params_value_to_text_callback,
    clap_plugin_params_text_to_value_callback,
    clap_plugin_params_flush_callback
  };

  //--------------------
  // clap.render
  //--------------------

  static void clap_plugin_render_set_callback(const clap_plugin_t *plugin, clap_plugin_render_mode mode) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->render_set(mode);
  }

  clap_plugin_render_t MExtRender = {
    clap_plugin_render_set_callback
  };

  //--------------------
  // clap.state
  //--------------------

  static bool clap_plugin_state_save_callback(const clap_plugin_t *plugin, clap_ostream_t *stream) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->state_save(stream);
  }

  static bool clap_plugin_state_load_callback(const clap_plugin_t *plugin, clap_istream_t *stream) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->state_load(stream);
  }

  clap_plugin_state_t MExtState {
    clap_plugin_state_save_callback,
    clap_plugin_state_load_callback
  };

  //--------------------
  // clap.thread-pool
  //--------------------

  static void clap_plugin_thread_pool_exec_callback(const clap_plugin_t *plugin, uint32_t task_index) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->thread_pool_exec(task_index);
  }

  clap_plugin_thread_pool_t MExtThreadPool = {
    clap_plugin_thread_pool_exec_callback
  };

  //--------------------
  // clap.timer-support
  //--------------------

  static void clap_plugin_timer_support_on_timer_callback(const clap_plugin_t *plugin, clap_id timer_id) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->timer_support_on_timer(timer_id);
  }

  clap_plugin_timer_support_t MExtTimerSupport = {
    clap_plugin_timer_support_on_timer_callback
  };

//------------------------------
// drafts
//------------------------------

  //--------------------
  // clap.check-for-update.draft/0
  //--------------------

//  static void clap_plugin_check_for_update_check_callback(const clap_host_t *host, bool include_beta) {
////    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
//    return plug->
//  }
//
//  clap_plugin_check_for_update MExtCheckForUpdate = {
//    clap_plugin_check_for_update_check_callback
//  };

  //--------------------
  // clap.file-reference.draft/0
  //--------------------

  static uint32_t clap_plugin_file_reference_count_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->file_reference_count();
  }

  static bool clap_plugin_file_reference_get_callback(const clap_plugin_t *plugin, uint32_t index, clap_file_reference_t *file_reference) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->file_reference_get(index,file_reference);
  }

  static bool clap_plugin_file_reference_get_hash_callback(const clap_plugin_t *plugin, clap_id resource_id, clap_hash hash, uint8_t* digest, uint32_t digest_size) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->file_reference_get_hash(resource_id,hash,digest,digest_size);
  }

  static bool clap_plugin_file_reference_update_path_callback(const clap_plugin_t *plugin, clap_id resource_id, const char *path) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->file_reference_update_path(resource_id,path);
  }

  static bool clap_plugin_file_reference_save_resources_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->file_reference_save_resources();
  }

  clap_plugin_file_reference_t MExtFileReference = {
    clap_plugin_file_reference_count_callback,
    clap_plugin_file_reference_get_callback,
    clap_plugin_file_reference_get_hash_callback,
    clap_plugin_file_reference_update_path_callback,
    clap_plugin_file_reference_save_resources_callback
  };

  //--------------------
  // clap.midi-mappings.draft/0
  //--------------------

  static uint32_t clap_plugin_midi_mappings_count_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->midi_mappings_count();
  }
  static bool clap_plugin_midi_mappings_get_callback(const clap_plugin_t *plugin, uint32_t index, clap_midi_mapping_t *mapping) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->midi_mappings_get(index,mapping);
  }

  clap_plugin_midi_mappings_t MExtMidiMappings = {
    clap_plugin_midi_mappings_count_callback,
    clap_plugin_midi_mappings_get_callback
  };

  //--------------------
  // clap.preset-load.draft/0
  //--------------------

  static bool clap_plugin_preset_load_from_file_callback(const clap_plugin_t *plugin, const char *path) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->preset_load_from_file(path);
  }

  clap_plugin_preset_load_t MExtPresetLoad = {
    clap_plugin_preset_load_from_file_callback
  };

  //--------------------
  // clap.quick-controls.draft/0
  //--------------------

  static uint32_t clap_plugin_quick_controls_count_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->quick_controls_count();
  }

  static bool clap_plugin_quick_controls_get_callback(const clap_plugin_t *plugin, uint32_t page_index, clap_quick_controls_page_t *page) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->quick_controls_get(page_index,page);
  }

  static void clap_plugin_quick_controls_select_callback(const clap_plugin_t *plugin, clap_id page_id) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->quick_controls_select(page_id);
  }

  static clap_id clap_plugin_quick_controls_get_selected_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->quick_controls_get_selected();
  }

  clap_plugin_quick_controls_t MExtQuickControls = {
    clap_plugin_quick_controls_count_callback,
    clap_plugin_quick_controls_get_callback,
    clap_plugin_quick_controls_select_callback,
    clap_plugin_quick_controls_get_selected_callback
  };

  //--------------------
  // clap.surround.draft/0
  //--------------------

//   static uint32_t clap_plugin_surround_get_channel_map_callback(const clap_plugin_t *plugin, bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) {
//      MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
//      return plug->surround_get_channel_map(is_input,port_index,channel_map,channel_map_capacity);
//    return 0;
//   }
//
//   static void clap_plugin_surround_changed_callback(const clap_plugin_t *plugin) {
//   }
//
//  clap_plugin_surround_t MExtSurround = {
//    clap_plugin_surround_get_channel_map_callback,
//    clap_plugin_surround_changed_callback
//
//  };

  //--------------------
  // clap.track-info.draft/0
  //--------------------

  static void clap_plugin_track_info_changed_callback(const clap_plugin_t *plugin) {
    MIP_ClapPlugin* plug = (MIP_ClapPlugin*)plugin->plugin_data;
    return plug->track_info_changed();
  }

  clap_plugin_track_info_t MExtTrackInfo = {
    clap_plugin_track_info_changed_callback
  };

};

//----------------------------------------------------------------------
#endif


