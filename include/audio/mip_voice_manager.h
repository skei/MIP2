#ifndef mip_voice_manager_included
#define mip_voice_manager_included
//----------------------------------------------------------------------

/*
  TODO:
  - redo this completely (in progress)

  - sample accurate events (in & out)
  - send event_mod events (end, choke?)
  - handle note_choke
  - handle volume expression


  - voice stealing.. if two keyboards play the same note..
    midi channel & key = the same..
    check if already playing/occupied.. kill already playing?

*/

//----------------------------------------------------------------------

#include "mip.h"
#include "audio/mip_audio_utils.h"
#include "plugin/clap/mip_clap.h"

//----------

#define MIP_VOICE_MAX_CHANNELS    16
#define MIP_VOICE_MAX_NOTES       128
#define MIP_VOICE_NOTES           (MIP_VOICE_MAX_CHANNELS * MIP_VOICE_MAX_NOTES)


#define MIP_MASTER_BEND_RANGE  2.0f
#define MIP_VOICE_BEND_RANGE   48.0f

#define MIP_INV_MASTER_BEND_RANGE  (1.0 / MIP_MASTER_BEND_RANGE)
#define MIP_INV_VOICE_BEND_RANGE   (1.0 / MIP_VOICE_BEND_RANGE)

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// one voice context per note port

struct MIP_VoiceContext {
  const clap_process_t*   process     = nullptr;
  double                  samplerate  = 0.0;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


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

  bool              MPrepared                     = false;

  MIP_VoiceContext  MVoiceContext                 = {0};
  VOICE             MVoices[NUM]                  = {};
  uint32_t          MVoiceState[NUM]              = {0};
  int32_t           MVoiceNote[NUM]               = {0};
  int32_t           MVoiceChannel[NUM]            = {0};
  int32_t           MNoteToVoice[MIP_VOICE_NOTES] = {0};

  //uint32_t          MVoiceAge[NUM]                = {0};

//------------------------------
public:
//------------------------------

  MIP_VoiceManager() {
    for (uint32_t i=0; i<(MIP_VOICE_NOTES); i++) MNoteToVoice[i] = -1;
    for (uint32_t i=0; i<NUM; i++) {
      MVoiceState[i]    = MIP_VOICE_OFF;
      MVoiceNote[i]     = -1;
      MVoiceChannel[i]  = -1;
    }
  }

  //----------

  ~MIP_VoiceManager() {
  }

//------------------------------
public:
//------------------------------

  void prepare(double samplerate) {
    //if (MPrepared) return;
    MVoiceContext.samplerate = samplerate;
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].prepare(&MVoiceContext);
    }
    //MPrepared = true;
  }

//------------------------------
public:
//------------------------------

  // CLAP_NOTE_DIALECT_CLAP

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

  void on_midi(clap_event_midi_t* event) {
    MIP_Print("todo\n");
  }

  void on_midi2(clap_event_midi2_t* event) {
    MIP_Print("todo\n");
  }

  void on_midi_sysex(clap_event_midi_sysex_t* event) {
    MIP_Print("todo\n");
  }

//------------------------------
public:
//------------------------------

  void on_parameter_value(clap_event_param_value_t* event) {
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
private:
//------------------------------

  // assumes AChannel >= 0

  void handle_voice_note_on(int32_t AChannel, int32_t AKey, float AVelocity) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = find_voice(true);
    if (voice >= 0) {
      kill_voice(voice);
      MNoteToVoice[note] = voice;
      float note = (float)AKey;
      MVoiceNote[voice]     = AKey;
      MVoiceChannel[voice]  = AChannel;
      MVoiceState[voice]    = MVoices[voice].note_on(note,AVelocity);
    }
  }

  //----------

  // assumes AChannel >= 0

  void handle_voice_note_off(int32_t AChannel, int32_t AKey, float AVelocity) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoiceState[voice] = MVoices[voice].note_off(AVelocity);
      MNoteToVoice[note] = -1;
    }
  }

  //----------

  // assumes AChannel >= 0

  void handle_voice_note_choke(int32_t AChannel, int32_t AKey) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].note_choke();
      MVoiceState[voice] = MIP_VOICE_FINISHED;
      MNoteToVoice[note] = -1;
    }
  }

  //----------

  // assumes AChannel >= 0

  void handle_voice_note_end(int32_t AChannel, int32_t AKey) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].note_end();
      MVoiceState[voice] = MIP_VOICE_FINISHED;
      MNoteToVoice[note] = -1;
    }
  }

  //----------

  //void handle_voice_bend(int32_t AChannel, uint32_t ABend) {
  //  float v = (ABend * MIP_INV8192F) - 1.0f;
  // tuning = -1 .. 1

  void handle_voice_tuning(int32_t AChannel, int32_t AKey, float ABend) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].tuning(ABend);
    }
  }

  //----------

  void handle_voice_pressure(int32_t AChannel, int32_t AKey, float APress) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].pressure(APress);
    }
  }

  //----------

  void handle_voice_brightness(int32_t AChannel, int32_t AKey, float ABrightness) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].brightness(ABrightness);
    }
  }

  void handle_voice_volume(int32_t AChannel, int32_t AKey, float AVolume) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].volume(AVolume);
    }
  }

  void handle_voice_pan(int32_t AChannel, int32_t AKey, float APan) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].pan(APan);
    }
  }

  void handle_voice_vibrato(int32_t AChannel, int32_t AKey, float AVibrato) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].vibrato(AVibrato);
    }
  }

  void handle_voice_expression(int32_t AChannel, int32_t AKey, float AExpression) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].expression(AExpression);
    }
  }

  //----------

  void handle_voice_ctrl(int32_t AChannel, int32_t AKey, uint32_t AIndex, uint32_t AValue) {
    uint32_t note = (AChannel * 128) + AKey;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MVoices[voice].ctrl(AIndex,AValue);
    }
  }

  //----------

  void handle_voice_param(int32_t AChannel, uint32_t AKey, uint32_t AIndex, float AValue) {
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
    //MIP_Print("master ctrl %i %.2f\n",AIndex,AValue);
    //for (uint32_t i=0; i<NUM; i++) {
    //  MVoices[i].master_ctrl(AIndex,AValue);
    //}
  }

  //----------

  void handle_master_param(uint32_t AIndex, float AValue) {
    //MIP_Print("master param %i %.2f\n",AIndex,AValue);
    //for (uint32_t i=0; i<NUM; i++) {
    //  MVoices[i].master_param(AIndex,AValue);
    //}
  }

  //----------

  void handle_master_mod(uint32_t AIndex, float AValue) {
    //MIP_Print("master mod %i %.2f\n",AIndex,AValue);
    //for (uint32_t i=0; i<NUM; i++) {
    //  MVoices[i].master_mod(AIndex,AValue);
    //}
  }

  //----------

  void handle_master_tuning(float ABend) {
    //MIP_Print("master bend %.2f\n",ABend);
    //float mb = (ABend * MIP_INV8192F) - 1.0f;
    //for (uint32_t i=0; i<NUM; i++) {
    //  MVoices[i].master_bend(mb);
    //}
  }

  //----------

  void handle_master_pressure(uint32_t APressure) {
    //MIP_Print("master pressure %.2f\n",APressure);
    //float mp = (float)AVelocity * MIP_INV127F;
    //for (uint32_t i=0; i<NUM; i++) {
    //  MVoices[i].master_pressure(mp);
    //}
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

  // should we tell the voice that we kill it?

  void kill_voice(uint32_t voice) {
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
//        uint32_t time = MVoiceContext.length - 1;
//        int32_t  port = 0;
//        int32_t  note = MVoiceNote[i];
//        int32_t  chan = MVoiceChannel[i];
//        send_note_end_event(MVoiceContext.process,time,port,note,chan);
        kill_voice(i);
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

//------------------------------
public:
//------------------------------

  /*
    TODO:
    * sample-accurate
      - split block? 16/64 samples?

    assumes output buffer is already cleared
    (voices adds to it)

  */

  void process(const clap_process_t *process) {
    MVoiceContext.process = process;
//    prepare(MSampleRate);
    preProcess();
    //MVoiceContext.process = process;
    float** outputs = process->audio_outputs->data32;
    uint32_t length = process->frames_count;
//    MVoiceContext.buffers = outputs;
//    MVoiceContext.length = length;
    MIP_ClearStereoBuffer(outputs,length);
    processPlayingVoices();
    processReleasedVoices();
    postProcess();
  }


};

//----------------------------------------------------------------------
#endif
