#ifndef mip_voice_included
#define mip_voice_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_note.h"

//----------------------------------------------------------------------

/*

#define MIP_VOICE_MAX_EVENTS_PER_BLOCK 256

//----------

enum MIP_EVoiceEvents {
  MIP_VOICE_EVENT_NOTE_ON         = 0,
  MIP_VOICE_EVENT_NOTE_OFF        = 1,
  MIP_VOICE_EVENT_NOTE_CHOKE      = 2,
  MIP_VOICE_EVENT_NOTE_EXPRESSION = 3,
  MIP_VOICE_EVENT_PARAMETER       = 4,
  MIP_VOICE_EVENT_MODULATION      = 5
};
*/

//----------

/*
struct MIP_VoiceEvent {
  uint32_t  type  = 0;
  uint32_t  time  = 0;
  uint32_t  index = 0;
  float     value = 0;
  MIP_VoiceEvent() {}
  MIP_VoiceEvent(uint32_t typ, uint32_t tim, uint32_t ind, float val) {
    type  = typ;
    time  = tim;
    index = ind;
    value = val;
  }
};
*/

//----------

struct MIP_VoiceContext {
//float*                voicebuffer0   = nullptr;
//float*                voicebuffer1   = nullptr;
  float*                voicebuffer   = nullptr;
  const clap_process_t* process       = nullptr;
  double                samplerate    = 0.0;
  double                invsamplerate = 0.0;
  MIP_ParameterArray*   parameters    = nullptr;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class VOICE>
class MIP_Voice {

//------------------------------
public:
//------------------------------

  VOICE             voice   = {};
  MIP_VoiceContext* context = nullptr;
  uint32_t          state   = MIP_VOICE_OFF;
  MIP_Note          note    = {};

  //MIP_Queue<MIP_VoiceEvent,MIP_VOICE_MAX_EVENTS_PER_BLOCK*2> events = {};

//------------------------------
public:
//------------------------------

  void prepare(uint32_t AIndex, MIP_VoiceContext* AContext) {
    context = AContext;
    voice.prepare(AIndex,context);
  }

  //----------

  double getLevel() {
    return voice.getEnvLevel();
  }

//------------------------------
public:
//------------------------------

  uint32_t process(uint32_t AState, uint32_t ASize, uint32_t AOffset) {
    return voice.process(AState,ASize,AOffset);
  }

//------------------------------
public:
//------------------------------

  /*
  void handleVoiceEvent(MIP_VoiceEvent event) {
    switch (event.type) {
      case MIP_VOICE_EVENT_NOTE_ON:         note_on(event.index,event.value);     break;
      case MIP_VOICE_EVENT_NOTE_OFF:        note_off(event.value);                break;
      case MIP_VOICE_EVENT_NOTE_CHOKE:      note_choke();                         break;
      case MIP_VOICE_EVENT_NOTE_EXPRESSION: expression(event.index,event.value);  break;
      case MIP_VOICE_EVENT_PARAMETER:       parameter(event.index,event.value);   break;
      case MIP_VOICE_EVENT_MODULATION:      modulation(event.index,event.value);  break;
    }
  }
  */

  //----------

  void note_on(int32_t key, double velocity) {
    state = voice.note_on(key,velocity);
  }

  //----------

  void note_off(double velocity) {
    state = voice.note_off(velocity);
  }

  //----------

  void note_choke() {
    voice.note_choke();
    state = MIP_VOICE_FINISHED;
  }

  //----------

  void expression(uint32_t type, double value) {
    switch (type) {
      case CLAP_NOTE_EXPRESSION_VOLUME:     voice.volume(value);      break;
      case CLAP_NOTE_EXPRESSION_PAN:        voice.pan(value);         break;
      case CLAP_NOTE_EXPRESSION_TUNING:     voice.tuning(value);      break;
      case CLAP_NOTE_EXPRESSION_VIBRATO:    voice.vibrato(value);     break;
      case CLAP_NOTE_EXPRESSION_EXPRESSION: voice.expression(value);  break;
      case CLAP_NOTE_EXPRESSION_BRIGHTNESS: voice.brightness(value);  break;
      case CLAP_NOTE_EXPRESSION_PRESSURE:   voice.pressure(value);    break;
    }
  }

  //----------

  void parameter(uint32_t index, double value) {
    voice.parameter(index,value);
  }

  //----------

  void modulation(uint32_t index, double value) {
    voice.modulation(index,value);
  }

  //----------


};

//----------------------------------------------------------------------
#endif





#if 0




//struct MIP_Note {
//  int32_t  port_index;
//  int32_t  channel;
//  int32_t  key;
//  int32_t  note_id;
//};




  //----------



//------------------------------
public:
//------------------------------

  void prepare_note_on(uint32_t time, int32_t key, double velocity) {
    MIP_VoiceEvent ev = MIP_VoiceEvent(MIP_VOICE_EVENT_NOTE_ON,time,key,velocity);
    events.write(ev);
    state = MIP_VOICE_WAITING;
  }

  void prepare_note_off(uint32_t time, double velocity) {
    MIP_VoiceEvent ev = MIP_VoiceEvent(MIP_VOICE_EVENT_NOTE_OFF,time,0,velocity);
    events.write(ev);
  }

  void prepare_note_choke(uint32_t time) {
    MIP_VoiceEvent ev = MIP_VoiceEvent(MIP_VOICE_EVENT_NOTE_CHOKE,time,0,0);
    events.write(ev);
  }

  void prepare_expression(uint32_t time, uint32_t type, double value) {
    MIP_VoiceEvent ev = MIP_VoiceEvent(MIP_VOICE_EVENT_NOTE_EXPRESSION,time,type,value);
    events.write(ev);
  }

  void prepare_parameter(uint32_t time, uint32_t index, double value) {
    MIP_VoiceEvent ev = MIP_VoiceEvent(MIP_VOICE_EVENT_PARAMETER,time,index,value);
    events.write(ev);
  }

  void prepare_modulation(uint32_t time, uint32_t index, double value) {
    MIP_VoiceEvent ev = MIP_VoiceEvent(MIP_VOICE_EVENT_MODULATION,time,index,value);
    events.write(ev);
  }

//------------------------------
public:
//------------------------------

  void processBlock(uint32_t AIndex) {
    uint32_t length = context->process->frames_count;
    state = voice.process(AIndex,state,length,0);
  }

  //----------

  /*
    todo: add offset to current_time before calling voice.process
    so we get rid of this in the voice:

    #ifdef MIP_VOICE_PREPARE_EVENTS
      MVoiceManager.processPrepared(process,MHost);
    #else
      MVoiceManager.processBlock(process);
    #endif

  */

  void processPrepared(uint32_t AIndex) {
    const clap_process_t* process = context->process;
    uint32_t current_time = 0;
    uint32_t remaining = process->frames_count;
    MIP_VoiceEvent next_event;
    while (remaining > 0) {
      if (events.read(&next_event)) {
        // we have more events

        //MIP_Print("current_time: %i next_event.time: %i\n",current_time,next_event.time);

        int32_t length = next_event.time - current_time;
        if (length > 0) {

          if (state != MIP_VOICE_WAITING) {
          //if ((state == MIP_VOICE_PLAYING) || ((state == MIP_VOICE_RELEASED)) {

            state = voice.process(AIndex,state,length,current_time);
          }
          remaining -= length;
          current_time += length;
        }
        handleVoiceEvent(next_event);
      } // event

      else {
        // no more events
        int32_t length = remaining;

        if (state != MIP_VOICE_WAITING) {
        //if ((state == MIP_VOICE_PLAYING) || ((state == MIP_VOICE_RELEASED)) {

          state = voice.process(AIndex,state,length,current_time);
        }
        remaining -= length;
        current_time += length;
      } // !event

    } // remaining > 0
    MIP_Assert( events.read(&next_event) == false );
  }



#endif // 0
