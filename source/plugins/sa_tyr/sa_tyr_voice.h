#ifndef sa_tyr_voice_incuded
#define sa_tyr_voice_incuded
//----------------------------------------------------------------------

#include "mip.h"
#include "audio/mip_audio_math.h"
#include "audio/filters/mip_rc_filter.h"
#include "audio/filters/mip_svf_filter.h"
#include "audio/processing/mip_delay.h"
#include "audio/synthesis/mip_oscillator2.h"
#include "audio/modulation/mip_envelope.h"
#include "audio/waveforms/mip_polyblep_waveform.h"
#include "plugin/mip_voice.h"
//#include "plugin/mip_voice_manager.h"

//----------------------------------------------------------------------

#include "sa_tyr_osc.h"
#include "sa_tyr_res.h"

//----------

// todo: redo the smoothing things..
#define SMOOTHER_FACTOR (1.0 / 250.0)

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class T>
class sa_tyr_voice {

//------------------------------
private:
//------------------------------

  uint32_t          MVoiceIndex   = 0;
  MIP_VoiceContext* MContext      = nullptr;
  MIP_Queue<const clap_event_note_t*,256> MEvents;

//------------------------------
private:
//------------------------------

  //MIP_VoiceContext* MContext      = nullptr;
  float* MVoiceBuffer = nullptr;

  T*                MParameters   = nullptr;
  T*                MModulations  = nullptr;
  sa_tyr_osc<T>     MOscillator1  = {};
  sa_tyr_osc<T>     MOscillator2  = {};
  sa_tyr_res<T>     MResonator1   = {};
  sa_tyr_res<T>     MResonator2   = {};
  MIP_Envelope<T>   MAmpEnvelope  = {};
  MIP_SvfFilter     MFilter       = {};

  int32_t           note_key      = -1;
  T                 note_onvel    = 0.0;
  T                 note_offvel   = 0.0;
  T                 note_vol      = 0.0;
  T                 note_pan      = 0.0;
  T                 note_tuning   = 0.0;
  T                 note_vibr     = 0.0;
  T                 note_expr     = 0.0;
  T                 note_bright   = 0.0;
  T                 note_press    = 0.0;
  T                 hz            = 0.0;  // note hz

  float             O1            = 0.0;
  float             O2            = 0.0;
  float             R1            = 0.0;
  float             R2            = 0.0;


//------------------------------
public:
//------------------------------

  sa_tyr_voice() {
  }

  //----------

  ~sa_tyr_voice() {
    if (MParameters) free(MParameters);
    if (MModulations) free(MModulations);
  }

//------------------------------
public:
//------------------------------

  void setup(uint32_t AIndex, MIP_VoiceContext* AContext) {
    MVoiceIndex = AIndex;
    MContext = AContext;

    #ifdef MIP_VOICE_ADD_TO_BUFFER
      MVoiceBuffer = AContext->voicebuffer;
    #else
      MVoiceBuffer = AContext->voicebuffer + (AIndex * MIP_VOICE_MAX_FRAMESIZE);
    #endif

    MOscillator1.setSampleRate(MContext->samplerate);
    MOscillator2.setSampleRate(MContext->samplerate);
    MResonator1.setSampleRate(MContext->samplerate);
    MResonator2.setSampleRate(MContext->samplerate);
    MAmpEnvelope.setSampleRate(MContext->samplerate);
    uint32_t num = MContext->parameters->size();
    MParameters  = (T*)malloc(num * sizeof(T));
    MModulations = (T*)malloc(num * sizeof(T));
    memset(MParameters,0,num * sizeof(T));
    memset(MModulations,0,num * sizeof(T));
  }

  //----------

  T getEnvLevel() {
    return MAmpEnvelope.getValue();
  }

//------------------------------
public:
//------------------------------

  //todo: reset modulations

  uint32_t note_on(int32_t key, T velocity) {
    //MIP_Print("%i velocity %.3f\n",MIndex,velocity);
    note_key      = key;
    note_onvel    = velocity;
    note_press    = 0.0;
    note_bright   = 0.0;
    note_tuning   = 0.0;
    MOscillator1.restart(true,false);
    MOscillator2.restart(true,false);
    MResonator1.restart();
    MResonator2.restart();
    MAmpEnvelope.noteOn();
    O1 = 0.0;
    O2 = 0.0;
    R1 = 0.0;
    R2 = 0.0;
    return MIP_VOICE_PLAYING;
  }

  //----------

  uint32_t note_off(T velocity) {
    //MIP_Print("%i velocity %.3f\n",MIndex,velocity);
    note_offvel = velocity;
    MAmpEnvelope.noteOff();
    return MIP_VOICE_RELEASED;
  }

  //----------

  void note_choke() {
    //MIP_Print("%i\n",MIndex);
  }

  //----------

  void volume(T value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    note_vol = value;
  }

  //----------

  void pan(T value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    note_pan = value;
  }

  //----------

  void tuning(T value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    note_tuning = value;

    //t = 0.0;
    //dt = 0.0;
    //hz = MIP_NoteToHz(note_key + note_tuning);
    //if (hz > 0.0) {
    //  dt = 1.0 / MIP_HzToSamples(hz,MContext->samplerate);
    //}

  }

  //----------

  void vibrato(T value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    note_vibr = value;
  }

  //----------

  void expression(T value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    note_expr = value;
  }

  //----------

  void brightness(T value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    note_bright = (value * 2.0) - 1.0;
  }

  //----------

  void pressure(T value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    note_press = value;
  }

  //----------

  void parameter(uint32_t index, T value) {
    //MIP_Print("index %i value %f\n",index,value);
    MParameters[index] = value;
    //MIP_Print("---\n");
    switch(index) {
      case PAR_ENV1_ATT:      MAmpEnvelope.setAttack(value*5);      break;
      case PAR_ENV1_DEC:      MAmpEnvelope.setDecay(value*5);       break;
      case PAR_ENV1_SUS:      MAmpEnvelope.setSustain(value*value); break;
      case PAR_ENV1_REL:      MAmpEnvelope.setRelease(value*5);     break;
      case PAR_OSC1_TYPE:     MOscillator1.setType(value);          break;
      case PAR_OSC1_PM_TYPE:  MOscillator1.setPhaseModType(value);  break;
      case PAR_OSC1_WM_TYPE:  MOscillator1.setWaveModType(value);   break;
      case PAR_OSC2_TYPE:     MOscillator2.setType(value);          break;
      case PAR_OSC2_PM_TYPE:  MOscillator2.setPhaseModType(value);  break;
      case PAR_OSC2_WM_TYPE:  MOscillator2.setWaveModType(value);   break;
      case PAR_FLT1_TYPE:     MFilter.setMode(value);               break;
    }
  }

  //----------

  void modulation(uint32_t index, T value) {
    //MIP_Print("%i index %i value %.3f\n",MIndex,index,value);
    MModulations[index] = value;
  }

//------------------------------
public:
//------------------------------

  uint32_t process(uint32_t AState, uint32_t ALength, uint32_t AOffset) {
    MIP_Assert(note_key >= 0);

    float* input0 = MContext->process->audio_inputs->data32[0];
    float* input1 = MContext->process->audio_inputs->data32[1];

    float* output = MVoiceBuffer + AOffset;

    // output buffer
//    float* output = MContext->voicebuffer;
//    output += (MVoiceIndex * MIP_VOICE_MAX_FRAMESIZE);
//    output += AOffset;




//    output += (MVoiceIndex * MIP_VOICE_MAX_FRAMESIZE);

    //-----

    MIP_Assert(output);

    //--------------------
    /*
      shouldn't therse be per sample too?
      or better, per 'tick'..
    */
    //--------------------

    // osc 1

    T o1_pitch  = (((int)MParameters[PAR_OSC1_OCT]  + (int)MModulations[PAR_OSC1_OCT]  ) * 12.0)
                + (((int)MParameters[PAR_OSC1_SEMI] + (int)MModulations[PAR_OSC1_SEMI] ) * 1.0)
                +  (MParameters[PAR_OSC1_CENT] + MModulations[PAR_OSC1_CENT] );
    T osc1_hz = MIP_NoteToHz(note_key + note_tuning + o1_pitch);
    osc1_hz = MIP_Clamp(osc1_hz,20,20000);

    T o1_shape      = MIP_Clamp( MParameters[PAR_OSC1_SHAPE]      + MModulations[PAR_OSC1_SHAPE],     0, 1);
    T o1_width      = MIP_Clamp( MParameters[PAR_OSC1_WIDTH]      + MModulations[PAR_OSC1_WIDTH],     0, 1);
    T o1_wave_mod   = MIP_Clamp( MParameters[PAR_OSC1_WM_AMOUNT]  + MModulations[PAR_OSC1_WM_AMOUNT], 0, 1);
    T o1_phase_mod  = MIP_Clamp( MParameters[PAR_OSC1_PM_AMOUNT]  + MModulations[PAR_OSC1_PM_AMOUNT], 0, 1);
    MOscillator1.setFrequency(osc1_hz);
    MOscillator1.setShape(o1_shape);
    MOscillator1.setWidth(o1_width);
    MOscillator1.setPhaseModAmount(o1_phase_mod);
    MOscillator1.setWaveModAmount(o1_wave_mod);

    // osc 2

    T o2_pitch  = (((int)MParameters[PAR_OSC2_OCT]  + (int)MModulations[PAR_OSC2_OCT]  ) * 12.0)
                + (((int)MParameters[PAR_OSC2_SEMI] + (int)MModulations[PAR_OSC2_SEMI] ) * 1.0)
                +  (MParameters[PAR_OSC2_CENT] + MModulations[PAR_OSC2_CENT] );
    T osc2_hz = MIP_NoteToHz(note_key + note_tuning + o2_pitch);
    osc2_hz = MIP_Clamp(osc2_hz,20,20000);

    T o2_shape      = MIP_Clamp( MParameters[PAR_OSC2_SHAPE]     + MModulations[PAR_OSC2_SHAPE],     0, 1);
    T o2_width      = MIP_Clamp( MParameters[PAR_OSC2_WIDTH]     + MModulations[PAR_OSC2_WIDTH],     0, 1);
    T o2_wave_mod   = MIP_Clamp( MParameters[PAR_OSC2_WM_AMOUNT] + MModulations[PAR_OSC2_WM_AMOUNT], 0, 1);
    T o2_phase_mod  = MIP_Clamp( MParameters[PAR_OSC2_PM_AMOUNT] + MModulations[PAR_OSC2_PM_AMOUNT], 0, 1);
    MOscillator2.setFrequency(osc2_hz);
    MOscillator2.setShape(o2_shape);
    MOscillator2.setWidth(o2_width);
    MOscillator2.setWaveModAmount(o2_wave_mod);
    MOscillator2.setPhaseModAmount(o2_phase_mod);

    // res 1

    T r1_pitch  = (((int)MParameters[PAR_RES1_OCT]  + (int)MModulations[PAR_RES1_OCT]  ) * 12.0)
                + (((int)MParameters[PAR_RES1_SEMI] + (int)MModulations[PAR_RES1_SEMI] ) * 1.0)
                +  (MParameters[PAR_RES1_CENT] + MModulations[PAR_RES1_CENT] );
    T res1_hz = MIP_NoteToHz(note_key + note_tuning + r1_pitch);
    res1_hz = MIP_Clamp(res1_hz,20,20000);
    MResonator1.setHz(res1_hz);

    T r1_shape  = MIP_Clamp( MParameters[PAR_RES1_SHAPE]     + MModulations[PAR_RES1_SHAPE],  0, 1);
    T r1_fb     = MIP_Clamp( MParameters[PAR_RES1_FB]        + MModulations[PAR_RES1_FB],     0, 1);
    T r1_damp   = MIP_Clamp( MParameters[PAR_RES1_DAMP]      + MModulations[PAR_RES1_DAMP],   0, 1);
    T r1_rough  = MIP_Clamp( MParameters[PAR_RES1_ROUGH]     + MModulations[PAR_RES1_ROUGH],  0, 1);
    MResonator1.setShape(r1_shape);
    MResonator1.setFeedback(r1_fb);
    MResonator1.setDamp(r1_damp);
    MResonator1.setRough(r1_rough);

    MResonator1.setMode( MParameters[PAR_RES1_TYPE] );
    T r1_spd = 1.0 - MParameters[PAR_RES1_SPEED];
    r1_spd = (r1_spd * r1_spd * r1_spd) * 10000;
    MResonator1.setSpeed( r1_spd );

    // res 2

    T r2_pitch  = (((int)MParameters[PAR_RES2_OCT]  + (int)MModulations[PAR_RES2_OCT]  ) * 12.0)
                + (((int)MParameters[PAR_RES2_SEMI] + (int)MModulations[PAR_RES2_SEMI] ) * 1.0)
                +  (MParameters[PAR_RES2_CENT] + MModulations[PAR_RES2_CENT] );
    T res2_hz = MIP_NoteToHz(note_key + note_tuning + r2_pitch);
    res2_hz = MIP_Clamp(res2_hz,20,20000);
    MResonator2.setHz(res2_hz);

    T r2_shape  = MIP_Clamp( MParameters[PAR_RES2_SHAPE]     + MModulations[PAR_RES2_SHAPE],  0, 1);
    T r2_fb     = MIP_Clamp( MParameters[PAR_RES2_FB]        + MModulations[PAR_RES2_FB],     0, 1);
    T r2_damp   = MIP_Clamp( MParameters[PAR_RES2_DAMP]      + MModulations[PAR_RES2_DAMP],   0, 1);
    T r2_rough  = MIP_Clamp( MParameters[PAR_RES2_ROUGH]     + MModulations[PAR_RES2_ROUGH],  0, 1);
    MResonator2.setShape(r2_shape);
    MResonator2.setFeedback(r2_fb);
    MResonator2.setDamp(r2_damp);
    MResonator2.setRough(r2_rough);

    MResonator2.setMode( MParameters[PAR_RES2_TYPE] );
    T r2_spd = 1.0 - MParameters[PAR_RES2_SPEED];
    r2_spd = (r2_spd * r2_spd * r2_spd) * 10000;
    MResonator2.setSpeed( r2_spd );

    // master

    T o1_out = MIP_Clamp( MParameters[PAR_MASTER_OSC1_OUT] + MModulations[PAR_MASTER_OSC1_OUT],     0, 1);
    T o2_out = MIP_Clamp( MParameters[PAR_MASTER_OSC2_OUT] + MModulations[PAR_MASTER_OSC2_OUT],     0, 1);
    T r1_out = MIP_Clamp( MParameters[PAR_MASTER_RES1_OUT] + MModulations[PAR_MASTER_RES1_OUT],     0, 1);
    T r2_out = MIP_Clamp( MParameters[PAR_MASTER_RES2_OUT] + MModulations[PAR_MASTER_RES2_OUT],     0, 1);

    //--------------------
    // per sample
    //--------------------

    for (uint32_t i=0; i<ALength; i++) {

      float in0 = *input0++;
      float in1 = *input1++;
      float input  = (in0 + in1) * 0.5;

      // osc 1

      T rnd = MIP_RandomSigned();
      T o1_in = O1    * MIP_Clamp( MParameters[PAR_OSC1_IN_O1] + MModulations[PAR_OSC1_IN_O1], 0, 1)
              + O2    * MIP_Clamp( MParameters[PAR_OSC1_IN_O2] + MModulations[PAR_OSC1_IN_O2], 0, 1)
              + R1    * MIP_Clamp( MParameters[PAR_OSC1_IN_R1] + MModulations[PAR_OSC1_IN_R1], 0, 1)
              + R2    * MIP_Clamp( MParameters[PAR_OSC1_IN_R2] + MModulations[PAR_OSC1_IN_R2], 0, 1)
              + rnd   * MIP_Clamp( MParameters[PAR_OSC1_IN_N]  + MModulations[PAR_OSC1_IN_N],  0, 1)
              + input * MIP_Clamp( MParameters[PAR_OSC1_IN_A]  + MModulations[PAR_OSC1_IN_A],  0, 1);
      O1 = MOscillator1.process(o1_in);

      // osc 2

      rnd = MIP_RandomSigned();
      T o2_in = O1    * MIP_Clamp( MParameters[PAR_OSC2_IN_O1] + MModulations[PAR_OSC2_IN_O1], 0, 1)
              + O2    * MIP_Clamp( MParameters[PAR_OSC2_IN_O2] + MModulations[PAR_OSC2_IN_O2], 0, 1)
              + R1    * MIP_Clamp( MParameters[PAR_OSC2_IN_R1] + MModulations[PAR_OSC2_IN_R1], 0, 1)
              + R2    * MIP_Clamp( MParameters[PAR_OSC2_IN_R2] + MModulations[PAR_OSC2_IN_R2], 0, 1)
              + rnd   * MIP_Clamp( MParameters[PAR_OSC2_IN_N]  + MModulations[PAR_OSC2_IN_N],  0, 1)
              + input * MIP_Clamp( MParameters[PAR_OSC2_IN_A]  + MModulations[PAR_OSC2_IN_A],  0, 1);
      O2 = MOscillator2.process(o2_in);

      // res 1

      rnd = MIP_RandomSigned();
      T r1_in = O1    * MIP_Clamp( MParameters[PAR_RES1_IN_O1] + MModulations[PAR_RES1_IN_O1], 0, 1)
              + O2    * MIP_Clamp( MParameters[PAR_RES1_IN_O2] + MModulations[PAR_RES1_IN_O2], 0, 1)
              + R1    * MIP_Clamp( MParameters[PAR_RES1_IN_R1] + MModulations[PAR_RES1_IN_R1], 0, 1)
              + R2    * MIP_Clamp( MParameters[PAR_RES1_IN_R2] + MModulations[PAR_RES1_IN_R2], 0, 1)
              + rnd   * MIP_Clamp( MParameters[PAR_RES1_IN_N]  + MModulations[PAR_RES1_IN_N],  0, 1)
              + input * MIP_Clamp( MParameters[PAR_RES1_IN_A]  + MModulations[PAR_RES1_IN_A],  0, 1);
      R1 = MResonator1.process(r1_in);

      // res 2

      rnd = MIP_RandomSigned();
      T r2_in = O1    * MIP_Clamp( MParameters[PAR_RES2_IN_O1] + MModulations[PAR_RES2_IN_O1], 0, 1)
              + O2    * MIP_Clamp( MParameters[PAR_RES2_IN_O2] + MModulations[PAR_RES2_IN_O2], 0, 1)
              + R1    * MIP_Clamp( MParameters[PAR_RES2_IN_R1] + MModulations[PAR_RES2_IN_R1], 0, 1)
              + R2    * MIP_Clamp( MParameters[PAR_RES2_IN_R2] + MModulations[PAR_RES2_IN_R2], 0, 1)
              + rnd   * MIP_Clamp( MParameters[PAR_RES2_IN_N]  + MModulations[PAR_RES2_IN_N],  0, 1)
              + input * MIP_Clamp( MParameters[PAR_RES2_IN_A]  + MModulations[PAR_RES2_IN_A],  0, 1);
      R2 = MResonator2.process(r2_in);

      // master

      T out = (O1 * o1_out)
            + (O2 * o2_out)
            + (R1 * r1_out)
            + (R2 * r2_out);

      // filter

      T fr = MParameters[PAR_FLT1_FREQ] + MModulations[PAR_FLT1_FREQ];
      T bw = MParameters[PAR_FLT1_RES]  + MModulations[PAR_FLT1_RES];
      MFilter.setFreq(fr * fr);
      MFilter.setBW(1.0 - bw);
      out = MFilter.process(out);

      // env, velocity, press..

      T amp = note_onvel + note_press;
      amp = MIP_Clamp(amp,0,1);
      amp *= MAmpEnvelope.process();
      out *= amp;

      // out

      #ifdef MIP_VOICE_ADD_TO_BUFFER
        *output++ += out;
      #else
        *output++ = out;
      #endif
    }

    // postproc

    uint32_t stage = MAmpEnvelope.getStage();
    if (stage == MIP_ENVELOPE_FINISHED) return MIP_VOICE_FINISHED;
    else return AState;
  }

};

//----------------------------------------------------------------------
#endif
