#ifndef mip_voice_manager_included
#define mip_voice_manager_included
//----------------------------------------------------------------------

#include "mip.h"
#include "audio/mip_audio_utils.h"
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

//#define MIP_VOICE_MODULATE_RELEASE
#define MIP_VOICE_STEAL_NOTES

#define MIP_VOICE_SLICE_SIZE        16
#define MIP_VOICE_MAX_OVERSAMPLE    1
#define MIP_VOICE_NUM_SLICE_BUFFERS 1
#define MIP_VOICE_BUFFERSIZE        (MIP_VOICE_SLICE_SIZE * MIP_VOICE_MAX_OVERSAMPLE * MIP_VOICE_NUM_SLICE_BUFFERS)
#define MIP_VOICE_MAX_EVENTS        1024

//----------

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

  int32_t                     MNotePortIndex                    = 0;
  uint32_t                    MNumActiveVoices                  = 0;
  uint32_t                    MNumActiveNotes                   = 0;
  MIP_NoteEndQueue            MNoteEndQueue                     = {};
  MIP_VoiceContext            MVoiceContext                     = {};

  uint32_t                    MNotes[MIP_NOTE_BUFFERSIZE]       = {0};  // index (chan*128)+key, 0=off, 1=on/held

  int32_t                     MNoteToVoice[MIP_NOTE_BUFFERSIZE] = {0};  // 128*16, chan/key -> voice, -1 = empty
  VOICE                       MVoices[NUM]                      = {};   // 0..15
  uint32_t                    MVoiceStates[NUM]                 = {0};  // off, playing, released, finished
  int32_t                     MVoiceNotes[NUM]                  = {0};  // off, playing, released, finished

  const clap_output_events_t* MOutEvents                        = nullptr;
  const clap_input_events_t*  MInEvents                         = nullptr;
  uint32_t                    MNumInEvents                      = 0;
  uint32_t                    MNextInEvent                      = 0;
  uint32_t                    MCurrInEvent                      = 0;
  uint32_t                    MCurrentTime                      = 0;

//------------------------------
public:
//------------------------------

  MIP_VoiceManager(uint32_t APortIndex=0) {
    MNotePortIndex = APortIndex;
    for (uint32_t i=0; i<MIP_NOTE_MAX_NOTES; i++) {
      MNotes[i] = 0;
      MNoteToVoice[i] = -1;
    }
  }

  //----------

  ~MIP_VoiceManager() {
  }

//------------------------------
public: // set/get
//------------------------------

  uint32_t* getNumActiveNotes() {
    return MNumActiveNotes;
  }

  uint32_t* getNoteStates() {
    return MNotes;
  }

  uint32_t getNoteState(uint32_t AIndex) {
    return MNotes[AIndex];
  }

  uint32_t getNoteState(uint32_t AChannel, uint32_t AKey) {
    uint32_t index = (AChannel * MIP_NOTE_MAX_NOTES) + AKey;
    return MNotes[index];
  }

  void setNoteState(uint32_t AIndex, uint32_t AState) {
    MNotes[AIndex] = AState;
  }

  void setNoteState(uint32_t AChannel, uint32_t AKey, uint32_t AState) {
    uint32_t index = (AChannel * MIP_NOTE_MAX_NOTES) + AKey;
    MNotes[index] = AState;
  }

  int32_t getNotePortIndex()  {
    return MNotePortIndex;
  }

  void setPortIndex(int32_t AIndex) {
    MNotePortIndex = AIndex;
  }

  uint32_t getMaxVoices() {
    return NUM;
  }

  uint32_t getVoiceState(uint32_t AIndex) {
    return MVoiceStates[AIndex];
  }

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

  void prepareVoices(double samplerate) {
    MVoiceContext.samplerate = samplerate;
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].prepare(&MVoiceContext);
      MVoiceStates[i] = MIP_VOICE_OFF;
      MVoiceNotes[i]  = -1;
    }
    for (uint32_t i=0; i<MIP_VOICE_BUFFERSIZE; i++) {
      MNotes[i]       = MIP_NOTE_IS_OFF;
      MNoteToVoice[i] = -1;
    }
  }

//------------------------------
private: // events
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
    int32_t   channel   = event->channel;
    int32_t   key       = event->key;
    uint32_t  note      = (channel * MIP_NOTE_MAX_NOTES) + key;
    float     velocity  = event->velocity;
    MNotes[note]        = 1;
    MNumActiveNotes     += 1;
    voiceNoteOn(channel,key,velocity);
  }

  //----------

  void handleNoteOffEvent(clap_event_note_t* event) {
    int32_t   channel   = event->channel;
    int32_t   key       = event->key;
    uint32_t  note      = (channel * MIP_NOTE_MAX_NOTES) + key;
    float     velocity  = event->velocity;
    MNotes[note]        = 0;
    MNumActiveNotes     -= 1;
    voiceNoteOff(channel,key,velocity);
  }

  //----------

  void handleNoteChokeEvent(clap_event_note_t* event) {
    int32_t channel     = event->channel;
    int32_t key         = event->key;
    uint32_t note       = (channel * MIP_NOTE_MAX_NOTES) + key;
    float     velocity  = event->velocity;
    MNotes[note]        = 0;
    MNumActiveNotes     -= 1;
    voiceChoke(channel,key,velocity);
  }

  //----------

  void handleNoteExpressionEvent(clap_event_note_expression_t* event) {
    int32_t channel     = event->channel;
    int32_t key         = event->key;
    uint32_t expression = event->expression_id;
    float value         = event->value;
    voiceExpression(channel,key,expression,value);
  }

  //----------

  void handleParamValueEvent(clap_event_param_value_t* event) {
    int32_t   channel   = event->channel;
    int32_t   key       = event->key;
    uint32_t  index     = event->param_id;
    float     value     = event->value;
    voiceParamValue(channel,key,index,value);
  }

  //----------

  void handleParamModEvent(clap_event_param_mod_t* event) {
    int32_t   channel   = event->channel;
    int32_t   key       = event->key;
    uint32_t  index     = event->param_id;
    float     amount    = event->amount;
    voiceParamMod(channel,key,index,amount);
  }

  //----------

  void handleMidiEvent(clap_event_midi_t* event) {
    MIP_PRINT;
  }

  //----------

  void handleMidi2Event(clap_event_midi2_t* event) {
    MIP_PRINT;
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
      //MIP_Print("NOTE_END.. chan %i key %i\n",channel,key);
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
//public: // voices
private: // voices
//------------------------------

  void voiceNoteOn(int32_t AChannel, int32_t AKey, float AVelocity) {
    MIP_Assert(AChannel >= 0);
    MIP_Assert(AChannel <= 15);
    MIP_Assert(AKey >= 0);
    MIP_Assert(AKey <= 127);
    int32_t note = (AChannel * MIP_NOTE_MAX_NOTES) + AKey;
    #ifdef MIP_VOICE_STEAL_NOTES
    int32_t prev_voice = MNoteToVoice[note];
    if (prev_voice >= 0) {
      if ((MVoiceStates[prev_voice] == MIP_VOICE_PLAYING) || (MVoiceStates[prev_voice] == MIP_VOICE_RELEASED)) {
        MVoices[prev_voice].note_off(AVelocity);
        MNoteToVoice[note]  = prev_voice;
        MVoiceStates[prev_voice] = MVoices[prev_voice].note_on(AKey,AVelocity);
        MVoiceNotes[prev_voice]  = note;
        #ifdef MIP_VOICE_MODULATE_RELEASE
        // this ends modulation for all voices for this chan/key
        queueNoteEnd(note);
        #endif
      }
    }
    else { // no prev voice
      int32_t new_voice = findVoice(false);
    #else
    {
      int32_t new_voice = findVoice(true);
    #endif
      if (new_voice >= 0) {
        //MIP_Print("starting voice.. chan %i key %i\n",AChannel,AKey);
        MNoteToVoice[note]  = new_voice;
        MVoiceStates[new_voice] = MVoices[new_voice].note_on(AKey,AVelocity);
        MVoiceNotes[new_voice]  = note;
      }
      else {
        // we don't need the modulator voice, since we didn't start a voice
        queueNoteEnd(AChannel,AKey);
      }

    } // prev_voice
  }

  //----------

  void voiceNoteOff(int32_t AChannel, int32_t AKey, float AVelocity) {
    MIP_Assert(AChannel >= 0);
    MIP_Assert(AChannel <= 15);
    MIP_Assert(AKey >= 0);
    MIP_Assert(AKey <= 127);
    int32_t note = (AChannel * MIP_NOTE_MAX_NOTES) + AKey;
    if (note >= 0) {
      int32_t voice = MNoteToVoice[note];
      if (voice >= 0) {
        // end already playing voice
        // update state
        MVoiceStates[voice] = MVoices[voice].note_off(AVelocity);
      }
      else {
        //MIP_Print("ouch! voice < 0\n");
      }
      // send this when the voice has fully faded out (flushVoices)
      #ifndef MIP_VOICE_MODULATE_RELEASE
      queueNoteEnd(AChannel,AKey);
      #endif
    }
  }

  //----------

  void voiceChoke(int32_t AChannel, int32_t AKey, float AVelocity) {
    MIP_Assert(AChannel >= 0);
    MIP_Assert(AChannel <= 15);
    MIP_Assert(AKey >= 0);
    MIP_Assert(AKey <= 127);
    int32_t note = (AChannel * MIP_NOTE_MAX_NOTES) + AKey;
    int32_t voice = MNoteToVoice[note];
    MNoteToVoice[note] = -1;
    MVoiceStates[voice] = MIP_VOICE_OFF;
    MVoiceNotes[voice]  = -1;
    queueNoteEnd(AChannel,AKey);
  }

  //----------

  void voiceExpression(int32_t AChannel, int32_t AKey, uint32_t AExpression, float AValue) {
    //if (AChannel == -1) { MIP_Print("AChannel = -1\n"); }
    //if (AKey == -1) { MIP_Print("AKey = -1\n"); }
    if (AChannel == -1) {
      // all channels
    }
    else if (AKey == -1) {
      // all keys
    }
    else {
      int32_t note = (AChannel * MIP_NOTE_MAX_NOTES) + AKey;
      int32_t voice = MNoteToVoice[note];
      if (voice >= 0) {
        switch (AExpression) {
          case CLAP_NOTE_EXPRESSION_VOLUME:     MVoices[voice].volume(AValue);      break;
          case CLAP_NOTE_EXPRESSION_PAN:        MVoices[voice].pan(AValue);         break;
          case CLAP_NOTE_EXPRESSION_TUNING:     MVoices[voice].tuning(AValue);      break;
          case CLAP_NOTE_EXPRESSION_VIBRATO:    MVoices[voice].vibrato(AValue);     break;
          case CLAP_NOTE_EXPRESSION_EXPRESSION: MVoices[voice].expression(AValue);  break;
          case CLAP_NOTE_EXPRESSION_BRIGHTNESS: MVoices[voice].brightness(AValue);  break;
          case CLAP_NOTE_EXPRESSION_PRESSURE:   MVoices[voice].pressure(AValue);    break;
        }
      }
    }
  }

  //----------

  void voiceParamValue(int32_t AChannel, int32_t AKey, uint32_t AIndex, float AValue) {
    //if (AChannel == -1) { MIP_Print("AChannel = -1\n"); }
    //if (AKey == -1) { MIP_Print("AKey = -1\n"); }
    if ((AChannel == -1) || (AKey == -1)) {
      for (uint32_t i=0; i<NUM; i++) {
        MVoices[i].parameter(AIndex,AValue);
      }
    }
    else {
      int32_t note = (AChannel * MIP_NOTE_MAX_NOTES) + AKey;
      int32_t voice = MNoteToVoice[note];
      if (voice >= 0) {
        MVoices[voice].parameter(AIndex,AValue);
      }
    }
  }

  //----------

  void voiceParamMod(int32_t AChannel, int32_t AKey, uint32_t AIndex, float AValue) {
    //if (AChannel == -1) { MIP_Print("AChannel = -1\n"); }
    //if (AKey == -1) { MIP_Print("AKey = -1\n"); }
    if ((AChannel == -1) || (AKey == -1)) {
      for (uint32_t i=0; i<NUM; i++) {
        MVoices[i].modulation(AIndex,AValue);
      }
    }
    else {
      //MIP_Assert(AChannel >= 0);
      MIP_Assert(AChannel <= 15);
      //MIP_Assert(AKey >= 0);
      MIP_Assert(AKey <= 127);
      int32_t note = (AChannel * MIP_NOTE_MAX_NOTES) + AKey;
      int32_t voice = MNoteToVoice[note];
      //if (voice == -1) { MIP_Print("voice = -1\n"); }
      if (voice >= 0) {
        MVoices[voice].modulation(AIndex,AValue);
      }
    }
  }

//------------------------------
private:
//------------------------------

  int32_t findVoice(bool ATryReleased=true) {
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceStates[i] == MIP_VOICE_OFF) return i;
      //if (MVoiceStates[i] == MIP_VOICE_FINISHED) return i;
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
        // we sent them when we received note_off
        #ifdef MIP_VOICE_MODULATE_RELEASE
        queueNoteEnd(note);
        #endif
        MVoiceStates[i] = MIP_VOICE_OFF;
        MNoteToVoice[note] = -1;
        MVoiceNotes[i] = -1;
      }
    }
  }

//------------------------------
public: // slices
//------------------------------

  // split block in slices (MIP_VOICE_SLICE_SIZE, currently 16 samples)
  // handle all events within each slice at the start of the slice
  // calculate all voices in slice sized pieces inbetween

  void processSlices(const clap_process_t* process, uint32_t AOffset, uint32_t ASize) {
    MVoiceContext.process = process;
    preProcessSliceVoices();
    MInEvents = process->in_events;
    MOutEvents = process->out_events;
    MNumInEvents = MInEvents->size(MInEvents);
    float* out0 = process->audio_outputs->data32[0];
    float* out1 = process->audio_outputs->data32[1];
    MCurrentTime = 0;
    uint32_t remaining = process->frames_count;
    preProcessSliceEvents();
    while (remaining > 0) {
      if (remaining >= MIP_VOICE_SLICE_SIZE) {
        processSliceEvents(MCurrentTime);
        processSliceVoices();
        MIP_CopyVoiceSliceBuffer(out0 + MCurrentTime);
        MIP_CopyVoiceSliceBuffer(out1 + MCurrentTime);
        remaining -= MIP_VOICE_SLICE_SIZE;
        MCurrentTime += MIP_VOICE_SLICE_SIZE;
      }
      else {
        processSliceEvents(MCurrentTime,remaining);
        processSliceVoices(remaining);
        MIP_CopyVoiceSliceBuffer(out0 + MCurrentTime,remaining);
        MIP_CopyVoiceSliceBuffer(out1 + MCurrentTime,remaining);
        remaining = 0;
      }
    }
    postProcessSliceEvents();
    postProcessSliceVoices();
    flushNoteEnds(0,process->out_events);
    //MInEvents = nullptr;
    //MOutEvents = nullptr;
    //MVoiceContext.process = nullptr;
  }

//------------------------------
private: // process events
//------------------------------

  void preProcessSliceEvents() {
    MCurrInEvent = 0;
    if (MNumInEvents > 0) {
      const clap_event_header* header = MInEvents->get(MInEvents,0);
      MNextInEvent = header->time;
    }
    else MNextInEvent = MIP_INT32_MAX;
  }

  //----------

  /*
    returns number of samples until next event,
    or MIP_INT32_MAX if no more events
  */

  uint32_t processSliceEvents(uint32_t AOffset, uint32_t ASize) {
    while (MNextInEvent < (AOffset + ASize)) {
      if (MCurrInEvent < MNumInEvents) {
        const clap_event_header_t* header = MInEvents->get(MInEvents,MCurrInEvent);
        if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
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
  uint32_t processSliceEvents(uint32_t AOffset) {
    return processSliceEvents(AOffset,MIP_VOICE_SLICE_SIZE);
  }

  //----------

  void postProcessSliceEvents() {
    MNumInEvents = 0;
  }

//------------------------------
private: // process voices
//------------------------------

  void preProcessSliceVoices() {
  }

  //----------

  // process all voices (for one slice)
  // irregular length

  void processSliceVoices(uint32_t ASize) {
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

  void processSliceVoices() {
    processSliceVoices(MIP_VOICE_SLICE_SIZE);
  }

  //----------

  void postProcessSliceVoices() {
    flushVoices();
  }

//------------------------------
public: // variable
//------------------------------

  // TODO
  // split block at events, calculate all voices inbetween

  void process(const clap_process_t* process, uint32_t AOffset, uint32_t ASize) {
    //MVoiceContext.process = process;
    //MInEvents = process->in_events;
    //MOutEvents = process->out_events;
    //MNumInEvents = MInEvents->size(MInEvents);
    //...
    //flushNoteEnds(0,process->out_events);
  }

//------------------------------
public: // buffer
//------------------------------

  // TODO
  // handle all events at start of block
  // then calculate all voices (blocksize)

  void processBuffer(const clap_process_t* process, uint32_t AOffset, uint32_t ASize) {
    //MVoiceContext.process = process;
    //MInEvents = process->in_events;
    //MOutEvents = process->out_events;
    //MNumInEvents = MInEvents->size(MInEvents);
    //..
    //flushNoteEnds(0,process->out_events);
  }

//------------------------------
public: // threaded
//------------------------------

  // TODO

  void processThreaded(const clap_process_t* process, uint32_t AOffset, uint32_t ASize) {
    //MVoiceContext.process = process;
    //MInEvents = process->in_events;
    //MOutEvents = process->out_events;
    //MNumInEvents = MInEvents->size(MInEvents);
    //..
    //flushNoteEnds(0,process->out_events);
  }

//------------------------------

};

//----------------------------------------------------------------------

//----------------------------------------------------------------------
#endif
