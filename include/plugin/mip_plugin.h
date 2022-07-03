#ifndef mip_plugin_included
#define mip_plugin_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_queue.h"
#include "plugin/mip_audio_port.h"
#include "plugin/mip_editor.h"
#include "plugin/mip_note_port.h"

#include "plugin/mip_parameter_manager.h"

#include "plugin/clap/mip_clap_plugin.h"
#include "plugin/clap/mip_clap_host.h"

#define MIP_PLUGIN_MAX_PARAM_EVENTS 4096
#define MIP_PLUGIN_MAX_GUI_EVENTS   32

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct MIP_ProcessContext {
  const clap_process_t* process;
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

  MIP_ProcessContext              MProcessContext;
  MIP_ParameterManager            MParameters = {};

  MIP_AudioPortArray              MAudioInputPorts  = {};
  MIP_AudioPortArray              MAudioOutputPorts = {};
  MIP_NotePortArray               MNoteInputPorts   = {};
  MIP_NotePortArray               MNoteOutputPorts  = {};

//------------------------------
public:
//------------------------------

  MIP_Plugin(const clap_plugin_descriptor_t* ADescriptor)
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

  //bool init() override {
  //  return true;
  //}

  //----------

  //void destroy() override {
  //}

  //----------

  //bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) override {
  //  return true;
  //}

  //----------

  //void deactivate() override {
  //}

  //----------

  //bool start_processing() override {
  //  return true;
  //}

  //----------

  //void stop_processing() override {
  //}

  //----------

  //void reset() override {
  //}

  //----------

  clap_process_status process(const clap_process_t *process) override {
    MProcessContext.process = process;
    processTransport(process->transport);
    preProcessEvents(process->in_events,process->out_events);
    MParameters.flushAudioParams();
    processAudioBlock(process);
    postProcessEvents(process->in_events,process->out_events);
    MParameters.flushHostParams(process->out_events);
    MProcessContext.process = nullptr;
    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  const void* get_extension(const char *id) override {
    //if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0) return &MAudioPorts;
    if (strcmp(id,CLAP_EXT_NOTE_PORTS) == 0) return &MNotePorts;
    if (strcmp(id,CLAP_EXT_PARAMS) == 0) return &MParams;
    return nullptr;
  }

  //----------

  //void on_main_thread() override {
  //}

//------------------------------
public: // clap audio poorts
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
public: // clap note ports
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
public: // clap params
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
    postProcessEvents(in,out);
  }

//------------------------------
public: // process events
//------------------------------

  virtual void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        processEvent(header);
      }
    }
  }

  //----------

  virtual void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
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
    //queueAudioParams
    //queueGuiParam
  }

  virtual void processParamModEvent(const clap_event_param_mod_t* event) {
    MIP_Print("PARAM MOD index %i value %.3f\n",event->param_id,event->amount);
    uint32_t index = event->param_id;
    double value = event->amount;
    MParameters[index]->setModulation(value);
    // notify plugin...
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
    //queueAudioParam(AIndex,AValue);
    //queueHostParam(AIndex,AValue);
  }

  //----------

  void on_editor_listener_resize_window(uint32_t AWidth, uint32_t AHeight) final {
  }

};

//----------------------------------------------------------------------
#endif
