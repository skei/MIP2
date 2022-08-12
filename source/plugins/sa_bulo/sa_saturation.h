#ifndef sa_saturation_included
#define sa_saturation_included
//----------------------------------------------------------------------

/*
  based on the 'saturation' jesusonic effect by tale/loser
  http://forum.cockos.com/showpost.php?p=1537127&postcount=3
  see bottom of this file for original
*/


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#define MIP_NO_GUI

//----------

#include "plugin/mip_plugin.h"

#define scale ( 1.1 / 3.0 )
const char* txt_func[2] = { "sine", "cubic" };

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* sa_saturation_features[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t sa_saturation_descriptor = {
  CLAP_VERSION,
  "skei.audio/sa_saturation",
  "sa_saturation",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.1",
  "sa_saturation",
  sa_saturation_features
};

//----------

//#define

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_saturation_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  bool need_recalc = true;
  float MSampleRate = 0.0;

//  float     p_saturation;
//  uint32_t  p_stages;
//  uint32_t  p_function;
  float     a, div, gain;

  const clap_audio_port_info_t myAudioInputPorts[1] = {
    { 0, "input1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  const clap_audio_port_info_t myAudioOutputPorts[1] = {
    { 0, "output1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
  };

  //appendAudioInputPort(  &myAudioInputPorts[0]  );
  //appendAudioOutputPort( &myAudioOutputPorts[0] );

//------------------------------
public:
//------------------------------

  sa_saturation_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_saturation_plugin() {
  }

//------------------------------
private:
//------------------------------

  //void precalc() {
  //}

  //----------

  void recalc(void) {
    //a = MIP_Max(0, MIP_Min(1, p_saturation));
    a = MIP_Max(0, MIP_Min(1, MParameters[0]->getValue()));
    if (a > 0) {
      gain = 1 - (1 - 1/(float)MParameters[1]->getValue()) * a;
      float f = 1.0f;
      switch( (int)MParameters[2]->getValue() ) {
        case 0:
          a *= MIP_PI05;
          f = sin(a);
          break;
        case 1:
          f = a - a*a*a * scale;
          break;
      }
      div = 1 / f;
    }
    need_recalc = false;
  }

//------------------------------
private:
//------------------------------

//  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
//    switch(AIndex) {
//      case 0: p_saturation = AValue; break;
//      case 1: p_stages = AValue;     break;
//      case 2: p_function = AValue;   break;
//    }
//    recalc();
//  }

  void processParamValue/*Event*/(const clap_event_param_value_t* param_value) final {
    //MIP_Plugin::processParamValueEvent(param_value);
    need_recalc = true;
  }

  //----------

  void processAudioBlock(const clap_process_t *process) final {

    //if (need_recalc) recalc(MSampleRate);
    if (need_recalc) recalc();

    uint32_t len = process->frames_count;
    float* input0  = process->audio_inputs[0].data32[0];
    float* input1  = process->audio_inputs[0].data32[1];
    float* output0 = process->audio_outputs[0].data32[0];
    float* output1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<len; i++) {

      float spl0 = *input0++;
      float spl1 = *input1++;
      //----------
      if (a>0) {
        float x0 = MIP_Max(-1, MIP_Min(1, spl0));
        float x1 = MIP_Max(-1, MIP_Min(1, spl1));
        switch( (int)MParameters[2]->getValue() ) {
          case 0:
            for (uint32_t j=0; j<MParameters[1]->getValue(); j++) {
              x0 = sin(x0 * a) * div;
              x1 = sin(x1 * a) * div;
            }
            break;
          case 1:
            for (uint32_t j=0; j<MParameters[1]->getValue(); j++) {
              x0 *= a; x0 = (x0 - x0*x0*x0 * scale) * div;
              x1 *= a; x1 = (x1 - x1*x1*x1 * scale) * div;
            }
            break;
        }
        spl0 = x0 * gain;
        spl1 = x1 * gain;
      } // a>0
      //----------
      *output0++ = spl0;
      *output1++ = spl1;

    }
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {

    appendAudioInputPort(  &myAudioInputPorts[0]  );
    appendAudioOutputPort( &myAudioOutputPorts[0] );

    appendParameter(new MIP_Parameter(     0, "Saturation", "",  0,   1,    0, CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter(new MIP_IntParameter(  1, "Stages",     "",  1,   10,   1, CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter(new MIP_TextParameter( 2, "Function",   "",  0,   1,    0, CLAP_PARAM_IS_AUTOMATABLE, txt_func ));

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
//
//
//
//----------------------------------------------------------------------

//#undef

//----------------------------------------------------------------------
#endif





#if 0

desc:Another loser/Saturation rewrite, thanks to SaulT!
slider1:0<0,1,0.01>Saturation
slider2:1<1,10,1>Stages
slider3:1<0,1,1{Sine,Cubic}>Function

// Sine:  f(x) = sin(x * a * 0.5 * pi)/sin(a * 0.5 * pi)
// Cubic: f(x) = ((x*a)-((x*a)^3)/3)/(a-(a^3)/3)

@init

scale = 1.1 / 3;

@slider

a = max(0, min(1, slider1));
a > 0 ? (
  n = max(1, floor(slider2 + 0.5));
  f = slider3 < 0.5;

  gain = 1 - (1 - 1/n) * a;
  div = 1/(f ? sin(a *= 0.5*$pi) : a - a*a*a * scale);
);

@sample

a > 0 ? (
  x0 = max(-1, min(1, spl0));
  x1 = max(-1, min(1, spl1));

  f ? loop(n,
    x0 = sin(x0 * a) * div;
    x1 = sin(x1 * a) * div;
  ) : loop(n,
    x0 *= a; x0 = (x0 - x0*x0*x0 * scale) * div;
    x1 *= a; x1 = (x1 - x1*x1*x1 * scale) * div;
  );

  spl0 = x0 * gain;
  spl1 = x1 * gain;
);

#endif // 0

