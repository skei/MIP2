#ifndef test_synth1_voice_included
#define test_synth1_voice_included
//----------------------------------------------------------------------

#include "mip.h"
#include "audio/mip_audio_math.h"
#include "audio/filters/mip_rc_filter.h"
#include "audio/filters/mip_svf_filter.h"
#include "audio/modulation/mip_envelope.h"
#include "audio/waveforms/mip_polyblep_waveform.h"

#include "plugin/mip_voice_manager.h"

//----------------------------------------------------------------------

// todo: redo the smoothing things..

#define SMOOTHER_FACTOR (1.0 / 250.0)

//----------

class myVoice {

//------------------------------
private:
//------------------------------

  MIP_VoiceContext* context           = nullptr;

  MIP_SvfFilter     filter            = {};
  MIP_Envelope      amp_env           = {};
  MIP_RcFilter      flt_freq_smoother = {};
  MIP_RcFilter      flt_res_smoother  = {};
  MIP_RcFilter      vol_smoother      = {};

  int32_t           note_key          = -1;
  float             note_onvel        = 0.0;
  float             note_offvel       = 0.0;
  float             note_vol          = 0.0;
  float             note_pan          = 0.0;
  float             note_tuning       = 0.0;
  float             note_vibr         = 0.0;
  float             note_expr         = 0.0;
  float             note_bright       = 0.0;
  float             note_press        = 0.0;

  float             hz                = 0.0;  // note hz
  float             ph                = 0.0;  // phase
  float             phadd             = 0.0;  // phase add
  float             pulse             = 1.0;
  float             width             = 0.5;
  float             filter_freq       = 0.5;
  float             filter_res        = 0.5;
  float             pitch             = 0.5;

  float             pulse_mod         = 0.0;
  float             width_mod         = 0.0;
  float             filter_freq_mod   = 0.0;
  float             filter_res_mod    = 0.0;
  float             pitch_mod         = 0.0;

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
    flt_freq_smoother.setup(filter_freq,filter_freq, SMOOTHER_FACTOR);
    flt_res_smoother.setup(filter_res,filter_res, SMOOTHER_FACTOR);
    vol_smoother.setup(0,0,SMOOTHER_FACTOR);
  }

  //----------

  uint32_t note_on(int32_t key, float velocity) {
    note_key        = key;
    note_onvel      = velocity;
    note_press      = 0.0;
    note_bright     = 0.0;
    note_tuning     = 0.0;
    ph              = 0.0;
    filter_freq_mod = 0.0;
    filter_res_mod  = 0.0;
    pulse_mod       = 0.0;
    width_mod       = 0.0;
    pitch_mod       = 0.0;
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
      case  2:  filter_freq = value;              break;
      case  3:  filter_res = value;               break;
      case  4:  pulse = value;                    break;
      case  5:  width = value;                    break;
      case  6:  amp_env.setAttack(value*5);       break;
      case  7:  amp_env.setDecay(value*5);        break;
      case  8:  amp_env.setSustain(value*value);  break;
      case  9:  amp_env.setRelease(value*5);      break;
      case 10:  pitch = value;                    break;
    }
  }

  void modulation(uint32_t index, float value) {
    switch (index) {
      case  2:  filter_freq_mod = value;  break;
      case  3:  filter_res_mod = value;   break;
      case  4:  pulse_mod = value;        break;
      case  5:  width_mod = value;        break;
      case 10:  pitch_mod = value;        break;
    }
  }

  //----------

  uint32_t process(uint32_t AIndex, uint32_t AState, uint32_t ALength, uint32_t AOffset) {
    MIP_Assert(note_key >= 0);
    if (note_key >= 0) {
      float p = (pitch * 2.0) - 1.0;
      p += pitch_mod;
      //todo: smoother
      hz = MIP_NoteToHz(note_key + note_tuning + p);
      phadd = hz * context->invsamplerate;
      float* output = context->voicebuffer;

      // should this already have been applied/prepared?
      // maybe when we setup the voicebuffer ptr (voice manager)
      #ifdef MIP_VOICE_PROCESS_THREADED
        output += (context->process->frames_count * AIndex);
      #endif

      output += AOffset;
      MIP_Assert(output);
      for (uint32_t i = 0; i < ALength; i++) {
        float t1 = ph + 0.5f;
        t1 = MIP_Fract(t1);
        float saw1 = 2.0 * t1 - 1.0;
        saw1 -= MIP_PolyBlep(t1,phadd);
        float w = width + width_mod;
        w = MIP_Clamp(w,0,1);
        float t2 = t1 + w;
        t2 = MIP_Fract(t2);
        float saw2 = 2.0 * t2 - 1.0;
        saw2 -= MIP_PolyBlep(t2,phadd);
        float sq = pulse + pulse_mod;
        sq = MIP_Clamp(sq,0,1);
        float squ = saw1 - (saw2 * sq);
        float ff = filter_freq + filter_freq_mod;
        ff += note_bright;
        ff = MIP_Clamp(ff,0,1);
        ff = (ff * ff); // ugh..
        ff = flt_freq_smoother.process(ff);
        float fr = filter_res + filter_res_mod;
        fr = MIP_Clamp(fr,0,1);
        fr = 1.0 - fr; // hhhh...
        fr = flt_res_smoother.process(fr);
        float amp = note_onvel + note_press;
        amp = MIP_Clamp(amp,0,1);
        amp *= amp_env.process();
        amp = vol_smoother.process(amp);
        filter.setMode(MIP_SVF_LP);
        filter.setFreq(ff);
        filter.setBW(fr);
        float out = squ;

        #ifdef MIP_VOICE_PROCESS_THREADED
          *output++ = (filter.process(out) * amp);
        #else
          *output++ += (filter.process(out) * amp);
        #endif

        ph += phadd;
        ph = MIP_Fract(ph);
      }
    }
    uint32_t stage = amp_env.getStage();
    if (stage == MIP_ENVELOPE_FINISHED) return MIP_VOICE_FINISHED;
    else return AState;
  }

};

//----------------------------------------------------------------------
#endif
