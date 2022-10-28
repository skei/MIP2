#ifndef sa_tyr_voice_incuded
#define sa_tyr_voice_incuded
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_voice.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_tyr_voice {

//------------------------------
private:
//------------------------------

  uint32_t MIndex = 0;

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
  }

  //----------

  double getEnvLevel() {
    return 1.0;
  }

  //----------


  uint32_t note_on(int32_t key, double velocity) {
    MIP_Print("%i key %i velocity %.3f\n",MIndex,key,velocity);
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

  //----------

};

//----------------------------------------------------------------------
#endif
