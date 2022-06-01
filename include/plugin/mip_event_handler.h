#ifndef mip_event_handler_included
#define mip_event_handler_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/mip_parameter_manager.h"
#include "plugin/mip_note_handler.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_EventListener {
public:
  virtual void on_event_listener_parameter(uint32_t AIndex, double AValue) {}
  virtual void on_event_listener_modulation(uint32_t AIndex, double AValue) {}
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_EventHandler {

//------------------------------
private:
//------------------------------

  MIP_EventListener*    MListener = nullptr;
  MIP_ParameterManager* MParameterManager = nullptr;
  MIP_NoteHandler*      MNoteHandler = nullptr;



//------------------------------
public:
//------------------------------

  MIP_EventHandler(MIP_EventListener* AListener, MIP_ParameterManager* AParameterManager=nullptr, MIP_NoteHandler* ANoteHandler=nullptr) {
    MListener = AListener;
    MParameterManager = AParameterManager;
    MNoteHandler = ANoteHandler;
  }

  //----------

  virtual ~MIP_EventHandler() {
  }

//------------------------------
public:
//------------------------------

  //void preProcess(const clap_process_t *process) {
  void preProcess(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
    //const clap_input_events_t* in_events = process->in_events;
    //const clap_output_events_t* out_events = process->out_events;
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        handle_event(header);
      }
    }
  }

  //----------

  //void postProcess(const clap_process_t *process) {
  void postProcess(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
    //#ifndef MIP_NO_GUI
    //  //flushHostBeginGestures(out_events);
    //  flushHostParams(out_events);
    //  //flushHostEndGestures(out_events);
    //#endif
  }

//------------------------------
public:
//------------------------------

  void handle_event(const clap_event_header_t* header) {
    switch (header->type) {
      case CLAP_EVENT_PARAM_VALUE:      handle_parameter_event((clap_event_param_value_t*)header); break;
      case CLAP_EVENT_PARAM_MOD:        handle_modulation_event((clap_event_param_mod_t*)header); break;
      case CLAP_EVENT_TRANSPORT:        handle_transport_event((clap_event_transport_t*)header); break;
      case CLAP_EVENT_NOTE_ON:          handle_note_on_event((clap_event_note_t*)header); break;
      case CLAP_EVENT_NOTE_OFF:         handle_note_off_event((clap_event_note_t*)header); break;
      case CLAP_EVENT_NOTE_END:         handle_note_end_event((clap_event_note_t*)header); break;
      case CLAP_EVENT_NOTE_CHOKE:       handle_note_choke_event((clap_event_note_t*)header); break;
      case CLAP_EVENT_NOTE_EXPRESSION:  handle_note_expression_event((clap_event_note_expression_t*)header); break;
      case CLAP_EVENT_MIDI:             handle_midi_event((clap_event_midi_t*)header); break;
      case CLAP_EVENT_MIDI2:            handle_midi2_event((clap_event_midi2_t*)header); break;
      case CLAP_EVENT_MIDI_SYSEX:       handle_midi_sysex_event((clap_event_midi_sysex_t*)header); break;
    }
  };

  //----------

  void handle_parameter_event(clap_event_param_value_t* event) {
    if (MParameterManager) MParameterManager->handle_value_event(event);
    if (MListener) MListener->on_event_listener_parameter(event->param_id,event->value);
  }

  void handle_modulation_event(clap_event_param_mod_t* event) {
    if (MParameterManager) MParameterManager->handle_modulation_event(event);
    if (MListener) MListener->on_event_listener_modulation(event->param_id,event->amount);
  }

  void handle_transport_event(clap_event_transport_t* event) {
    MIP_Print("TODO\n");
  }

  void handle_note_on_event(clap_event_note_t* event) {
    if (MNoteHandler) MNoteHandler->handle_note_on_event(event);
  }

  void handle_note_off_event(clap_event_note_t* event) {
    if (MNoteHandler) MNoteHandler->handle_note_off_event(event);
  }

  void handle_note_end_event(clap_event_note_t* event) {
    MIP_Print("ERROR\n");
    //if (MNoteHandler) MNoteHandler->handle_note_end_event(event);
  }

  void handle_note_choke_event(clap_event_note_t* event) {
    if (MNoteHandler) MNoteHandler->handle_note_choke_event(event);
  }

  void handle_note_expression_event(clap_event_note_expression_t* event) {
    if (MNoteHandler) MNoteHandler->handle_note_expression_event(event);
  }

  //----------

  void handle_midi_event(clap_event_midi_t* event) {
    MIP_Print("TODO\n");
  }

  void handle_midi2_event(clap_event_midi2_t* event) {
    MIP_Print("TODO\n");
  }

  void handle_midi_sysex_event(clap_event_midi_sysex_t* event) {
    MIP_Print("TODO\n");
  }

};

//----------------------------------------------------------------------
#endif
