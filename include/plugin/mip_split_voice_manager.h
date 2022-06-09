#ifndef mip_split_voice_manager_included
#define mip_split_voice_manager_included
//----------------------------------------------------------------------

/*
  keeps track of, but doesn't use port_index for anything..
  use one of these per note port..
*/

//----------------------------------------------------------------------

// split events into queues for each voice
#define MIP_VOICE_PREPARE_EVENTS

// process voices separately in threads(thread-pool)
//#define MIP_VOICE_PROCESS_THREADED

//----------

#include "mip.h"
#include "audio/mip_audio_utils.h"
#include "base/types/mip_queue.h"
#include "plugin/clap/mip_clap.h"

#include "plugin/mip_note.h"
#include "plugin/mip_voice.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class VOICE, int VOICE_COUNT>
class MIP_SplitVoiceManager {

  /*
    why size * 2..
  */

  // note_end queue
  typedef MIP_Queue<MIP_Note,VOICE_COUNT*2> MIP_NoteQueue;

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

  MIP_SplitVoiceManager() {
  }

  //----------

  ~MIP_SplitVoiceManager() {
  }

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
public: // process threaded
//------------------------------

  /*
    voices are processed individually, bu host-provided thread pool
    (or manually one by one if thread pool not availablle
  */

  void process(const clap_process_t *process) {
    //count_param_events(process);
    MVoiceContext.process = process;
    float* out0 = process->audio_outputs->data32[0];
    float* out1 = process->audio_outputs->data32[1];
    MVoiceContext.voicebuffer = MVoiceBuffer;//out0;
    uint32_t length = process->frames_count;
    // clear frame buffer (voices are added to it)
    MIP_ClearMonoBuffer(MFrameBuffer,length);
    // prepare events (separate them for each individual voice)
    handleEvents(process->in_events,process->out_events);
    // setup threaded voice array
    uint32_t num_voices  = 0;
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      if ((MVoices[i].state == MIP_VOICE_PLAYING)
        || (MVoices[i].state == MIP_VOICE_RELEASED)
        || (MVoices[i].state == MIP_VOICE_WAITING)) {
        MThreadedVoices[num_voices++] = i;
      }
    }
    // calc voices
    if (num_voices > 0) {
      manuallyProcessVoiceThreads(num_voices);
    }
    // add all voice buffers to frame buffer
    for (uint32_t i=0; i<num_voices; i++) {
      int32_t voice = MThreadedVoices[i];
      float* src = MVoiceBuffer + (process->frames_count * voice);
      MIP_AddMonoBuffer(MFrameBuffer,src,length);
    }
    // cleanup..
    flushFinishedVoices();
    flushNoteEnds();
    // copy frame buffer to output
    MIP_CopyMonoBuffer(out0,MFrameBuffer,length);
    MIP_CopyMonoBuffer(out1,MFrameBuffer,length);
  }

  //----------

  /*
    called bu host for each voice
  */

  void processVoiceThread(uint32_t index) {
    int32_t voice = MThreadedVoices[index];
    if (voice >= 0) {
      MVoices[voice].processPrepared(voice);
    }
  }

  //----------

  /*
    if there is no thread pool, or host refused for some reason,
    we just calculate the voices manyally, one by one..
    coud do our own threading...
  */

  void manuallyProcessVoiceThreads(uint32_t num) {
    for (uint32_t i=0; i<num; i++) {
      processVoiceThread(i);
    }
  }

//------------------------------
public: // post process
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

//------------------------------
private: // events
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

  void handleEvent(const clap_event_header_t* header) {
    switch (header->type) {
      case CLAP_EVENT_NOTE_ON:          handleNoteOnEvent((const clap_event_note_t*)header);                break;
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

  void handleNoteOnEvent(const clap_event_note_t* event) {
    //MIP_Print("NOTE ON: key %i channel %i port %i note_id %i\n",event->key,event->channel,event->port_index,event->note_id);
    int32_t voice = findAvailableVoice(true); // true = search released voices too
    if (voice >= 0) {
      // let the host know that the note that was playing for this voice
      // now has ended
      if (MVoices[voice].state == MIP_VOICE_RELEASED) {
        queueNoteEnd(MVoices[voice].note);
      }
      // start new voice
      MVoices[voice].prepare_note_on(event->header.time,event->key,event->velocity);
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
            MVoices[i].prepare_note_off(event->header.time,event->velocity);
          }
        //}
      }
    }
  }

  //----------

  void handleNoteChoke(const clap_event_note_t* event) {
    MIP_Print("NOTE CHOKE: key %i channel %i port %i note_id %i\n",event->key,event->channel,event->port_index,event->note_id);
  }

  //----------

  // note_id is always -1

  void handleNoteExpression(const clap_event_note_expression_t* event) {
    //MIP_Print("id %i port %i chan %i key %i val %.3f\n",event->note_id,event->port_index,event->channel,event->key,event->value);
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      //if (MVoices[i].note.note_id == event->note_id) {
        if ((MVoices[i].note.key == event->key) && (MVoices[i].note.channel == event->channel)) {
          MVoices[i].prepare_expression(event->header.time,event->expression_id,event->value);
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
          MVoices[i].prepare_parameter(event->header.time,event->param_id,event->value);
        //}
        //}
      }
    }
    else {
      // voice
      for (uint32_t i=0; i<VOICE_COUNT; i++) {
        if (MVoices[i].note.note_id == event->note_id) {
          //if ((MVoices[i].note.key == event->key) && (MVoices[i].note.channel == event->channel)) {
            MVoices[i].prepare_parameter(event->header.time,event->param_id,event->value);
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
          MVoices[i].prepare_modulation(event->header.time,event->param_id,event->amount);
        //}
        }
      }
    }
    else {
      // voice
      for (uint32_t i=0; i<VOICE_COUNT; i++) {
        if (MVoices[i].note.note_id == event->note_id) {
          //if ((MVoices[i].note.key == event->key) && (MVoices[i].note.channel == event->channel)) {
          MVoices[i].prepare_modulation(event->header.time,event->param_id,event->amount);
          //}
        }
      }
    }
  }

//------------------------------
//
//------------------------------

  void handleMidi(const clap_event_midi_t* event) {
    MIP_Print("port %i data %02x,%02x,%02x\n",event->port_index,event->data[0],event->data[1],event->data[2]);
  }

  //----------

  void handleMidiSysex(const clap_event_midi_sysex_t* event) {
    MIP_Print("port %i size %i\n",event->port_index,event->size);
  }

  //----------

  void handleMidi2(const clap_event_midi2_t* event) {
    MIP_Print("port %i data %i,%i,%i,%i\n",event->port_index,event->data[0],event->data[1],event->data[2],event->data[3]);
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
private: // note_end
//------------------------------

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


