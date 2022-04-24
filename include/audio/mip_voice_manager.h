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

#define MIP_VOICE_TICKSIZE        16
#define MIP_VOICE_MAX_OVERSAMPLE  1
#define MIP_VOICE_BUFFERSIZE      (MIP_VOICE_TICKSIZE * MIP_VOICE_MAX_OVERSAMPLE)
#define MIP_VOICE_MAX_EVENTS      1024

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// one voice context per note port?

struct MIP_VoiceContext {
  const clap_process_t* process     = nullptr;
  double                samplerate  = 0.0;
  float*                voicebuffer = nullptr;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

/*
  2 buffers, each 16 samples (64 bytes)
  = 1 cache line each..
  each voice fills the voice buffer, which is then added to the tick buffer
  (..hardcoded and unrolled, plus optimizer magic..)
  events for each 16-sample block are handled at the beginning of the block,
  so we might get some jittering (max 15 samples)..
*/

__MIP_ALIGNED(MIP_ALIGNMENT_CACHE)
float MIP_VoiceBuffer[MIP_VOICE_BUFFERSIZE] = {0};

__MIP_ALIGNED(MIP_ALIGNMENT_CACHE)
float MIP_TickBuffer[MIP_VOICE_BUFFERSIZE] = {0};

//----------

void MIP_ClearTickBuffer(uint32_t ASize) {
  memset(MIP_TickBuffer,0,ASize*sizeof(float));
}

void MIP_CopyTickBuffer(float* ADst, uint32_t ASize) {
  memcpy(ADst,MIP_TickBuffer,ASize*sizeof(float));
}

void MIP_AddVoiceToTickBuffer(uint32_t ASize) {
  for (uint32_t i=0; i<ASize; i++) {
    MIP_TickBuffer[i] += MIP_VoiceBuffer[i];
  }
}

//----------

// the optimizer will probably automatically unroll the loop if we pass a const
// as length?

void MIP_ClearTickBuffer() {
  //MIP_ClearTickBuffer(MIP_VOICE_TICKSIZE);
  float* dst = MIP_TickBuffer;
  *dst++ = 0.0;
  *dst++ = 0.0;
  *dst++ = 0.0;
  *dst++ = 0.0;
  *dst++ = 0.0;
  *dst++ = 0.0;
  *dst++ = 0.0;
  *dst++ = 0.0;
  *dst++ = 0.0;
  *dst++ = 0.0;
  *dst++ = 0.0;
  *dst++ = 0.0;
  *dst++ = 0.0;
  *dst++ = 0.0;
  *dst++ = 0.0;
  *dst   = 0.0;
}

//----------

void MIP_CopyTickBuffer(float* ADst) {
  //MIP_CopyTickBuffer(ADst,MIP_VOICE_TICKSIZE);
  float* src = MIP_TickBuffer;
  float* dst = ADst;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst   = *src;
}

//----------

void MIP_AddVoiceToTickBuffer() {
  //MIP_AddVoiceToTickBuffer(MIP_VOICE_TICKSIZE);
  float* src = MIP_VoiceBuffer;;
  float* dst = MIP_TickBuffer;
  *dst++ += *src++;
  *dst++ += *src++;
  *dst++ += *src++;
  *dst++ += *src++;
  *dst++ += *src++;
  *dst++ += *src++;
  *dst++ += *src++;
  *dst++ += *src++;
  *dst++ += *src++;
  *dst++ += *src++;
  *dst++ += *src++;
  *dst++ += *src++;
  *dst++ += *src++;
  *dst++ += *src++;
  *dst++ += *src++;
  *dst   += *src;
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

  MIP_VoiceContext            MVoiceContext                 = {0};
  VOICE                       MVoices[NUM]                  = {};
  uint32_t                    MVoiceState[NUM]              = {0};
  int32_t                     MVoiceNote[NUM]               = {0};
  int32_t                     MVoiceChannel[NUM]            = {0};
  int32_t                     MNoteToVoice[MIP_VOICE_NOTES] = {0};
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
      MVoiceState[i] = MIP_VOICE_OFF;
      MVoiceNote[i] = -1;
      MVoiceChannel[i] = -1;
    }
  }

  //----------

  void prepare(double samplerate) {
    MVoiceContext.samplerate = samplerate;
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].prepare(&MVoiceContext);
    }
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
    int32_t channel = event->channel;
    int32_t key = event->key;
    float velocity = event->velocity;
    handle_voice_note_on(channel,key,velocity);
  }

  void on_note_off(clap_event_note* event) {
    int32_t channel = event->channel;
    int32_t key = event->key;
    float velocity = event->velocity;
    handle_voice_note_off(channel,key,velocity);
  }

  void on_note_choke(clap_event_note* event) {
    int32_t channel = event->channel;
    int32_t key = event->key;
    handle_voice_note_choke(channel,key);
  }

  void on_note_end(clap_event_note* event) {
    int32_t channel = event->channel;
    int32_t key = event->key;
    handle_voice_note_end(channel,key);
  }

  void on_note_tuning_expression(clap_event_note_expression_t* event) {
    int32_t channel = event->channel;
    int32_t key = event->key;
    float tuning = event->value;
    handle_voice_tuning(channel,key,tuning);
  }

  void on_note_pressure_expression(clap_event_note_expression_t* event) {
    int32_t channel = event->channel;
    int32_t key = event->key;
    float press = event->value;
    handle_voice_pressure(channel,key,press);
  }

  void on_note_brightness_expression(clap_event_note_expression_t* event) {
    int32_t channel = event->channel;
    int32_t key = event->key;
    float brightness = event->value;
    handle_voice_brightness(channel,key,brightness);
  }

  void on_note_volume_expression(clap_event_note_expression_t* event) {
    int32_t channel = event->channel;
    int32_t key = event->key;
    float volume = event->value;
    handle_voice_volume(channel,key,volume);
  }

  void on_note_pan_expression(clap_event_note_expression_t* event) {
    int32_t channel = event->channel;
    int32_t key = event->key;
    float pan = event->value;
    handle_voice_pan(channel,key,pan);
  }

  void on_note_vibrato_expression(clap_event_note_expression_t* event) {
    int32_t channel = event->channel;
    int32_t key = event->key;
    float vibrato = event->value;
    handle_voice_vibrato(channel,key,vibrato);
  }

  void on_note_expression_expression(clap_event_note_expression_t* event) {
    int32_t channel = event->channel;
    int32_t key = event->key;
    float expression = event->value;
    handle_voice_expression(channel,key,expression);
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
    uint32_t index = event->param_id;
    float value = event->value;
    int32_t channel = event->channel;
    int32_t key = event->key;

//MIP_Print("channel %i key %i\n",channel,key);

    if ((channel >= 0) && (key >= 0)) {
      handle_voice_param(channel,key,index,value);
    }
    else {
      handle_master_param(index,value);
    }
  }

  //----------

  void on_parameter_modulation(clap_event_param_mod_t* event) {
    uint32_t index = event->param_id;
    float amount = event->amount;
    int32_t channel = event->channel;
    int32_t key = event->key;
    if ((channel >= 0) && (key >= 0)) {
      handle_voice_mod(channel,key,index,amount);
    }
    else {
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

  void handle_voice_note_on(int32_t AChannel, int32_t AKey, float AVelocity) {
    //MIP_PRINT;
    start_voice(AChannel,AKey,AVelocity);
  }

  //----------

  // assumes AChannel >= 0

  void handle_voice_note_off(int32_t AChannel, int32_t AKey, float AVelocity) {
    //MIP_PRINT;
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoiceState[voice] = MVoices[voice].note_off(AVelocity);
//      MNoteToVoice[note] = -1;
    }
  }

  //----------

  // NOTE_CHOKE is meant to choke the voice(s), like in a drum machine when a closed hihat
  // chokes an open hihat.

  // assumes AChannel >= 0

  void handle_voice_note_choke(int32_t AChannel, int32_t AKey) {
    MIP_PRINT;
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

  void handle_voice_note_end(int32_t AChannel, int32_t AKey) {
    MIP_PRINT;
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

  void handle_voice_volume(int32_t AChannel, int32_t AKey, float AVolume) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].volume(AVolume);
    }
  }

  //----------

  // pan, 0 left, 0.5 center, 1 right

  void handle_voice_pan(int32_t AChannel, int32_t AKey, float APan) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].pan(APan);
    }
  }

  //----------

  // relative tuning in semitone, from -120 to +120

  void handle_voice_tuning(int32_t AChannel, int32_t AKey, float ABend) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].tuning(ABend);
    }
  }

  //----------

  // 0..1

  void handle_voice_vibrato(int32_t AChannel, int32_t AKey, float AVibrato) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].vibrato(AVibrato);
    }
  }

  //----------

  // 0..1

  void handle_voice_expression(int32_t AChannel, int32_t AKey, float AExpression) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].expression(AExpression);
    }
  }

  //----------

  // 0..1

  void handle_voice_brightness(int32_t AChannel, int32_t AKey, float ABrightness) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].brightness(ABrightness);
    }
  }

  //----------

  // 0..1

  void handle_voice_pressure(int32_t AChannel, int32_t AKey, float APress) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].pressure(APress);
    }
  }

  //----------

  // TODO:

  void handle_voice_ctrl(int32_t AChannel, int32_t AKey, uint32_t AIndex, uint32_t AValue) {
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

  void handle_voice_param(int32_t AChannel, uint32_t AKey, uint32_t AIndex, float AValue) {
    //MIP_Print("channel %i key %i index %i value %.3f\n",AChannel,AKey,AIndex,AValue);
    int32_t voice = MNoteToVoice[(AChannel * 128) + AKey];
    if (voice >= 0) {
      MVoices[voice].parameter(AIndex,AValue);
    }
  }

  //----------

  void handle_voice_mod(int32_t AChannel, uint32_t AKey, uint32_t AIndex, float AValue) {
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
    //MIP_Print("%i %.3f\n",AIndex,AValue);
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].parameter(AIndex,AValue);
    }
  }

  //----------

  void handle_master_mod(uint32_t AIndex, float AValue) {
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

  int32_t start_voice(int32_t AChannel, int32_t AKey, float AVelocity) {
    uint32_t note = (AChannel * 128) + AKey;
    // kill potentially already playing voice
    int32_t prev_voice = MNoteToVoice[note];
    if (prev_voice >= 0) kill_voice(prev_voice);
    int32_t voice = find_voice(true);
    if (voice >= 0) {
      MNoteToVoice[note] = voice;
      MVoiceNote[voice] = AKey;
      MVoiceChannel[voice] = AChannel;
      MVoiceState[voice] = MVoices[voice].note_on(AKey,AVelocity);
      return voice;
    }
    return -1;
  }

  //----------

  void kill_voice(int32_t voice) {
    if (voice < 0) return;
    int32_t key = MVoiceNote[voice];
    int32_t channel = MVoiceChannel[voice];
    if (key < 0) return;
    if (channel < 0) return;
    uint32_t note = (channel * 128) + key;
    MVoiceNote[voice] = -1;
    MVoiceChannel[voice] = -1;
    MVoiceState[voice] = MIP_VOICE_OFF;
    MNoteToVoice[note] = -1;
  }

//------------------------------
//public:
private:
//------------------------------

  void send_note_end(uint32_t index) {
    clap_event_note_t event;
    clap_event_header_t* header = (clap_event_header_t*)&event;
    header->size      = sizeof(clap_event_note_t);
    header->time      = MVoiceContext.process->frames_count - 1;
    header->space_id  = CLAP_CORE_EVENT_SPACE_ID;
    header->type      = CLAP_EVENT_NOTE_END;
    header->flags     = 0;
    event.port_index  = 0;
    event.key         = MVoiceNote[index];
    event.channel     = MVoiceChannel[index];
    event.velocity    = 0.0;
    const clap_output_events_t* out_events = MVoiceContext.process->out_events;
    out_events->try_push(out_events,header);
  }

//------------------------------
private:
//------------------------------

  /*
    returns number of samples until next event, or -1 for no more (all done)
  */

  uint32_t preProcess() {
    return 0;
  }

  //----------

  void postProcess() {
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceState[i] == MIP_VOICE_FINISHED) {
        kill_voice(i);
        send_note_end(i);
      }
      //TODO: send PARAM_MOD events
    }
  }

  //----------

//  void processPlayingVoices() {
//    for (uint32_t i=0; i<NUM; i++) {
//      if (MVoiceState[i] == MIP_VOICE_PLAYING) {
//        MVoiceState[i] = MVoices[i].process(MIP_VOICE_PLAYING);
//      }
//    }
//  }

  //----------

//  void processReleasedVoices() {
//    for (uint32_t i=0; i<NUM; i++) {
//      if (MVoiceState[i] == MIP_VOICE_RELEASED) {
//        MVoiceState[i] = MVoices[i].process(MIP_VOICE_RELEASED);
//      }
//    }
//  }

//------------------------------
public:
//------------------------------

  /*
    assumes output buffer is already cleared
    (voices adds to it)
  */

//  void process(const clap_process_t *process) {
//    MVoiceContext.process = process;
//    preProcess();
//    float** outputs = process->audio_outputs->data32;
//    uint32_t length = process->frames_count;
//    MIP_ClearStereoBuffer(outputs,length);
//    processPlayingVoices();
//    processReleasedVoices();
//    postProcess();
//  }

//------------------------------------------------------------
// ticks
//------------------------------------------------------------

//------------------------------
private: // process
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

  void postProcessEvents() {
    MNumInEvents = 0;
  }

  //----------

  /*
    returns number of samples until next event,
    or MIP_INT32_MAX if no more events
  */

  uint32_t processEvents(uint32_t AOffset, uint32_t ASize) {
    while (MNextInEvent < (AOffset + ASize)) {
      //MCurrInEvent++;
      if (MCurrInEvent < MNumInEvents) {
        const clap_event_header* header = MInEvents->get(MInEvents,MCurrInEvent);
        on_event(header);
        MCurrInEvent++;
        MNextInEvent = header->time;
      }
      else MNextInEvent = MIP_INT32_MAX;
    }
    uint32_t res = MNextInEvent - AOffset;
    return res;
  }

  //----------

  // TODO

  uint32_t processEvents(uint32_t AOffset) {
    return processEvents(AOffset,MIP_VOICE_TICKSIZE);
  }

  //----------

  void processTick(uint32_t ASize) {
    MIP_ClearTickBuffer(ASize);

    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceState[i] == MIP_VOICE_PLAYING) {
        MVoiceState[i] = MVoices[i].process(MIP_VOICE_PLAYING,ASize);
        MIP_AddVoiceToTickBuffer(ASize);
      }
      if (MVoiceState[i] == MIP_VOICE_RELEASED) {
        MVoiceState[i] = MVoices[i].process(MIP_VOICE_RELEASED,ASize);
        MIP_AddVoiceToTickBuffer(ASize);
      }
    }
    // post-process per voice effects here..
  }

  //----------

  // TODO (hardcode size)

  void processTick() {
    MIP_ClearTickBuffer();

    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceState[i] == MIP_VOICE_PLAYING) {
        MVoiceState[i] = MVoices[i].process(MIP_VOICE_PLAYING);
        MIP_AddVoiceToTickBuffer();
      }
      if (MVoiceState[i] == MIP_VOICE_RELEASED) {
        MVoiceState[i] = MVoices[i].process(MIP_VOICE_RELEASED);
        MIP_AddVoiceToTickBuffer();
      }
    }
    // post-process per voice effects here..
  }

//------------------------------
public:
//------------------------------

  /*
    this could just as well be used for effects too, i guess?
    1 voice = 1 audio-stream
  */

  void processTicks(const clap_process_t *process) {
    MVoiceContext.process = process;
    MVoiceContext.voicebuffer = MIP_VoiceBuffer;
    preProcess();
    MInEvents = process->in_events;
    MNumInEvents = MInEvents->size(MInEvents);
    float* out0 = process->audio_outputs->data32[0];
    float* out1 = process->audio_outputs->data32[1];
    uint32_t offset = 0;
    uint32_t remaining = process->frames_count;
    preProcessEvents();
    while (remaining > 0) {
      if (remaining >= MIP_VOICE_TICKSIZE) {
        processEvents(offset);
        processTick();
        MIP_CopyTickBuffer(out0 + offset);
        MIP_CopyTickBuffer(out1 + offset);
        remaining -= MIP_VOICE_TICKSIZE;
        offset += MIP_VOICE_TICKSIZE;
      }
      else {
        processEvents(offset,remaining);
        processTick(remaining);
        MIP_CopyTickBuffer(out0 + offset,remaining);
        MIP_CopyTickBuffer(out1 + offset,remaining);
        remaining = 0;
      }
    }
    postProcessEvents();
    postProcess();
  }


//------------------------------
//------------------------------

};

//----------------------------------------------------------------------
#endif
