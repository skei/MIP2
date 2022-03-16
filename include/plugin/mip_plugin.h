#ifndef mip_plugin_included
#define mip_plugin_included
//----------------------------------------------------------------------

/*
  notes:
  - close editor, no gui_hide
  - delete plugin, no gui_destroy
*/

//----------------------------------------------------------------------


#include "mip.h"
#include "base/types/mip_array.h"
#include "base/types/mip_queue.h"
#include "audio/mip_audio_utils.h"
#include "plugin/clap/mip_clap.h"
//#include "plugin/clap/mip_clap_entry.h"
#include "plugin/clap/mip_clap_host.h"
#include "plugin/clap/mip_clap_plugin.h"

#include "plugin/mip_parameter.h"

#ifndef MIP_NO_GUI
#include "plugin/mip_editor.h"
#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#include "plugin/clap/mip_clap_entry.h"

#ifdef MIP_EXE
  #include "plugin/exe/mip_exe_entry.h"
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

/*

struct MIP_Parameter {
  const clap_param_info_t* ptr = nullptr;
  float       from01(float value) {}
  float       to01(float value) {}
  const char* getDisplayText(float value) {}
};

struct MIP_AudioPort {
  const clap_audio_port_info_t* ptr = nullptr;
};

struct MIP_NotePort {
  const clap_note_port_info_t* ptr = nullptr;
};

struct MIP_QuickControl {
  const clap_quick_controls_page_t* ptr = nullptr;
};

typedef MIP_Array<MIP_Parameter*>     MIP_Parameters;
typedef MIP_Array<MIP_AudioPort*>     MIP_AudioPorts;
typedef MIP_Array<MIP_NotePort*>      MIP_NotePorts;
typedef MIP_Array<MIP_QuickControl*>  MIP_QuickControls;

*/

//----------

//typedef MIP_Array<const clap_param_info_t*>           MIP_Parameters;
typedef MIP_Array<MIP_Parameter*> MIP_Parameters;

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
private:
//------------------------------

  MIP_ClapIntQueue                MAudioParamQueue        = {};
  float*                          MAudioParamVal          = nullptr;

  MIP_ClapIntQueue                MHostParamQueue         = {};
  float*                          MHostParamVal           = nullptr;
  float*                          MHostParamMod           = nullptr;

  MIP_ClapIntQueue                MHostBeginGestureQueue  = {};
  MIP_ClapIntQueue                MHostEndGestureQueue    = {};

//------------------------------
protected:
//------------------------------

  const clap_plugin_descriptor_t* MDescriptor             = nullptr;
  MIP_ClapHost*                   MHost                   = nullptr;

  MIP_Parameters                  MParameters             = {};
  MIP_AudioPorts                  MAudioInputs            = {};
  MIP_AudioPorts                  MAudioOutputs           = {};
  MIP_NotePorts                   MNoteInputs             = {};
  MIP_NotePorts                   MNoteOutputs            = {};
  MIP_QuickControls               MQuickControls          = {};

  float*                          MParameterValues        = nullptr;
  float*                          MParameterModulations   = nullptr;
  bool                            MIsProcessing           = false;
  bool                            MIsActivated            = false;

  #ifndef MIP_NO_GUI
  MIP_Editor*                     MEditor                 = nullptr;
  bool                            MEditorIsOpen           = false;
  #endif


//------------------------------
public:
//------------------------------

  MIP_Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_ClapPlugin(ADescriptor,AHost) {
    MDescriptor = ADescriptor;
    MHost = new MIP_ClapHost(AHost);
    //uint32_t num = params_count();
    uint32_t num = MParameters.size();
    uint32_t size = num * sizeof(float);
    MAudioParamVal = (float*)malloc(size);
    MHostParamVal = (float*)malloc(size);
    MHostParamMod = (float*)malloc(size);
    memset(MAudioParamVal,0,size);
    memset(MHostParamVal,0,size);
    memset(MHostParamMod,0,size);
  }

  //----------

  virtual ~MIP_Plugin() {
    #ifndef MIP_NO_AUTODELETE
    deleteParameters();
    #endif
    delete MHost;
    free (MAudioParamVal);
    free (MHostParamVal);
    free (MHostParamMod);
  }

//------------------------------
public:
//------------------------------

  //#ifndef MIP_NO_GUI
  //  MIP_Editor* getEditor() { return MEditor; }
  //  bool isOpen() { return MEditorIsOpen; }
  //#endif

  float getParameterModulation(uint32_t AIndex) {
    return MParameterModulations[AIndex];
  }

  float getParameterValue(uint32_t AIndex) {
    return MParameterValues[AIndex];
  }

  void setParameterModulation(uint32_t AIndex, float AValue) {
    MParameterModulations[AIndex] = AValue;
  }

  void setParameterValue(uint32_t AIndex, float AValue) {
    MParameterValues[AIndex] = AValue;
  }

//  #ifndef MIP_NO_GUI
//  MIP_Editor* getEditor() { return MEditor; }
//  bool        isEditorOpen() { return MEditorIsOpen; }
//  #endif

//------------------------------
public:
//------------------------------

  virtual bool getParameterValueText(char* ABuffer, uint32_t AIndex, float AValue) {
    return false;
  }

//------------------------------
//private: // ??
protected: // ??
//------------------------------

  /*
    called from editor when widget changes (gui thread)
    editor.on_updateWidgetFromWindow
    editor_listener->on_updateParameterFromEditor
    flush queue in process (audio thread)
  */

  void queueAudioParam(uint32_t AIndex) {
    MAudioParamQueue.write(AIndex);
  }

  //----------

  /*
    called from start of process()
    flush all parameters queued from gui
    todo: check if value really changed (if multiple events)
  */

  void flushAudioParams() {
    uint32_t index = 0;
    while (MAudioParamQueue.read(&index)) {
      float value = MAudioParamVal[index];
      // if we already set this, it should be (bit) identical?
      if (value != MParameterValues[index]) {
        MParameterValues[index] = value;

        // notify plugin
        //.on_plugin_parameter(index,value);

      }
    }
  }

  //----------

  // editor_updateParameter()

  void queueHostParam(uint32_t AIndex) {
    MHostParamQueue.write(AIndex);
  }

  //----------

  // queued in on_updateParameterFromEditor()
  // called from handle_events_output (end of process), flush

  void flushHostParams(const clap_output_events_t* out_events) {
    uint32_t index = 0;
    while (MHostParamQueue.read(&index)) {
      float value = MHostParamVal[index];
      //todo: check if value reallyt changed (if multiple events)
      send_param_value_event(index,value,out_events);
    }
  }

  //----------

  // gestures

  void queueHostBeginGesture(uint32_t AIndex) {
    MHostBeginGestureQueue.write(AIndex);
  }

  //----------

  void queueHostEndGesture(uint32_t AIndex) {
    MHostEndGestureQueue.write(AIndex);
  }

  //----------

  void flushHostBeginGestures(const clap_output_events_t* out_events) {
    uint32_t index = 0;
    while (MHostBeginGestureQueue.read(&index)) {
      send_param_gesture_event(index,CLAP_EVENT_PARAM_GESTURE_BEGIN,out_events);
    }
  }

  //----------

  void flushHostEndGestures(const clap_output_events_t* out_events) {
    uint32_t index = 0;
    while (MHostEndGestureQueue.read(&index)) {
      send_param_gesture_event(index,CLAP_EVENT_PARAM_GESTURE_END,out_events);
    }
  }

//------------------------------
public: // editor listener
//------------------------------

  #ifndef MIP_NO_GUI

  void on_beginUpdateParameterFromEditor(uint32_t AIndex) final {
    queueHostBeginGesture(AIndex);
  }

  void on_endUpdateParameterFromEditor(uint32_t AIndex) final {
    queueHostEndGesture(AIndex);
  }

  /*
    called from editor when widget changes (gui thread)
    - editor.on_updateWidgetFromWindow
    - editor_listener->on_updateParameterFromEditor
    flushed in start of process() (audio thread)
  */

  void on_updateParameterFromEditor(uint32_t AIndex, float AValue) final {
    //MIP_Print("%i = %.3f\n",AIndex,AValue);
    MAudioParamVal[AIndex] = AValue;
    queueAudioParam(AIndex);
    MHostParamVal[AIndex] = AValue;
    queueHostParam(AIndex);
  }

  //----------

  void on_resizeFromEditor(uint32_t AWidth, uint32_t AHeight) final {
    if (MHost && MHost->gui) {
      if (!MHost->gui->request_resize(MHost->host,AWidth,AHeight)) {
        MIP_Print("host->gui->request_resize(%i,%i) returned false\n",AWidth,AHeight);
      }
    }
  }


  #endif

//------------------------------
public: // handle
//------------------------------

  /*
    called from process (audio thread)
  */

  virtual void handle_note_on_event(clap_event_note_t* event) {}
  virtual void handle_note_off_event(clap_event_note_t* event) {}
  virtual void handle_note_end_event(clap_event_note_t* event) {}
  virtual void handle_note_choke_event(clap_event_note_t* event) {}
  virtual void handle_note_expression_event(clap_event_note_expression_t* event) {}

  virtual void handle_midi_event(clap_event_midi_t* event) {}
  virtual void handle_midi2_event(clap_event_midi2_t* event) {}
  virtual void handle_midi_sysex_event(clap_event_midi_sysex_t* event) {}
  virtual void handle_transport_event(clap_event_transport_t* event) {}

  //----------

  virtual void handle_parameter_event(const clap_event_param_value_t* param_value) {
    uint32_t i = param_value->param_id;
    float v = param_value->value;
    setParameterValue(i,v);
    #ifndef MIP_NO_GUI
    if (MEditor && MEditorIsOpen) MEditor->updateParameterInProcess(i,v);
    #endif
  }

  //----------

  virtual void handle_modulation_event(const clap_event_param_mod_t* param_mod) {
    uint32_t i = param_mod->param_id;
    float v = param_mod->amount;
    setParameterModulation(i,v);
    #ifndef MIP_NO_GUI
    if (MEditor && MEditorIsOpen) MEditor->updateModulationFromHost(i,v);
    #endif
  }

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

    //float** inputs = process->audio_inputs[0].data32;
    //float** outputs = process->audio_outputs[0].data32;
    //uint32_t length = process->frames_count;
    //MIP_CopyStereoBuffer(outputs,inputs,length);

  }

//------------------------------
protected: // handle
//------------------------------

  virtual void handle_events_input(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
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
          case CLAP_EVENT_MIDI2:            handle_midi2_event((clap_event_midi2_t*)header); break;
          case CLAP_EVENT_MIDI_SYSEX:       handle_midi_sysex_event((clap_event_midi_sysex_t*)header); break;
          case CLAP_EVENT_TRANSPORT:        handle_transport_event((clap_event_transport_t*)header); break;
        }
      }
    }
  }

  //----------

  virtual void handle_events_output(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
    #ifndef MIP_NO_GUI
    //if (MEditor && MIsEditorOpen) {

      flushHostBeginGestures(out_events);
      flushHostParams(out_events);
      flushHostEndGestures(out_events);
    //}
    #endif
  }

//------------------------------
protected: // setup
//------------------------------

  //void setupParameters(clap_param_info_t* params, uint32_t num) {
  //  for (uint32_t i=0; i<num; i++) {
  //    const clap_param_info_t* info = &params[i];
  //    MParameters.append(info);
  //  }
  //}

  MIP_Parameter* appendParameter(MIP_Parameter* AParameter) {
    uint32_t index = MParameters.size();
    AParameter->setIndex(index);
    MParameters.append(AParameter);
    return AParameter;
  }

  //----------

  void deleteParameters() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      delete MParameters[i];
    }
  }

  //----------

  void setDefaultParameterValues() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      //const clap_param_info_t* info = &params[i];
      MParameterValues[i] = MParameters[i]->info.default_value;
      MParameterModulations[i] = 0.0;
    }
  }

  //----------

  void setEditorParameterValues() {
    #ifndef MIP_NO_GUI
    for (uint32_t i=0; i<MParameters.size(); i++) {
      if (MEditor) MEditor->setParameterValue(i,MParameterValues[i]);
    }
    #endif
  }

//------------------------------
protected: // setup
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

  //TODO: MIP_EditorListener -> MIP_Plugin

  void send_param_value_event(uint32_t index, float value, const clap_output_events_t* out_events) {
    clap_event_param_value_t param_value;
    param_value.header.size     = sizeof (clap_event_param_value_t);
    param_value.header.time     = 0;
    param_value.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    param_value.header.type     = CLAP_EVENT_PARAM_VALUE;
    param_value.header.flags    = 0;//CLAP_EVENT_DONT_RECORD;// | CLAP_EVENT_IS_LIVE;
    param_value.param_id        = index;
    param_value.cookie          = nullptr;
    param_value.port_index      = -1;
    param_value.key             = -1;
    param_value.channel         = -1;
    param_value.value           = value;
    clap_event_header_t* header = (clap_event_header_t*)&param_value;
    out_events->try_push(out_events,header);
  }

  //----------

  void send_param_mod_event(uint32_t index, float value, const clap_output_events_t* out_events) {
    clap_event_param_mod_t param_mod;
    param_mod.header.size     = sizeof (clap_event_param_mod_t);
    param_mod.header.time     = 0;
    param_mod.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    param_mod.header.type     = CLAP_EVENT_PARAM_MOD;
    param_mod.header.flags    = 0;//CLAP_EVENT_DONT_RECORD;// | CLAP_EVENT_IS_LIVE;
    param_mod.param_id        = index;
    param_mod.cookie          = nullptr;
    param_mod.port_index      = -1;
    param_mod.key             = -1;
    param_mod.channel         = -1;
    param_mod.amount          = value;
    clap_event_header_t* header = (clap_event_header_t*)&param_mod;
    out_events->try_push(out_events,header);
  }

  //----------

  void send_param_gesture_event(uint32_t index, int32_t param_gesture, const clap_output_events_t* out_events) {
    clap_event_param_gesture_t gesture;
    gesture.header.size     = sizeof (clap_event_param_gesture_t);
    gesture.header.time     = 0;
    gesture.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    gesture.header.type     = param_gesture;  //CLAP_EVENT_PARAM_GESTURE_BEGIN;
    gesture.header.flags    = 0;              //CLAP_EVENT_DONT_RECORD;// | CLAP_EVENT_IS_LIVE;
    gesture.param_id        = index;
    clap_event_header_t* header = (clap_event_header_t*)&gesture;
    out_events->try_push(out_events,header);
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() override {
    uint32_t num = MParameters.size();
    MParameterValues = (float*)malloc(num * sizeof(float));
    MParameterModulations = (float*)malloc(num * sizeof(float));
    for (uint32_t i=0; i<num; i++) {
      float v = MParameters[i]->info.default_value;
      setParameterValue(i,v);
      setParameterModulation(i,0);
    }
    return true;
  }

  //----------

  void destroy() override {
    free(MParameterValues);
    free(MParameterModulations);
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
    handle_events_input(process->in_events,process->out_events);
    handle_process(process);
    handle_events_output(process->in_events,process->out_events);
    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  const void* get_extension(const char *id) override {
    //MIP_Print("host asks for: %s\n",id);
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
        //case CLAP_EVENT_MIDI_SYSEX:       return true;
        //case CLAP_EVENT_MIDI2:            return true;
      }
    }
    return false;
  }

//------------------------------
public: // params
//------------------------------

  uint32_t params_count() override {
    //return MParameters.size();
    return MParameters.size();
  }

  //----------

  bool params_get_info(uint32_t param_index, clap_param_info_t* param_info) override {
    //memcpy(param_info,&MParameters[param_index]->info,sizeof(clap_param_info_t));
    clap_param_info_t* info = &MParameters[param_index]->info;
    memcpy(param_info,info,sizeof(clap_param_info_t));
    return true;
  }

  //----------

  bool params_get_value(clap_id param_id, double *value) override {
    *value = getParameterValue(param_id);
    return true;
  }

  //----------

  bool params_value_to_text(clap_id param_id, double value, char* display, uint32_t size) override {
    //sprintf(display,"%.3f",value);
    //return true;
    bool result = MParameters[param_id]->value_to_text(value,display,size);
    return result;
  }

  //----------

  bool params_text_to_value(clap_id param_id, const char* display, double* value) override {
    //float f = atof(display);
    //*value = f;
    //return true;
    bool result = MParameters[param_id]->text_to_value(display,value);
    return result;
  }

  //----------

  void params_flush(const clap_input_events_t* in, const clap_output_events_t* out) override {
    handle_events_input(in,out);
    handle_events_output(in,out);
  }

//------------------------------
public: // gui
//------------------------------

  #ifndef MIP_NO_GUI

  bool gui_is_api_supported(const char *api, bool is_floating) override {
    if (is_floating) return false;
    if (strcmp(api,CLAP_WINDOW_API_X11) == 0) return true;
    return false;
  }

  //----------

  bool gui_create(const char *api, bool is_floating) override {
    MIP_Print("api: %s floating: %s\n",api,is_floating?"true":"false");
    //MEditorIsOpen = false;
    if (is_floating) return false;
    MEditor = new MIP_Editor(this,this,256,256,false);
    if (MEditor) return true;
    return false;
  }

  //----------

  void gui_destroy() override {
    //MIP_Print("\n");
    if (MEditorIsOpen) {
      MEditorIsOpen = false;
      MEditor->hide();
    }
    delete MEditor;
    MEditor = nullptr;
  }

  //----------

  bool gui_set_scale(double scale) override {
    //MIP_Print("scale: %.2f\n",scale);
    if (MEditor) {
      bool res = MEditor->setScale(scale);
      //MIP_Print("%.3f -> %s\n", scale,res?"true":"false");
      return res;
    }
    else {
      //MIP_Print("!MEditor -> false\n");
      return true;
    }
    return false;
  }

  //----------

  bool gui_get_size(uint32_t *width, uint32_t *height) override {
    //MIP_Print("\n");
    if (MEditor) {
      bool res = MEditor->getSize(width,height);
      //MIP_Print("-> (%i,%i) %s\n", *width,*height,res?"true":"false");
      return res;
    }
    else {
      //MIP_Print("!MEditor -> false\n");
      return false;
    }
    return false;
  }

  //----------

  bool gui_can_resize() override {
    //MIP_Print("\n");
    if (MEditor) {
      bool res = MEditor->canResize();
      //MIP_Print("-> %s\n", res?"true":"false");
      return res;
    }
    else {
      //MIP_Print("!MEditor -> false\n");
      return false;
    }
    return false;
  }

  //----------

  bool gui_adjust_size(uint32_t *width, uint32_t *height) override {
    //MIP_Print("*width: %i *height: %i\n",*width,*height);
    if (MEditor) {
      //MIP_Print("(%i,%i) -> ", *width,*height);
      MEditor->adjustSize(width,height);
      //MIP_DPrint("(%i,%i)\n", *width,*height);
    }
    else {
      *width = 0;
      *height = 0;
      //MIP_Print("!MEditor -> (%i,%i)\n", *width,*height);
    }
    return false;
  }

  //----------

  bool gui_set_size(uint32_t width, uint32_t height) override {
    //MIP_Print("width: %i height: %i\n",width,height);
    if (MEditor) {
      bool res = MEditor->setSize(width,height);
      //MIP_Print("%i,%i -> %s\n", width,height,res?"true":"false");
      return res;
    }
    else {
      //MIP_Print("%i,%i : !MEditor -> false\n",width,height);
      return false;
    }
    return false;
  }

  //----------

  bool gui_set_parent(const clap_window_t *window) override {
    //MIP_Print("window: %p\n",window);
    if (MEditor) {
      //MIP_Print("'%s',%i -> true\n",display_name,window);
      MEditor->attach(nullptr,window->x11);
      return true;
    }
    else {
      //MIP_Print("!Meditor-> false\n");
    return false;
    }
  return false;
  }

  //----------

  bool gui_set_transient(const clap_window_t *window) override {
    MIP_Print("window: %p\n",window);
    return false;
  }

  //----------

  void gui_suggest_title(const char *title) override {
    MIP_Print("title: %s\n",title);
  }

  //----------

  bool gui_show() override {
    //MIP_Print("\n");
    if (MEditor) {

      setEditorParameterValues();

      //MIP_Print("\n");
      MEditor->show();
      MEditorIsOpen = true;
    }
    else {
      //MIP_Print("!MEditor\n");
    }
    return false;
  }

  //----------

  bool gui_hide() override {
    //MIP_Print("\n");
    if (MEditor) {
      //MIP_Print("\n");
      MEditorIsOpen = false;
      MEditor->hide();
    }
    else {
      //MIP_Print("!MEditor\n");
    }
    return false;
  }

  #endif // MIP_NO_GUI

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
