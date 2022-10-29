#ifndef mip_voice_manager_included
#define mip_voice_manager_included
//----------------------------------------------------------------------

#include "mip.h"
#include "audio/mip_audio_utils.h"
//#include "base/types/mip_queue.h"
//#include "plugin/clap/mip_clap.h"
#include "plugin/mip_note.h"
#include "plugin/mip_voice.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class VOICE, int VOICE_COUNT>
class MIP_VoiceManager {

  // note_end queue
  typedef MIP_Queue<MIP_Note,VOICE_COUNT * 2> MIP_NoteQueue; // why *2 ?

  #define MIP_VOICE_NUM_CHANNELS    16
  #define MIP_VOICE_NUM_NOTES       128
  #define MIP_VOICE_MAX_FRAMESIZE   4096
  #define MIP_VOICE_BUFFERSIZE      (VOICE_COUNT * MIP_VOICE_MAX_FRAMESIZE)

//------------------------------
private:
//------------------------------

  __MIP_ALIGNED(MIP_ALIGNMENT_CACHE) float MVoiceBuffer[MIP_VOICE_BUFFERSIZE]     = {0};
  __MIP_ALIGNED(MIP_ALIGNMENT_CACHE) float MFrameBuffer[MIP_VOICE_MAX_FRAMESIZE]  = {0};

  MIP_VoiceContext  MVoiceContext                 = {};
  MIP_NoteQueue     MNoteEndQueue                 = {};
  MIP_Voice<VOICE>  MVoices[VOICE_COUNT]          = {};
  uint32_t          MThreadedVoices[VOICE_COUNT]  = {0};

//------------------------------
public:
//------------------------------

  MIP_VoiceManager() {
  }

  //----------

  ~MIP_VoiceManager() {
  }

//------------------------------
public:
//------------------------------

  void setup(float ASampleRate, uint32_t min_frames_count, uint32_t max_frames_count, MIP_ParameterArray* AParameters) {
    MIP_Assert(ASampleRate > 0);
    MVoiceContext.samplerate = ASampleRate;
    MVoiceContext.invsamplerate = 1.0 / ASampleRate;
    MVoiceContext.parameters = AParameters;
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      MVoices[i].state = MIP_VOICE_OFF;
      MVoices[i].prepare(i,&MVoiceContext);
    }
  }

  //----------

  uint32_t getVoiceState(uint32_t AIndex) {
    return MVoices[AIndex].state;
  }

  float* getFrameBuffer() {
    return MFrameBuffer;
  }

  //----------

//  void setParameter(uint32_t AIndex, double AValue) {
//    for (uint32_t i=0; i<VOICE_COUNT; i++) {
//      MVoices[i].parameter(AIndex,AValue);
//    }
//  }

  //----------

//  void setModulation(uint32_t AIndex, double AValue) {
//    for (uint32_t i=0; i<VOICE_COUNT; i++) {
//      MVoices[i].modulation(AIndex,AValue);
//    }
//  }

  //

  //void updateParameterFromEditor(uint32_t AIndex, double AValue) {
  void updateParameterFromEditor(MIP_Parameter* AParameter) {
    //MVoices.setParameter(AIndex,AValue);
    clap_event_param_value_t event;
    event.header.size     = sizeof(clap_event_param_value_t);
    event.header.time     = 0;
    event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    event.header.type     = CLAP_EVENT_PARAM_VALUE;
    event.header.flags    = 0; //CLAP_EVENT_IS_LIVE; // _DONT_RECORD
    event.param_id        = AParameter->getIndex();//AIndex;
    event.cookie          = AParameter;//MParameters[AIndex];
    event.note_id         = -1;
    event.port_index      = -1;
    event.channel         = -1;
    event.key             = -1;
    event.value           = AParameter->getValue();//AValue;
    //processParamValue/*Event*/(&event);
    const clap_event_header_t* header = (const clap_event_header_t*)&event;
    handleEvent(header);
  }


//------------------------------
public:
//------------------------------

  void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  }

  //----------

  //void processEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  //  // do nothing..
  //}

  //----------

  void processAudioBlock(MIP_ProcessContext* AContext) {
    const clap_process_t* process = AContext->process;
    MVoiceContext.process = process;
    float* out0 = process->audio_outputs->data32[0];
    float* out1 = process->audio_outputs->data32[1];
    uint32_t length = process->frames_count;

    MIP_ClearMonoBuffer(MVoiceBuffer,length);
    MVoiceContext.voicebuffer = MVoiceBuffer;

    uint32_t current_time = 0;
    uint32_t remaining = process->frames_count;
    uint32_t num_events = process->in_events->size(process->in_events);
    for (uint32_t ev=0; ev<num_events; ev++) {
      const clap_event_header_t* header = process->in_events->get(process->in_events,ev);
      // todo if header space..
      if (header->time > current_time) {
        // event is in the 'future'
        uint32_t num = header->time - current_time;
        // process num samples from current_time
        for (uint32_t v=0; v<VOICE_COUNT; v++) {
          uint32_t state = MVoices[v].state;
          if ((state == MIP_VOICE_PLAYING)
            || (state == MIP_VOICE_RELEASED)
            || (state == MIP_VOICE_WAITING)) {
            MVoices[v].state = MVoices[v].process( state, num, current_time);

            //MIP_AddMonoBuffer(MFrameBuffer + current_time,MVoiceBuffer + current_time,num);

          }
        }
        current_time += num;
        remaining -= num;
      }
      handleEvent(header);
    }
    // calculate remaining samples (no more events)
    if (remaining > 0) {
      for (uint32_t v=0; v<VOICE_COUNT; v++) {
        uint32_t state = MVoices[v].state;
        if ((state == MIP_VOICE_PLAYING)
          || (state == MIP_VOICE_RELEASED)
          /*|| (state == MIP_VOICE_WAITING)*/) {
          MVoices[v].state = MVoices[v].process( state, remaining, current_time);

          //MIP_AddMonoBuffer(MFrameBuffer + current_time,MVoiceBuffer + current_time,remaining);

        }
      }
    }

    MIP_CopyMonoBuffer(out0,MVoiceBuffer,length);
    MIP_CopyMonoBuffer(out1,MVoiceBuffer,length);

  }

  //----------

  void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
    flushFinishedVoices();
    flushNoteEnds();
  }

//------------------------------
public:
//------------------------------

  void handleEvent(const clap_event_header_t* header) {
    switch (header->type) {
      case CLAP_EVENT_NOTE_ON:          handleNoteOn((const clap_event_note_t*)header);                     break;
      case CLAP_EVENT_NOTE_OFF:         handleNoteOff((const clap_event_note_t*)header);                    break;
      case CLAP_EVENT_NOTE_CHOKE:       handleNoteChoke((const clap_event_note_t*)header);                  break;
      case CLAP_EVENT_NOTE_EXPRESSION:  handleNoteExpression((const clap_event_note_expression_t*)header);  break;
      case CLAP_EVENT_PARAM_VALUE:      handleParamValue((const clap_event_param_value_t*)header);          break;
      case CLAP_EVENT_PARAM_MOD:        handleParamMod((const clap_event_param_mod_t*)header);              break;
      case CLAP_EVENT_MIDI:             handleMidi((const clap_event_midi_t*)header);                       break;
      case CLAP_EVENT_MIDI_SYSEX:       handleMidiSysex((const clap_event_midi_sysex_t*)header);            break;
      case CLAP_EVENT_MIDI2:            handleMidi2((const clap_event_midi2_t*)header);                     break;
    }
  }

  //----------

  /*
    note_id: 16,32, 48, ...
    start new voices at the start of the audio block
  */

  void handleNoteOn(const clap_event_note_t* event) {
    //MIP_Print("NOTE ON: key %i channel %i port %i note_id %i\n",event->key,event->channel,event->port_index,event->note_id);
    int32_t voice = findAvailableVoice(true); // true = search released voices too
    if (voice >= 0) {
      // let the host know that the note that was playing for this voice
      // now has ended
      if (MVoices[voice].state == MIP_VOICE_RELEASED) {
        queueNoteEnd(MVoices[voice].note);
      }
      // start new voice
      MVoices[voice].note_on(event->key,event->velocity);
      //MIP_Print("new note..\n");
      MVoices[voice].note.key         = event->key;
      MVoices[voice].note.channel     = event->channel;
      MVoices[voice].note.port_index  = event->port_index;
      MVoices[voice].note.note_id     = event->note_id;
    }
    else {
      //MIP_Print("no note..\n");
      MIP_Note note;
      note.key         = event->key;
      note.channel     = event->channel;
      note.port_index  = event->port_index;
      note.note_id     = event->note_id;
      queueNoteEnd(note);
    }
  }

  //----------

  // note_id is always -1

  void handleNoteOff(const clap_event_note_t* event) {
    //MIP_Print("NOTE OFF: key %i channel %i port %i note_id %i\n",event->key,event->channel,event->port_index,event->note_id);
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      if (MVoices[i].state == MIP_VOICE_PLAYING) {
        //if (MVoices[i].note.note_id == event->note_id) {
          if ((MVoices[i].note.channel == event->channel) && (MVoices[i].note.key == event->key)) {
            MVoices[i].note_off(event->velocity);
          }
        //}
      }
    }
  }

  //----------

  void handleNoteChoke(const clap_event_note_t* event) {
    //MIP_Print("NOTE CHOKE: key %i channel %i port %i note_id %i\n",event->key,event->channel,event->port_index,event->note_id);
  }

  //----------

  // note_id is always -1

  void handleNoteExpression(const clap_event_note_expression_t* event) {
    //MIP_Print("id %i port %i chan %i key %i val %.3f\n",event->note_id,event->port_index,event->channel,event->key,event->value);
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      //if (MVoices[i].note.note_id == event->note_id) {
        if ((MVoices[i].note.key == event->key) && (MVoices[i].note.channel == event->channel)) {
          MVoices[i].expression(event->expression_id,event->value);
        }
      //}
    }
  }

  //----------

  /*
    parameter.flags : CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID

    called from event
    see also: voiceParameter (called from gui)
  */

  void handleParamValue(const clap_event_param_value_t* event) {
    //MIP_Print("param %i note %i port %i chan %i key %i value %.3f\n",event->param_id,event->note_id,event->port_index,event->channel,event->key,event->value);
    if (event->note_id == -1) {
      // global
      for (uint32_t i=0; i<VOICE_COUNT; i++) {
        //if ((MVoices[i].state == MIP_VOICE_PLAYING) || (MVoices[i].state == MIP_VOICE_RELEASED)) {
        //if ((MVoices[i].note.key == event->key) && (MVoices[i].note.channel == event->channel)) {
          MVoices[i].parameter(event->param_id,event->value);
        //}
        //}
      }
    }
    else {
      // voice
      for (uint32_t i=0; i<VOICE_COUNT; i++) {
        if (MVoices[i].note.note_id == event->note_id) {
          //if ((MVoices[i].note.key == event->key) && (MVoices[i].note.channel == event->channel)) {
            MVoices[i].parameter(event->param_id,event->value);
          //}
        }
      }
    }
  }

  //----------

  // parameter.flags : CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID
  // mono: all is -l
  // poly: all is ok

  void handleParamMod(const clap_event_param_mod_t* event) {
    //MIP_Print("param %i note %i port %i chan %i key %i amount %.3f\n",event->param_id,event->note_id,event->port_index,event->channel,event->key,event->amount);
    if (event->note_id == -1) {
      // global
      for (uint32_t i=0; i<VOICE_COUNT; i++) {
        if ((MVoices[i].state == MIP_VOICE_PLAYING) || (MVoices[i].state == MIP_VOICE_RELEASED)) {
        //if ((MVoices[i].note.key == event->key) && (MVoices[i].note.channel == event->channel)) {
          MVoices[i].modulation(event->param_id,event->amount);
        //}
        }
      }
    }
    else {
      // voice
      for (uint32_t i=0; i<VOICE_COUNT; i++) {
        if (MVoices[i].note.note_id == event->note_id) {
          //if ((MVoices[i].note.key == event->key) && (MVoices[i].note.channel == event->channel)) {
          MVoices[i].modulation(event->param_id,event->amount);
          //}
        }
      }
    }
  }

//------------------------------
//
//------------------------------

  void handleMidi(const clap_event_midi_t* event) {
    //MIP_Print("port %i data %02x,%02x,%02x\n",event->port_index,event->data[0],event->data[1],event->data[2]);
  }

  //----------

  void handleMidiSysex(const clap_event_midi_sysex_t* event) {
    //MIP_Print("port %i size %i\n",event->port_index,event->size);
  }

  //----------

  void handleMidi2(const clap_event_midi2_t* event) {
    //MIP_Print("port %i data %i,%i,%i,%i\n",event->port_index,event->data[0],event->data[1],event->data[2],event->data[3]);
  }

//------------------------------
private: // voices
//------------------------------

  int32_t findQuietestReleasedVoice() {
    float lowest = 999999.0;
    int32_t voice = -1;
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      if (MVoices[i].state == MIP_VOICE_RELEASED) {
        float env = MVoices[i].getLevel();
        if (env < lowest) {
          voice = i;
          lowest = env;
        }
      }
    }
    return voice;
  }

  //----------

  int32_t findAvailableVoice(bool ATryReleased=false) {
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      if (MVoices[i].state == MIP_VOICE_OFF) return i;
    }
    if (ATryReleased) {
      return findQuietestReleasedVoice();
    }
    return -1;
  }

//------------------------------
public:
//------------------------------

  void flushFinishedVoices() {
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      if (MVoices[i].state == MIP_VOICE_FINISHED) {
        queueNoteEnd(MVoices[i].note);
        MVoices[i].state = MIP_VOICE_OFF;
        MVoices[i].note = MIP_Note();
      }
      if (MVoices[i].state == MIP_VOICE_WAITING) {
        /*
          wasn't started, or stopped again?
          do we need to send note_end?
          (could there be dangling mod voices?)
        */
        MVoices[i].state = MIP_VOICE_OFF;
        MVoices[i].note = MIP_Note();
      }
    }
  }

  //----------

  void queueNoteEnd(MIP_Note ANote) {
    MNoteEndQueue.write(ANote);
  }

  //----------

  void flushNoteEnds() {
    const clap_process_t* process = MVoiceContext.process;
    const clap_output_events_t* out_events = process->out_events;
    MIP_Note note;
    while (MNoteEndQueue.read(&note)) {
      sendNoteEnd(note,out_events);
    }
  }

  //----------

  void sendNoteEnd(MIP_Note ANote, const clap_output_events_t* out_events) {
    clap_event_note_t event;
    clap_event_header_t* header = (clap_event_header_t*)&event;
    header->size      = sizeof(clap_event_note_t);
    header->time      = 0; // crashed if this was anything else than 0
    header->space_id  = CLAP_CORE_EVENT_SPACE_ID;
    header->type      = CLAP_EVENT_NOTE_END;
    header->flags     = 0;
    event.note_id     = ANote.note_id;
    event.port_index  = ANote.port_index;
    event.key         = ANote.key;
    event.channel     = ANote.channel;
    event.velocity    = 0.0;
    out_events->try_push(out_events,header);
  }

};

//----------------------------------------------------------------------
#endif

























#if 0





//------------------------------
public:
//------------------------------

  void prepareVoices(float ASampleRate, MIP_ParameterArray* AParameters) {
    MIP_Assert(ASampleRate > 0);
    MVoiceContext.samplerate = ASampleRate;
    MVoiceContext.invsamplerate = 1.0 / ASampleRate;
    MVoiceContext.parameters = AParameters;
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      MVoices[i].state = MIP_VOICE_OFF;
      MVoices[i].prepare(&MVoiceContext);
    }
  }

  //----------

  uint32_t getVoiceState(uint32_t AIndex) {
    return MVoices[AIndex].state;
  }

  //----------

  void setParameter(uint32_t AIndex, float AValue) {
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      MVoices[i].parameter(AIndex,AValue);
    }
  }

  //----------

  void setModulation(uint32_t AIndex, float AValue) {
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      MVoices[i].modulation(AIndex,AValue);
    }
  }

//------------------------------
public: // process
//------------------------------

  void process_() {
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      if ((MVoices[i].state == MIP_VOICE_PLAYING)
        || (MVoices[i].state == MIP_VOICE_RELEASED)
        || (MVoices[i].state == MIP_VOICE_WAITING)) {
        //MThreadedVoices[num_voices++] = i;
      }
    }
  }


  void process(const clap_process_t *process) {
    MVoiceContext.process = process;
    //float* out0 = process->audio_outputs->data32[0];
    //float* out1 = process->audio_outputs->data32[1];
    MVoiceContext.voicebuffer = MVoiceBuffer;//out0;
    uint32_t length = process->frames_count;
    MIP_ClearMonoBuffer(MFrameBuffer,length);
    uint32_t current_time = 0;
    uint32_t remaining = process->frames_count;
    uint32_t num_events = process->in_events->size(process->in_events);
    for (uint32_t ev=0; ev<num_events; ev++) {
      const clap_event_header_t* header = process->in_events->get(process->in_events,ev);
      if (header->time > current_time) {
        // event is in the 'future'
        uint32_t num = header->time - current_time;
        // process num samples from current_time

        //processPlayingVoices();
        for (uint32_t v=0; v<VOICE_COUNT; v++) {
          uint32_t state = MVoices[v].state;
          if ((state == MIP_VOICE_PLAYING)
            || (state == MIP_VOICE_RELEASED)
            || (state == MIP_VOICE_WAITING)) {
            MVoices[v].state = MVoices[v].process( state, num, current_time);
          }
        }

        current_time += num;
        remaining -= num;
      }
      // process event
    }
    // calculate remaining samples (no more events)
    if (remaining > 0) {

      for (uint32_t v=0; v<VOICE_COUNT; v++) {
        uint32_t state = MVoices[v].state;
        if ((state == MIP_VOICE_PLAYING)
          || (state == MIP_VOICE_RELEASED)
          /*|| (state == MIP_VOICE_WAITING)*/) {
          MVoices[v].state = MVoices[v].process( state, remaining, current_time);
        }
      }

    }

  }







//        // we have more events
//        int32_t length = next_event.time - current_time;
//        if (length > 0) {
//
//          if (state != MIP_VOICE_WAITING) {
//          //if ((state == MIP_VOICE_PLAYING) || ((state == MIP_VOICE_RELEASED)) {
//
//            state = voice.process(AIndex,state,length,current_time);
//          }
//          remaining -= length;
//          current_time += length;
//        }
//        handleVoiceEvent(next_event);
//      } // event
//
//      else {
//        // no more events
//        int32_t length = remaining;
//
//        if (state != MIP_VOICE_WAITING) {
//        //if ((state == MIP_VOICE_PLAYING) || ((state == MIP_VOICE_RELEASED)) {
//
//          state = voice.process(AIndex,state,length,current_time);
//        }
//        remaining -= length;
//        current_time += length;
//      } // !event
//
//    MIP_Assert( events.read(&next_event) == false );
//  }

  //----------

//------------------------------
public: // events
//------------------------------

  // called from:
  // - processBlock
  // - processThreaded

  void handleEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  //void handleEvents(const clap_process_t* process) {
    //const clap_input_events_t* in_events = process->in_events;
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        handleEvent(header);
      }
    }
  }

  //----------

  // AIndex = event index
  // AOffset = budffer offset
  // handle all events from AIndex, with offset AOffset
  // returns samples until next event, or MIP_UINT32_MAX if no more

  //uint32_t handleEventsAt(const clap_process_t* process, uint32_t AIndex, uint32_t AOffset) {
  //  return MIP_UINT32_MAX;
  //}

  //----------






#endif // 0
