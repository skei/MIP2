#ifndef mip_voice_manager_included
#define mip_voice_manager_included
//----------------------------------------------------------------------

/*
  TODO:
  - handle all note expressions (vol, pan, etc)
  - send event_mod events (end, choke?)
  - handle note_choke/end?
*/

//----------------------------------------------------------------------

#include "mip.h"
#include "audio/mip_audio_utils.h"
#include "plugin/clap/mip_clap.h"

//----------

#define MIP_VOICE_MAX_CHANNELS    16
#define MIP_VOICE_MAX_NOTES       128
#define MIP_VOICE_NOTES           (MIP_VOICE_MAX_CHANNELS * MIP_VOICE_MAX_NOTES)
#define MIP_MASTER_BEND_RANGE     2.0f
#define MIP_VOICE_BEND_RANGE      48.0f
#define MIP_INV_MASTER_BEND_RANGE (1.0 / MIP_MASTER_BEND_RANGE)
#define MIP_INV_VOICE_BEND_RANGE  (1.0 / MIP_VOICE_BEND_RANGE)

#define MIP_VOICE_SLICE_SIZE      16
#define MIP_VOICE_MAX_OVERSAMPLE  1
#define MIP_VOICE_BUFFERSIZE      (MIP_VOICE_SLICE_SIZE * MIP_VOICE_MAX_OVERSAMPLE)
#define MIP_VOICE_MAX_EVENTS      1024

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// one voice context per note port..  ??

struct MIP_VoiceContext {
  const clap_process_t* process     = nullptr;
  double                samplerate  = 0.0;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

/*
  16 samples, 64 bytes, 1 cache line each..
  each voice adds to the slice buffer
  (..hardcoded and unrolled, plus optimizer magic..)
  events are handled inbetween each 16-sample block
  so we might get some jittering (max 15 samples)..
*/

__MIP_ALIGNED(MIP_ALIGNMENT_CACHE)
float MIP_VoiceSliceBuffer[MIP_VOICE_BUFFERSIZE] = {0};

//----------------------------------------------------------------------

void MIP_ClearVoiceSliceBuffer(uint32_t ASize) {
  memset(MIP_VoiceSliceBuffer,0,ASize*sizeof(float));
}

void MIP_CopyVoiceSliceBuffer(float* ADst, uint32_t ASize) {
  memcpy(ADst,MIP_VoiceSliceBuffer,ASize*sizeof(float));
}

//----------

void MIP_ClearVoiceSliceBuffer() {
  MIP_ClearVoiceSliceBuffer(MIP_VOICE_SLICE_SIZE);
}

void MIP_CopyVoiceSliceBuffer(float* ADst) {
  MIP_CopyVoiceSliceBuffer(ADst,MIP_VOICE_SLICE_SIZE);
}

//void MIP_AddVoiceToVoiceSliceBuffer() {
//  MIP_AddVoiceToVoiceSliceBuffer(MIP_VOICE_SLICE_SIZE);
//}

/*
  consider:
  similar global/const functions for other dsp
  16 samples = 4 simd
  or 4 voices at a time
  avx/256/512...
*/

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

  uint32_t                    MVoiceState[NUM]              = {0};

  int32_t                     MVoicePort[NUM]               = {0};
  int32_t                     MVoiceChannel[NUM]            = {0};
  int32_t                     MVoiceNote[NUM]               = {0};
  int32_t                     MNoteToVoice[MIP_VOICE_NOTES] = {0};
  MIP_VoiceContext            MVoiceContext                 = {0};

  VOICE                       MVoices[NUM]                  = {};

  const clap_input_events_t*  MInEvents                     = nullptr;
  uint32_t                    MNumInEvents                  = 0;
  uint32_t                    MNextInEvent                  = 0;
  uint32_t                    MCurrInEvent                  = 0;

//------------------------------
public:
//------------------------------

  MIP_VoiceManager() {
    reset();
  }

  //----------

  ~MIP_VoiceManager() {
  }

//------------------------------
public:
//------------------------------

  void reset() {
    for (uint32_t i=0; i<(MIP_VOICE_NOTES); i++) {
      MNoteToVoice[i] = -1;
    }
    for (uint32_t i=0; i<NUM; i++) {
      MVoiceState[i]    = MIP_VOICE_OFF;
      MVoicePort[i]     = -1;
      MVoiceChannel[i]  = -1;
      MVoiceNote[i]     = -1;
    }
  }

  //----------

  void prepare(double samplerate) {
    MVoiceContext.samplerate = samplerate;
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].prepare(&MVoiceContext);
    }
  }

  //----------

  uint32_t  getVoiceState(uint32_t v)   { return MVoiceState[v]; }
  int32_t   getVoicePort(uint32_t v)    { return MVoicePort[v]; }
  int32_t   getVoiceChannel(uint32_t v) { return MVoiceChannel[v]; }
  int32_t   getVoiceNote(uint32_t v)    { return MVoiceNote[v]; }

  //----------

  int32_t getVoice(int32_t APort, int32_t AChannel, int32_t AKey) {
    //if (APort < 0) return -1;
    if (AChannel < 0) return -1;
    if (AKey < 0) return -1;
    uint32_t note = (AChannel * 128) + AKey;
    return MNoteToVoice[note];
  }

//------------------------------
public:
//------------------------------

  void on_event(const clap_event_header_t* header) {
    clap_event_note_expression_t* event;
    switch (header->type) {
      case CLAP_EVENT_NOTE_ON:
        on_note_on((clap_event_note_t*)header);
        break;
      case CLAP_EVENT_NOTE_OFF:
        on_note_off((clap_event_note_t*)header);
        break;
      case CLAP_EVENT_NOTE_END:
        on_note_end((clap_event_note_t*)header);
        break;
      case CLAP_EVENT_NOTE_CHOKE:
        on_note_choke((clap_event_note_t*)header);
        break;
      case CLAP_EVENT_PARAM_VALUE:
        on_parameter_value((clap_event_param_value_t*)header);
        break;
      case CLAP_EVENT_PARAM_MOD:
        //MIP_PRINT;
        on_parameter_modulation((clap_event_param_mod_t*)header);
        break;
      //case CLAP_EVENT_MIDI:
      //  break;
      //case CLAP_EVENT_MIDI2:
      //  break;
      //case CLAP_EVENT_MIDI_SYSEX:
      //  break;
      //case CLAP_EVENT_TRANSPORT:
      //  break;
      case CLAP_EVENT_NOTE_EXPRESSION:
        event = (clap_event_note_expression_t*)header;
        switch (event->expression_id) {
          case CLAP_NOTE_EXPRESSION_VOLUME:     on_note_volume_expression(event);     break;
          case CLAP_NOTE_EXPRESSION_PAN:        on_note_pan_expression(event);        break;
          case CLAP_NOTE_EXPRESSION_TUNING:     on_note_tuning_expression(event);     break;
          case CLAP_NOTE_EXPRESSION_VIBRATO:    on_note_vibrato_expression(event);    break;
          case CLAP_NOTE_EXPRESSION_EXPRESSION: on_note_expression_expression(event); break;
          case CLAP_NOTE_EXPRESSION_BRIGHTNESS: on_note_brightness_expression(event); break;
          case CLAP_NOTE_EXPRESSION_PRESSURE:   on_note_pressure_expression(event);   break;
        }
        break;
    }
  }

//------------------------------
public:
//------------------------------

// CLAP_NOTE_DIALECT_CLAP

//------------------------------

  void on_note_on(clap_event_note* event) {
    int32_t port      = event->port_index;
    int32_t channel   = event->channel;
    int32_t key       = event->key;
    float   velocity  = event->velocity;
    handle_voice_note_on(port,channel,key,velocity);
  }

  void on_note_off(clap_event_note* event) {
    int32_t port      = event->port_index;
    int32_t channel   = event->channel;
    int32_t key       = event->key;
    float   velocity  = event->velocity;
    handle_voice_note_off(port,channel,key,velocity);
  }

  void on_note_choke(clap_event_note* event) {
    int32_t port      = event->port_index;
    int32_t channel   = event->channel;
    int32_t key       = event->key;
    handle_voice_note_choke(port,channel,key);
  }

  void on_note_end(clap_event_note* event) {
    int32_t port      = event->port_index;
    int32_t channel   = event->channel;
    int32_t key       = event->key;
    handle_voice_note_end(port,channel,key);
  }

  void on_note_tuning_expression(clap_event_note_expression_t* event) {
    int32_t port      = event->port_index;
    int32_t channel   = event->channel;
    int32_t key       = event->key;
    float   tuning    = event->value;
    handle_voice_tuning(port,channel,key,tuning);
  }

  void on_note_pressure_expression(clap_event_note_expression_t* event) {
    int32_t port      = event->port_index;
    int32_t channel   = event->channel;
    int32_t key       = event->key;
    float   press     = event->value;
    handle_voice_pressure(port,channel,key,press);
  }

  void on_note_brightness_expression(clap_event_note_expression_t* event) {
    int32_t port      = event->port_index;
    int32_t channel   = event->channel;
    int32_t key       = event->key;
    float brightness  = event->value;
    handle_voice_brightness(port,channel,key,brightness);
  }

  void on_note_volume_expression(clap_event_note_expression_t* event) {
    int32_t port      = event->port_index;
    int32_t channel   = event->channel;
    int32_t key       = event->key;
    float   volume    = event->value;
    handle_voice_volume(port,channel,key,volume);
  }

  void on_note_pan_expression(clap_event_note_expression_t* event) {
    int32_t port      = event->port_index;
    int32_t channel   = event->channel;
    int32_t key       = event->key;
    float   pan       = event->value;
    handle_voice_pan(port,channel,key,pan);
  }

  void on_note_vibrato_expression(clap_event_note_expression_t* event) {
    int32_t port      = event->port_index;
    int32_t channel   = event->channel;
    int32_t key       = event->key;
    float   vibrato   = event->value;
    handle_voice_vibrato(port,channel,key,vibrato);
  }

  void on_note_expression_expression(clap_event_note_expression_t* event) {
    int32_t port      = event->port_index;
    int32_t channel   = event->channel;
    int32_t key       = event->key;
    float expression  = event->value;
    handle_voice_expression(port,channel,key,expression);
  }

//------------------------------
public:
//------------------------------

// CLAP_NOTE_DIALECT_MIDI
// CLAP_NOTE_DIALECT_MIDI_MPE

//------------------------------

  void on_midi(clap_event_midi_t* event) {
    /*
    uint32_t evt    = event->data[0] & 0xf0;
    uint32_t chan   = event->data[0] & 0x0f;
    uint32_t index  = event->data[1] & 0x7f;
    uint32_t val    = event->data[2] & 0x7f;
    float    fval   = (float)val * MIP_INV127F;
    float    fval14 = ((index * 128) + val) * MIP_INV127F;
    switch (evt) {
      case MIP_MIDI_NOTE_OFF:
        handle_voice_note_off(chan,index,fval);
        break;
      case MIP_MIDI_NOTE_ON:
        if (val == 0) handle_voice_note_off(chan,index,0);
        else handle_voice_note_on(chan,index,fval);
        break;
      case MIP_MIDI_POLY_AFTERTOUCH:
        if (chan == 0) {
          handle_master_pressure(fval);
        }
        else {
          handle_voice_pressure(chan,index,fval);
        }
        break;
      case MIP_MIDI_CONTROL_CHANGE:
        if (chan == 0) {
          handle_master_ctrl(index,fval);
        }
        else {
          for (uint32_t i=0; i<NUM; i++) {
            if (MVoiceChannel[i] == chan) {
              handle_voice_ctrl(chan,MVoiceNote[i],fval14);
            }
          }
        }
        break;
      case MIP_MIDI_PROGRAM_CHANGE:
        //if (chan == 0) {
        //}
        //else {
        //}
        break;
      case MIP_MIDI_CHANNEL_AFTERTOUCH:
        if (chan == 0) {
          handle_master_pressure(fval);
        }
        else {
          handle_voice_pressure(chan,index,fval);
        }
        break;
      case MIP_MIDI_PITCHBEND:
        if (chan == 0) {
          handle_master_tuning(fval14);
        }
        else {
          for (uint32_t i=0; i<NUM; i++) {
            if (MVoiceChannel[i] == chan) {
              handle_voice_tuning(chan,MVoiceNote[i],fval14);
            }
          }
        }
        break;
      case MIP_MIDI_SYS:
        break;
      //default:
      //  break;
    }
    */
  }

  //----------

  void on_midi_sysex(clap_event_midi_sysex_t* event) {
  }

//------------------------------
public:
//------------------------------

  // CLAP_NOTE_DIALECT_MIDI2

//------------------------------

  void on_midi2(clap_event_midi2_t* event) {
  }

//------------------------------
public:
//------------------------------

  void on_parameter_value(clap_event_param_value_t* event) {
    int32_t   port    = event->port_index;
    int32_t   channel = event->channel;
    int32_t   key     = event->key;
    uint32_t  index   = event->param_id;
    float     value   = event->value;
    if ((channel >= 0) && (key >= 0)) {
      handle_voice_param(port,channel,key,index,value);
    }
    else {
      handle_master_param(index,value);
    }
  }

  //----------

  void on_parameter_modulation(clap_event_param_mod_t* event) {
    //MIP_PRINT;
    int32_t   port    = event->port_index;
    int32_t   channel = event->channel;
    int32_t   key     = event->key;
    uint32_t  index   = event->param_id;
    float     amount  = event->amount;
    //MIP_Print("channel %i key %i index %i amount %.2f\n",channel,key,amount,index);
    if ((channel >= 0) && (key >= 0)) {
      //MIP_PRINT;
      handle_voice_mod(port,channel,key,index,amount);
    }
    else {
      //MIP_PRINT;
      handle_master_mod(index,amount);
    }
  }

//------------------------------
public:
//------------------------------

  /*
    Note on, off, end and choke events.
    In the case of note choke or end events:
    - the velocity is ignored.
    - key and channel are used to match active notes, a value of -1 matches all.
  */

//------------------------------

  /*
    todo:
      consider note already playing.. (multiple keyboards)
      note to voice only handles one voice/note
  */

  // assumes AChannel >= 0

  void handle_voice_note_on(int32_t APort, int32_t AChannel, int32_t AKey, float AVelocity) {
    uint32_t voice = start_voice(APort,AChannel,AKey,AVelocity);
    if (voice < 0) {
    }
  }

  //----------

  // assumes AChannel >= 0

  void handle_voice_note_off(int32_t APort, int32_t AChannel, int32_t AKey, float AVelocity) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoiceState[voice] = MVoices[voice].note_off(AVelocity);
      //MNoteToVoice[note] = -1;
    }
  }

  //----------

  // NOTE_CHOKE is meant to choke the voice(s), like in a drum machine when a closed hihat
  // chokes an open hihat.

  // assumes AChannel >= 0

  void handle_voice_note_choke(int32_t APort, int32_t AChannel, int32_t AKey) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].note_choke();
      MVoiceState[voice] = MIP_VOICE_FINISHED;
      MNoteToVoice[note] = -1;
    }
  }

  //----------

  // NOTE_END is sent by the plugin to the host, when a voice terminates.
  // When using polyphonic modulations, the host has to start voices for its modulators.
  // This message helps the host to track the plugin's voice management.

  // assumes AChannel >= 0

  void handle_voice_note_end(int32_t APort, int32_t AChannel, int32_t AKey) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].note_end();
      MVoiceState[voice] = MIP_VOICE_FINISHED;
      MNoteToVoice[note] = -1;
    }
  }

  //----------

  // with 0 < x <= 4, plain = 20 * log(x)

  void handle_voice_volume(int32_t APort, int32_t AChannel, int32_t AKey, float AVolume) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].volume(AVolume);
    }
  }

  //----------

  // pan, 0 left, 0.5 center, 1 right

  void handle_voice_pan(int32_t APort, int32_t AChannel, int32_t AKey, float APan) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].pan(APan);
    }
  }

  //----------

  // relative tuning in semitone, from -120 to +120

  void handle_voice_tuning(int32_t APort, int32_t AChannel, int32_t AKey, float ABend) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].tuning(ABend);
    }
  }

  //----------

  // 0..1

  void handle_voice_vibrato(int32_t APort, int32_t AChannel, int32_t AKey, float AVibrato) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].vibrato(AVibrato);
    }
  }

  //----------

  // 0..1

  void handle_voice_expression(int32_t APort, int32_t AChannel, int32_t AKey, float AExpression) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].expression(AExpression);
    }
  }

  //----------

  // 0..1

  void handle_voice_brightness(int32_t APort, int32_t AChannel, int32_t AKey, float ABrightness) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].brightness(ABrightness);
    }
  }

  //----------

  // 0..1

  void handle_voice_pressure(int32_t APort, int32_t AChannel, int32_t AKey, float APress) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].pressure(APress);
    }
  }

  //----------

  void handle_voice_ctrl(int32_t APort, int32_t AChannel, int32_t AKey, uint32_t AIndex, uint32_t AValue) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].ctrl(AIndex,AValue);
    }
  }

  //----------

  // PARAM_VALUE sets the parameter's value; uses clap_event_param_value.
  // PARAM_MOD sets the parameter's modulation amount; uses clap_event_param_mod.
  //
  // The value heard is: param_value + param_mod.
  //
  // In case of a concurrent global value/modulation versus a polyphonic one,
  // the voice should only use the polyphonic one and the polyphonic modulation
  // amount will already include the monophonic signal.

  void handle_voice_param(int32_t APort, int32_t AChannel, uint32_t AKey, uint32_t AIndex, float AValue) {
    int32_t voice = MNoteToVoice[(AChannel * 128) + AKey];
    if (voice >= 0) {
      MVoices[voice].parameter(AIndex,AValue);
    }
  }

  //----------

  void handle_voice_mod(int32_t APort, int32_t AChannel, uint32_t AKey, uint32_t AIndex, float AValue) {
    //MIP_PRINT;
    //MIP_Print("channel %i key %i index %i value %.3f\n",AChannel,AKey,AIndex,AValue);
    int32_t voice = MNoteToVoice[(AChannel * 128) + AKey];
    if (voice >= 0) {
      MVoices[voice].modulation(AIndex,AValue);
    }
  }

  //------------------------------

  void handle_master_ctrl(uint32_t AIndex, uint32_t AValue) {
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].ctrl(AIndex,AValue);
    }
  }

  //----------

  void handle_master_param(uint32_t AIndex, float AValue) {
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].parameter(AIndex,AValue);
    }
  }

  //----------

  void handle_master_mod(uint32_t AIndex, float AValue) {
    //MIP_PRINT;
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].modulation(AIndex,AValue);
    }
  }

  //----------

  // midi channel pitchbend

  void handle_master_tuning(float ABend) {
  }

  //----------

  // midi polyphonic aftertouch

  void handle_master_pressure(uint32_t APressure) {
  }

//------------------------------
private:
//------------------------------

  int32_t find_voice(bool ATryReleased=true) {
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceState[i] == MIP_VOICE_OFF) return i;
      if (MVoiceState[i] == MIP_VOICE_FINISHED) return i;
    }
    if (ATryReleased) {
      for (uint32_t i=0; i<NUM; i++) {
        if (MVoiceState[i] == MIP_VOICE_RELEASED) return i;
      }
    }
    return -1;
  }

  //----------

  int32_t start_voice(int32_t APort, int32_t AChannel, int32_t AKey, float AVelocity) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t new_voice = find_voice(true);
    if (new_voice >= 0) {
      // if we stole a released voice..
      if (MVoiceState[new_voice] == MIP_VOICE_RELEASED) {
        int32_t prev_voice = MNoteToVoice[note];
        if (prev_voice >= 0) {
          send_note_end(prev_voice);
          kill_voice(prev_voice);
        }
      }
      MNoteToVoice[note]        = new_voice;
      MVoicePort[new_voice]     = APort;
      MVoiceChannel[new_voice]  = AChannel;
      MVoiceNote[new_voice]     = AKey;
      MVoiceState[new_voice]    = MVoices[new_voice].note_on(AKey,AVelocity);
      return new_voice;
    }
    return -1;
  }

  //----------

  void kill_voice(int32_t voice) {
    if (voice < 0) return;
    //switch (MVoiceState[voice]) {
    //  case MIP_VOICE_OFF:
    //    break;
    //  case MIP_VOICE_PLAYING:
    //    break;
    //  case MIP_VOICE_RELEASED:
    //    break;
    //  case MIP_VOICE_FINISHED:
    //    break;
    //}
    MVoicePort[voice]     = -1;
    MVoiceChannel[voice]  = -1;
    MVoiceNote[voice]     = -1;
    MVoiceState[voice]    = MIP_VOICE_OFF;
    //int32_t port    = MVoiceNote[voice];
    int32_t key     = MVoiceNote[voice];
    int32_t channel = MVoiceChannel[voice];
    if (key < 0) return;
    if (channel < 0) return;
    uint32_t note = (channel * 128) + key;
    MNoteToVoice[note] = -1;
  }

//------------------------------
private:
//------------------------------

  void send_note_end(uint32_t voice) {
    clap_event_note_t event;
    clap_event_header_t* header = (clap_event_header_t*)&event;
    header->size      = sizeof(clap_event_note_t);
    header->time      = MVoiceContext.process->frames_count;// - 1;
    header->space_id  = CLAP_CORE_EVENT_SPACE_ID;
    header->type      = CLAP_EVENT_NOTE_END;
    header->flags     = 0;
    event.port_index  = MVoicePort[voice];
    event.key         = MVoiceNote[voice];
    event.channel     = MVoiceChannel[voice];
    event.velocity    = 0.0;
    const clap_output_events_t* out_events = MVoiceContext.process->out_events;
    out_events->try_push(out_events,header);
  }

//------------------------------
private:
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

  /*
    returns number of samples until next event,
    or MIP_INT32_MAX if no more events
  */

  uint32_t processEvents(uint32_t AOffset, uint32_t ASize) {
    while (MNextInEvent < (AOffset + ASize)) {
      if (MCurrInEvent < MNumInEvents) {
        const clap_event_header_t* header = MInEvents->get(MInEvents,MCurrInEvent);
        if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
          on_event(header);
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

  //uint32_t preProcessVoices() {
  //  return 0;
  //}

  //----------

  // process all voices (for one slice)
  // irregular length

  void processVoices(uint32_t ASize) {
    MIP_ClearVoiceSliceBuffer(ASize);
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceState[i] == MIP_VOICE_PLAYING){
        MVoiceState[i] = MVoices[i].process(MIP_VOICE_PLAYING,ASize);
      }
      if (MVoiceState[i] == MIP_VOICE_RELEASED) {
        MVoiceState[i] = MVoices[i].process(MIP_VOICE_RELEASED,ASize);
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
      if (MVoiceState[i] == MIP_VOICE_FINISHED) {
        send_note_end(i);
        kill_voice(i);
      }
      //TODO: send PARAM_MOD events
    }
  }



//------------------------------
public:
//------------------------------

  /*
    process entire audio buffer..
    voices fill VoiceSliceBuffer
    this could just as well be used for effects too, i guess?
    (1 voice = 1 audio-stream)
  */

  void process(const clap_process_t *process) {
    MVoiceContext.process = process;
    //MVoiceContext.slicebuffer = MIP_VoiceSliceBuffer;
    //preProcessVoices();
    MInEvents = process->in_events;
    MNumInEvents = MInEvents->size(MInEvents);
    float* out0 = process->audio_outputs->data32[0];
    float* out1 = process->audio_outputs->data32[1];
    uint32_t offset = 0;
    uint32_t remaining = process->frames_count;
    preProcessEvents();
    while (remaining > 0) {
      if (remaining >= MIP_VOICE_SLICE_SIZE) {
        processEvents(offset);
        processVoices();
        MIP_CopyVoiceSliceBuffer(out0 + offset);
        MIP_CopyVoiceSliceBuffer(out1 + offset);
        remaining -= MIP_VOICE_SLICE_SIZE;
        offset += MIP_VOICE_SLICE_SIZE;
      }
      else {
        processEvents(offset,remaining);
        processVoices(remaining);
        MIP_CopyVoiceSliceBuffer(out0 + offset,remaining);
        MIP_CopyVoiceSliceBuffer(out1 + offset,remaining);
        remaining = 0;
      }
    }
    postProcessEvents();
    postProcessVoices();
  }


//------------------------------
//------------------------------

};

//----------------------------------------------------------------------
#endif
