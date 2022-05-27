#ifndef test_synth1_voice_included
#define test_synth1_voice_included
//----------------------------------------------------------------------

#include "mip.h"
#include "audio/mip_audio_math.h"
#include "audio/filters/mip_rc_filter.h"
#include "audio/filters/mip_svf_filter.h"
#include "audio/processing/mip_delay.h"
#include "audio/synthesis/mip_oscillator2.h"
#include "audio/modulation/mip_envelope.h"
#include "audio/waveforms/mip_polyblep_waveform.h"

#include "plugin/mip_voice_manager.h"

//----------------------------------------------------------------------

// todo: redo the smoothing things..

#define SMOOTHER_FACTOR (1.0 / 250.0)

struct myDelayFx {
  MIP_RcFilter<float> flt = {};
  float process(float x) {
    return flt.process(x);
  }
};

typedef MIP_InterpolatedDelay<4096,myDelayFx> myDelay;


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myVoice {

//------------------------------
private:
//------------------------------

  MIP_VoiceContext*   context           = nullptr;

  MIP_Oscillator2     osc1              = {};
  MIP_SvfFilter       flt1              = {};
  myDelay             res1              = {};
  MIP_Envelope        amp_env           = {};

  MIP_RcFilter<float> flt1_freq_smoother = {};
  MIP_RcFilter<float> flt1_res_smoother  = {};
  MIP_RcFilter<float> vol_smoother      = {};

  int32_t             note_key          = -1;
  float               note_onvel        = 0.0;
  float               note_offvel       = 0.0;
  float               note_vol          = 0.0;
  float               note_pan          = 0.0;
  float               note_tuning       = 0.0;
  float               note_vibr         = 0.0;
  float               note_expr         = 0.0;
  float               note_bright       = 0.0;
  float               note_press        = 0.0;
  float               hz                = 0.0;  // note hz

  float               p_osc1_pulse        = 0.0;
  float               p_osc1_width        = 0.0;
  float               p_osc1_tri          = 0.0;
  float               p_osc1_sin          = 0.0;
//  float               p_osc1_noise        = 0.0;
  float               p_osc1_cent         = 0.5;
  float               p_flt1_freq         = 0.5;
  float               p_flt1_res          = 0.5;
  float               p_res1_noise        = 0.0;
  float               p_res1_nshape       = 0.0;
  float               p_res1_fb           = 0.5;
  float               p_res1_damp         = 0.0;
  float               p_res1_semi         = 0.5;

  float               p_osc1_out          = 0.0;
  float               p_res1_out          = 0.0;

  float               osc1_pulse_mod     = 0.0;
  float               osc1_width_mod     = 0.0;
  float               osc1_tri_mod       = 0.0;
  float               osc1_sin_mod       = 0.0;
  float               flt1_freq_mod      = 0.0;
  float               flt1_res_mod       = 0.0;

  float               pitch_mod         = 0.0;

  //float               fb_mod            = 0.5;
  //float               fb_tune_mod       = 0.5;
  //float               fb_damp_mod       = 0.0;


//------------------------------
public:
//------------------------------

  float getEnvLevel() {
    return amp_env.getValue();
  }

  //----------

  void prepare(MIP_VoiceContext* AContext) {
    context = AContext;
    osc1.setSampleRate(context->samplerate);
    amp_env.setSampleRate(context->samplerate);
    flt1_freq_smoother.setup(p_flt1_freq,p_flt1_freq, SMOOTHER_FACTOR);
    flt1_res_smoother.setup(p_flt1_res,p_flt1_res, SMOOTHER_FACTOR);
    vol_smoother.setup(0,0,SMOOTHER_FACTOR);
  }

  //----------

  uint32_t note_on(int32_t key, float velocity) {
    note_key      = key;
    note_onvel    = velocity;
    note_press    = 0.0;
    note_bright   = 0.0;
    note_tuning   = 0.0;
    osc1_pulse_mod = 0.0;
    osc1_width_mod = 0.0;
    osc1_tri_mod   = 0.0;
    osc1_sin_mod   = 0.0;
    osc1.setPhase(0);
    //res1.getFeedbackFX()->flt.setMode(MIP_SVF_LP);
    //res1.getFeedbackFX()->flt.setFreq(fb_damp);
    //res1.getFeedbackFX()->flt.setBW(1);
    res1.clear();
    res1.start();
    flt1_freq_mod = 0.0;
    flt1_res_mod  = 0.0;
    pitch_mod     = 0.0;
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

  void volume(float amount) {
    note_vol = amount;
  }

  //----------

  void pan(float amount) {
    note_pan = amount;
  }

  //----------

  void tuning(float amount) {
    note_tuning = amount;
  }

  //----------

  void vibrato(float amount) {
    note_vibr = amount;
  }

  //----------

  void expression(float amount) {
    note_expr = amount;
  }

  //----------

  void brightness(float amount) {
    note_bright = (amount * 2.0) - 1.0;
  }

  //----------

  void pressure(float amount) {
    note_press = amount;
  }

  //----------

  void parameter(uint32_t index, float value) {
    //MIP_Print("%i = %.3f\n",index,value);
    switch (index) {
      case PAR_OSC1_OUT:    p_osc1_out = value;               break;
      case PAR_RES1_OUT:    p_res1_out = value;               break;
      case PAR_OSC1_PULSE:  p_osc1_pulse = value;             break;
      case PAR_OSC1_WIDTH:  p_osc1_width = value;             break;
      case PAR_OSC1_TRI:    p_osc1_tri = value;               break;
      case PAR_OSC1_SIN:    p_osc1_sin = value;               break;
//      case PAR_OSC1_NOISE:  p_osc1_noise = value;             break;
      case PAR_OSC1_CENT:   p_osc1_cent = value;              break;
      case PAR_RES1_NOISE:  p_res1_noise = value;             break;
      case PAR_RES1_NSHAPE: p_res1_nshape = value;            break;
      case PAR_RES1_FB:     p_res1_fb = value;                break;
      case PAR_RES1_DAMP:   p_res1_damp = value;              break;
      case PAR_RES1_SEMI:   p_res1_semi = value;              break;
      case PAR_FLT1_FREQ:   p_flt1_freq = value;              break;
      case PAR_FLT1_RES:    p_flt1_res = value;               break;
      case PAR_ENV1_ATT:    amp_env.setAttack(value*5);       break;
      case PAR_ENV1_DEC:    amp_env.setDecay(value*5);        break;
      case PAR_ENV1_SUS:    amp_env.setSustain(value*value);  break;
      case PAR_ENV1_REL:    amp_env.setRelease(value*5);      break;
    }
  }

  void modulation(uint32_t index, float value) {
    switch (index) {
      case PAR_OSC1_PULSE:  osc1_pulse_mod = value;   break;
      case PAR_OSC1_WIDTH:  osc1_width_mod = value;   break;
      case PAR_OSC1_TRI:    osc1_tri_mod = value;     break;
      case PAR_OSC1_SIN:    osc1_sin_mod = value;     break;
      case PAR_OSC1_CENT:   pitch_mod = value;        break;
      case PAR_FLT1_FREQ:   flt1_freq_mod = value;    break;
      case PAR_FLT1_RES:    flt1_res_mod = value;     break;
    }
  }

  //----------

  uint32_t process(uint32_t AIndex, uint32_t AState, uint32_t ALength, uint32_t AOffset) {
    MIP_Assert(note_key >= 0);
    if (note_key >= 0) {

      // pitch

      float p;

      //p = (p_osc1_oct * 48) + (p_osc1_semi * 12) + p_osc1_cent;
      p = p_osc1_cent;
      //float osc_pitch = (p_osc1_cent * 2.0) - 1.0;
      float osc_pitch = (p * 2.0) - 1.0;
      osc_pitch += pitch_mod;
      float osc_hz = MIP_NoteToHz(note_key + note_tuning + osc_pitch);

      // osc

      float osq = MIP_Clamp( p_osc1_pulse + osc1_pulse_mod, 0, 1);
      float owi = MIP_Clamp( p_osc1_width + osc1_width_mod, 0, 1);
      float otr = MIP_Clamp( p_osc1_tri   + osc1_tri_mod,   0, 1);
      float osi = MIP_Clamp( p_osc1_sin   + osc1_sin_mod,   0, 1);

      osc1.setFrequency(osc_hz);
      osc1.setSawSqu(osq);
      osc1.setPulseWidth(owi);
      osc1.setSquTri(otr);
      osc1.setTriSin(osi);

      // res

      //float res_pitch = p_res1_semi;
      //res_pitch = (res_pitch * 2.0) - 1.0;
      //res_pitch = powf(2,res_pitch);

      p = p_res1_semi;
      //float res_pitch = (p_res1_semi * 2.0) - 1.0;
      float res_pitch = (p * 2.0) - 1.0;
      float res_hz = MIP_NoteToHz(note_key + note_tuning + res_pitch);

      float delay = (context->samplerate / res_hz);
      //delay /= res_hz;

      float fb = p_res1_fb;
      fb = 1.0 - fb;
      fb = fb * fb * fb * fb;
      fb = 1.0 - fb;

      float damp = p_res1_damp;
      damp = damp * damp * damp;
      damp = 1.0 - damp;

      res1.getFeedbackFX()->flt.setWeight(damp);

      // buffer

      float* output = context->voicebuffer;
      output += AOffset;

      // should this already have been applied/prepared? maybe when we setup
      // the voicebuffer ptr (voice manager).. who calls this? (use AOffset)..

      #ifdef MIP_VOICE_PREPARE_EVENTS
        output += (context->process->frames_count * AIndex);
      #endif

      MIP_Assert(output);

      // loop

      for (uint32_t i = 0; i < ALength; i++) {

        //

        float ff = p_flt1_freq + flt1_freq_mod;
        ff += note_bright;
        ff = MIP_Clamp(ff,0,1);
        ff = (ff * ff); // ugh..
        ff = flt1_freq_smoother.process(ff);

        float fr = p_flt1_res + flt1_res_mod;
        fr = MIP_Clamp(fr,0,1);
        fr = 1.0 - fr; // hhhh...
        fr = flt1_res_smoother.process(fr);

        flt1.setMode(MIP_SVF_LP);
        flt1.setFreq(ff);
        flt1.setBW(fr);

        float amp = note_onvel + note_press;
        amp = MIP_Clamp(amp,0,1);
        amp *= amp_env.process();
        amp = vol_smoother.process(amp);

        //osc
        float osc = osc1.process();


        // res

        // noise
        float rnd = MIP_RandomSigned();
        float noise = (rnd * p_res1_noise) + (osc * (1.0 - p_res1_noise));
        //res = (noise * p_res1_noise) + (res * (1.0 - p_res1_noise));

        float res = 0.0;
        if (res1.hasWrapped()) res = res1.process(0,fb,delay);
        else res = res1.process(noise,fb,delay);

        // out
        float out = (osc * p_osc1_out) + (res * p_res1_out);

        // filter
        out = flt1.process(out);

        // end
        out *= amp;

        //#ifdef MIP_VOICE_PROCESS_THREADED
        #ifdef MIP_VOICE_PREPARE_EVENTS
          *output++ = out;
        #else
          *output++ += out;
        #endif

      }
    }

    // env

    uint32_t stage = amp_env.getStage();
    if (stage == MIP_ENVELOPE_FINISHED) return MIP_VOICE_FINISHED;
    else return AState;

  }

};

//----------------------------------------------------------------------
#endif
