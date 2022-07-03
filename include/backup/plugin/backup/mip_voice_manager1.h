#ifndef mip_voice_manager2_included
#define mip_voice_manager2_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"

#define MIP_VOICE_SEND_NOTE_END_AFTER_RELEASE

#define MIP_NOTE_NUM_CHANNELS       16
#define MIP_NOTE_NUM_NOTES          128
#define MIP_NOTE_MAX_NOTE_ENDS      128
#define MIP_NOTE_BUFFERSIZE         (MIP_NOTE_NUM_CHANNELS * MIP_NOTE_NUM_NOTES)

#define MIP_VOICE_SLICE_SIZE        16
#define MIP_VOICE_MAX_OVERSAMPLE    1
#define MIP_VOICE_NUM_SLICE_BUFFERS 1
#define MIP_VOICE_BUFFERSIZE        (MIP_VOICE_SLICE_SIZE * MIP_VOICE_MAX_OVERSAMPLE * MIP_VOICE_NUM_SLICE_BUFFERS)
#define MIP_VOICE_MAX_EVENTS        1024

#define MIP_VOICE_MAX_NOTES         128

//----------------------------------------------------------------------

struct MIP_VoiceContext {
  const clap_process_t* process     = nullptr;
  double                samplerate  = 0.0;
};

//----------

typedef MIP_Queue<uint32_t,MIP_VOICE_MAX_NOTES> MIP_NoteEndQueue;

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
// voice manager
//
//----------------------------------------------------------------------

template <class VOICE, uint32_t NUM>
class MIP_VoiceManager {

//------------------------------
private:
//------------------------------

  VOICE               MVoices[NUM]                      = {};
  MIP_VoiceContext    MVoiceContext                     = {};
  MIP_NoteEndQueue    MNoteEndQueue                     = {};
  const clap_plugin*  MClapPlugin                       = nullptr;

  uint32_t            MNumActiveVoices                  = 0;
  uint32_t            MVoiceStates[NUM]                 = {};   // off, playing, released, finished
  int32_t             MVoiceNotes[NUM]                  = {};   // -1 = none, else (chan * 128) + key

  uint32_t            MNumActiveNotes                   = 0;
  uint32_t            MNoteStates[MIP_NOTE_BUFFERSIZE]  = {};   // NIP_NOTE_OFF, MIP_NOTE_ON
  int32_t             MNoteToVoice[MIP_NOTE_BUFFERSIZE] = {};   // -1 = none, else voice index

//------------------------------
public:
//------------------------------

  MIP_VoiceManager(const clap_plugin *plugin) {
    MClapPlugin = plugin;
    for (uint32_t i=0; i<NUM; i++) {
      //MVoices[i].prepare(MVoiceContext);
      MVoiceStates[i] = MIP_VOICE_OFF;
      MVoiceNotes[i] = -1;
    }
    for (uint32_t i=0; i<MIP_NOTE_BUFFERSIZE; i++) {
      MNoteStates[i] = MIP_NOTE_IS_OFF;
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

  uint32_t getNumActiveNotes() {
    return MNumActiveNotes;
  }

  //----------

  uint32_t* getNoteStates() {
    return MNoteStates;
  }

  //----------

  uint32_t getNoteState(uint32_t AIndex) {
    return MNoteStates[AIndex];
  }

  //----------

  uint32_t getNoteState(uint32_t AChannel, uint32_t AKey) {
    uint32_t index = (AChannel * MIP_NOTE_NUM_NOTES) + AKey;
    return MNoteStates[index];
  }

  //----------

  void setNoteState(uint32_t AIndex, uint32_t AState) {
    MNoteStates[AIndex] = AState;
  }

  //----------

  void setNoteState(uint32_t AChannel, uint32_t AKey, uint32_t AState) {
    uint32_t index = (AChannel * MIP_NOTE_NUM_NOTES) + AKey;
    MNoteStates[index] = AState;
  }

  //----------

  // voices

  uint32_t getMaxVoices() {
    return NUM;
  }

  //----------

  uint32_t* getNumActiveVoices() {
    return MNumActiveVoices;
  }

  //----------

  uint32_t* getVoiceStates() {
    return MVoiceStates;
  }

  //----------

  uint32_t getVoiceState(uint32_t AIndex) {
    return MVoiceStates[AIndex];
  }

  //----------

  void setVoiceState(uint32_t AIndex, uint32_t AState) {
    MVoiceStates[AIndex] = AState;
  }

//------------------------------
public:
//------------------------------

  void prepareVoices(float samplerate) {
    initVoices(samplerate);
  }

  //----------

  void voiceParameter(uint32_t index, float value) {
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].parameter(index,value);
    }
  }

  //----------

  void processVoices(const clap_process_t* process, uint32_t ofset, uint32_t length) {
  }


//------------------------------
public: // process
//------------------------------

  // events

  void preProcessEvents(const clap_process_t* process) {
  }

  //----------

  void postProcessEvents() {
  }

  //----------

  // handle all events

  void processEvents(const clap_process_t* process) {
    const clap_input_events_t* in_events = process->in_events;
    const clap_output_events_t* out_events = process->out_events;
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        handleEvent(header);
      }
    }
  }

  //----------

  // voices

  void preProcessVoices(const clap_process_t* process) {
  }

  //----------

  void postProcessVoices() {
  }

  //----------

  void processVoices(const clap_process_t* process) {
    for (uint32_t i=0; i<NUM; i++) {
      if ((MVoiceStates[i] == MIP_VOICE_PLAYING) || (MVoiceStates[i] == MIP_VOICE_RELEASED)) {
        processVoice(i,process);
      }
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
//
//------------------------------

  void handleNoteOnEvent(clap_event_note_t* event) {
    int32_t   channel   = event->channel;
    int32_t   key       = event->key;
    float     velocity  = event->velocity;
    voiceNoteOn(channel,key,velocity);
  }

  //----------

  void handleNoteOffEvent(clap_event_note_t* event) {
    int32_t   channel   = event->channel;
    int32_t   key       = event->key;
    float     velocity  = event->velocity;
    voiceNoteOff(channel,key,velocity);
  }

  //----------

  void handleNoteChokeEvent(clap_event_note_t* event) {
    int32_t   channel   = event->channel;
    int32_t   key       = event->key;
    float     velocity  = event->velocity;
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
    int32_t   channel = event->channel;
    int32_t   key     = event->key;
    uint32_t  index   = event->param_id;
    float     value   = event->value;
    voiceParamValue(channel,key,index,value);
  }
  //----------

  void handleParamModEvent(clap_event_param_mod_t* event) {
    int32_t   channel = event->channel;
    int32_t   key     = event->key;
    uint32_t  index   = event->param_id;
    float     amount  = event->amount;
    voiceParamMod(channel,key,index,amount);
  }

  //----------

  void handleMidiEvent(clap_event_midi_t* event) {
  }

  //----------

  void handleMidi2Event(clap_event_midi2_t* event) {
  }

//------------------------------
private:
//------------------------------

  void voiceNoteOn(int32_t channel, int32_t key, float velocity) {
    MIP_Print("channel %i key %i velocity %.2f\n",channel,key,velocity);
    uint32_t note = (channel * MIP_NOTE_NUM_NOTES) + key;
    MNoteStates[note] = MIP_NOTE_IS_ON;
    MNumActiveNotes += 1;
    int32_t voice = findVoice(MIP_VOICE_OFF);
    if (voice < 0) voice = findVoice(MIP_VOICE_FINISHED);
    if (voice < 0) voice = findVoice(MIP_VOICE_RELEASED);
    if (voice >= 0) {
      startVoice(voice);
      MVoiceStates = MVoices[voice].note_on(key,velocity);
      MVoiceNotes = note;
    }
  }

  void voiceNoteOff(int32_t channel, int32_t key, float velocity) {
    MIP_Print("channel %i key %i velocity %.2f\n",channel,key,velocity);
    uint32_t note = (channel * MIP_NOTE_NUM_NOTES) + key;
    MNoteStates[note] = MIP_NOTE_IS_OFF;
    MNumActiveNotes -= 1;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      //stopVoice(voice);
      MVoiceStates = MVoices[voice].note_off(key,velocity);
      //MVoiceNotes = note;
      #ifndef MIP_VOICE_SEND_NOTE_END_AFTER_RELEASE
      queueNoteEnd(channel,key);
      #endif
    }
    else {
      MIP_Print("Error! No voice playing for %i/%i\n",channel,key);
    }
  }

  void voiceChoke(int32_t channel, int32_t key, float velocity) {
    MIP_Print("* channel %i key %i velocity %.2f\n",channel,key,velocity);
    voiceNoteOff(channel,key,velocity);
  }

  void voiceExpression(int32_t channel, int32_t key, uint32_t expression, float value) {
    MIP_Print("channel %i key %i expression %i value %.2f\n",channel,key,expression,value);
  }

  void voiceParamValue(int32_t channel, int32_t key, uint32_t index, float value) {
    MIP_Print("channel %i key %i index %i value %.2f\n",channel,key,index,value);
  }

  void voiceParamMod(int32_t channel, int32_t key, uint32_t index, float value) {
    MIP_Print("channel %i key %i index %i value %.2f\n",channel,key,index,value);
  }

//------------------------------
private: // NOTE_END
//------------------------------

  void queueNoteEnd(uint32_t AIndex) {
    MNoteEndQueue.write(AIndex);
  }

  //----------

  void queueNoteEnd(uint32_t AChannel, uint32_t AKey) {
    uint32_t index = (AChannel * MIP_NOTE_NUM_NOTES) + AKey;
    MNoteEndQueue.write(index);
  }

//------------------------------
public:
//------------------------------

  void flushNoteEnds(uint32_t time, int16_t note_port_index, const clap_output_events_t* out_events) {
    uint32_t index = 0;
    while (MNoteEndQueue.read(&index)) {
      uint32_t key      = index        & 127;
      uint32_t channel  = (index >> 7) & 127;
      //MIP_Print("NOTE_END.. chan %i key %i\n",channel,key);
      clap_event_note_t event;
      clap_event_header_t* header = (clap_event_header_t*)&event;
      header->size      = sizeof(clap_event_note_t);
      header->time      = 0;// time; // crashed if this was 1023 (buffersize-1)
      header->space_id  = CLAP_CORE_EVENT_SPACE_ID;
      header->type      = CLAP_EVENT_NOTE_END;
      header->flags     = 0;
      event.port_index  = note_port_index;//MNotePortIndex;
      event.key         = key;
      event.channel     = channel;
      event.velocity    = 0.0;
      out_events->try_push(out_events,header);
    }
  }

//------------------------------
private: // voices
//------------------------------

  void processVoice(uint32_t index, const clap_process_t* process) {
    //MVoices[i].process();
  }

  //void processVoices(const clap_process_t* process) {
  //}

//------------------------------
// voices
//------------------------------

  void initVoices(float samplerate) {
    MVoiceContext.samplerate = samplerate;
  }

  //----------

  void startVoice(uint32_t index) {
    MIP_Assert(index < NUM);
    MVoiceStates[index] = MIP_VOICE_PLAYING;
    MNumActiveVoices += 1;
  }

  //----------

  void stopVoice(uint32_t index) {
    MIP_Assert(index < NUM);
    MVoiceStates[index] = MIP_VOICE_OFF;
    MVoiceNotes[index] = -1;
    MNumActiveVoices -= 1;
  }

  //----------

  int32_t findVoice(uint32_t state=MIP_VOICE_OFF) {
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceStates[i] == state) return i;
    }
    return -1;
  }

  //----------

  void flushVoices() {
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceStates[i] == MIP_VOICE_FINISHED) {
        int32_t note = MVoiceNotes[i];
        stopVoice(i);
        MVoiceStates[i] = MIP_VOICE_OFF;
        MVoiceNotes[i] = -1;
        MNoteToVoice[note] = -1;
        #ifdef MIP_VOICE_SEND_NOTE_END_AFTER_RELEASE
        queueNoteEnd(note);
        #endif
      }
    }
  }
};

//----------------------------------------------------------------------
#endif
