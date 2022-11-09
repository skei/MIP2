#ifndef sa_tyr_voice_incuded
#define sa_tyr_voice_incuded
//----------------------------------------------------------------------

#include "mip.h"
#include "audio/mip_audio_math.h"
#include "audio/filters/mip_allpass_filter.h"
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

#define PARAM_SMOOTH_FACTOR   (1.0 / 200.0)
#define MOD_SMOOTH_FACTOR     (1.0 / 200.0)
#define EXPR_SMOOTH_FACTOR    (1.0 / 500.0)
#define PARMOD_SMOOTH_FACTOR  (1.0 / 200.0)

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

  float*            MVoiceBuffer    = nullptr;

  T*                MParameters     = nullptr;
  T*                MModulations    = nullptr;
  T*                MParMod         = nullptr;
  T*                MParModTargets  = nullptr;
  T*                MParModFactors  = nullptr;

  sa_tyr_osc<T>     MOscillator1    = {};
  sa_tyr_osc<T>     MOscillator2    = {};
  sa_tyr_res<T>     MResonator1     = {};
  sa_tyr_res<T>     MResonator2     = {};
  MIP_Envelope<T>   MAmpEnvelope    = {};
  MIP_SvfFilter     MFilter         = {};

  MIP_RcFilter<T>   MO1InputFilter  = {};
  MIP_RcFilter<T>   MO2InputFilter  = {};
  MIP_RcFilter<T>   MR1InputFilter  = {};
  MIP_RcFilter<T>   MR2InputFilter  = {};

  int32_t           note_key        = -1;
  T                 note_onvel      = 0.0;
  T                 note_offvel     = 0.0;

  // note expressions

  T                 note_vol        = 0.0;
  T                 note_pan        = 0.0;
  T                 note_tuning     = 0.0;
  T                 note_vibr       = 0.0;
  T                 note_expr       = 0.0;
  T                 note_bright     = 0.0;
  T                 note_press      = 0.0;

  T                 note_vol_tgt    = 0.0;
  T                 note_pan_tgt    = 0.0;
  T                 note_tuning_tgt = 0.0;
  T                 note_vibr_tgt   = 0.0;
  T                 note_expr_tgt   = 0.0;
  T                 note_bright_tgt = 0.0;
  T                 note_press_tgt  = 0.0;

  // processing

  float             O1              = 0.0;
  float             O2              = 0.0;
  float             R1              = 0.0;
  float             R2              = 0.0;

//------------------------------
public:
//------------------------------

  sa_tyr_voice() {
  }

  //----------

  ~sa_tyr_voice() {
    if (MParameters)        free(MParameters);
    if (MModulations)       free(MModulations);
    if (MParMod)            free(MParMod);
    if (MParModTargets)     free(MParModTargets);
    if (MParModFactors)     free(MParModFactors);
  }

//------------------------------
public:
//------------------------------

  void setup(uint32_t AIndex, MIP_VoiceContext* AContext) {
    MVoiceIndex = AIndex;
    MContext = AContext;

    // define in voice_manager
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

    MO1InputFilter.setSampleRate(MContext->samplerate);
    MO2InputFilter.setSampleRate(MContext->samplerate);
    MR1InputFilter.setSampleRate(MContext->samplerate);
    MR2InputFilter.setSampleRate(MContext->samplerate);

    uint32_t num = MContext->parameters->size();

    MParameters         = (T*)malloc(num * sizeof(T));
    MModulations        = (T*)malloc(num * sizeof(T));
    MParMod             = (T*)malloc(num * sizeof(T));
    MParModTargets      = (T*)malloc(num * sizeof(T));
    MParModFactors      = (T*)malloc(num * sizeof(T));

    for (uint32_t i=0; i<num; i++) {
      MParameters[i]        = 0.0;
      MModulations[i]       = 0.0;
      MParMod[i]            = 0.0;
      MParModTargets[i]     = 0.0;
      MParModFactors[i]     = PARMOD_SMOOTH_FACTOR;
    }

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

    note_key          = key;
    note_onvel        = velocity;

    note_press        = 0.0;
    note_bright       = 0.0;
    note_tuning       = 0.0;
    note_vol          = 0.0;
    note_pan          = 0.0;
    note_vibr         = 0.0;
    note_expr         = 0.0;

    note_press_tgt    = 0.0;
    note_bright_tgt   = 0.0;
    note_tuning_tgt   = 0.0;
    note_vol_tgt      = 0.0;
    note_pan_tgt      = 0.0;
    note_vibr_tgt     = 0.0;
    note_expr_tgt     = 0.0;

    MOscillator1.restart(true,false); // reset phase, random phase
    MOscillator2.restart(true,false);
    MResonator1.restart();
    MResonator2.restart();
    MAmpEnvelope.noteOn();
    MFilter.reset();

    O1 = 0.0;
    O2 = 0.0;
    R1 = 0.0;
    R2 = 0.0;

    for (uint32_t i=0; i<PARAM_COUNT; i++) {
      MModulations[i] = 0.0;
      MParMod[i] = MParameters[i];
      MParModTargets[i] = MParameters[i];
    }

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
    //note_vol = value;
    note_vol_tgt = value;
  }

  //----------

  void pan(T value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    //note_pan = value;
    note_pan_tgt = value;
  }

  //----------

  void tuning(T value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    //note_tuning = value;
    note_tuning_tgt = value;
  }

  //----------

  void vibrato(T value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    //note_vibr = value;
    note_vibr_tgt = value;
  }

  //----------

  void expression(T value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    //note_expr = value;
    note_expr_tgt = value;
  }

  //----------

  void brightness(T value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    //note_bright = (value * 2.0) - 1.0;
    note_bright_tgt = (value * 2.0) - 1.0;
  }

  //----------

  void pressure(T value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    //note_press = value;
    note_press_tgt = value;
  }

  //----------

  void parameter(uint32_t index, T value) {
    //MIP_Print("index %i value %f\n",index,value);

    MParameters[index] = value;
    //MParameterTargets[index] = value;

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
    //MModulationTargets[index] = value;

  }

//------------------------------
public:
//------------------------------

  uint32_t process(uint32_t AState, uint32_t ALength, uint32_t AOffset) {

    MIP_Assert(note_key >= 0);

    float* input0 = MContext->process->audio_inputs->data32[0];
    float* input1 = MContext->process->audio_inputs->data32[1];
    float* output = MVoiceBuffer + AOffset;

    //output += (MVoiceIndex * MIP_VOICE_MAX_FRAMESIZE);
    //MIP_Assert(output);

    //------------------------------
    // per sample
    //------------------------------

    for (uint32_t i=0; i<ALength; i++) {

      //------------------------------
      // prepare
      //------------------------------

      // smooth parameters / modulations
      // not all parameters are 0..1 !!!

      for (uint32_t j=0; j<PARAM_COUNT; j++) {
        //MParModTargets[j] = MIP_Clamp( (MParameters[j] + MModulations[j]), 0, 1);
        MParModTargets[j] = MParameters[j] + MModulations[j];
        T diff = MParModTargets[j] - MParMod[j];
        if (diff < 0.001) MParMod[j] = MParModTargets[j];
        else MParMod[j] += (diff  * MParModFactors[j]);
      }

      // smooth note expression values

      note_vol    += (note_vol_tgt    - note_vol)    * EXPR_SMOOTH_FACTOR;
      note_pan    += (note_pan_tgt    - note_pan)    * EXPR_SMOOTH_FACTOR;
      note_tuning += (note_tuning_tgt - note_tuning) * EXPR_SMOOTH_FACTOR;
      note_vibr   += (note_vibr_tgt   - note_vibr)   * EXPR_SMOOTH_FACTOR;
      note_expr   += (note_expr_tgt   - note_expr)   * EXPR_SMOOTH_FACTOR;
      note_bright += (note_bright_tgt - note_bright) * EXPR_SMOOTH_FACTOR;
      note_press  += (note_press_tgt  - note_press)  * EXPR_SMOOTH_FACTOR;

      // prepare osc 1

      T o1_pitch  = ((int)MParMod[PAR_OSC1_OCT]  * 12.0)
                  + ((int)MParMod[PAR_OSC1_SEMI] * 1.0 )
                  + (     MParMod[PAR_OSC1_CENT]       );

      T osc1_hz = MIP_NoteToHz(note_key + note_tuning + o1_pitch);
      osc1_hz = MIP_Clamp(osc1_hz,20,20000);
      MOscillator1.setFrequency(osc1_hz);

      T o1_shape      = MIP_Clamp( MParMod[PAR_OSC1_SHAPE], 0, 1 );
      T o1_width      = MIP_Clamp( MParMod[PAR_OSC1_WIDTH], 0, 1 );
      T o1_phase_mod  = MIP_Clamp( MParMod[PAR_OSC1_PM_AMOUNT], 0, 1 );
      T o1_wave_mod   = MIP_Clamp( MParMod[PAR_OSC1_WM_AMOUNT], 0, 1 );

      MOscillator1.setShape(o1_shape);
      MOscillator1.setWidth(o1_width);
      MOscillator1.setPhaseModAmount(o1_phase_mod);
      MOscillator1.setWaveModAmount(o1_wave_mod);

      uint32_t o1_mode = MParMod[PAR_OSC1_TYPE];
      o1_mode = MIP_Clamp( o1_mode, 0, SA_TYR_OSC_TYPE_COUNT - 1);
      MOscillator1.setType(o1_mode);

      T o1s = MIP_Clamp( MParMod[PAR_OSC1_IN_S], 0, 1 );
      o1s   = 1.0 - o1s;
      o1s   = (o1s * o1s * o1s * o1s);
      MO1InputFilter.setWeight(o1s);

      // prepare osc 2

      T o2_pitch  = ((int)MParMod[PAR_OSC2_OCT]  * 12.0)
                  + ((int)MParMod[PAR_OSC2_SEMI] * 1.0 )
                  + (     MParMod[PAR_OSC2_CENT]       );

      T osc2_hz = MIP_NoteToHz(note_key + note_tuning + o2_pitch);
      osc2_hz = MIP_Clamp(osc2_hz,20,20000);
      MOscillator2.setFrequency(osc2_hz);

      T o2_shape      = MIP_Clamp( MParMod[PAR_OSC2_SHAPE], 0, 1 );
      T o2_width      = MIP_Clamp( MParMod[PAR_OSC2_WIDTH], 0, 1 );
      T o2_wave_mod   = MIP_Clamp( MParMod[PAR_OSC2_WM_AMOUNT], 0, 1 );
      T o2_phase_mod  = MIP_Clamp( MParMod[PAR_OSC2_PM_AMOUNT], 0, 1 );

      MOscillator2.setShape(o2_shape);
      MOscillator2.setWidth(o2_width);
      MOscillator2.setWaveModAmount(o2_wave_mod);
      MOscillator2.setPhaseModAmount(o2_phase_mod);

      uint32_t o2_mode = MParMod[PAR_OSC2_TYPE];
      o2_mode = MIP_Clamp( o2_mode, 0, SA_TYR_OSC_TYPE_COUNT - 1);
      MOscillator2.setType(o2_mode);

      T o2s = MIP_Clamp( MParMod[PAR_OSC2_IN_S], 0, 1 );
      o2s = 1.0 - o2s;
      o2s = (o2s * o2s * o2s * o2s);
      MO2InputFilter.setWeight(o2s);

      // prepare res 1

      T r1_pitch  = ((int)MParMod[PAR_RES1_OCT]  * 12.0)
                  + ((int)MParMod[PAR_RES1_SEMI] * 1.0 )
                  + (     MParMod[PAR_RES1_CENT]       );

      T res1_hz = MIP_NoteToHz(note_key + note_tuning + r1_pitch);
      res1_hz = MIP_Clamp(res1_hz,20,20000);
      MResonator1.setHz(res1_hz);

    //T r1_shape  = MIP_Clamp( MParMod[PAR_RES1_SHAPE], 0, 1 );
      T r1_fb     = MIP_Clamp( MParMod[PAR_RES1_FB], 0, 1 );
      T r1_damp   = MIP_Clamp( MParMod[PAR_RES1_DAMP], 0, 1 );
      T r1_rough  = MIP_Clamp( MParMod[PAR_RES1_ROUGH], 0, 1 );

      //MResonator1.setShape(r1_shape);
      MResonator1.setFeedback(r1_fb);
      MResonator1.setDamp(r1_damp);
      MResonator1.setRough(r1_rough);

      T r1s = MIP_Clamp( MParMod[PAR_RES1_IN_S], 0, 1 );
      r1s = 1.0 - r1s;
      r1s = (r1s * r1s * r1s * r1s);
      MR1InputFilter.setWeight(r1s);

      uint32_t r1_mode = MParMod[PAR_RES1_TYPE];
      r1_mode = MIP_Clamp( r1_mode, 0, SA_TYR_RES_TYPE_COUNT - 1);
      MResonator1.setMode(r1_mode);

      T imp1 = MIP_Clamp( MParMod[PAR_RES1_IN_I], 0, 1 );
      imp1 = (imp1 * imp1 * imp1 * imp1 * imp1);
      MResonator1.setImpulse(imp1);

      T r1_spd = 1.0 - MIP_Clamp(MParMod[PAR_RES1_SPEED],0,1);
      r1_spd = (r1_spd * r1_spd * r1_spd) * 10000;
      MResonator1.setSpeed( r1_spd );

      // prepare res 2

      T r2_pitch  = ((int)MParMod[PAR_RES2_OCT]  * 12.0)
                  + ((int)MParMod[PAR_RES2_SEMI] * 1.0 )
                  + (     MParMod[PAR_RES2_CENT]       );

      T res2_hz = MIP_NoteToHz(note_key + note_tuning + r2_pitch);
      res2_hz = MIP_Clamp(res2_hz,20,20000);
      MResonator2.setHz(res2_hz);

      //T r2_shape  = MIP_Clamp( MParMod[PAR_RES2_SHAPE], 0, 1 );
      T r2_fb     = MIP_Clamp( MParMod[PAR_RES2_FB], 0, 1 );
      T r2_damp   = MIP_Clamp( MParMod[PAR_RES2_DAMP], 0, 1 );
      T r2_rough  = MIP_Clamp( MParMod[PAR_RES2_ROUGH], 0, 1 );

      //MResonator2.setShape(r2_shape);
      MResonator2.setFeedback(r2_fb);
      MResonator2.setDamp(r2_damp);
      MResonator2.setRough(r2_rough);

      T r2s = MIP_Clamp( MParMod[PAR_RES2_IN_S], 0, 1 );
      r2s = 1.0 - r2s;
      r2s = (r2s * r2s * r2s * r2s);
      MR2InputFilter.setWeight(r2s);

      uint32_t r2_mode = MParMod[PAR_RES2_TYPE];
      r2_mode = MIP_Clamp( r2_mode, 0, SA_TYR_RES_TYPE_COUNT - 1);
      MResonator2.setMode( r2_mode );

      T imp2 = MIP_Clamp( MParMod[PAR_RES2_IN_I], 0,1 );;
      imp2 = (imp2 * imp2 * imp2 * imp2 * imp2);
      MResonator2.setImpulse(imp2);

      T r2_spd = 1.0 - MIP_Clamp( MParMod[PAR_RES2_SPEED], 0,1 );
      r2_spd = (r2_spd * r2_spd * r2_spd) * 10000;
      MResonator2.setSpeed( r2_spd );

      //------------------------------
      // processing
      //------------------------------

      float in0 = *input0++;
      float in1 = *input1++;
      float input  = (in0 + in1) * 0.5;

      // sum inputs

      // osc 1

      T rnd = MIP_RandomSigned();

      T o1_in = O1    * MIP_Clamp( MParMod[PAR_OSC1_IN_O1], 0, 1 )
              + O2    * MIP_Clamp( MParMod[PAR_OSC1_IN_O2], 0, 1 )
              + R1    * MIP_Clamp( MParMod[PAR_OSC1_IN_R1], 0, 1 )
              + R2    * MIP_Clamp( MParMod[PAR_OSC1_IN_R2], 0, 1 )
              + rnd   * MIP_Clamp( MParMod[PAR_OSC1_IN_N],  0, 1 )
              + input * MIP_Clamp( MParMod[PAR_OSC1_IN_A],  0, 1 );

      o1_in = MO1InputFilter.process(o1_in);
      //O1 = MOscillator1.process(o1_in);

      // osc 2

      rnd = MIP_RandomSigned();

      T o2_in = O1    * MIP_Clamp( MParMod[PAR_OSC2_IN_O1], 0, 1 )
              + O2    * MIP_Clamp( MParMod[PAR_OSC2_IN_O2], 0, 1 )
              + R1    * MIP_Clamp( MParMod[PAR_OSC2_IN_R1], 0, 1 )
              + R2    * MIP_Clamp( MParMod[PAR_OSC2_IN_R2], 0, 1 )
              + rnd   * MIP_Clamp( MParMod[PAR_OSC2_IN_N],  0, 1 )
              + input * MIP_Clamp( MParMod[PAR_OSC2_IN_A],  0, 1 );

      o2_in = MO2InputFilter.process(o2_in);
      //O2 = MOscillator2.process(o2_in);

      // res 1

      rnd = MIP_RandomSigned();

      T r1_in = O1    * MIP_Clamp( MParMod[PAR_RES1_IN_O1], 0, 1 )
              + O2    * MIP_Clamp( MParMod[PAR_RES1_IN_O2], 0, 1 )
              + R1    * MIP_Clamp( MParMod[PAR_RES1_IN_R1], 0, 1 )
              + R2    * MIP_Clamp( MParMod[PAR_RES1_IN_R2], 0, 1 )
              + rnd   * MIP_Clamp( MParMod[PAR_RES1_IN_N],  0, 1 )
              + input * MIP_Clamp( MParMod[PAR_RES1_IN_A],  0, 1 );

      r1_in = MR1InputFilter.process(r1_in);
      //R1 = MResonator1.process(r1_in);

      // res 2

      rnd = MIP_RandomSigned();

      T r2_in = O1    * MIP_Clamp( MParMod[PAR_RES2_IN_O1], 0, 1 )
              + O2    * MIP_Clamp( MParMod[PAR_RES2_IN_O2], 0, 1 )
              + R1    * MIP_Clamp( MParMod[PAR_RES2_IN_R1], 0, 1 )
              + R2    * MIP_Clamp( MParMod[PAR_RES2_IN_R2], 0, 1 )
              + rnd   * MIP_Clamp( MParMod[PAR_RES2_IN_N],  0, 1 )
              + input * MIP_Clamp( MParMod[PAR_RES2_IN_A],  0, 1 );

      r2_in = MR2InputFilter.process(r2_in);
      //R2 = MResonator2.process(r2_in);

      // process

      O1 = MOscillator1.process(o1_in);
      O2 = MOscillator2.process(o2_in);
      R1 = MResonator1.process(r1_in);
      R2 = MResonator2.process(r2_in);

      T out = (O1 * MIP_Clamp( MParMod[PAR_MASTER_OSC1_OUT], 0, 1 ))
            + (O2 * MIP_Clamp( MParMod[PAR_MASTER_OSC2_OUT], 0, 1 ))
            + (R1 * MIP_Clamp( MParMod[PAR_MASTER_RES1_OUT], 0, 1 ))
            + (R2 * MIP_Clamp( MParMod[PAR_MASTER_RES2_OUT], 0, 1 ));

      //------------------------------
      // post
      //------------------------------

      // filter

      T fr = MIP_Clamp( MParMod[PAR_FLT1_FREQ], 0, 1 );
      T bw = MIP_Clamp( MParMod[PAR_FLT1_RES],  0, 1 );

      MFilter.setBW(1.0 - bw);
      MFilter.setFreq(fr * fr);
      out = MFilter.process(out);

      // env, velocity, press..

      T amp = note_onvel + note_press;
      amp = MIP_Clamp( amp, 0, 1 );

      amp *= MAmpEnvelope.process();
      out *= amp;

      // out

      // define in voice_manager
      #ifdef MIP_VOICE_ADD_TO_BUFFER
        *output++ += out;
      #else
        *output++ = out;
      #endif

      // prepare for next sample

      //

    }

    //------------------------------
    //
    //------------------------------

    uint32_t stage = MAmpEnvelope.getStage();
    if (stage == MIP_ENVELOPE_FINISHED) return MIP_VOICE_FINISHED;
    else return AState;

  }

};

//----------------------------------------------------------------------
#endif
