#ifndef mip_clap_voices_included
#define mip_clap_voices_included
//----------------------------------------------------------------------

/*
  TODO:
  - redo this completely
  in the meanwhile:
  - sample accurate events (in & out)
  - send event_mod events
  - handle note_choke
  - handle volume expression
*/

//----------------------------------------------------------------------

#include "mip.h"
#include "audio/mip_audio_utils.h"
#include "plugin/clap/mip_clap.h"

#define MIP_MASTER_BEND_RANGE  2.0f
#define MIP_VOICE_BEND_RANGE   48.0f

#define MIP_INV_MASTER_BEND_RANGE  (1.0 / MIP_MASTER_BEND_RANGE)
#define MIP_INV_VOICE_BEND_RANGE   (1.0 / MIP_VOICE_BEND_RANGE)

//----------------------------------------------------------------------

struct MIP_ClapVoiceContext {
  const clap_process_t* process     = nullptr;
  float                 samplerate  = 0.0;
  float**               buffers     = nullptr;
  uint32_t              length      = 0;
 };

//----------------------------------------------------------------------

template <class VOICE, int NUM>
class MIP_ClapVoices {

//------------------------------
private:
//------------------------------

  MIP_ClapVoiceContext  MVoiceContext         = {0};
  VOICE                 MVoices[NUM]          = {};
  uint32_t              MVoiceState[NUM]      = {0};
  int32_t               MVoiceNote[NUM]       = {0};
  int32_t               MVoiceChannel[NUM]    = {0};
  int32_t               MNoteToVoice[16*128]  = {0};

//------------------------------
public:
//------------------------------

  MIP_ClapVoices() {
    for (uint32_t i=0; i<(16*128); i++) MNoteToVoice[i] = -1;
    for (uint32_t i=0; i<NUM; i++) {
      MVoiceState[i]    = MIP_VOICE_OFF;
      MVoiceNote[i]     = -1;
      MVoiceChannel[i]  = -1;
    }
  }

//------------------------------
public:
//------------------------------

//------------------------------
public:
//------------------------------

  void prepare(float ASampleRate) {
    MVoiceContext.samplerate = ASampleRate;
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].prepare(&MVoiceContext);
    }
  }

  //----------

  void note_on(const clap_event_note_t* event) {
    handle_voice_strike(event->port_index,event->channel,event->key,event->velocity);
  }

  //----------

  void note_off(const clap_event_note_t* event) {
    handle_voice_lift(event->port_index,event->channel,event->key,event->velocity);
  }

  //----------

  void note_end(const clap_event_note_t* event) {
  }

  //----------

  /*
    NOTE_CHOKE is meant to choke the voice, like in a drum machine when a closed hihat chokes an open hihat
    (note_end) You may get both events, but if you receive a NOTE_CHOKE it terminates the voice(s) anyway
  */

  void note_choke(const clap_event_note_t* event) {
  }

  //----------

  void note_expression(const clap_event_note_expression_t* event) {
    switch(event->expression_id) {
      case CLAP_NOTE_EXPRESSION_VOLUME:
        // // x >= 0, use 20 * log(4 * x)
        break;
      case CLAP_NOTE_EXPRESSION_PAN:
        // pan, 0 left, 0.5 center, 1 right
        break;
      case CLAP_NOTE_EXPRESSION_TUNING:
        // relative tuning in semitone, from -120 to +120
        handle_voice_bend(event->channel,event->value * MIP_INV_VOICE_BEND_RANGE);
        break;
      case CLAP_NOTE_EXPRESSION_VIBRATO:
        break;
      case CLAP_NOTE_EXPRESSION_BRIGHTNESS:
        break;
      case CLAP_NOTE_EXPRESSION_BREATH:
        break;
      case CLAP_NOTE_EXPRESSION_PRESSURE:
        handle_voice_press(event->channel,event->value);
        break;
      case CLAP_NOTE_EXPRESSION_TIMBRE:
        handle_voice_timbre(event->channel,event->value);
        break;
    }
  }

  //----------

  void note_mask(const clap_event_note_mask_t* event) {
    MIP_PRINT;
  }

  //----------

  void midi(const clap_event_midi_t* event) {
    uint32_t  msg   = event->data[0] & 0xf0;
    uint32_t  ch    = event->data[0] & 0x0f;
    uint32_t  v1    = event->data[1] & 0x7f;
    uint32_t  v2    = event->data[2] & 0x7f;
    uint32_t  bend  = v1 + (v2 * 128);
    float     fbend = (bend * MIP_INV8192F) - 1.0f;

    switch (msg) {
      case MIP_MIDI_NOTE_OFF:
        handle_voice_lift(0,ch,v1,v2);
        break;
      case MIP_MIDI_NOTE_ON:
        if (v2 == 0) handle_voice_lift(0,ch,v1,0);
        else handle_voice_strike(0,ch,v1,v2);
        break;
      case MIP_MIDI_CONTROL_CHANGE:
        if (ch == 0) handle_master_ctrl(v1,v2);
        else {
          if (v1 == 74) handle_voice_timbre(ch,v2);
          else handle_voice_ctrl(ch,v1,v2);
        }
        break;
      case MIP_MIDI_PITCHBEND:
        if (ch == 0) handle_master_bend(bend);
        else handle_voice_bend(ch,fbend);
        break;
      case MIP_MIDI_CHANNEL_AFTERTOUCH:
        if (ch == 0) handle_master_press(v1);
        else handle_voice_press(ch,v1);
        break;
      case MIP_MIDI_POLY_AFTERTOUCH:
        handle_voice_press(ch,v1);
        break;
    }
  }

  //----------

  void parameter(const clap_event_param_value_t* event) {
    uint32_t index = event->param_id;
    float value = event->value;
    int32_t channel = event->channel;
    int32_t key = event->key;
    if ((channel >= 0) && (key >= 0)) {
      handle_voice_param(channel,key,index,value);
    }
    else {
      handle_master_param(index,value);
    }
  }

  //----------

  void modulation(const clap_event_param_mod_t* event) {
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

  //----------

  void process(const clap_process_t *process) {
    preProcess();
    MVoiceContext.process = process;
    float** outputs = process->audio_outputs->data32;
    uint32_t length = process->frames_count;
    MVoiceContext.buffers = outputs;
    MVoiceContext.length = length;
    MIP_ClearStereoBuffer(outputs,length);
    processPlayingVoices();
    processReleasedVoices();
    postProcess();
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
        if (MVoiceState[i] == MIP_VOICE_RELEASED) {
          return i;
        }
      }
    }
    return -1;
  }

  //----------

  /*
    should we tell the voice that we kill it?
  */

  void clear_voice(uint32_t voice) {
    int32_t n  = MVoiceNote[voice];
    int32_t ch = MVoiceChannel[voice];
    if ((n>=0) && (ch>=0)) {
      uint32_t note = (ch * 128) + n;
      MNoteToVoice[note]    = -1;
      MVoiceNote[voice]     = -1;
      MVoiceChannel[voice]  = -1;
      MVoiceState[voice]    = MIP_VOICE_OFF;
    }
  }

  //----------

  void send_note_end_event(const clap_process* process, uint32_t time, int32_t port, int32_t key, int32_t chan) {
    clap_event event;
    event.type            = CLAP_EVENT_NOTE_END;
    event.time            = time;
    event.note.port_index = port;
    event.note.key        = key;
    event.note.channel    = chan;
    event.note.velocity   = 0.0;
    process->out_events->push_back(process->out_events,&event);
  }

  /*
    PARAM_MOD, yes send it out :-) Once per block and for the latest sample of the block
  */

  void send_param_mod_event(const clap_process* process, uint32_t time, clap_id param_id, double amount, int32_t port, int32_t key, int32_t chan) {
    MIP_PRINT;
    clap_event event;
    event.type                  = CLAP_EVENT_PARAM_MOD;
    event.time                  = time;
    event.param_mod.cookie      = nullptr;
    event.param_mod.param_id    = param_id;
    event.param_mod.port_index  = port;       // -1 for global
    event.param_mod.key         = key;        // -1 for global
    event.param_mod.channel     = chan;       // -1 for global
    event.param_mod.amount      = amount;
    process->out_events->push_back(process->out_events,&event);
  }

//------------------------------
//private:
public:
//------------------------------

  void handle_voice_strike(int32_t APort, int32_t AChannel, uint32_t ANote, uint32_t AVelocity) {
    uint32_t note = (AChannel * 128) + ANote;
    int32_t voice = find_voice(true);
    if (voice >= 0) {
      clear_voice(voice);
      MNoteToVoice[note] = voice;
      float v = (float)AVelocity * MIP_INV127F;
      float n = (float)ANote;
      MVoiceNote[voice]     = ANote;
      MVoiceChannel[voice]  = AChannel;
      MVoiceState[voice]    = MVoices[voice].strike(n,v);
    }
  }

  //----------

  void handle_voice_lift(int32_t APort, int32_t AChannel, uint32_t ANote, uint32_t AVelocity) {
    uint32_t note = (AChannel * 128) + ANote;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      float v = (float)AVelocity * MIP_INV127F;
      MVoiceState[voice] = MVoices[voice].lift(v);
      MNoteToVoice[note] = -1;
      //clear_voice(voice);
    }
  }

  //----------

  //void handle_voice_bend(int32_t AChannel, uint32_t ABend) {
  //  float v = (ABend * MIP_INV8192F) - 1.0f;
  // ABend = -1 .. 1

  void handle_voice_bend(int32_t AChannel, float ABend) {
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceChannel[i] == AChannel) {
        MVoices[i].bend(ABend);
      }
    }
  }

  //----------

  void handle_voice_press(int32_t AChannel, uint32_t APress) {
    float v = (float)APress * MIP_INV127F;
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceChannel[i] == AChannel) {
        MVoices[i].press(v);
      }
    }
  }

  //----------

  void handle_voice_timbre(int32_t AChannel, uint32_t ATimbre) {
    float s = (float)ATimbre * MIP_INV127F;
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceChannel[i] == AChannel) {
        MVoices[i].timbre(s);
      }
    }
  }

  //----------

  void handle_voice_ctrl(int32_t AChannel, uint32_t AIndex, uint32_t AValue) {
    //float v = (float)AValue * MIP_INV127F;
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceChannel[i] == AChannel) {
        MVoices[i].ctrl(AIndex,AValue);
      }
    }
  }

  //----------

  void handle_voice_param(int32_t AChannel, uint32_t AKey, uint32_t AIndex, float AValue) {
    //int32_t v = MNoteToVoice[(AChannel * 128) + AKey];
    //if (v >= 0) {
    //  MVoices[v].parameter(AIndex,AValue);
    //}
  }

  //----------

  void handle_voice_mod(int32_t AChannel, uint32_t AKey, uint32_t AIndex, float AValue) {
    //int32_t v = MNoteToVoice[(AChannel * 128) + AKey];
    //if (v >= 0) {
    //  MVoices[v].modulation(AIndex,AValue);
    //}
  }

  //----------

  void handle_master_ctrl(uint32_t AIndex, uint32_t AValue) {
    //float mc = (float)AValue * MIP_INV127F;
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].master_ctrl(AIndex,AValue);
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
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].modulation(AIndex,AValue);
    }
  }

  //----------

  void handle_master_bend(uint32_t ABend) {
    float mb = (ABend * MIP_INV8192F) - 1.0f;
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].master_bend(mb);
    }
  }

  //----------

  void handle_master_press(uint32_t AVelocity) {
    float mp = (float)AVelocity * MIP_INV127F;
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].master_press(mp);
    }
  }

  //----------

  //void handle_parameter(uint32_t AIndex) {
  //}

//------------------------------
private:
//------------------------------

  void preProcess() {
  }

  //----------

  void postProcess() {
    for (uint32_t i=0; i<NUM; i++) {

      if (MVoiceState[i] == MIP_VOICE_FINISHED) {
        // shound be sample-time at event...
        uint32_t time = MVoiceContext.length - 1;;
        int32_t  port = 0;
        int32_t  note = MVoiceNote[i];
        int32_t  chan = MVoiceChannel[i];
        send_note_end_event(MVoiceContext.process,time,port,note,chan);
        clear_voice(i);
      }

      //TODO: send PARAM_MOD events

    }
  }

  //----------

  void processPlayingVoices() {
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceState[i] == MIP_VOICE_PLAYING) {
        MVoiceState[i] = MVoices[i].process(MIP_VOICE_PLAYING);
      }
    }
  }

  //----------

  void processReleasedVoices() {
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceState[i] == MIP_VOICE_RELEASED) {
        MVoiceState[i] = MVoices[i].process(MIP_VOICE_RELEASED);
      }
    }
  }

};

//----------------------------------------------------------------------
#endif
