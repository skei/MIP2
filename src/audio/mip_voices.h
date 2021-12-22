#ifndef mip_voices_included
#define mip_voices_included
//----------------------------------------------------------------------

#include "mip.h"
#include "audio/mip_audio_utils.h"
#include "plugin/clap/mip_clap.h"

//#define MIP_MASTER_BEND_RANGE  2.0f
//#define MIP_VOICE_BEND_RANGE   48.0f

//----------------------------------------------------------------------

struct MIP_VoiceContext {
  const clap_process_t* process     = nullptr;
  float                 samplerate  = 0.0;
  float**               buffers     = nullptr;
  uint32_t              length      = 0;
 };

//----------------------------------------------------------------------

template <class VOICE, int NUM>
class MIP_Voices {

//------------------------------
private:
//------------------------------

  MIP_VoiceContext  MVoiceContext         = {0};
  VOICE             MVoices[NUM]          = {};
  uint32_t          MVoiceState[NUM]      = {0};
  int32_t           MVoiceNote[NUM]       = {0};
  int32_t           MVoiceChannel[NUM]    = {0};
  int32_t           MNoteToVoice[16*128]  = {0};

//------------------------------
public:
//------------------------------

  MIP_Voices() {
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

  void prepare(float ASampleRate/*, uint32_t ABlockSize*/) {
    MVoiceContext.samplerate = ASampleRate;
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].prepare(&MVoiceContext);
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

  //----------

  /*
    Note on, off, end and choke events.
    In the case of note choke or end events:
    - the velocity is ignored.
    - key and channel are used to match active notes, a value of -1 matches all.

    typedef struct clap_event_note {
      int32_t port_index;
      int32_t key;        // 0..127
      int32_t channel;    // 0..15
      double  velocity;   // 0..1
    } clap_event_note_t;
  */

  //------------------------------

  void note_on(const clap_event_note_t* event) {
    MIP_Print("port %i chan %i key %i vel %.2f\n",event->port_index,event->channel,event->key,event->velocity);
    handle_voice_strike(event->channel,event->key,event->velocity);
  }

  //----------

  void note_off(const clap_event_note_t* event) {
    MIP_Print("port %i chan %i key %i vel %.2f\n",event->port_index,event->channel,event->key,event->velocity);
    handle_voice_lift(event->channel,event->key,event->velocity);
  }

  //----------

  void note_end(const clap_event_note_t* event) {
    MIP_Print("port %i chan %i key %i vel %.2f\n",event->port_index,event->channel,event->key,event->velocity);
  }

  //----------

  void note_choke(const clap_event_note_t* event) {
    MIP_Print("port %i chan %i key %i vel %.2f\n",event->port_index,event->channel,event->key,event->velocity);
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
        break;
      case CLAP_NOTE_EXPRESSION_VIBRATO:
        break;
      case CLAP_NOTE_EXPRESSION_BRIGHTNESS:
        break;
      case CLAP_NOTE_EXPRESSION_BREATH:
        break;
      case CLAP_NOTE_EXPRESSION_PRESSURE:
        break;
      case CLAP_NOTE_EXPRESSION_TIMBRE:
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
    switch (msg) {
      case MIP_MIDI_NOTE_OFF:
        handle_voice_lift(ch,v1,v2);
        break;
      case MIP_MIDI_NOTE_ON:
        if (v2 == 0) handle_voice_lift(ch,v1,0);
        else handle_voice_strike(ch,v1,v2);
        break;
      case MIP_MIDI_CONTROL_CHANGE:
        if (ch == 0) handle_master_ctrl(v1,v2);
        else {
          if (v1 == 74) handle_voice_slide(ch,v2);
          else handle_voice_ctrl(ch,v1,v2);
        }
        break;
      case MIP_MIDI_PITCHBEND:
        if (ch == 0) handle_master_bend(bend);
        else handle_voice_bend(ch,bend);
        break;
      case MIP_MIDI_CHANNEL_AFTERTOUCH:
        if (ch == 0) handle_master_press(v1);
        else handle_voice_press(ch,v1);
        break;
      case MIP_MIDI_POLY_AFTERTOUCH:
        handle_voice_press(ch,v1);
        break;
    } // switch
    MIP_PRINT;
  }

//------------------------------
private:
//------------------------------

  void handle_voice_strike(int32_t AChannel, uint32_t ANote, uint32_t AVelocity) {
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

  void handle_voice_lift(int32_t AChannel, uint32_t ANote, uint32_t AVelocity) {
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

  void handle_voice_bend(int32_t AChannel, uint32_t ABend) {
    float v = (ABend * MIP_INV8192F) - 1.0f;
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceChannel[i] == AChannel) {
        MVoices[i].bend(v);
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

  void handle_voice_slide(int32_t AChannel, uint32_t ASlide) {
    float s = (float)ASlide * MIP_INV127F;
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceChannel[i] == AChannel) {
        MVoices[i].slide(s);
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

  void handle_master_ctrl(uint32_t AIndex, uint32_t AValue) {
    //float mc = (float)AValue * MIP_INV127F;
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].master_ctrl(AIndex,AValue);
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

//------------------------------
private:
//------------------------------

  void preProcess() {
  }

  //----------

  void postProcess() {
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceState[i] == MIP_VOICE_FINISHED) {
        clear_voice(i);
      }
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

  //----------

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

  void clear_voice(uint32_t voice) {
    int32_t n  = MVoiceNote[voice];
    int32_t ch = MVoiceChannel[voice];
    if ((n>=0) && (ch>=0)) {
      uint32_t note = (MVoiceChannel[voice] * 128) + MVoiceNote[voice];
      MNoteToVoice[note] = -1;
    }
    MVoiceNote[voice]    = -1;
    MVoiceChannel[voice] = -1;
    MVoiceState[voice]   = MIP_VOICE_OFF;
  }

};

//----------------------------------------------------------------------
#endif

//
////----------------------------------------------------------------------
//
//#include "mip.h"
//#include "audio/mip_audio_math.h"
//#include "audio/mip_audio_utils.h"
//#include "plugin/mip_process_context.h"
//
////----------------------------------------------------------------------
//
//enum MIP_EVoiceStates {
//  MIP_VOICE_OFF      = 0,
//  MIP_VOICE_PLAYING  = 1,
//  MIP_VOICE_RELEASED = 2,
//  MIP_VOICE_FINISHED = 3
//};
//
////----------------------------------------------------------------------
//
//struct MIP_VoiceContext {
//  MIP_ProcessContext*  processContext  = MIP_NULL;
//};
//
////----------------------------------------------------------------------
//
///*
//class MIP_Voice {
//public:
//  void strike(float note, float vel) {}
//  void lift(float vel) {}
//  void bend(float v) {}
//  void press(float v) {}
//  void slide(float v) {}
//  void ctrl(uint32_t i, uint32_t v) {}
//  void master_bend(float mb) {}
//  void master_press(float mp) {}
//  void master_ctrl(uint32_t i, uint32_t v) {}
//  void parameter(uint32_t i, float v) {}
//  void process(MIP_VoiceContext* AContext) {}
//};
//*/
//
////----------------------------------------------------------------------
////
////
////
////----------------------------------------------------------------------
//
//template <class VOICE, int NUM=16>
//class MIP_Voices {
//
////------------------------------
//private:
////------------------------------
//
//  MIP_VoiceContext MVoiceContext;
//  VOICE             MVoices[NUM];
//  uint32_t          MVoiceState[NUM];
//  int32_t           MVoiceNote[NUM];
//  int32_t           MVoiceChannel[NUM];
//  int32_t           MNoteToVoice[16*128];
//
////------------------------------
//public:
////------------------------------
//
//  MIP_Voices() {
//    for (uint32_t i=0; i<(16*128); i++) MNoteToVoice[i] = -1;
//    for (uint32_t i=0; i<NUM; i++) {
//      MVoiceState[i]    = MIP_VOICE_OFF;
//      MVoiceNote[i]     = -1;
//      MVoiceChannel[i]  = -1;
//    }
//  }
//
//  //----------
//
//  ~MIP_Voices() {
//  }
//
////------------------------------
//public:
////------------------------------
//
//  void prepare(float ASampleRate/*, uint32_t ABlockSize*/) {
//    for (uint32_t i=0; i<NUM; i++) {
//      MVoices[i].prepare(&MVoiceContext,ASampleRate/*,ABlockSize*/);
//    }
//  }
//
//  //----------
//
//  void parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) {
//    for (uint32_t i=0; i<NUM; i++) {
//      MVoices[i].parameter(AIndex,AValue);
//    }
//  }
//
//  //----------
//
//  void midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AMode=0) {
//    uint32_t  msg   = AMsg1 & 0xf0;
//    uint32_t  ch    = AMsg1 & 0x0f;
//    uint32_t  v1    = AMsg2 & 0x7f;
//    uint32_t  v2    = AMsg3 & 0x7f;
//    uint32_t  bend  = v1 + (v2 * 128);
//    switch (msg) {
//      case MIP_MIDI_NOTE_OFF:
//        handle_voice_lift(ch,v1,v2);
//        break;
//      case MIP_MIDI_NOTE_ON:
//        if (v2 == 0) handle_voice_lift(ch,v1,0);
//        else handle_voice_strike(ch,v1,v2);
//        break;
//      case MIP_MIDI_CONTROL_CHANGE:
//        if (ch == 0) handle_master_ctrl(v1,v2);
//        else {
//          if (v1 == 74) handle_voice_slide(ch,v2);
//          else handle_voice_ctrl(ch,v1,v2);
//        }
//        break;
//      case MIP_MIDI_PITCHBEND:
//        if (ch == 0) handle_master_bend(bend);
//        else handle_voice_bend(ch,bend);
//        break;
//      case MIP_MIDI_CHANNEL_AFTERTOUCH:
//        if (ch == 0) handle_master_press(v1);
//        else handle_voice_press(ch,v1);
//        break;
//      case MIP_MIDI_POLY_AFTERTOUCH:
//        handle_voice_press(ch,v1);
//        break;
//    } // switch
//  }
//
//  //----------
//
//  void processBlock(MIP_ProcessContext* AContext) {
//    preProcess();
//    MVoiceContext.processContext = AContext;
//    MIP_ClearStereoBuffer(AContext->outputs,AContext->numsamples);
//    processPlayingVoices();
//    processReleasedVoices();
//    postProcess();
//  }
//
////------------------------------
//private:
////------------------------------
//
//  void preProcess() {
//  }
//
//  //----------
//
//  void postProcess() {
//    for (uint32_t i=0; i<NUM; i++) {
//      if (MVoiceState[i] == MIP_VOICE_FINISHED) {
//        clear_voice(i);
//      }
//    }
//  }
//
//  //----------
//
//  void processPlayingVoices() {
//    for (uint32_t i=0; i<NUM; i++) {
//      if (MVoiceState[i] == MIP_VOICE_PLAYING) {
//        MVoiceState[i] = MVoices[i].process(MIP_VOICE_PLAYING);
//      }
//    }
//  }
//
//  //----------
//
//  void processReleasedVoices() {
//    for (uint32_t i=0; i<NUM; i++) {
//      if (MVoiceState[i] == MIP_VOICE_RELEASED) {
//        MVoiceState[i] = MVoices[i].process(MIP_VOICE_RELEASED);
//      }
//    }
//  }
//
//  //----------
//
//  int32_t find_voice(bool ATryReleased=true) {
//    for (uint32_t i=0; i<NUM; i++) {
//      if (MVoiceState[i] == MIP_VOICE_OFF) return i;
//      if (MVoiceState[i] == MIP_VOICE_FINISHED) return i;
//    }
//    if (ATryReleased) {
//      for (uint32_t i=0; i<NUM; i++) {
//        if (MVoiceState[i] == MIP_VOICE_RELEASED) {
//          return i;
//        }
//      }
//    }
//    return -1;
//  }
//
//  //----------
//
//  void clear_voice(uint32_t voice) {
//    int32_t n  = MVoiceNote[voice];
//    int32_t ch = MVoiceChannel[voice];
//    if ((n>=0) && (ch>=0)) {
//      uint32_t note = (MVoiceChannel[voice] * 128) + MVoiceNote[voice];
//      MNoteToVoice[note] = -1;
//    }
//    MVoiceNote[voice]    = -1;
//    MVoiceChannel[voice] = -1;
//    MVoiceState[voice]   = MIP_VOICE_OFF;
//  }
//
//
//
////------------------------------
//private:
////------------------------------
//
//  void handle_voice_strike(int32_t AChannel, uint32_t ANote, uint32_t AVelocity) {
//    uint32_t note = (AChannel * 128) + ANote;
//    int32_t voice = find_voice(true);
//    if (voice >= 0) {
//      clear_voice(voice);
//      MNoteToVoice[note] = voice;
//      float v = (float)AVelocity * MIP_INV127F;
//      float n = (float)ANote;
//      MVoiceNote[voice]     = ANote;
//      MVoiceChannel[voice]  = AChannel;
//      MVoiceState[voice]    = MVoices[voice].strike(n,v);
//    }
//  }
//
//  //----------
//
//  void handle_voice_lift(int32_t AChannel, uint32_t ANote, uint32_t AVelocity) {
//    uint32_t note = (AChannel * 128) + ANote;
//    int32_t voice = MNoteToVoice[note];
//    if (voice >= 0) {
//      float v = (float)AVelocity * MIP_INV127F;
//      MVoiceState[voice] = MVoices[voice].lift(v);
//      MNoteToVoice[note] = -1;
//      //clear_voice(voice);
//    }
//  }
//
//  //----------
//
//  void handle_voice_bend(int32_t AChannel, uint32_t ABend) {
//    float v = (ABend * MIP_INV8192F) - 1.0f;
//    for (uint32_t i=0; i<NUM; i++) {
//      if (MVoiceChannel[i] == AChannel) {
//        MVoices[i].bend(v);
//      }
//    }
//  }
//
//  //----------
//
//  void handle_voice_press(int32_t AChannel, uint32_t APress) {
//    float v = (float)APress * MIP_INV127F;
//    for (uint32_t i=0; i<NUM; i++) {
//      if (MVoiceChannel[i] == AChannel) {
//        MVoices[i].press(v);
//      }
//    }
//  }
//
//  //----------
//
//  void handle_voice_slide(int32_t AChannel, uint32_t ASlide) {
//    float s = (float)ASlide * MIP_INV127F;
//    for (uint32_t i=0; i<NUM; i++) {
//      if (MVoiceChannel[i] == AChannel) {
//        MVoices[i].slide(s);
//      }
//    }
//  }
//
//  //----------
//
//  void handle_voice_ctrl(int32_t AChannel, uint32_t AIndex, uint32_t AValue) {
//    //float v = (float)AValue * MIP_INV127F;
//    for (uint32_t i=0; i<NUM; i++) {
//      if (MVoiceChannel[i] == AChannel) {
//        MVoices[i].ctrl(AIndex,AValue);
//      }
//    }
//  }
//
//  //----------
//
//  void handle_master_ctrl(uint32_t AIndex, uint32_t AValue) {
//    //float mc = (float)AValue * MIP_INV127F;
//    for (uint32_t i=0; i<NUM; i++) {
//      MVoices[i].master_ctrl(AIndex,AValue);
//    }
//  }
//
//  //----------
//
//  void handle_master_bend(uint32_t ABend) {
//    float mb = (ABend * MIP_INV8192F) - 1.0f;
//    for (uint32_t i=0; i<NUM; i++) {
//      MVoices[i].master_bend(mb);
//    }
//  }
//
//  //----------
//
//  void handle_master_press(uint32_t AVelocity) {
//    float mp = (float)AVelocity * MIP_INV127F;
//    for (uint32_t i=0; i<NUM; i++) {
//      MVoices[i].master_press(mp);
//    }
//  }
//
//  //----------
//
//};




