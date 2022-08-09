#ifndef sa_pitch_included
#define sa_pitch_included
//----------------------------------------------------------------------

#include "plugin/mip_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* sa_pitch_features[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t sa_pitch_descriptor = {
  CLAP_VERSION,
  "skei.audio/sa_pitch",
  "sa_pitch",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.1",
  "sa_pitch",
  sa_pitch_features
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_pitch_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  bool need_recalc          = false;
  float MSampleRate         = 0.0;
  float buffer[1024*1024*8] = {0};
  float in_pos              = 0;
  float out_pos             = 0;
  float base                = 0;
  float old_base            = 0;
  float fade_base           = 0;
  float inc_                = 0;
  float len_                = 0;
  float fade_               = 0;

//------------------------------
public:
//------------------------------

  sa_pitch_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_pitch_plugin() {
  }

//------------------------------
private:
//------------------------------

  //void precalc() {
  //}

  //----------

  void recalc(float srate) {
    need_recalc = false;
    //inc_  = AValue/100;
    if (MParameterValues[0] >= 0) inc_ = 1 + MParameterValues[0];
    else if (MParameterValues[0] < 0) inc_ = 1 + (MParameterValues[0]*0.5); // -1 -> 1+(-1*0.5) = 1 + -0.5 = 0.5
    //else inc_ = 1;
    len_  = floorf(MParameterValues[1]/1000*MSampleRate/2)*2;
    fade_ = MParameterValues[2]/100 * len_/2;
  }

//------------------------------
private:
//------------------------------

  //  inc = slider1/100;
  //  // len must be multiple of 2 otherwise there will be drift due to floating point math in 1sdft93hgosdh
  //  len = floor(slider2/1000*srate/2)*2;
  //  fade = slider3/100 * len/2;

  void handle_parameter_event(clap_event_param_value_t* param_value) final {
    MIP_Plugin::handle_parameter_event(param_value);
//    uint32_t index = param_value->param_id;
//    float value = param_value->value;
//    switch(index) {
//      case 0:
//        //inc_  = AValue/100;
//        if (value >= 0) inc_ = 1 + value;
//        else if (value < 0) inc_ = 1 + (value*0.5); // -1 -> 1+(-1*0.5) = 1 + -0.5 = 0.5
//        //else inc_ = 1;
//        break;
//      case 1:
//        len_  = floorf(value/1000*MSampleRate/2)*2;
//        break;
//      case 2:
//        fade_ = value/100 * len_/2;
//    }
    need_recalc = true;
  }

  //----------

  void handle_process(const clap_process_t *process) final {
    if (need_recalc) recalc(MSampleRate);
    uint32_t len = process->frames_count;
    float* in0  = process->audio_inputs[0].data32[0];
    float* in1  = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<len; i++) {
      //*out0++ = *in0++;
      //*out1++ = *in1++;
      float spl0 = *in0++;
      float spl1 = *in1++;

      uint32_t p = (uint32_t)floorf(in_pos);
      buffer[ p*2] = spl0;
      buffer[ p*2+1] = spl1;
      in_pos = (uint32_t)floorf(in_pos+1) % (uint32_t)floorf(len_);
      float gain = MIP_Min(out_pos/fade_,1);
      uint32_t p0 = (uint32_t)floorf(base+out_pos) % (uint32_t)floorf(len_);
      uint32_t p1 = (uint32_t)floorf(fade_base+out_pos) % (uint32_t)floorf(len_);
      spl0 = buffer[ p0*2] * gain
           + buffer[ p1*2] * (1-gain);
      spl1 = buffer[ p0*2+1] * gain
           + buffer[ p1*2+1] * (1-gain);
      out_pos += inc_;
      if (out_pos >= (len_-1-fade_)) {
        fade_base = base + len_ - 1 - fade_;
        out_pos = 0;
        base = in_pos;
      }

      *out0++ = spl0;
      *out1++ = spl1;

    }
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    appendParameter(new MIP_Parameter( 0, CLAP_PARAM_IS_AUTOMATABLE, "Pitch",     "", -1, 1,   0   ));
    appendParameter(new MIP_Parameter( 1, CLAP_PARAM_IS_AUTOMATABLE, "Window",    "",  1, 200, 100 ));
    appendParameter(new MIP_Parameter( 2, CLAP_PARAM_IS_AUTOMATABLE, "CrossFade", "",  0, 100, 100 ));
    bool result = MIP_Plugin::init();
    if (result) {
      setDefaultParameterValues();
      need_recalc = true;
      return true;
    }
    return false;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MSampleRate = sample_rate;
    return MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

};

//----------------------------------------------------------------------
#endif
