#ifndef mip_plugin_included
#define mip_plugin_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"
#include "base/types/mip_queue.h"
#include "audio/mip_audio_utils.h"
#include "plugin/clap/mip_clap.h"
//#include "plugin/clap/mip_clap_entry.h"
#include "plugin/clap/mip_clap_host.h"
#include "plugin/clap/mip_clap_plugin.h"

#ifndef MIP_NO_GUI
#include "plugin/mip_editor.h"
#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#include "plugin/clap/mip_clap_invalidation.h"
#include "plugin/clap/mip_clap_factory.h"
#include "plugin/clap/mip_clap_entry.h"

//#include "plugin/wrapper/mip_exe_wrapper.h"
//#include "plugin/wrapper/mip_lv2_wrapper.h"
//#include "plugin/wrapper/mip_vst2_wrapper.h"
//#include "plugin/wrapper/mip_vst3_wrapper.h"


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#define MIP_CLAP_MESSAGE_QUEUE_SIZE 1024
//typedef MIP_Queue<uint32_t,MIP_CLAP_MESSAGE_QUEUE_SIZE> MIP_ClapIntQueue;

typedef MIP_Array<const clap_param_info_t*>           MIP_Parameters;
typedef MIP_Array<const clap_audio_port_info_t*>      MIP_AudioPorts;
typedef MIP_Array<const clap_note_port_info_t*>       MIP_NotePorts;
typedef MIP_Array<const clap_quick_controls_page_t*>  MIP_QuickControls;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Plugin
: public MIP_ClapPlugin
#ifndef MIP_NO_GUI
, public MIP_EditorListener {
#else
{
  #endif

//------------------------------
protected:
//------------------------------

  const clap_plugin_descriptor_t* MDescriptor = nullptr;
  MIP_ClapHost*                   MHost       = nullptr;

  MIP_Parameters    MParameters       = {};
  MIP_AudioPorts    MAudioInputs      = {};
  MIP_AudioPorts    MAudioOutputs     = {};
  MIP_NotePorts     MNoteInputs       = {};
  MIP_NotePorts     MNoteOutputs      = {};
  MIP_QuickControls MQuickControls    = {};

  float*            MParamVal         = nullptr;
  float*            MParamMod         = nullptr;
  bool              MIsProcessing     = false;
  bool              MIsActivated      = false;

  MIP_ClapIntQueue  MAudioParamQueue  = {};
  float*            MAudioParamVal    = nullptr;

  #ifndef MIP_NO_GUI
  MIP_Editor*       MEditor           = nullptr;
  bool              MIsEditorOpen     = false;
  #endif


//------------------------------
public:
//------------------------------

  MIP_Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_ClapPlugin(ADescriptor,AHost) {
    MDescriptor = ADescriptor;
    MHost = new MIP_ClapHost(AHost);
    uint32_t num = params_count();
    uint32_t size = num * sizeof(float);
    MAudioParamVal = (float*)malloc(size);
    memset(MAudioParamVal,0,size);
  }

  //----------

  virtual ~MIP_Plugin() {
    delete MHost;
    free (MAudioParamVal);
  }

//------------------------------
public:
//------------------------------

  float getParamMod(uint32_t AIndex) { return MParamMod[AIndex]; }
  float getParamVal(uint32_t AIndex) { return MParamVal[AIndex]; }

  void  setParamMod(uint32_t AIndex, float AValue) { MParamMod[AIndex] = AValue; }
  void  setParamVal(uint32_t AIndex, float AValue) { MParamVal[AIndex] = AValue; }

  //----------

private: // ??

  void queueAudioParam(uint32_t AIndex) {
    MAudioParamQueue.write(AIndex);
  }

  //----------

  //todo: check if value really changed (if multiple events)

  void flushAudioParams() {
    uint32_t index = 0;
    while (MAudioParamQueue.read(&index)) {
      float value = MAudioParamVal[index];
      // if we already set this, it should be (bit) identical?
      if (value != MParamVal[index]) {
        MParamVal[index] = value;
        //...
      }
    }
  }

//------------------------------
public: // editor listener
//------------------------------

  #ifndef MIP_NO_GUI

  // todo: queue, and flush in process?

  void on_editor_updateParameter(uint32_t AIndex, float AValue) final {
    //MIP_Print("%i = %.3f\n",AIndex,AValue);
    MAudioParamVal[AIndex] = AValue;
    queueAudioParam(AIndex);
  }

  #endif

//------------------------------
public:
//------------------------------

  virtual void handle_note_on_event(clap_event_note_t* event) {}
  virtual void handle_note_off_event(clap_event_note_t* event) {}
  virtual void handle_note_end_event(clap_event_note_t* event) {}
  virtual void handle_note_choke_event(clap_event_note_t* event) {}
  virtual void handle_note_expression_event(clap_event_note_expression_t* event) {}

  //----------

  virtual void handle_parameter_event(const clap_event_param_value_t* param_value) {
    uint32_t i = param_value->param_id;
    float v = param_value->value;
    setParamVal(i,v);
    #ifndef MIP_NO_GUI
    if (MEditor && MIsEditorOpen) MEditor->updateParameterFromHost(i,v);
    #endif
  }

  //----------

  virtual void handle_modulation_event(const clap_event_param_mod_t* param_mod) {
    uint32_t i = param_mod->param_id;
    float v = param_mod->amount;
    setParamMod(i,v);
    #ifndef MIP_NO_GUI
    if (MEditor && MIsEditorOpen) MEditor->updateModulationFromHost(i,v);
    #endif
  }

  //----------

  virtual void handle_midi_event(clap_event_midi_t* event) {}
  virtual void handle_midi_event(clap_event_midi2_t* event) {}
  virtual void handle_midi_event(clap_event_midi_sysex_t* event) {}
  virtual void handle_transport_event(clap_event_transport_t* event) {}

  //----------

  virtual void handle_process(const clap_process_t *process) {
    //float* in0 = process->audio_inputs[0].data32[0];
    //float* in1 = process->audio_inputs[0].data32[1];
    //float* out0 = process->audio_outputs[0].data32[0];
    //float* out1 = process->audio_outputs[0].data32[1];
    //uint32_t num = process->frames_count;
    //for (uint32_t i=0; i<num; i++) {
    //  *out0++ = *in0++;
    //  *out1++ = *in1++;
    //}
//    float** inputs = process->audio_inputs[0].data32;
//    float** outputs = process->audio_outputs[0].data32;
//    uint32_t length = process->frames_count;
//    MIP_CopyStereoBuffer(outputs,inputs,length);
  }

//------------------------------
protected:
//------------------------------

  virtual void handle_input_events(const clap_input_events_t* in_events) {
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        switch (header->type) {
          case CLAP_EVENT_NOTE_ON:          handle_note_on_event((clap_event_note_t*)header); break;
          case CLAP_EVENT_NOTE_OFF:         handle_note_off_event((clap_event_note_t*)header); break;
          case CLAP_EVENT_NOTE_END:         handle_note_end_event((clap_event_note_t*)header); break;
          case CLAP_EVENT_NOTE_CHOKE:       handle_note_choke_event((clap_event_note_t*)header); break;
          case CLAP_EVENT_NOTE_EXPRESSION:  handle_note_expression_event((clap_event_note_expression_t*)header); break;
          case CLAP_EVENT_PARAM_VALUE:      handle_parameter_event((clap_event_param_value_t*)header); break;
          case CLAP_EVENT_PARAM_MOD:        handle_modulation_event((clap_event_param_mod_t*)header); break;
          case CLAP_EVENT_MIDI:             handle_midi_event((clap_event_midi_t*)header); break;
          case CLAP_EVENT_MIDI2:            handle_midi_event((clap_event_midi2_t*)header); break;
          case CLAP_EVENT_MIDI_SYSEX:       handle_midi_event((clap_event_midi_sysex_t*)header); break;
          case CLAP_EVENT_TRANSPORT:        handle_transport_event((clap_event_transport_t*)header); break;
        }
      }
    }
  }

  //----------

  virtual void handle_output_events(const clap_output_events_t* out_events) {
    #ifndef MIP_NO_GUI
    if (MEditor && MIsEditorOpen) MEditor->flushHostParams(out_events);
    #endif
  }

//------------------------------
protected:
//------------------------------

  void setupParameters(clap_param_info_t* params, uint32_t num) {
    for (uint32_t i=0; i<num; i++) {
      const clap_param_info_t* info = &params[i];
      MParameters.append(info);
    }
  }

  //----------

  void setDefaultParameterValues(clap_param_info_t* params, uint32_t num) {
    for (uint32_t i=0; i<num; i++) {
      const clap_param_info_t* info = &params[i];
      MParamVal[i] = info->default_value;
      MParamMod[i] = 0.0;
    }
  }

  //----------

  void setEditorParameterValues(clap_param_info_t* params, uint32_t num) {
    for (uint32_t i=0; i<num; i++) {
      #ifndef MIP_NO_GUI
      if (MEditor) MEditor->setParameterValue(i,MParamVal[i]);
      #endif
    }
  }

//------------------------------
protected:
//------------------------------

  void setupAudioInputs(clap_audio_port_info_t* inputs, uint32_t num) {
    for (uint32_t i=0; i<num; i++) {
      const clap_audio_port_info_t* info = &inputs[i];
      MAudioInputs.append(info);
    }
  }

  //----------

  void setupAudioOutputs(clap_audio_port_info_t* outputs, uint32_t num) {
    for (uint32_t i=0; i<num; i++) {
      const clap_audio_port_info_t* info = &outputs[i];
      MAudioOutputs.append(info);
    }
  }

  //----------

  void setupNoteInputs(clap_note_port_info_t* inputs, uint32_t num) {
    for (uint32_t i=0; i<num; i++) {
      const clap_note_port_info_t* info = &inputs[i];
      MNoteInputs.append(info);
    }
  }

  //----------

  void setupNoteOutputs(clap_note_port_info_t* outputs, uint32_t num) {
    for (uint32_t i=0; i<num; i++) {
      const clap_note_port_info_t* info = &outputs[i];
      MNoteOutputs.append(info);
    }
  }

  //----------

  void setupQuickControls(clap_quick_controls_page_t* page, uint32_t num) {
    for (uint32_t i=0; i<num; i++) {
      const clap_quick_controls_page_t* info = &page[i];
      MQuickControls.append(info);
    }
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() override {
    uint32_t num = MParameters.size();
    MParamVal    = (float*)malloc(num * sizeof(float));
    MParamMod    = (float*)malloc(num * sizeof(float));
    for (uint32_t i=0; i<num; i++) {
      float v = MParameters[i]->default_value;
      setParamVal(i,v);
      setParamMod(i,0);
    }
    return true;
  }

  //----------

  void destroy() override {
    free(MParamVal);
    free(MParamMod);
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) override {
    MIsActivated = true;
    return true;
  }

  //----------

  void deactivate() override {
    MIsActivated = false;
  }

  //----------

  bool start_processing() override {
    MIsProcessing = true;
    return true;
  }

  //----------

  void stop_processing() override {
    MIsProcessing = false;
  }

  //----------

  clap_process_status process(const clap_process_t *process) override {
    flushAudioParams();
    handle_input_events(process->in_events);
    handle_process(process);
    handle_output_events(process->out_events);
    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  const void* get_extension(const char *id) override {
    //MIP_Print("id: %s\n",id);
    //if (strcmp(id,CLAP_EXT_AMBISONIC) == 0)           return &MAmbisonic;
    //if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0)         return &MAudioPorts;
    //if (strcmp(id,CLAP_EXT_CHECK_FOR_UPDATE) == 0)    return &MCheckForUpdate;
    //if (strcmp(id,CLAP_EXT_CV) == 0)                  return &MCV;
    //if (strcmp(id,CLAP_EXT_AUDIO_PORTS_CONFIG) == 0)  return &MAudioPortsConfig;
    if (strcmp(id,CLAP_EXT_EVENT_FILTER) == 0)        return &MEventFilter;
    //if (strcmp(id,CLAP_EXT_FILE_REFERENCE) == 0)      return &MFileReference;
    //if (strcmp(id,CLAP_EXT_GUI) == 0)                 return &MGui;
    //if (strcmp(id,CLAP_EXT_GUI_X11) == 0)             return &MGuiX11;
    //if (strcmp(id,CLAP_EXT_LATENCY) == 0)             return &MLatency;
    //if (strcmp(id,CLAP_EXT_MIDI_MAPPINGS) == 0)       return &MMidiMappings;
    //if (strcmp(id,CLAP_EXT_NOTE_NAME) == 0)           return &MNoteName;
    //if (strcmp(id,CLAP_EXT_NOTE_PORTS) == 0)          return &MNotePorts;
    if (strcmp(id,CLAP_EXT_PARAMS) == 0)              return &MParams;
    //if (strcmp(id,CLAP_EXT_POSIX_FD_SUPPORT) == 0)    return &MPosixFdSupport;
    if (strcmp(id,CLAP_EXT_PRESET_LOAD) == 0)         return &MPresetLoad;
    //if (strcmp(id,CLAP_EXT_QUICK_CONTROLS) == 0)      return &MQuickControls;
    //if (strcmp(id,CLAP_EXT_RENDER) == 0)              return &MRender;
    if (strcmp(id,CLAP_EXT_STATE) == 0)               return &MState;
    //if (strcmp(id,CLAP_EXT_SURROUND) == 0)            return &MSurround;
    //if (strcmp(id,CLAP_EXT_THREAD_POOL) == 0)         return &MThreadPool;
    if (strcmp(id,CLAP_EXT_TIMER_SUPPORT) == 0)       return &MTimerSupport;
    //if (strcmp(id,CLAP_EXT_TRACK_INFO) == 0)          return &MTrackInfo;
    return nullptr;
  }

  //----------

  //void on_main_thread() override {
  //}

//------------------------------
public: // event-filter
//------------------------------

  bool event_filter_accepts(uint16_t space_id, uint16_t event_type) override {
    if (space_id == CLAP_CORE_EVENT_SPACE_ID) {
      switch (event_type) {
        case CLAP_EVENT_NOTE_ON:          return true;
        case CLAP_EVENT_NOTE_OFF:         return true;
        case CLAP_EVENT_NOTE_CHOKE:       return true;
        case CLAP_EVENT_NOTE_END:         return true;
        case CLAP_EVENT_NOTE_EXPRESSION:  return true;
        case CLAP_EVENT_PARAM_VALUE:      return true;
        case CLAP_EVENT_PARAM_MOD:        return true;
        case CLAP_EVENT_TRANSPORT:        return true;
        case CLAP_EVENT_MIDI:             return true;
        case CLAP_EVENT_MIDI_SYSEX:       return true;
        case CLAP_EVENT_MIDI2:            return true;
      }
    }
    return false;
  }

//------------------------------
public: // params
//------------------------------

  uint32_t params_count() override {
    return MParameters.size();
  }

  //----------

  bool params_get_info(uint32_t param_index, clap_param_info_t* param_info) override {
    memcpy(param_info,MParameters[param_index],sizeof(clap_param_info_t));
    return true;
  }

  //----------

  bool params_get_value(clap_id param_id, double *value) override {
    *value = getParamVal(param_id);
    return true;
  }

  //----------

  bool params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) override {
    sprintf(display,"%.3f",value);
    return true;
  }

  //----------

  bool params_text_to_value(clap_id param_id, const char *display, double *value) override {
    float f = atof(display);
    *value = f;
    return true;
  }

  //----------

  void params_flush(const clap_input_events_t* in, const clap_output_events_t* out) override {
    handle_input_events(in);
    handle_output_events(out);
  }

//------------------------------
public: // gui-x11
//------------------------------

  #ifndef MIP_NO_GUI

  bool gui_x11_attach(const char *display_name, unsigned long window) override {
    if (MEditor) MEditor->attach(display_name,window);
    return true;
  }

  #endif

//------------------------------
public: // gui
//------------------------------

  #ifndef MIP_NO_GUI

  bool gui_create() override {
    MEditor = new MIP_Editor(this,this);
    MIsEditorOpen = false;
    return true;
  }

  //----------

  void gui_destroy() override {
    MIsEditorOpen = false;
    delete MEditor;
    MEditor = nullptr;
  }

  //----------

  bool gui_set_scale(double scale) override {
    if (MEditor) MEditor->setScale(scale);
    return true;
  }

  //----------

  bool gui_get_size(uint32_t *width, uint32_t *height) override {
    if (MEditor) return MEditor->getSize(width,height);
    return false;
  }

  //----------

  bool gui_can_resize() override {
    if (MEditor) return MEditor->canResize();
    return false;
  }

  //----------

  void gui_round_size(uint32_t *width, uint32_t *height) override {
    //MIP_Print("%i,%i\n",*width,*height);
    if (MEditor) MEditor->roundSize(width,height);

  }

  //----------

  bool gui_set_size(uint32_t width, uint32_t height) override {
    if (MEditor) return MEditor->setSize(width,height);
    return true;
  }

  //----------

  void gui_show() override {
    if (MEditor) {
      MEditor->show();
      MIsEditorOpen = true;
    }
  }

  //----------

  void gui_hide() override {
    if (MEditor) {
      MIsEditorOpen = false;
      MEditor->hide();
    }
  }

  #endif

//------------------------------
public: // audio-ports
//------------------------------

  uint32_t audio_ports_count(bool is_input) override {
    if (is_input) return MAudioInputs.size();
    else return MAudioOutputs.size();
  }

  //----------

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t* info) override {
    if (is_input) {
      memcpy(info,MAudioInputs[index],sizeof(clap_audio_port_info_t));
      return true;
    }
    else {
      memcpy(info,MAudioOutputs[index],sizeof(clap_audio_port_info_t));
      return true;
    }
    return false;
  }

//------------------------------
public: // note-ports
//------------------------------

  uint32_t note_ports_count(bool is_input) override {
    if (is_input) return MNoteInputs.size();
    else return MNoteOutputs.size();
  }

  //----------

  bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t* info) override {
    if (is_input) {
      memcpy(info,MNoteInputs[index],sizeof(clap_note_port_info_t));
      return true;
    }
    else {
      memcpy(info,MNoteOutputs[index],sizeof(clap_note_port_info_t));
      return true;
    }
    return false;
  }

//------------------------------
public: // quick-controls
//------------------------------

  uint32_t quick_controls_count() override {
    MIP_PRINT;
    return MQuickControls.size();
  }

  //----------

  bool quick_controls_get(uint32_t page_index, clap_quick_controls_page_t *page) override {
    MIP_PRINT;
    memcpy(page,MQuickControls[page_index],sizeof(clap_quick_controls_page_t));
    return true;
  }

  //----------

  //void quick_controls_select(clap_id page_id) override {
  //  //MIP_PRINT;
  //}

  //----------

  //clap_id quick_controls_get_selected() override {
  //  //MIP_PRINT;
  //  return 0;
  //}

//------------------------------
public: // timer-support
//------------------------------

  //void timer_support_on_timer(clap_id timer_id) override {
  //};

//------------------------------
public: // state
//------------------------------

  //bool state_save(clap_ostream_t *stream) override {
  //  return true;
  //}

  //----------

  //bool state_load(clap_istream_t *stream) override {
  //  return true;
  //};

//------------------------------
public: // preset-load
//------------------------------

  //bool preset_load_from_file(const char *path) override {
  //  return true;
  //}

//------------------------------
public: // latency
//------------------------------

  //uint32_t latency_get() override {
  //  return 0;
  //};

};

//----------------------------------------------------------------------
#endif
