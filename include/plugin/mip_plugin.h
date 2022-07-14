#ifndef mip_plugin_included
#define mip_plugin_included
//----------------------------------------------------------------------

// TODO: #ifndef MIP_NO_GUI around all editor stuff..

#include "mip.h"
#include "base/types/mip_queue.h"
#include "plugin/mip_audio_port.h"
#include "plugin/mip_editor.h"
#include "plugin/mip_note_port.h"
#include "plugin/mip_parameter_manager.h"
#include "plugin/clap/mip_clap_plugin.h"
#include "plugin/clap/mip_clap_host.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct MIP_ProcessContext {
  const clap_process_t* process     = nullptr;
  double                samplerate  = 0.0;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Plugin
: public MIP_ClapPlugin
, public MIP_EditorListener {

//------------------------------
protected:
//------------------------------

  MIP_AudioPortArray              MAudioInputPorts          = {};
  MIP_AudioPortArray              MAudioOutputPorts         = {};
  MIP_NotePortArray               MNoteInputPorts           = {};
  MIP_NotePortArray               MNoteOutputPorts          = {};
  MIP_ParameterManager            MParameters               = {};
  MIP_ProcessContext              MProcessContext           = {};
  uint32_t                        MSelectedAudioPortsConfig = 0;
  int32_t                         MRenderMode               = CLAP_RENDER_REALTIME;
  uint32_t                        MEditorWidth              = 300;
  uint32_t                        MEditorHeight             = 300;
  MIP_Editor*                     MEditor                   = {};

  //MIP_AudioProcess
  //MIP_VoiceManager

//------------------------------
public:
//------------------------------

  MIP_Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_ClapPlugin(ADescriptor) {
  }

  //----------

  virtual ~MIP_Plugin() {
    #ifndef MIP_NO_AUTODELETE
      deleteParameters();
    #endif
  }

//------------------------------
public: // clap plugin
//------------------------------

  bool init() override {
    return true;
  }

  //----------

  void destroy() override {
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) override {
    MProcessContext.samplerate = sample_rate;
    return true;
  }

  //----------

  void deactivate() override {
  }

  //----------

  bool start_processing() override {
    return true;
  }

  //----------

  void stop_processing() override {
  }

  //----------

  void reset() override {
  }

  //----------

  clap_process_status process(const clap_process_t *process) override {
    MProcessContext.process = process;
    processTransport(process->transport);
    preProcessEvents(process->in_events,process->out_events);
    processEvents(process->in_events,process->out_events);
    MParameters.flushAudioParams();
    processAudioBlock(process);
    postProcessEvents(process->in_events,process->out_events);
    MParameters.flushHostParams(process->out_events);
    MProcessContext.process = nullptr;
    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  const void* get_extension(const char *id) override {
    if (strcmp(id,CLAP_EXT_AMBISONIC)           == 0) return &MAmbisonic;       // draft
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS_CONFIG)  == 0) return &MAudioPortsConfig;
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS)         == 0) return &MAudioPorts;
    if (strcmp(id,CLAP_EXT_CHECK_FOR_UPDATE)    == 0) return &MCheckForUpdate;  // draft
    if (strcmp(id,CLAP_EXT_CV)                  == 0) return &MCv;              // draft
    if (strcmp(id,CLAP_EXT_FILE_REFERENCE)      == 0) return &MFileReference;   // draft
    if (strcmp(id,CLAP_EXT_GUI)                 == 0) return &MGui;
    if (strcmp(id,CLAP_EXT_LATENCY)             == 0) return &MLatency;
    if (strcmp(id,CLAP_EXT_MIDI_MAPPINGS)       == 0) return &MMidiMappings;    // draft
    if (strcmp(id,CLAP_EXT_NOTE_NAME)           == 0) return &MNoteName;
    if (strcmp(id,CLAP_EXT_NOTE_PORTS)          == 0) return &MNotePorts;
    if (strcmp(id,CLAP_EXT_PARAMS)              == 0) return &MParams;
    if (strcmp(id,CLAP_EXT_PRESET_LOAD)         == 0) return &MPresetLoad;      // draft
    if (strcmp(id,CLAP_EXT_QUICK_CONTROLS)      == 0) return &MQuickControls;   // draft
    if (strcmp(id,CLAP_EXT_RENDER)              == 0) return &MRender;
    if (strcmp(id,CLAP_EXT_STATE)               == 0) return &MState;
    if (strcmp(id,CLAP_EXT_SURROUND)            == 0) return &MSurround;        // draft
    if (strcmp(id,CLAP_EXT_THREAD_POOL)         == 0) return &MThreadPool;
    if (strcmp(id,CLAP_EXT_TIMER_SUPPORT)       == 0) return &MTimerSupport;
    if (strcmp(id,CLAP_EXT_TRACK_INFO)          == 0) return &MTrackInfo;       // draft
    if (strcmp(id,CLAP_EXT_TUNING)              == 0) return &MTuning;          // draft
    if (strcmp(id,CLAP_EXT_VOICE_INFO)          == 0) return &MVoiceInfo;       // draft
    return nullptr;
  }

  //----------

  void on_main_thread() override {
  }

//------------------------------
public: // DRAFT ambisonic
//------------------------------

  bool ambisonic_get_info(bool is_input,  uint32_t port_index, clap_ambisonic_info_t *info) override {
    return false;
  }

//------------------------------
public: // EXT audio ports config
//------------------------------

  uint32_t audio_ports_config_count() override {
    return 1;
  }

  //----------

  bool audio_ports_config_get(uint32_t index, clap_audio_ports_config_t *config) override {
    config->id = 0;
    strcpy(config->name,"config1");
    config->input_port_count = 1;
    config->output_port_count = 1;
    // main input info
    config->has_main_input            = true;
    config->main_input_channel_count  = 2;
    config->main_input_port_type      = CLAP_PORT_STEREO;
    // main output info
    config->has_main_output           = true;
    config->main_output_channel_count = 2;
    config->main_output_port_type     = CLAP_PORT_STEREO;
    return true;
  }

  //----------

  bool audio_ports_config_select(clap_id config_id) override {
    MSelectedAudioPortsConfig = config_id;
    return true;
  }

//------------------------------
public: // EXT audio ports
//------------------------------

  uint32_t audio_ports_count(bool is_input) override {
    if (is_input) return MAudioInputPorts.size();
    else return MAudioOutputPorts.size();
  }

  //----------

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t *info) override {
    if (is_input) {
      memcpy(info,MAudioInputPorts[index],sizeof(clap_audio_port_info_t));
      return true;
    }
    else { // output
      memcpy(info,MAudioOutputPorts[index],sizeof(clap_audio_port_info_t));
      return true;
    }
    return false;
  }

//------------------------------
public: // DRAFT check for updated
//------------------------------

  void check_fur_updates_check(bool include_preview) override {
  }

//------------------------------
public: // DRAFT cv
//------------------------------

  bool cv_get_channel_type(bool is_input, uint32_t port_index, uint32_t channel_index, uint32_t* channel_type) override {
    return false;
  }

//------------------------------
public: // DRAFT file reference
//------------------------------

  uint32_t file_reference_count() override {
    return 0;
  }

  //----------

  bool file_reference_get(uint32_t index, clap_file_reference_t *file_reference) override {
    return false;
  }

  //----------

  bool file_reference_get_blake3_digest(clap_id resource_id, uint8_t *digest) override {
    return false;
  }

  //----------

  bool file_reference_get_file_size(clap_id resource_id, uint64_t *size) override {
    return false;
  }

  //----------

  bool file_reference_update_path(clap_id resource_id, const char *path) override {
    return false;
  }

  //----------

  bool file_reference_save_resources() override {
    return false;
  }

//------------------------------
public: // EXT gui
//------------------------------

  bool gui_is_api_supported(const char *api, bool is_floating) override {
    if ((strcmp(api,CLAP_WINDOW_API_X11) == 0) && (is_floating == false)) {
      return true;
    }
    return false;
  }

  //----------

  bool gui_get_preferred_api(const char **api, bool *is_floating) override {
    *api = CLAP_WINDOW_API_X11;
    *is_floating = false;
    return true;
  }

  //----------

  bool gui_create(const char *api, bool is_floating) override {
    MEditor = new MIP_Editor(this,MEditorWidth,MEditorHeight);
    return true;
  }

  //----------

  void gui_destroy() override {
    delete MEditor;
  }

  //----------

  bool gui_set_scale(double scale) override {
    return MEditor->gui_set_scale(scale);
  }

  //----------

  bool gui_get_size(uint32_t *width, uint32_t *height) override {
    return MEditor->gui_get_size(width,height);
  }

  //----------

  bool gui_can_resize() override {
    return MEditor->gui_can_resize();
  }

  //----------

  bool gui_get_resize_hints(clap_gui_resize_hints_t *hints) override {
    return MEditor->gui_get_resize_hints(hints);
  }

  //----------

  bool gui_adjust_size(uint32_t *width, uint32_t *height) override {
    return MEditor->gui_adjust_size(width,height);
  }

  //----------

  bool gui_set_size(uint32_t width, uint32_t height) override {
    return MEditor->gui_set_size(width,height);
  }

  //----------

  bool gui_set_parent(const clap_window_t *window) override {
    return MEditor->gui_set_parent(window);
  }

  //----------

  bool gui_set_transient(const clap_window_t *window) override {
    return MEditor->gui_set_transient(window);
  }

  //----------

  void gui_suggest_title(const char *title) override {
    MEditor->gui_suggest_title(title);
  }

  //----------

  bool gui_show() override {
    return MEditor->gui_show();
  }

  //----------

  bool gui_hide() override {
    return MEditor->gui_hide();
  }

//------------------------------
public: // EXT latency
//------------------------------

  uint32_t latency_get() override {
    return 0;
  }

//------------------------------
public: // DRAFT midi mappings
//------------------------------

  uint32_t  midi_mappings_count() override {
    return 0;
  }

  //----------

  bool midi_mappings_get(uint32_t index, clap_midi_mapping_t *mapping) override {
    return false;
  }

//------------------------------
public: // EXT note names
//------------------------------

  uint32_t note_names_count() override {
    return 0;
  }

  //----------

  bool note_names_get(uint32_t index, clap_note_name_t *note_name) override {
    return false;
  }

//------------------------------
public: // EXT note ports
//------------------------------

  uint32_t note_ports_count(bool is_input) override {
    if (is_input) return MNoteInputPorts.size();
    else return MNoteOutputPorts.size();
  }

  //----------

  bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t *info) override {
    if (is_input) {
      memcpy(info,MNoteInputPorts[index],sizeof(clap_note_port_info_t));
      return true;
    }
    else { // output
      memcpy(info,MNoteOutputPorts[index],sizeof(clap_note_port_info_t));
      return true;
    }
    return false;
  }

//------------------------------
public: // EXT params
//------------------------------

  uint32_t params_count() override {
    return MParameters.size();
  }

  //----------

  bool params_get_info(uint32_t param_index, clap_param_info_t *param_info) override {
    MIP_Parameter* parameter = MParameters[param_index];
    param_info->id            = param_index;
    param_info->flags         = CLAP_PARAM_IS_AUTOMATABLE;;
    param_info->cookie        = parameter;
    param_info->min_value     = parameter->getMinValue();
    param_info->max_value     = parameter->getMaxValue();
    param_info->default_value = parameter->getDefaultValue();
    strcpy(param_info->name,parameter->getName());
    strcpy(param_info->module,parameter->getModule());
    return true;
  }

  //----------

  bool params_get_value(clap_id param_id, double *value) override {
    MIP_Parameter* parameter = MParameters[param_id];
    *value = parameter->getValue();
    return true;
  }

  //----------

  bool params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) override {
    MIP_Parameter* parameter = MParameters[param_id];
    parameter->valueToText(value,display,size);
    return true;
  }

  //----------

  bool params_text_to_value(clap_id param_id, const char *display, double *value) override {
    MIP_Parameter* parameter = MParameters[param_id];
    *value = parameter->textToValue(display);
    return false;
  }

  //----------

  void params_flush(const clap_input_events_t *in, const clap_output_events_t *out) override {
    //MIP_PRINT;
    preProcessEvents(in,out);
    processEvents(in,out);
    postProcessEvents(in,out);
  }

//------------------------------
public: // EXT posix fd support
//------------------------------

  void posix_fd_support_on_fd(int fd, clap_posix_fd_flags_t flags) override {
  }

//------------------------------
public: // DRAFT preset load
//------------------------------

  bool preset_load_from_file(const char *path) override {
    return false;
  }

//------------------------------
public: // DRAFT quick controls
//------------------------------

  uint32_t quick_controls_count() override {
    return 1;
  }

  //----------

  bool quick_controls_get(uint32_t page_index, clap_quick_controls_page_t *page) override {
    switch (page_index) {
      case 0:
        page->id = 0;
        strcpy(page->name,"page1");
        for (uint32_t i=0; i<CLAP_QUICK_CONTROLS_COUNT; i++) {
          page->param_ids[i] = i;
        }
        return true;
    }
    return false;
  }

//------------------------------
public: // EXT render
//------------------------------

  bool render_has_hard_realtime_requirement() override {
    return false;
  }

  //----------

  bool render_set(clap_plugin_render_mode mode) override {
    MRenderMode = mode;
    return true;
  }

//------------------------------
public: // EXT state
//------------------------------

  bool state_save(const clap_ostream_t *stream) override {
    uint32_t version = 0;
    uint32_t written = stream->write(stream,&version,sizeof(uint32_t));
    MIP_Assert(written == sizeof(uint32_t));
    return true;
  }

  //----------

  bool state_load(const clap_istream_t *stream) override {
    uint32_t version = 0;
    uint32_t written = stream->read(stream,&version,sizeof(uint32_t));
    MIP_Assert(written == sizeof(uint32_t));
    return true;
  }

//------------------------------
public: // DRAFT surround
//------------------------------

  uint32_t surround_get_channel_map(bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) override {
    return 0;
  }

  //----------

  void surround_changed() override {
  }

//------------------------------
public: // EXT tail
//------------------------------

  uint32_t tail_get() override {
    return 0;
  }

//------------------------------
public: // EXT thread pool
//------------------------------

  void thread_pool_exec(uint32_t task_index) override {
  }

//------------------------------
public: // EXT timer support
//------------------------------

  void timer_support_on_timer(clap_id timer_id) override {
  }

//------------------------------
public: // DRAFT track info
//------------------------------

  void track_info_changed() override {
  }

//------------------------------
public: // DRAFT tuning
//------------------------------

  void tuning_changed() override {
  }

//------------------------------
public: // DRAFT voice info
//------------------------------

  bool voice_info_get(clap_voice_info_t *info) override {
    return false;
  }

//------------------------------
public:
//------------------------------

//------------------------------
public:

//------------------------------

//------------------------------
public: // process events
//------------------------------

  virtual void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  }

  //----------

  virtual void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  }

  //----------

  virtual void processEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        processEvent(header);
      }
    }
  }

  //----------

  virtual void processEvent(const clap_event_header_t* header) {
    switch (header->type) {
      case CLAP_EVENT_NOTE_ON:              processNoteOnEvent(             (const clap_event_note_t*)            header  );  break;
      case CLAP_EVENT_NOTE_OFF:             processNoteOffEvent(            (const clap_event_note_t*)            header  );  break;
      case CLAP_EVENT_NOTE_CHOKE:           processNoteChokeEvent(          (const clap_event_note_t*)            header  );  break;
      case CLAP_EVENT_NOTE_END:             processNoteEndEvent(            (const clap_event_note_t*)            header  );  break;
      case CLAP_EVENT_NOTE_EXPRESSION:      processNoteExpressionEvent(     (const clap_event_note_expression_t*) header  );  break;
      case CLAP_EVENT_PARAM_VALUE:          processParamValueEvent(         (const clap_event_param_value_t*)     header  );  break;
      case CLAP_EVENT_PARAM_MOD:            processParamModEvent(           (const clap_event_param_mod_t*)       header  );  break;
      case CLAP_EVENT_PARAM_GESTURE_BEGIN:  processParamGestureBeginEvent(  (const clap_event_param_gesture_t*)   header  );  break;
      case CLAP_EVENT_PARAM_GESTURE_END:    processParamGestureEndEvent(    (const clap_event_param_gesture_t*)   header  );  break;
      case CLAP_EVENT_TRANSPORT:            processTransportEvent(          (const clap_event_transport_t*)       header  );  break;
      case CLAP_EVENT_MIDI:                 processMidiEvent(               (const clap_event_midi_t*)            header  );  break;
      case CLAP_EVENT_MIDI_SYSEX:           processMidiSysexEvent(          (const clap_event_midi_sysex_t*)      header  );  break;
      case CLAP_EVENT_MIDI2:                processMidi2Event(              (const clap_event_midi2_t*)           header  );  break;
    }
  }

  //----------

  virtual void processNoteOnEvent(const clap_event_note_t* event) {
    MIP_Print("NOTE ON key %i note_id %i\n",event->key,event->note_id);
  }

  virtual void processNoteOffEvent(const clap_event_note_t* event) {
    MIP_Print("NOTE OFF key %i note_id %i\n",event->key,event->note_id);
  }

  virtual void processNoteChokeEvent(const clap_event_note_t* event) {
    MIP_Print("NOTE CHOKE key %i note_id %i\n",event->key,event->note_id);
  }

  virtual void processNoteEndEvent(const clap_event_note_t* event) {
    MIP_Print("NOTE END !\n");
  }

  virtual void processNoteExpressionEvent(const clap_event_note_expression_t* event) {
    MIP_Print("NOTE EXPRESSION expr %i key %i note_id %i value %.3f\n",event->expression_id,event->key,event->note_id,event->value);
  }

  virtual void processParamValueEvent(const clap_event_param_value_t* event) {
    MIP_Print("PARAM VALUE index %i value %.3f\n",event->param_id,event->value);
    uint32_t index = event->param_id;
    double value = event->value;
    MParameters[index]->setValue(value);
  }

  virtual void processParamModEvent(const clap_event_param_mod_t* event) {
    MIP_Print("PARAM MOD index %i value %.3f\n",event->param_id,event->amount);
    uint32_t index = event->param_id;
    double value = event->amount;
    MParameters[index]->setModulation(value);
    // notify audio processor
    // notify editor
  }

  virtual void processParamGestureBeginEvent(const clap_event_param_gesture_t* event) {
    MIP_Print("PARAM GESTURE BEGIN\n");
  }

  virtual void processParamGestureEndEvent(const clap_event_param_gesture_t* event) {
    MIP_Print("PARAM GESTURE END\n");
  }

  virtual void processTransportEvent(const clap_event_transport_t* event) {
    MIP_Print("TRANSPORT\n");
  }

  virtual void processMidiEvent(const clap_event_midi_t* event) {
    MIP_Print("MIDI\n");
  }

  virtual void processMidiSysexEvent(const clap_event_midi_sysex_t* event) {
    MIP_Print("MIDI SYSEX\n");
  }

  virtual void processMidi2Event(const clap_event_midi2_t* event) {
    MIP_Print("MIDI2\n");
  }

//------------------------------
public: // process audio
//------------------------------

  virtual void processAudioBlock(const clap_process_t* process) {
  }

//------------------------------
public: // process transport
//------------------------------

  virtual void processTransport(const clap_event_transport_t* transport) {
  }

//------------------------------
public: // parameters
//------------------------------

  void appendParameter(MIP_Parameter* AParameter) {
    MParameters.append(AParameter);
  }

  //----------

  void deleteParameters() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      delete MParameters[i];
      //MParameters[i] = nullptr;
    }
  }

  //----------

  //MIP_AudioPort* getAudioInputPort(uint32_t AIndex) {
  const MIP_Parameter* getParameter(uint32_t AIndex) {
    return MParameters[AIndex];
  }

//------------------------------
public: // audio inputs
//------------------------------

  //void appendAudioInputPort(MIP_AudioPort* APort) {
  void appendAudioInputPort(const clap_audio_port_info_t* info) {
    MAudioInputPorts.append(info);
  }

  //----------

  void deleteAudioInputPorts() {
    for (uint32_t i=0; i<MAudioInputPorts.size(); i++) {
      //delete MAudioInputPorts[i];
      MAudioInputPorts[i] = nullptr;
    }
  }

  //----------

  //MIP_AudioPort* getAudioInputPort(uint32_t AIndex) {
  const clap_audio_port_info_t* getAudioInputPort(uint32_t AIndex) {
    return MAudioInputPorts[AIndex];
  }

//------------------------------
public: // audio outputs
//------------------------------

  //void appendAudioOutputPort(MIP_AudioPort* APort) {
  void appendAudioOutputPort(const clap_audio_port_info_t* info) {
    MAudioOutputPorts.append(info);
  }

  //----------

  void deleteAudioOutputPorts() {
    for (uint32_t i=0; i<MAudioOutputPorts.size(); i++) {
      //delete MAudioOutputPorts[i];
      MAudioOutputPorts[i] = nullptr;
    }
  }

  //----------

  //MIP_AudioPort* getAudioOutputPort(uint32_t AIndex) {
  const clap_audio_port_info_t* getAudioOutputPort(uint32_t AIndex) {
    return MAudioOutputPorts[AIndex];
  }

//------------------------------
public: // note inputs
//------------------------------

  //void appendNoteInputPort(MIP_NotePort* APort) {
  void appendNoteInputPort(const clap_note_port_info_t* info) {
    MNoteInputPorts.append(info);
  }

  //----------

  void deleteNoteInputPorts() {
    for (uint32_t i=0; i<MNoteInputPorts.size(); i++) {
      //delete MNoteInputPorts[i];
      MNoteInputPorts[i] = nullptr;
    }
  }

  //----------

  //MIP_NotePort* getNoteInputPort(uint32_t AIndex) {
  const clap_note_port_info_t* getNoteInputPort(uint32_t AIndex) {
    return MNoteInputPorts[AIndex];
  }

//------------------------------
public: // note outputs
//------------------------------

  //void appendNoteOutputPort(MIP_NotePort* APort) {
  void appendNoteOutputPort(const clap_note_port_info_t* info) {
    MNoteOutputPorts.append(info);
  }

  //----------

  void deleteNoteOutputPorts() {
    for (uint32_t i=0; i<MNoteOutputPorts.size(); i++) {
      //delete MNoteOutputPorts[i];
      MNoteOutputPorts[i] = nullptr;
    }
  }

  //----------

  //MIP_NotePort* getNoteOutputPort(uint32_t AIndex) {
  const clap_note_port_info_t* getNoteOutputPort(uint32_t AIndex) {
    return MNoteOutputPorts[AIndex];
  }

//------------------------------
public: // wrapper listener
//------------------------------

//------------------------------
public: // editor listener
//------------------------------

  void on_editor_listener_update_parameter(uint32_t AIndex, double AValue) final {
    MIP_Print("%i = %.3f\n",AIndex,AValue);
    // notify host
    // notify audio processor
  }

  //----------

  void on_editor_listener_resize_window(uint32_t AWidth, uint32_t AHeight) final {
  }

};

//----------------------------------------------------------------------
#endif
