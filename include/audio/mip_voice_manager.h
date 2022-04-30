#ifndef mip_voice_manager_included
#define mip_voice_manager_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_queue.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define MIP_NOTE_MAX_CHANNELS    16
#define MIP_NOTE_MAX_NOTES       128
#define MIP_NOTE_MAX_NOTE_ENDS   128
#define MIP_NOTE_BUFFERSIZE      (MIP_NOTE_MAX_CHANNELS * MIP_NOTE_MAX_NOTES)

//#define MIP_MASTER_BEND_RANGE     2.0f
//#define MIP_VOICE_BEND_RANGE      48.0f
//#define MIP_INV_MASTER_BEND_RANGE (1.0 / MIP_MASTER_BEND_RANGE)
//#define MIP_INV_VOICE_BEND_RANGE  (1.0 / MIP_VOICE_BEND_RANGE)

#define MIP_VOICE_SLICE_SIZE        16
#define MIP_VOICE_MAX_OVERSAMPLE    1
#define MIP_VOICE_NUM_SLICE_BUFFERS 1
#define MIP_VOICE_BUFFERSIZE        (MIP_VOICE_SLICE_SIZE * MIP_VOICE_MAX_OVERSAMPLE * MIP_VOICE_NUM_SLICE_BUFFERS)
#define MIP_VOICE_MAX_EVENTS        1024

struct MIP_VoiceContext {
  const clap_process_t* process     = nullptr;
  double                samplerate  = 0.0;
};

//----------

typedef MIP_Queue<uint32_t,MIP_NOTE_MAX_NOTE_ENDS> MIP_NoteEndQueue;

//----------------------------------------------------------------------
//
// slice buffer
//
//----------------------------------------------------------------------

__MIP_ALIGNED(MIP_ALIGNMENT_CACHE)
float MIP_VoiceSliceBuffer[MIP_VOICE_BUFFERSIZE] = {0};

//----------

void MIP_ClearVoiceSliceBuffer(uint32_t ASize) {
  memset(MIP_VoiceSliceBuffer,0,ASize*sizeof(float));
}

void MIP_CopyVoiceSliceBuffer(float* ADst, uint32_t ASize) {
  memcpy(ADst,MIP_VoiceSliceBuffer,ASize*sizeof(float));
}

void MIP_ClearVoiceSliceBuffer() {
  MIP_ClearVoiceSliceBuffer(MIP_VOICE_SLICE_SIZE);
}

void MIP_CopyVoiceSliceBuffer(float* ADst) {
  MIP_CopyVoiceSliceBuffer(ADst,MIP_VOICE_SLICE_SIZE);
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class VOICE, int NUM>
class MIP_VoiceManager {

//------------------------------
private:
//------------------------------

  int32_t           MNotePortIndex                    = 0;
  uint32_t          MNumActiveVoices                  = 0;
  uint32_t          MNumActiveNotes                   = 0;
  MIP_NoteEndQueue  MNoteEndQueue                     = {};
  MIP_VoiceContext  MVoiceContext                     = {};

  uint32_t          MNoteStates[MIP_NOTE_BUFFERSIZE]  = {0};  // index (chan*128)+key, 0=off, 1=on/held
  int32_t           MNoteToVoice[MIP_NOTE_BUFFERSIZE] = {0};  // 128*16, chan/key -> voice, -1 = empty
  VOICE             MVoices[NUM]                      = {};   // 0..15
  uint32_t          MVoiceStates[NUM]                 = {0};  // off, playing, released, finished
  int32_t           MVoiceNotes[NUM]                  = {0};  // off, playing, released, finished

  const clap_output_events_t* MOutEvents                    = nullptr;
  const clap_input_events_t*  MInEvents                     = nullptr;
  uint32_t                    MNumInEvents                  = 0;
  uint32_t                    MNextInEvent                  = 0;
  uint32_t                    MCurrInEvent                  = 0;
  uint32_t                    MCurrentTime                  = 0;


//------------------------------
public:
//------------------------------

  MIP_VoiceManager(uint32_t APortIndex=0) {
    MNotePortIndex = APortIndex;
    for (uint32_t i=0; i<MIP_NOTE_MAX_NOTES; i++) {
      MNoteStates[i] = 0;
      MNoteToVoice[i] = -1;
    }
  }

  //----------

  ~MIP_VoiceManager() {
  }

//------------------------------
public: // set/get
//------------------------------

  // notes

  uint32_t* getNumActiveNotes() {
    return MNumActiveNotes;
  }

  uint32_t* getNoteStates() {
    return MNoteStates;
  }

  int32_t getNotePortIndex()  {
    return MNotePortIndex;
  }

  void setPortIndex(int32_t AIndex) {
    MNotePortIndex = AIndex;
  }

  uint32_t getNoteState(uint32_t AIndex) {
    return MNoteStates[AIndex];
  }

  uint32_t getNoteState(uint32_t AChannel, uint32_t AKey) {
    uint32_t index = (AChannel * MIP_NOTE_MAX_NOTES) + AKey;
    return MNoteStates[index];
  }

  void setNoteState(uint32_t AIndex, uint32_t AState) {
    MNoteStates[AIndex] = AState;
  }

  void setNoteState(uint32_t AChannel, uint32_t AKey, uint32_t AState) {
    uint32_t index = (AChannel * MIP_NOTE_MAX_NOTES) + AKey;
    MNoteStates[index] = AState;
  }

  // voices

  uint32_t getMaxVoices() {
    return NUM;
  }

  uint32_t getVoiceState(uint32_t AIndex) {
    return MVoiceStates[AIndex];
  }

//------------------------------
public: // events
//------------------------------

  void handleEvent(const clap_event_header_t* header) {
    switch (header->type) {
      case CLAP_EVENT_NOTE_ON:
        handleNoteOnEvent((clap_event_note_t*)header);
        break;
      case CLAP_EVENT_NOTE_OFF:
        handleNoteOffEvent((clap_event_note_t*)header);
        break;
      case CLAP_EVENT_NOTE_CHOKE:
        handleNoteChokeEvent((clap_event_note_t*)header);
        break;
      case CLAP_EVENT_NOTE_EXPRESSION:
        handleNoteExpressionEvent((clap_event_note_expression_t*)header);
        break;
      case CLAP_EVENT_MIDI:
        handleMidiEvent((clap_event_midi_t*)header);
        break;
      case CLAP_EVENT_MIDI2:
        handleMidi2Event((clap_event_midi2_t*)header);
        break;
      case CLAP_EVENT_PARAM_VALUE:
        handleParamValueEvent((clap_event_param_value_t*)header);
        break;
      case CLAP_EVENT_PARAM_MOD:
        handleParamModEvent((clap_event_param_mod_t*)header);
        break;
    }
  }

//------------------------------

  void handleNoteOnEvent(clap_event_note_t* event) {
    int32_t channel = event->channel;
    int32_t key     = event->key;
    uint32_t note   = (channel * MIP_NOTE_MAX_NOTES) + key;
    MNoteStates[note] = 1;
    MNumActiveNotes += 1;
    if (!voiceStart(channel,key,event->velocity)) {
      // no voice were started, so we tell the host that it can shut down
      // the modulation voice for this chan/key
      queueNoteEnd(channel,key);
    }
  }

  //----------

  void handleNoteOffEvent(clap_event_note_t* event) {
    int32_t channel   = event->channel;
    int32_t key       = event->key;
    uint32_t note     = (channel * MIP_NOTE_MAX_NOTES) + key;
    MNoteStates[note] = 0;
    MNumActiveNotes  -= 1;
    voiceRelease(channel,key,event->velocity);
    //queueNoteEnd(channel,key); // called in voiceRelease
  }

  //----------

  void handleNoteChokeEvent(clap_event_note_t* event) {
    int32_t channel   = event->channel;
    int32_t key       = event->key;
    uint32_t note     = (channel * MIP_NOTE_MAX_NOTES) + key;
    MNoteStates[note] = 0;
    MNumActiveNotes  -= 1;
    voiceChoke(channel,key,event->velocity);
  }

  //----------

  void handleNoteExpressionEvent(clap_event_note_expression_t* event) {
    int32_t channel = event->channel;
    int32_t key     = event->key;
    //uint32_t note   = (channel * MIP_NOTE_MAX_NOTES) + key;
    voiceExpression(channel,key,event->value);
  }


  //----------

  void handleMidiEvent(clap_event_midi_t* event) {
    MIP_PRINT;
  }

  //----------

  void handleMidi2Event(clap_event_midi2_t* event) {
    MIP_PRINT;
  }

  //----------

  void handleParamValueEvent(clap_event_param_value_t* event) {
    int32_t channel = event->channel;
    int32_t key     = event->key;
    //uint32_t note   = (channel * MIP_NOTE_MAX_NOTES) + key;
    uint32_t index  = event->param_id;
    float value     = event->value;
    voiceParamValue(channel,key,index,value);
  }
  //----------

  void handleParamModEvent(clap_event_param_mod_t* event) {
    int32_t channel = event->channel;
    int32_t key     = event->key;
    //uint32_t note   = (channel * MIP_NOTE_MAX_NOTES) + key;
    uint32_t index  = event->param_id;
    float amount    = event->amount;
    voiceParamMod(channel,key,index,amount);
  }

//------------------------------
public: // NOTE_END
//------------------------------

  void queueNoteEnd(uint32_t AIndex) {
    MNoteEndQueue.write(AIndex);
  }

  //----------

  void queueNoteEnd(uint32_t AChannel, uint32_t AKey) {
    uint32_t index = (AChannel * MIP_NOTE_MAX_NOTES) + AKey;
    MNoteEndQueue.write(index);
  }

  //----------

  void flushNoteEnds(uint32_t time, const clap_output_events_t* out_events) {
    uint32_t index = 0;
    while (MNoteEndQueue.read(&index)) {
      uint32_t key = index & 127;
      uint32_t channel = index >> 7;
      clap_event_note_t event;
      clap_event_header_t* header = (clap_event_header_t*)&event;
      header->size      = sizeof(clap_event_note_t);
      header->time      = 0;// time; // crashed if this was 1023 (buffersize-1)
      header->space_id  = CLAP_CORE_EVENT_SPACE_ID;
      header->type      = CLAP_EVENT_NOTE_END;
      header->flags     = 0;
      event.port_index  = MNotePortIndex;
      event.key         = key;
      event.channel     = channel;
      event.velocity    = 0.0;
      out_events->try_push(out_events,header);
    }
  }

//------------------------------
public: // voices
//------------------------------

  void prepareVoices(double samplerate) {
    MVoiceContext.samplerate = samplerate;
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].prepare(&MVoiceContext);
      MVoiceStates[i] = MIP_VOICE_OFF;
      MVoiceNotes[i] = -1;
    }
  }

  //----------

  bool voiceStart(int32_t AChannel, int32_t AKey, float AVelocity) {
    //MIP_Print("start voice\n");
    int32_t note = (AChannel * MIP_NOTE_MAX_NOTES) + AKey;
    int32_t voice = MNoteToVoice[note];
    // did a voice alreay play for this chan/key
    if (voice >= 0) {
      // then steal it
      //MIP_Print("note already playing\n");
    }
    else {
      // else find a non-playing, or released voice
      //MIP_Print("find voice\n");
      voice = findVoice();
      voice = findVoice();
    }
    // do we have a voice?
    if (voice >= 0) {
      // did we steal a released voice?
      if (MVoiceStates[voice] == MIP_VOICE_RELEASED) {
        //MIP_Print("was RELEASED\n");
      }
      MNoteToVoice[note]  = voice;
      MVoiceStates[voice] = MVoices[voice].note_on(AKey,AVelocity);
      MVoiceNotes[voice]  = note;
      //return MIP_VOICE_PLAYING;
      return true;
    }
    else {
      // no voice
      //MIP_Print("no voice\n");
      return false;
    }
  }

  //----------

  void voiceRelease(int32_t AChannel, int32_t AKey, float AVelocity) {
    //MIP_Print("release voice\n");
    int32_t note = (AChannel * MIP_NOTE_MAX_NOTES) + AKey;
    if (note >= 0) {

      int32_t voice = MNoteToVoice[note];
      if (voice >= 0) {
        MVoiceStates[voice] = MVoices[voice].note_off(AVelocity);
        MVoiceNotes[voice]  = -1;
      }
      MNoteToVoice[note]  = -1;
      queueNoteEnd(AChannel,AKey);
    }

  }

  //----------

  void voiceChoke(int32_t AChannel, int32_t AKey, float AVelocity) {
    //MIP_Print("choke voice\n");
    int32_t note = (AChannel * MIP_NOTE_MAX_NOTES) + AKey;
    int32_t voice = MNoteToVoice[note];
    MNoteToVoice[note] = -1;
    MVoiceStates[voice] = MIP_VOICE_OFF;
    MVoiceNotes[voice]  = -1;
    queueNoteEnd(AChannel,AKey);
  }

  //----------

  void voiceExpression(int32_t AChannel, int32_t AKey, float AVelocity) {
  }

  //----------

  void voiceParamValue(int32_t AChannel, int32_t AKey, uint32_t AIndex, float AValue) {
    int32_t note = (AChannel * MIP_NOTE_MAX_NOTES) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].parameter(AIndex,AValue);
    }

  }

  //----------

  void voiceParamMod(int32_t AChannel, int32_t AKey, uint32_t AIndex, float AValue) {
    //MIP_Print("chan %i key %i index %i value %.2f\n",AChannel,AKey,AIndex,AValue);
    if ((AChannel == -1) || (AKey == -1)) {
      for (uint32_t i=0; i<NUM; i++) {
        MVoices[i].modulation(AIndex,AValue);
      }
    }
    int32_t note = (AChannel * MIP_NOTE_MAX_NOTES) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].modulation(AIndex,AValue);
    }
  }

  //void handle_master_param(uint32_t AIndex, float AValue) {
  //}

  //uint32_t  getVoiceState(uint32_t v) {
  //  return 0;
  //}

  //----------

  void voiceParameter(int32_t AIndex, float AValue) {
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].parameter(AIndex,AValue);
    }
  }

  //----------

  void voiceModulation(int32_t AIndex, float AValue) {
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].modulation(AIndex,AValue);
    }
  }

  //----------

  //void process(const clap_process_t *process) {
  //}

//------------------------------
//private:
public:
//------------------------------

  int32_t findVoice(bool ATryReleased=true) {
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceStates[i] == MIP_VOICE_OFF) return i;
      if (MVoiceStates[i] == MIP_VOICE_FINISHED) return i;
    }
    if (ATryReleased) {
      for (uint32_t i=0; i<NUM; i++) {
        if (MVoiceStates[i] == MIP_VOICE_RELEASED) return i;
      }
    }
    return -1;
  }

  //----------

  void flushVoices() {
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceStates[i] == MIP_VOICE_FINISHED) {
        MVoiceStates[i] = MIP_VOICE_OFF;
        uint32_t note = MVoiceNotes[i];
        //queueNoteEnd(channel,key);
        queueNoteEnd(note);
      }
    }
  }

//------------------------------
public: // process
//------------------------------

  void preProcessEvents() {
    MCurrInEvent = 0;
    if (MNumInEvents > 0) {
      const clap_event_header* header = MInEvents->get(MInEvents,0);
      MNextInEvent = header->time;
    }
    else MNextInEvent = MIP_INT32_MAX;
  }

  //----------

  //void processEvents(uint32_t AOffset, uint32_t ASize, const clap_input_events_t* in_events) {
  //  uint32_t num = in_events->size(in_events);
  //  for (uint32_t i=0; i<num; i++) {
  //    const clap_event_header_t* header = in_events->get(in_events,i);
  //    if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
  //      handleEvent(header);
  //    }
  //  }
  //}

  /*
    returns number of samples until next event,
    or MIP_INT32_MAX if no more events
  */

  uint32_t processEvents(uint32_t AOffset, uint32_t ASize) {
    while (MNextInEvent < (AOffset + ASize)) {
      if (MCurrInEvent < MNumInEvents) {
        const clap_event_header_t* header = MInEvents->get(MInEvents,MCurrInEvent);
        if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {

//          on_event(header);
          handleEvent(header);

          MCurrInEvent++;
          MNextInEvent = header->time;
        }
      }
      else MNextInEvent = MIP_INT32_MAX;
    }
    uint32_t res = MNextInEvent - AOffset;
    return res;
  }

  //----------

  //TODO

  uint32_t processEvents(uint32_t AOffset) {
    return processEvents(AOffset,MIP_VOICE_SLICE_SIZE);
  }

  //----------

  void postProcessEvents() {
    MNumInEvents = 0;
  }

//------------------------------
private:
//------------------------------

  void preProcessVoices() {
  }

  //----------

  // process all voices (for one slice)
  // irregular length

  void processVoices(uint32_t ASize) {
    MIP_ClearVoiceSliceBuffer(ASize);
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceStates[i] == MIP_VOICE_PLAYING){
        MVoiceStates[i] = MVoices[i].process(MIP_VOICE_PLAYING,ASize);
      }
      if (MVoiceStates[i] == MIP_VOICE_RELEASED) {
        MVoiceStates[i] = MVoices[i].process(MIP_VOICE_RELEASED,ASize);
      }
    }
  }

  //----------

  void processVoices() {
    processVoices(MIP_VOICE_SLICE_SIZE);
  }

  //----------

  void postProcessVoices() {
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceStates[i] == MIP_VOICE_FINISHED) {
        //send_note_end(i);
//        uint32_t channel = MVoiceChannel[i];
//        uint32_t key = MVoiceNote[i];
//        uint32_t index = (channel * 128) + key;
//        queueNoteEnd(index);

          uint32_t note = MVoiceNotes[i];
          queueNoteEnd(note);
          MVoiceStates[i] = MIP_VOICE_OFF;

        //MVoiceState[i] = MIP_VOICE_OFF;
//        reset_voice(i);
      }
    }
  }

//------------------------------
public:
//------------------------------

  void processVoices(uint32_t AOffset, uint32_t ASize, const clap_process_t* process) {
    MVoiceContext.process = process;
    //MVoiceContext.slicebuffer = MIP_VoiceSliceBuffer;
    preProcessVoices();
    MInEvents = process->in_events;
    MOutEvents = process->out_events;
    MNumInEvents = MInEvents->size(MInEvents);
    float* out0 = process->audio_outputs->data32[0];
    float* out1 = process->audio_outputs->data32[1];
    //uint32_t offset = 0;
    MCurrentTime = 0;
    uint32_t remaining = process->frames_count;
    preProcessEvents();
    while (remaining > 0) {
      if (remaining >= MIP_VOICE_SLICE_SIZE) {
        processEvents(MCurrentTime);
        processVoices();
        MIP_CopyVoiceSliceBuffer(out0 + MCurrentTime);
        MIP_CopyVoiceSliceBuffer(out1 + MCurrentTime);
        remaining -= MIP_VOICE_SLICE_SIZE;
        MCurrentTime += MIP_VOICE_SLICE_SIZE;
      }
      else {
        processEvents(MCurrentTime,remaining);
        processVoices(remaining);
        MIP_CopyVoiceSliceBuffer(out0 + MCurrentTime,remaining);
        MIP_CopyVoiceSliceBuffer(out1 + MCurrentTime,remaining);
        remaining = 0;
      }
    }
    postProcessEvents();
    postProcessVoices();
    flushNoteEnds(0,process->out_events);
    MInEvents = nullptr;
    MOutEvents = nullptr;
    MVoiceContext.process = nullptr;
  }

};

//----------------------------------------------------------------------

#undef MAX_NOTES
#undef MAX_CHANNELS
#undef MAX_NOTE_BUFFER
#undef MAX_ACTIVE_NOTES

//----------------------------------------------------------------------
#endif
