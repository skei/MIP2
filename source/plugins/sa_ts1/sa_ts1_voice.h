#ifndef sa_ts1_voice_included
#define sa_ts1_voice_included
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

struct sa_ts1_DelayFx {

  MIP_RcFilter<float> flt   = {};
  float               rough = 0.0;

  float process(float x) {
    float r = MIP_Random();
    if (r >= rough) x = -x;
    return flt.process(x);
  }
};

typedef MIP_InterpolatedDelay<4096,sa_ts1_DelayFx> sa_ts1_Delay;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_ts1_Voice {

//------------------------------
private:
//------------------------------

  MIP_VoiceContext*   context             = nullptr;

  MIP_Oscillator2     osc1                = {};
  MIP_SvfFilter       flt1                = {};
  MIP_SvfFilter       nsh1                = {};
  sa_ts1_Delay        res1                = {};
  MIP_Envelope        amp_env             = {};

  MIP_RcFilter<float> flt1_freq_smoother  = {};
  MIP_RcFilter<float> flt1_res_smoother   = {};
  MIP_RcFilter<float> vol_smoother        = {};

  int32_t             note_key            = -1;
  float               note_onvel          = 0.0;
  float               note_offvel         = 0.0;
  float               note_vol            = 0.0;
  float               note_pan            = 0.0;
  float               note_tuning         = 0.0;
  float               note_vibr           = 0.0;
  float               note_expr           = 0.0;
  float               note_bright         = 0.0;
  float               note_press          = 0.0;
  float               hz                  = 0.0;  // note hz

  float               p_osc1_in_o1        = 0.0;
  float               p_osc1_in_o2        = 0.0;
  float               p_osc1_in_r1        = 0.0;
  float               p_osc1_in_r2        = 0.0;
  float               p_osc1_in_n         = 0.0;
  float               p_osc1_shape        = 0.0;
  float               p_osc1_width        = 0.0;
  //float               p_osc1_tri          = 0.0;
  //float               p_osc1_sin          = 0.0;
  float               p_osc1_oct          = 0.5;
  float               p_osc1_semi         = 0.5;
  float               p_osc1_cent         = 0.5;

  float               p_res1_in_o1        = 0.0;
  float               p_res1_in_o2        = 0.0;
  float               p_res1_in_r1        = 0.0;
  float               p_res1_in_r2        = 0.0;
  float               p_res1_in_n         = 0.0;
  //float               p_res1_noise        = 0.0;
  float               p_res1_nshape       = 0.0;
  float               p_res1_fb           = 0.5;
  float               p_res1_damp         = 0.0;
  float               p_res1_oct          = 0.5;
  float               p_res1_semi         = 0.5;
  float               p_res1_cent         = 0.5;
  float               p_res1_rough        = 0.0;

  float               p_flt1_freq         = 0.5;
  float               p_flt1_res          = 0.5;

  float               p_osc1_out          = 0.0;
  float               p_res1_out          = 0.0;

  //----------

  float               osc1_pulse_mod      = 0.0;
  float               osc1_width_mod      = 0.0;
  float               osc1_tri_mod        = 0.0;
  float               osc1_sin_mod        = 0.0;
  float               osc1_cent_mod       = 0.0;

  float               flt1_freq_mod       = 0.0;
  float               flt1_res_mod        = 0.0;


  float O1 = 0.0;
  float O2 = 0.0;
  float R1 = 0.0;
  float R2 = 0.0;

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
    osc1_cent_mod = 0.0;
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
      case PAR_OSC1_IN_O1:  p_osc1_in_o1 = value;             break;
      case PAR_OSC1_IN_O2:  p_osc1_in_o2 = value;             break;
      case PAR_OSC1_IN_R1:  p_osc1_in_r1 = value;             break;
      case PAR_OSC1_IN_R2:  p_osc1_in_r2 = value;             break;
      case PAR_OSC1_IN_N:   p_osc1_in_n = value;             break;
      case PAR_OSC1_SHAPE:  p_osc1_shape = value;             break;
      case PAR_OSC1_WIDTH:  p_osc1_width = value;             break;
      //case PAR_OSC1_TRI:    p_osc1_tri = value;               break;
      //case PAR_OSC1_SIN:    p_osc1_sin = value;               break;
      case PAR_OSC1_OCT:    p_osc1_oct = value;               break;
      case PAR_OSC1_SEMI:   p_osc1_semi = value;              break;
      case PAR_OSC1_CENT:   p_osc1_cent = value;              break;
      case PAR_RES1_IN_O1:  p_res1_in_o1 = value;             break;
      case PAR_RES1_IN_O2:  p_res1_in_o2 = value;             break;
      case PAR_RES1_IN_R1:  p_res1_in_r1 = value;             break;
      case PAR_RES1_IN_R2:  p_res1_in_r2 = value;             break;
      case PAR_RES1_IN_N:   p_res1_in_n = value;             break;
      //case PAR_RES1_NOISE:  p_res1_noise = value;             break;
      case PAR_RES1_SHAPE:  p_res1_nshape = value;             break;
      case PAR_RES1_FB:     p_res1_fb = value;                break;
      case PAR_RES1_DAMP:   p_res1_damp = value;              break;
      case PAR_RES1_OCT:    p_res1_oct = value;               break;
      case PAR_RES1_SEMI:   p_res1_semi = value;              break;
      case PAR_RES1_CENT:   p_res1_cent = value;              break;
      case PAR_RES1_ROUGH:  p_res1_rough = value;             break;
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
      case PAR_OSC1_SHAPE:  osc1_pulse_mod = value;   break;
      case PAR_OSC1_WIDTH:  osc1_width_mod = value;   break;
      //case PAR_OSC1_TRI:    osc1_tri_mod = value;     break;
      //case PAR_OSC1_SIN:    osc1_sin_mod = value;     break;
      case PAR_OSC1_CENT:   osc1_cent_mod = value;        break;
      case PAR_FLT1_FREQ:   flt1_freq_mod = value;    break;
      case PAR_FLT1_RES:    flt1_res_mod = value;     break;
    }
  }

  //----------

  uint32_t process(uint32_t AIndex, uint32_t AState, uint32_t ALength, uint32_t AOffset) {
    MIP_Assert(note_key >= 0);
    if (note_key >= 0) {

      // shapes

      //float shape = p_osc1_shape; // TODO, instead of pulse,tri,sin knobs

      float shape       = p_osc1_shape * 3.0;
      float osc1_pulse  = 0.0;
      float osc1_tri    = 0.0;
      float osc1_sin    = 0.0;

      if (shape < 1.0) {
        osc1_pulse  = shape;
        osc1_tri    = 0.0;
        osc1_sin    = 0.0;
      }
      else if (shape < 2.0) {
        osc1_pulse  = 1.0;
        osc1_tri    = shape - 1.0;
        osc1_sin    = 0.0;
      }
      else {
        osc1_pulse  = 1.0;
        osc1_tri    = 1.0;
        osc1_sin    = shape - 2.0;
      }

      // pitch

      float o1_pitch  = ((p_osc1_oct  /*+ osc1_oct_mod*/ ) * 12.0)
                      + ((p_osc1_semi /*+ osc1_semi_mod*/) * 1.0)
                      +  (p_osc1_cent   + osc1_cent_mod  );

      float osc_hz = MIP_NoteToHz(note_key + note_tuning + o1_pitch);
      osc_hz = MIP_Clamp(osc_hz,20,10000);

      // osc

      float osq = MIP_Clamp( osc1_pulse + osc1_pulse_mod, 0, 1);
      float owi = MIP_Clamp( p_osc1_width + osc1_width_mod, 0, 1);
      float otr = MIP_Clamp( osc1_tri   + osc1_tri_mod,   0, 1);
      float osi = MIP_Clamp( osc1_sin   + osc1_sin_mod,   0, 1);

      osc1.setFrequency(osc_hz);
      osc1.setSawSqu(osq);
      osc1.setPulseWidth(owi);
      osc1.setSquTri(otr);
      osc1.setTriSin(osi);

      // res

      float r1_pitch  = ((p_res1_oct  /*+ res1_oct_mod*/ ) * 12.0)
                      + ((p_res1_semi /*+ res1_semi_mod*/) * 1.0)
                      +  (p_res1_cent /*+ res1_cent_mod*/  );

      float res_hz = MIP_NoteToHz(note_key + note_tuning + r1_pitch);
      res_hz = MIP_Clamp(res_hz,1,20000);

      float delay = (context->samplerate / res_hz);
      delay *= 0.5;
      //delay /= res_hz;

      float fb = p_res1_fb;
      fb = 1.0 - fb;
      fb = fb * fb * fb * fb;
      fb = 1.0 - fb;

      float damp = p_res1_damp;
      damp = damp * damp * damp;
      damp = 1.0 - damp;

      res1.getFeedbackFX()->flt.setWeight(damp);

      //res1.getFeedbackFX()->rough = p_res1_rough;

      float ro = p_res1_rough;
      if (ro < 0.5) {
        ro = ro * 2.0; // 0..0,5 -> 0..1
        ro = MIP_Curve(ro,0.01);
      }
      else {
        ro = (ro - 0.5) * 2.0; // 0.5..1 -> 0..1
        ro = MIP_Curve(ro,0.99);
        ro += 1.0;
      }

      // 0..2 -> 0..1
      ro *= 0.5;

      res1.getFeedbackFX()->rough = ro;

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

//

        float osc1_in = 0.0;
        osc1_in += p_osc1_in_o1 * O1;
        osc1_in += p_osc1_in_o2 * O2;
        osc1_in += p_osc1_in_r1 * R1;
        osc1_in += p_osc1_in_r2 * R2;
        osc1_in += p_osc1_in_n * MIP_RandomSigned();

        float res1_in = 0.0;
        res1_in += p_res1_in_o1 * O1;
        res1_in += p_res1_in_o2 * O2;
        res1_in += p_res1_in_r1 * R1;
        res1_in += p_res1_in_r2 * R2;
        res1_in += p_res1_in_n * MIP_RandomSigned();

        O1 = osc1.process();

        // res shape

        nsh1.setMode(MIP_SVF_LP);
        float ns = 1.0 - p_res1_nshape;
        nsh1.setFreq(ns * ns);
        nsh1.setBW(1);
        res1_in = nsh1.process(res1_in);
        if (res1.hasWrapped()) R1 = res1.process(0,fb,delay);
        else R1 = res1.process(res1_in,fb,delay);

        // out
        float out = (O1 * p_osc1_out) + (R1 * p_res1_out);

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
