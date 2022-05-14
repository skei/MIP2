#ifndef sa_tyr_voice_included
#define sa_tyr_voice_included
//----------------------------------------------------------------------

#include "mip.h"

//#include "plugin/mip_plugin.h"
//#include "plugin/mip_editor.h"
//#include "gui/mip_widgets.h"

#include "audio/mip_audio_math.h"
//#include "audio/mip_voice_manager.h"
#include "audio/modulation/mip_envelope.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_tyr_voice {

//------------------------------
public:
//------------------------------

  uint32_t  state = MIP_VOICE_OFF;
  MIP_Note  note  = {};

//------------------------------
private:
//------------------------------

  MIP_VoiceContext* context = nullptr;
  MIP_Envelope amp_env = {};

  int32_t note_key        = -1;

  float   note_volume     = 0.0;
  float   note_onvel      = 0.0;
  float   note_offvel     = 0.0;
  float   note_pan        = 0.0;
  float   note_tuning     = 0.0;
  float   note_vibrato    = 0.0;
  float   note_expression = 0.0;
  float   note_brightness = 0.0;
  float   note_pressure   = 0.0;

  float   hz              = 0.0;
  float   ph              = 0.0;
  float   phadd           = 0.0;

//------------------------------
public:
//------------------------------

  float getEnvLevel() {
    return amp_env.getValue();
  }

  //----------

  void prepare(MIP_VoiceContext* AContext) {
    context = AContext;
    amp_env.setSampleRate(context->samplerate);
    amp_env.setAttack(0);
    amp_env.setDecay(0);
    amp_env.setSustain(1);
    amp_env.setRelease(2);
  }

  //----------

  uint32_t note_on(int32_t key, float velocity) {
    ph = 0.0;
    note_key = key;
    note_onvel = velocity;
    note_tuning = 0.0;
    amp_env.noteOn();
    return MIP_VOICE_PLAYING;
  }

  //----------

  uint32_t note_off(float velocity) {
    note_offvel = velocity;
    amp_env.noteOff();
    return MIP_VOICE_RELEASED;
  }

  //----------

  void note_choke() {
  }

  //----------

  void volume(float amount)     { note_volume = amount; }
  void pan(float amount)        { note_pan = amount;}
  void tuning(float amount)     { note_tuning = amount; }
  void vibrato(float amount)    { note_vibrato = amount; }
  void expression(float amount) { note_expression = amount; }
  void brightness(float amount) { note_brightness = amount; }
  void pressure(float amount)   { note_pressure = amount; }

  //----------

  void parameter(uint32_t index, float value) {
    //switch (index) {
    //  case: break;
    //}
  }

  void modulation(uint32_t index, float value) {
    //switch (index) {
    //  case: break;
    //}
  }

  //----------

  // ASize  = 0..15
  // AState = MIP_VOICE_PLAYING/MIP_VOICE_RELEASED

  uint32_t process(uint32_t AIndex, uint32_t AState, uint32_t ASize, uint32_t ATime) {

    hz = MIP_NoteToHz(note_key + note_tuning);
    phadd = hz * context->invsamplerate;

    float* output = context->voicebuffer;
    for (uint32_t i = 0; i < ASize; i++) {

      float t1 = ph + 0.5f;
      t1 = MIP_Fract(t1);
      float saw1 = 2.0 * t1 - 1.0;
      //saw1 -= MIP_PolyBlep(t1,phadd);

      float amp = note_onvel + note_pressure;
      amp = MIP_Clamp(amp,0,1);
      amp *= amp_env.process();
      //amp = vol_smoother.process(amp);

      float out = saw1;
      *output++ += (out * amp);

      ph += phadd;
      ph = MIP_Fract(ph);
    }

    if (amp_env.getStage() == MIP_ENVELOPE_FINISHED) return MIP_VOICE_FINISHED;
    else return AState;

  }

  //----------

  //uint32_t process(uint32_t AIndex,uint32_t AState) {
  //  return process(AIndex,AState,MIP_VOICE_SLICE_SIZE);
  //}

};

//----------------------------------------------------------------------
#endif
