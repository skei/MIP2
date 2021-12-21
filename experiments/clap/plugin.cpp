
// nc -U -l -k /tmp/mip.socket
#define MIP_DEBUG_PRINT_SOCKET
#define MIP_NO_PLUGIN
#define MIP_NO_GUI
#define MIP_USE_XCB
#include "mip.h"

#include "CLAP_plugin_entry.h"
#include "CLAP_plugin.h"

int main() {
  return 0;
}

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin
: public CLAP_Plugin {

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor* desc, const clap_host *host)
  : CLAP_Plugin(desc,host) {
  }

  virtual ~myPlugin() {
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() override {
    MIP_PRINT;
    return true;
  }

  //----------

  void destroy() override {
    MIP_PRINT;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) override {
    MIP_PRINT;
    return true;
  }

  //----------

  void deactivate() override {
    MIP_PRINT;
  }

  //----------

  bool start_processing() override {
    MIP_PRINT;
    return true;
  }

  //----------

  void stop_processing() override {
    MIP_PRINT;
  }

  //----------

  clap_process_status process(const clap_process *process) override {
    //MIP_PRINT;

    // events

    if (process->in_events) {
      uint32_t num_events = process->in_events->size(process->in_events);
      for (uint32_t i=0; i<num_events; i++) {
        const clap_event* event = process->in_events->get(process->in_events,i);
        if (event) {
          switch (event->type) {

            case CLAP_EVENT_PARAM_VALUE:
              //handleParamValue(event);
              {
                uint32_t i = event->param_value.param_id;
                float v = event->param_value.value;
                if (i == 0) MParameterValue = (v*v);
              }
              break;

            //case CLAP_EVENT_NOTE_ON:          handleNoteOn(event);          break;
            //case CLAP_EVENT_NOTE_OFF:         handleNoteOff(event);         break;
            //case CLAP_EVENT_NOTE_END:         handleNoteEnd(event);         break;
            //case CLAP_EVENT_NOTE_CHOKE:       handleNoteChoke(event);       break;
            //case CLAP_EVENT_NOTE_EXPRESSION:  handleNoteExpression(event);  break;
            //case CLAP_EVENT_NOTE_MASK:        handleNoteMask(event);        break;
            //case CLAP_EVENT_PARAM_MOD:        handleParamMod(event);        break;
            //case CLAP_EVENT_TRANSPORT:        handleTransport(event);       break;
            //case CLAP_EVENT_MIDI:             handleMidi(event);            break;
            //case CLAP_EVENT_MIDI_SYSEX:       handleMidiSysex(event);       break;
          }
        }
      }
    }

    // process

    float* in0 = process->audio_inputs[0].data32[0];
    float* in1 = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<process->frames_count; i++) {
      *out0++ = *in0++ * MParameterValue;
      *out1++ = *in1++ * MParameterValue;
    }

    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  // return NULL for extensions you don't suooprt/want

  const void* get_extension(const char *id) override {
    MIP_Print("id: %s\n",id);

    if (strcmp(id, CLAP_EXT_AUDIO_PORTS         ) == 0) return &MClapAudioPorts;
    if (strcmp(id, CLAP_EXT_AUDIO_PORTS_CONFIG  ) == 0) return &MClapAudioPortsConfig;
  //if (strcmp(id, CLAP_EXT_CHECK_FOR_UPDATE    ) == 0) return &MClapCheckForUpdate;
    if (strcmp(id, CLAP_EXT_EVENT_FILTER        ) == 0) return &MClapEventFilter;
    if (strcmp(id, CLAP_EXT_FD_SUPPORT          ) == 0) return &MClapFdSupport;
    if (strcmp(id, CLAP_EXT_FILE_REFERENCE      ) == 0) return &MClapFileReference;
    if (strcmp(id, CLAP_EXT_GUI                 ) == 0) return &MClapGui;
    if (strcmp(id, CLAP_EXT_GUI_X11             ) == 0) return &MClapGuiX11;
    if (strcmp(id, CLAP_EXT_LATENCY             ) == 0) return &MClapLatency;
    if (strcmp(id, CLAP_EXT_MIDI_MAPPINGS       ) == 0) return &MClapMidiMappings;
    if (strcmp(id, CLAP_EXT_NOTE_NAME           ) == 0) return &MClapNoteName;
    if (strcmp(id, CLAP_EXT_NOTE_PORTS          ) == 0) return &MClapNotePorts;
    if (strcmp(id, CLAP_EXT_PARAMS              ) == 0) return &MClapParams;
    if (strcmp(id, CLAP_EXT_PRESET_LOAD         ) == 0) return &MClapPresetLoad;
    if (strcmp(id, CLAP_EXT_QUICK_CONTROLS      ) == 0) return &MClapQuickControls;
    if (strcmp(id, CLAP_EXT_RENDER              ) == 0) return &MClapRender;
    if (strcmp(id, CLAP_EXT_STATE               ) == 0) return &MClapState;
    if (strcmp(id, CLAP_EXT_TIMER_SUPPORT       ) == 0) return &MClapTimerSupport;
    if (strcmp(id, CLAP_EXT_THREAD_POOL         ) == 0) return &MClapThreadPool;
    if (strcmp(id, CLAP_EXT_TRACK_INFO          ) == 0) return &MClapTrackInfo;
    if (strcmp(id, CLAP_EXT_VST2_CONVERT        ) == 0) return &MClapVst2Convert;
    if (strcmp(id, CLAP_EXT_VST3_CONVERT        ) == 0) return &MClapVst3Convert;
    return nullptr;
  }

  //----------

  void on_main_thread() override {
    MIP_PRINT;
  }

//------------------------------
public: // extensions
//------------------------------

  //------------------------------
  // audio ports
  //------------------------------

  uint32_t audio_ports_count(bool is_input) override {
    MIP_PRINT;
    if (is_input) return 1;
    else return 1;
  }

  //----------

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info *info) override {
    MIP_PRINT;
    if (is_input) {
      info->id            = 0;
      strncpy(info->name,"input",CLAP_NAME_SIZE-1);
      info->channel_count = 2;
      info->channel_map   = CLAP_CHMAP_STEREO;
      info->sample_size   = 32;
      info->is_main       = true;
      info->is_cv         = false;
      info->in_place      = true;
      return true;
    }
    else { // output
      info->id            = 0;
      strncpy(info->name,"output",CLAP_NAME_SIZE-1);
      info->channel_count = 2;
      info->channel_map   = CLAP_CHMAP_STEREO;
      info->sample_size   = 32;
      info->is_main       = true;
      info->is_cv         = false;
      info->in_place      = true;
      return true;
    }
    return false;
  }

  //------------------------------
  // audio ports config
  //------------------------------

  uint32_t audio_ports_config_count() override {
    MIP_PRINT;
    return 1;
  }

  //----------

  bool audio_ports_config_get(uint32_t index, clap_audio_ports_config *config) override {
    MIP_PRINT;
    switch(index) {
      case 0:
        config->id                    = 0;
        strncpy(config->name,"port config 1",CLAP_NAME_SIZE-1);
        config->input_channel_count   = 2;
        config->input_channel_map     = CLAP_CHMAP_STEREO;
        config->output_channel_count  = 2;
        config->output_channel_map    = CLAP_CHMAP_STEREO;
        return true;
    }
    return false;
  }

  //----------

  bool audio_ports_config_select(clap_id config_id) override {
    MIP_PRINT;
    if (config_id == 0) {
      MSelectedPortConfig = config_id;
    }
    return true;
  }

  //------------------------------
  // check for update
  //------------------------------

  void check_for_update_check(bool include_beta) override {
    MIP_PRINT;
  }

  //------------------------------
  // event filter
  //------------------------------

  // return false for events you don't want

  bool event_filter_accepts(clap_event_type event_type) override {
    MIP_PRINT;
    switch (event_type) {
      case CLAP_EVENT_NOTE_ON:          return true;
      case CLAP_EVENT_NOTE_OFF:         return true;
      case CLAP_EVENT_NOTE_END:         return true;
      case CLAP_EVENT_NOTE_CHOKE:       return true;
      case CLAP_EVENT_NOTE_EXPRESSION:  return true;
      case CLAP_EVENT_NOTE_MASK:        return true;
      case CLAP_EVENT_PARAM_VALUE:      return true;
      case CLAP_EVENT_PARAM_MOD:        return true;
      case CLAP_EVENT_TRANSPORT:        return true;
      case CLAP_EVENT_MIDI:             return true;
      case CLAP_EVENT_MIDI_SYSEX:       return true;
    }
    return false;
  }

  //------------------------------
  // fd support
  //------------------------------

  void fd_support_on_fd(clap_fd fd, clap_fd_flags flags) override {
    MIP_PRINT;
  }

  //------------------------------
  // file reference
  //------------------------------

  uint32_t file_reference_count() override {
    MIP_PRINT;
    return 0;
  }

  //----------

  bool file_reference_get(uint32_t index, clap_file_reference *file_reference) override {
    MIP_PRINT;
    return false;
  }

  //----------

  bool file_reference_get_hash(clap_id resource_id, clap_hash hash, uint8_t* digest, uint32_t digest_size) override {
    MIP_PRINT;
    return false;
  }

  //----------

  bool file_reference_update_path(clap_id resource_id, const char *path) override {
    MIP_PRINT;
    return false;
  }

  //----------

  bool file_reference_save_resources() override {
    MIP_PRINT;
    return false;
  }

  //------------------------------
  // gui
  //------------------------------

  bool gui_create() override {
    MIP_PRINT;
    MEditor = new Editor();
    return true;
  }

  //----------

  void gui_destroy() override {
    MIP_PRINT;
    delete MEditor;
  }

  //----------

  void gui_set_scale(double scale) override {
    MIP_PRINT;
    MEditor->set_scale(scale);
  }

  //----------

  bool gui_get_size(uint32_t* width, uint32_t* height) override {
    MIP_PRINT;
    *width = MEditor->get_width();
    *width = MEditor->get_height();
    return true;
  }

  //----------

  bool gui_can_resize() override {
    MIP_PRINT;
    return false;
  }

  //----------

  void gui_round_size(uint32_t* width, uint32_t* height) override {
    MIP_PRINT;
    *width = MEditor->get_width();
    *width = MEditor->get_height();
  }

  //----------

  bool gui_set_size(uint32_t width, uint32_t height) override {
    MIP_PRINT;
    MEditor->set_width(width);
    MEditor->set_height(height);
    return true;
  }

  //----------

  void gui_show() override {
    MIP_PRINT;
    MEditor->show();
  }

  //----------

  void gui_hide() override {
    MIP_PRINT;
    MEditor->hide();
  }

  //------------------------------
  // gui x11
  //------------------------------

  bool gui_x11_attach(const char* display_name, unsigned long window) override {
    MIP_PRINT;
    return MEditor->attach(display_name,window);
  }

  //------------------------------
  // latency
  //------------------------------

  uint32_t latency_get() override {
    MIP_PRINT;
    return 0;
  }

  //------------------------------
  // midi mappings
  //------------------------------

  uint32_t midi_mappings_count() override {
    MIP_PRINT;
    return 0;
  }

  //----------

  bool midi_mappings_get(uint32_t index, clap_midi_mapping* mapping) override {
    MIP_PRINT;
    //mapping->channel  =
    //mapping->number   =
    //mapping->param_id =
    return false;
  }

  //------------------------------
  // note name
  //------------------------------

  uint32_t note_name_count() override {
    MIP_PRINT;
    return 0;
  }

  //----------

  bool note_name_get(uint32_t index, clap_note_name* note_name) override {
    MIP_PRINT;
    //strncpy(note_name->name,"note",CLAP_NAME_SIZE-1);
    //note_name->port     =
    //ote_name->key      =
    //note_name->channel  =
    return false;
  }

  //------------------------------
  // note-ports
  //------------------------------

  uint32_t note_ports_count(bool is_input) override {
    MIP_PRINT;
    return 0;
  }

  //----------

  bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info* info) override {
    MIP_PRINT;
    //info->id = 0;
    //strncpy(info->name,"port",CLAP_NAME_SIZE-1);
    return false;
  }

  //------------------------------
  // params
  //------------------------------

  uint32_t params_count() override {
    MIP_PRINT;
    return 1;
  }

  //----------

  bool params_get_info(int32_t param_index, clap_param_info *param_info) override {
    MIP_PRINT;
    if (param_index == 0) {
      param_info->id            = param_index;
      param_info->flags         = 0; // CLAP_PARAM_IS_HIDDEN, CLAP_PARAM_IS_READONLY, CLAP_PARAM_IS_MODULATABLE, CLAP_PARAM_IS_STEPPED, ..
      param_info->cookie        = nullptr;
      strncpy(param_info->name,"parameter",CLAP_NAME_SIZE-1);
      strncpy(param_info->module,"",CLAP_MODULE_SIZE-1);
      param_info->min_value     = 0.0;
      param_info->max_value     = 1.0;
      param_info->default_value = 0.5;
      return true;
    }
    return false;
  }

  //----------

  bool params_get_value(clap_id param_id, double* value) override {
    MIP_PRINT;
    *value = MParameterValue;;
    return true;
  }

  //----------

  bool params_value_to_text(clap_id param_id, double value, char* display, uint32_t size) override {
    MIP_PRINT;
    strncpy(display,"0.0 %",CLAP_NAME_SIZE-1);
    return false;
  }

  //----------

  bool params_text_to_value(clap_id param_id, const char* display, double* value) override {
    MIP_PRINT;
    float f = atof(display);
    *value = f;
    return true;
  }

  //----------

  void params_flush(const clap_event_list *input_parameter_changes, const clap_event_list *output_parameter_changes) override {
    MIP_PRINT;
  }

  //------------------------------
  // preset load
  //------------------------------

  bool preset_load_from_file(const char *path) override {
    MIP_PRINT;
    return false;
  }

  //------------------------------
  // quick controls
  //------------------------------

  uint32_t quick_controls_count() override {
    MIP_PRINT;
    return 0;
  }

  //----------

  //typedef struct clap_quick_controls_page {
  //   clap_id id;
  //   char    name[CLAP_NAME_SIZE];
  //   char    keywords[CLAP_KEYWORDS_SIZE];
  //   clap_id param_ids[CLAP_QUICK_CONTROLS_COUNT];
  //} clap_quick_controls_page;

  bool quick_controls_get(uint32_t page_index, clap_quick_controls_page *page) override {
    MIP_PRINT;
    return false;
  }

  //----------

  void quick_controls_select(clap_id page_id) override {
    MIP_PRINT;
  }

  //----------

  clap_id quick_controls_get_selected() override {
    MIP_PRINT;
    return 0;
  }

  //------------------------------
  // render
  //------------------------------

  //CLAP_RENDER_REALTIME
  //CLAP_RENDER_OFFLINE

  void render_set(clap_plugin_render_mode mode) override {
    MIP_PRINT;
  }

  //------------------------------
  // state
  //------------------------------

  //typedef struct clap_ostream {
  //   void *ctx;
  //   int64_t (*write)(struct clap_ostream *stream, const void *buffer, uint64_t size);
  //} clap_ostream

  bool state_save(clap_ostream *stream) override {
    MIP_PRINT;
    return true;
  }

  //----------

  //typedef struct clap_istream {
  //  void *ctx;
  //  int64_t (*read)(struct clap_istream *stream, void *buffer, uint64_t size);
  //} clap_istream;

  bool state_load(clap_istream *stream) override {
    MIP_PRINT;
    return true;
  }

  //------------------------------
  // thread pool
  //------------------------------

  void thread_pool_exec(uint32_t task_index) override {
    MIP_PRINT;
  }

  //------------------------------
  // timer support
  //------------------------------

  void timer_support_on_timer(clap_id timer_id) override {
    MIP_PRINT;
  }

  //------------------------------
  // track info
  //------------------------------

  void track_info_changed() override {
    MIP_PRINT;
  }

  //------------------------------
  // vst2 convert
  //------------------------------

  int32_t vst2_convert_get_vst2_plugin_id(uint32_t* vst2_plugin_id, char* name, uint32_t name_size) override {
    MIP_PRINT;
    return 0;
  }

  //----------

  bool vst2_convert_restore_vst2_state(clap_istream *stream) override {
    MIP_PRINT;
    return false;
  }

  //----------

  bool vst2_convert_convert_normalized_value(uint32_t vst2_param_id, double vst2_normalized_value, clap_id* clap_param_id, double* clap_plain_value) override {
    MIP_PRINT;
    return false;
  }

  //----------

  bool vst2_convert_convert_plain_value(uint32_t vst2_param_id, double vst2_plain_value, clap_id* clap_param_id, double* clap_plain_value) override {
    MIP_PRINT;
    return false;
  }

  //------------------------------
  // vst3 convert
  //------------------------------

  void vst3_convert_get_vst3_plugin_id(const clap_plugin *plugin, uint8_t *vst3_plugin_uuid) override {
    MIP_PRINT;
  }

  //----------

  bool vst3_convert_restore_vst3_state(const clap_plugin *plugin, clap_istream *stream) override {
    MIP_PRINT;
    return false;
  }

  //----------

  bool vst3_convert_convert_normalized_value(const clap_plugin *plugin, uint32_t vst3_param_id, double vst3_normalized_value, clap_id* clap_param_id, double* clap_plain_value) override {
    MIP_PRINT;
    return false;
  }

  //----------

  bool vst3_convert_convert_plain_value(const clap_plugin *plugin, uint32_t vst3_param_id, double vst3_plain_value, clap_id* clap_param_id, double* clap_plain_value) override {
    MIP_PRINT;
    return false;
  }



};

//----------------------------------------------------------------------
//
// plugin entry
//
//----------------------------------------------------------------------

class myPluginEntry
: public CLAP_PluginEntry {

private:

  clap_plugin_descriptor  MDescriptor = {
    CLAP_VERSION,
    "CLAP_Plugin",              // id
    "CLAP_Plugin",              // name
    "Tor-Helge Skei",           // vendor
    "https://torhelgeskei.com", // url
    "manual_url",               // manual url
    "support_url",              // support url
    "0.0.1",                    // version
    "clap test plugin",         // description
    "clap;test;plugin",         // keywords
    CLAP_PLUGIN_AUDIO_EFFECT
  };

public:

  bool init(const char *plugin_path) final {
    MIP_PRINT;
    return true;
  }

  void deinit(void) final {
    MIP_PRINT;
  }

  uint32_t get_plugin_count(void) final {
    MIP_PRINT;
    return 1;
  }

  const clap_plugin_descriptor* get_plugin_descriptor(uint32_t index) final {
    MIP_PRINT;
    return &MDescriptor;
  }

  const clap_plugin* create_plugin(const clap_host *host, const char *plugin_id) final {
    MIP_PRINT;
    myPlugin* plugin = new myPlugin(&MDescriptor,host);
    return plugin->getClapPlugin();
    //return nullptr;
  }

  uint32_t get_invalidation_source_count(void) final {
    MIP_PRINT;
    return 0;
  }

  const clap_plugin_invalidation_source* get_invalidation_source(uint32_t index) final {
    MIP_PRINT;
    return nullptr;
  }

  void refresh(void) final {
    MIP_PRINT;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//CLAP_PluginEntry MY_PLUGIN_ENTRY;
//CLAP_PLUGIN(MY_PLUGIN_ENTRY);

CLAP_PLUGIN(myPluginEntry);


