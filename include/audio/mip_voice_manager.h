#ifndef mip_voice_manager_included
#define mip_voice_manager_included
//----------------------------------------------------------------------

#include "mip.h"
#include "audio/mip_audio_utils.h"
#include "base/types/mip_queue.h"
#include "plugin/clap/mip_clap.h"

#define MIP_VOICE_NUM_CHANNELS  16
#define MIP_VOICE_NUM_NOTES     128
#define MIP_VOICE_MAX_NOTE_ENDS 128
#define MIP_VOICE_SLICE_SIZE    16
#define MIP_VOICE_BUFFERSIZE    1024

//----------------------------------------------------------------------

struct MIP_Note {
  int32_t note_id     = -1; // -1 if unspecified, otherwise >0
  int32_t port_index  = 0;
  int32_t channel     = 0;  // 0..15
  int32_t key         = 0;  // 0..127
};

//----------

struct MIP_Voice {
  MIP_Note  note = {};
  uint32_t  state = MIP_VOICE_OFF;
};

//----------

struct MIP_VoiceContext {
  const clap_process_t* process = nullptr;
  double samplerate = 0.0;
};

//----------

typedef MIP_Queue<MIP_Note,MIP_VOICE_MAX_NOTE_ENDS> MIP_NoteQueue;

//----------

//----------------------------------------------------------------------
//
// voice buffers
//
//----------------------------------------------------------------------

__MIP_ALIGNED(MIP_ALIGNMENT_CACHE)
float MIP_VoiceBuffer[MIP_VOICE_BUFFERSIZE] = {0};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class VOICE, int NUM_VOICES>
class MIP_VoiceManager {

//------------------------------
private:
//------------------------------

  MIP_VoiceContext  MVoiceContext             = {};
  VOICE             MVoices[NUM_VOICES]       = {};
  MIP_NoteQueue     MNoteEndQueue             = {};

//------------------------------
public:
//------------------------------

  MIP_VoiceManager(int dummy) {
  }

  //----------

  ~MIP_VoiceManager() {
  }

//------------------------------
public: // events
//------------------------------

  void handleEvents(const clap_process_t* process) {
    const clap_input_events_t* in_events = process->in_events;
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        handleEvent(header);
      }
    }
  }

  //----------

  void handleEvent(const clap_event_header_t* header) {
    switch (header->type) {
      case CLAP_EVENT_NOTE_ON:              handleNoteOnEvent((const clap_event_note_t*)header); break;
      case CLAP_EVENT_NOTE_OFF:             handleNoteOff((const clap_event_note_t*)header); break;
      case CLAP_EVENT_NOTE_CHOKE:           handleNoteChoke((const clap_event_note_t*)header); break;
      case CLAP_EVENT_NOTE_EXPRESSION:      handleNoteExpression((const clap_event_note_expression_t*)header); break;
      case CLAP_EVENT_PARAM_VALUE:          handleParamValue((const clap_event_param_value_t*)header); break;
      case CLAP_EVENT_PARAM_MOD:            handleParamMod((const clap_event_param_mod_t*)header); break;
      case CLAP_EVENT_MIDI:                 handleMidi((const clap_event_midi_t*)header); break;
      case CLAP_EVENT_MIDI_SYSEX:           handleMidiSysex((const clap_event_midi_sysex_t*)header); break;
      case CLAP_EVENT_MIDI2:                handleMidi2((const clap_event_midi2_t*)header); break;
    }
  }

//------------------------------
private: // events
//------------------------------

  void handleNoteOnEvent(const clap_event_note_t* event) {
    //MIP_Print("id %i port %i chan %i key %i val %.3f\n",event->note_id,event->port_index,event->channel,event->key,event->velocity);
    int32_t voice = findFreeVoice(true);
    if (voice >= 0) {
      MVoices[voice].note.note_id     = event->note_id;
      MVoices[voice].note.port_index  = event->port_index;
      MVoices[voice].note.channel     = event->channel;
      MVoices[voice].note.key         = event->key;
      MVoices[voice].state            = MVoices[voice].note_on(event->key,event->velocity);
    }
  }

  //----------

  void handleNoteOff(const clap_event_note_t* event) {
    //MIP_Print("id %i port %i chan %i key %i val %.3f\n",event->note_id,event->port_index,event->channel,event->key,event->velocity);
    for (uint32_t i=0; i<NUM_VOICES; i++) {
      if (MVoices[i].state == MIP_VOICE_PLAYING) {
        //if (MVoices[i].note.note_id == event->note_id) {
          if ((MVoices[i].note.key == event->key) && (MVoices[i].note.channel == event->channel)) {
            MVoices[i].state = MVoices[i].note_off(event->velocity);
          }
        //}
      }
    }
  }

  //----------

  void handleNoteChoke(const clap_event_note_t* event) {
    //MIP_Print("id %i port %i chan %i key %i val %.3f\n",event->note_id,event->port_index,event->channel,event->key,event->velocity);
  }

  //----------

  void handleNoteExpression(const clap_event_note_expression_t* event) {
    //MIP_Print("id %i port %i chan %i key %i val %.3f\n",event->note_id,event->port_index,event->channel,event->key,event->velocity);
    for (uint32_t i=0; i<NUM_VOICES; i++) {
      //if (MVoices[i].note.note_id == event->note_id) {
        if ((MVoices[i].note.key == event->key) && (MVoices[i].note.channel == event->channel)) {
          //MVoices[i].expression(event->expression_id,event->value);
          switch (event->expression_id) {
            case CLAP_NOTE_EXPRESSION_VOLUME:     MVoices[i].volume(event->value);      break;
            case CLAP_NOTE_EXPRESSION_PAN:        MVoices[i].pan(event->value);         break;
            case CLAP_NOTE_EXPRESSION_TUNING:     MVoices[i].tuning(event->value);      break;
            case CLAP_NOTE_EXPRESSION_VIBRATO:    MVoices[i].vibrato(event->value);     break;
            case CLAP_NOTE_EXPRESSION_EXPRESSION: MVoices[i].expression(event->value);  break;
            case CLAP_NOTE_EXPRESSION_BRIGHTNESS: MVoices[i].brightness(event->value);  break;
            case CLAP_NOTE_EXPRESSION_PRESSURE:   MVoices[i].pressure(event->value);    break;
          }
        }
      //}
    }
  }

  //----------

  void handleParamValue(const clap_event_param_value_t* event) {
    //MIP_Print("id %i port %i chan %i key %i value %.3f\n",event->note_id,event->port_index,event->channel,event->key,event->value);
    for (uint32_t i=0; i<NUM_VOICES; i++) {
      if (MVoices[i].note.note_id == event->note_id) {
        //if ((MVoices[i].note.key == event->key) && (MVoices[i].note.channel == event->channel)) {
          MVoices[i].parameter(event->param_id,event->value);
        //}
      }
    }
  }

  //----------

  void handleParamMod(const clap_event_param_mod_t* event) {
    //MIP_Print("id %i port %i chan %i key %i amount %.3f\n",event->note_id,event->port_index,event->channel,event->key,event->amount);
    for (uint32_t i=0; i<NUM_VOICES; i++) {
      if (MVoices[i].note.note_id == event->note_id) {
        //if ((MVoices[i].note.key == event->key) && (MVoices[i].note.channel == event->channel)) {
          MVoices[i].modulation(event->param_id,event->amount);
        //}
      }
    }
  }

  //----------

  void handleMidi(const clap_event_midi_t* event) {
    //MIP_Print("port %i data %02x,%02x,%02x\n",event->port_index,event->data[0],event->data[1],event->data[2]);
  }

  //----------

  void handleMidiSysex(const clap_event_midi_sysex_t* event) {
    //IP_Print("port %i size %i\n",event->port_index,event->size);
  }

  //----------

  void handleMidi2(const clap_event_midi2_t* event) {
    //MIP_Print("port %i data %i,%i,%i,%i\n",event->port_index,event->data[0],event->data[1],event->data[2],event->data[3]);
  }

//------------------------------
private: // note_end
//------------------------------

  void sendNoteEnd(MIP_Note ANote, const clap_output_events_t* out_events) {
    //MIP_Print("NOTE_END id %i channel %i key %i\n",ANote.note_id,ANote.channel,ANote.key);
    clap_event_note_t event;
    clap_event_header_t* header = (clap_event_header_t*)&event;
    header->size      = sizeof(clap_event_note_t);
    header->time      = 0;//time; // crashed if this was anything else than 0
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

  void queueNoteEnd(MIP_Note ANote) {
    MNoteEndQueue.write(ANote);
  }

  //----------

  void flushNoteEnds(const clap_output_events_t* out_events) {
    MIP_Note note;
    while (MNoteEndQueue.read(&note)) {
      //MIP_Print("NOTE_END.. chan %i key %i\n",channel,key);
      sendNoteEnd(note,out_events);
    }
  }

//------------------------------
public: // voices
//------------------------------

  void prepareVoices(double ASampleRate) {
    MVoiceContext.samplerate = ASampleRate;
    for (uint32_t i=0; i<NUM_VOICES; i++) {
      MVoices[i].state = MIP_VOICE_OFF;
      MVoices[i].prepare(&MVoiceContext);
    }
  }

  //----------

  uint32_t getVoiceState(uint32_t AIndex) {
    return MVoices[AIndex].state;
  }

  //----------

  void voiceParameter(uint32_t AIndex, float AValue) {
    for (uint32_t i=0; i<NUM_VOICES; i++) {
      MVoices[i].parameter(AIndex,AValue);
    }
  }

  //----------

  void voiceModulation(uint32_t AIndex, float AValue) {
    for (uint32_t i=0; i<NUM_VOICES; i++) {
      MVoices[i].modulation(AIndex,AValue);
    }
  }

//------------------------------
private: // voices
//------------------------------

  int32_t findFreeVoice(bool ATryReleased=false) {
    for (uint32_t i=0; i<NUM_VOICES; i++) {
      if (MVoices[i].state == MIP_VOICE_OFF) return i;
      //if (MVoices[i].state == MIP_VOICE_FINISHED) return i;
    }
    /*
      find released voice with lowest amp_env.getValue();
    */
    if (ATryReleased) {
      for (uint32_t i=0; i<NUM_VOICES; i++) {
        if (MVoices[i].state == MIP_VOICE_RELEASED) return i;
      }
    }
    return -1;
  }

  //----------

  void flushVoices() {
    for (uint32_t i=0; i<NUM_VOICES; i++) {
      if (MVoices[i].state == MIP_VOICE_FINISHED) {
        queueNoteEnd(MVoices[i].note);
        //stopVoice(i);
        MVoices[i].state = MIP_VOICE_OFF;
        MVoices[i].note = MIP_Note();
      }
    }
  }

  void processPlayingVoices(uint32_t AOffset, uint32_t ALength) {
    for (uint32_t i=0; i<NUM_VOICES; i++) {
      if (MVoices[i].state == MIP_VOICE_PLAYING) {
        MVoices[i].state = MVoices[i].process(MIP_VOICE_PLAYING,ALength);
      }
    }
  }

  void processReleasedVoices(uint32_t AOffset, uint32_t ALength) {
    for (uint32_t i=0; i<NUM_VOICES; i++) {
      if (MVoices[i].state == MIP_VOICE_RELEASED) {
        MVoices[i].state = MVoices[i].process(MIP_VOICE_RELEASED,ALength);
      }
    }
  }

//------------------------------
public: // process
//------------------------------

  void process(const clap_process_t* process) {
    MVoiceContext.process = process;
    uint32_t length = process->frames_count;
    float* out0 = process->audio_outputs->data32[0];
    float* out1 = process->audio_outputs->data32[1];
    MIP_ClearMonoBuffer(MIP_VoiceBuffer,length);
    MIP_ClearMonoBuffer(MIP_VoiceBuffer,length);
    handleEvents(process);
    processPlayingVoices(0,length);
    processReleasedVoices(0,length);
    MIP_CopyMonoBuffer(out0,MIP_VoiceBuffer,length);
    MIP_CopyMonoBuffer(out1,MIP_VoiceBuffer,length);
    flushVoices();
    flushNoteEnds(process->out_events);
  }

};

//----------------------------------------------------------------------
#endif
