#ifndef sa_tyr_voice_incuded
#define sa_tyr_voice_incuded
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_voice.h"
#include "audio/mip_audio_math.h"
//#include "audio/mip_audio_utils.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_tyr_voice {

//------------------------------
private:
//------------------------------


  uint32_t          MIndex    = 0;
  MIP_VoiceContext* MContext  = nullptr;

  double t  = 0.0;
  double dt = 0.0;

//------------------------------
public:
//------------------------------

  sa_tyr_voice() {
  }

  //----------

  ~sa_tyr_voice() {
  }

//------------------------------
public:
//------------------------------

  void  prepare(uint32_t AIndex, MIP_VoiceContext* AContext) {
    MIndex = AIndex;
    MContext = AContext;
  }

  //----------

  double getEnvLevel() {
    return 1.0;
  }

  //----------


  uint32_t note_on(int32_t key, double velocity) {
    MIP_Print("%i key %i velocity %.3f\n",MIndex,key,velocity);
    t = 0.0;
    double hz = MIP_NoteToHz(key);
    dt = 1.0 / MIP_HzToSamples(hz,MContext->samplerate);
    return MIP_VOICE_PLAYING;
  }

  //----------


  uint32_t note_off(double velocity) {
    MIP_Print("%i velocity %.3f\n",MIndex,velocity);
    return MIP_VOICE_FINISHED;
  }

  //----------


  void note_choke() {
    MIP_Print("%i\n",MIndex);
  }

  //----------


  void volume(double value) {
    MIP_Print("%i value %.3f\n",MIndex,value);
  }

  //----------


  void pan(double value) {
    MIP_Print("%i value %.3f\n",MIndex,value);
  }

  //----------


  void tuning(double value) {
    MIP_Print("%i value %.3f\n",MIndex,value);
  }

  //----------


  void vibrato(double value) {
    MIP_Print("%i value %.3f\n",MIndex,value);
  }

  //----------


  void expression(double value) {
    MIP_Print("%i value %.3f\n",MIndex,value);
  }

  //----------

  void brightness(double value) {
    MIP_Print("%i value %.3f\n",MIndex,value);
  }

  //----------


  void pressure(double value) {
    MIP_Print("%i value %.3f\n",MIndex,value);
  }

  //----------


  void parameter(uint32_t index, double value) {
    MIP_Print("%i index %i value %.3f\n",MIndex,index,value);
  }

  //----------


  void modulation(uint32_t index, double value) {
    MIP_Print("%i index %i value %.3f\n",MIndex,index,value);
  }

//------------------------------
public:
//------------------------------

  uint32_t process(uint32_t AState, uint32_t ASize, uint32_t AOffset) {

    float* buffer = MContext->voicebuffer;
    buffer += AOffset;
    for (uint32_t i=0; i<ASize; i++) {

      *buffer++ += t;//MIP_Random() * 0.1;

      t += dt;
      t = MIP_Fract(t);
    }

    return MIP_VOICE_PLAYING;
  }

};

//----------------------------------------------------------------------
#endif
