#ifndef mip_event_handler_included
#define mip_event_handler_included
//----------------------------------------------------------------------

//  void handleEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
//  //void handleEvents(const clap_process_t* process) {
//    //const clap_input_events_t* in_events = process->in_events;
//    uint32_t num_events = in_events->size(in_events);
//    for (uint32_t i=0; i<num_events; i++) {
//      const clap_event_header_t* header = in_events->get(in_events,i);
//      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
//        handleEvent(header);
//      }
//    }
//  }

  //----------

//  void handleEvent(const clap_event_header_t* header) {
//    switch (header->type) {
//      case CLAP_EVENT_NOTE_ON:          handleNoteOnEvent((const clap_event_note_t*)header);                break;
//      case CLAP_EVENT_NOTE_OFF:         handleNoteOff((const clap_event_note_t*)header);                    break;
//      case CLAP_EVENT_NOTE_CHOKE:       handleNoteChoke((const clap_event_note_t*)header);                  break;
//      case CLAP_EVENT_NOTE_EXPRESSION:  handleNoteExpression((const clap_event_note_expression_t*)header);  break;
//      case CLAP_EVENT_PARAM_VALUE:      handleParamValue((const clap_event_param_value_t*)header);          break;
//      case CLAP_EVENT_PARAM_MOD:        handleParamMod((const clap_event_param_mod_t*)header);              break;
//      case CLAP_EVENT_MIDI:             handleMidi((const clap_event_midi_t*)header);                       break;
//      case CLAP_EVENT_MIDI_SYSEX:       handleMidiSysex((const clap_event_midi_sysex_t*)header);            break;
//      case CLAP_EVENT_MIDI2:            handleMidi2((const clap_event_midi2_t*)header);                     break;
//    }
//  }


//----------------------------------------------------------------------
#endif
