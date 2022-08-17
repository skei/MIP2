#ifndef sa_sonic_maximizer_included
#define sa_sonic_maximizer_included
//----------------------------------------------------------------------

/*
  based on:
    BBE Sonic Maximizer modelisation, (C) 20011, Dominique Guichaoua.
    http://forum.cockos.com/showthread.php?t=91439
    license: ?
    from the forum:
    skei (me) "i converted both the sonic maximizer (just the first version,
               yet..) and the sonic enhancer to vst, available here [...] i
               haven't studied the licensing agreements, so if you don't want
               me to port your plugins, let me know, and i'll remove it, no
               problem.."
    domino49: "great!!"
    so i think it should be ok?
*/

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define MIP_NO_GUI

//----------

#include "mip.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_parameter.h"
#include <math.h>

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* sa_sonic_maximizer_features[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t sa_sonic_maximizer_descriptor = {
  CLAP_VERSION,
  "skei.audio/sa_sonic_maximizer",
  "sa_sonic_maximizer",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.1",
  "sa_ports/sa_sonic_maximizer",
  sa_sonic_maximizer_features
};

//----------

//#define cDenorm   10e-30
#define c_ampdB   8.65617025
#define freqHP    2243.2
#define freqLP    224.32
#define freqHP_p2 (-MIP_PI2 * freqHP)
#define freqLP_p2 (-MIP_PI2 * freqLP)

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_sonic_maximizer_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

//  #define NUM_PARAMS 3

  // id, flags, cookie, name, module, min, max, def

//  clap_param_info_t myParameters[NUM_PARAMS] = {
//    { 0, 0, nullptr, "Low Cont", "Params",   0.0, 10.0,  1.0 },
//    { 1, 0, nullptr, "Process",  "Params",   0.0, 10.0,  1.0 },
//    { 2, 0, nullptr, "Output",   "Params", -30.0,  0.0, -3.0 }
//  };

  #undef MAX_PARAMS

  float MSampleRate = 0.0;

  bool      need_recalc = true;

  float     param1      = 0.0f;
  float     param2      = 0.0f;
  float     param3      = 0.0f;

  float     band1       = 0.0f;
//float     band2       = 1.0f;
  float     band3       = 0.0f;
  float     tmplLP      = 0.0f;
  float     tmprLP      = 0.0f;
  float     tmplHP      = 0.0f;
  float     tmprHP      = 0.0f;
  float     amp         = 0.0f;
  float     xLP         = 0.0f;
  float     xHP         = 0.0f;
  float     a0LP        = 0.0f;
  float     a0HP        = 0.0f;
  float     b1LP        = 0.0f;
  float     b1HP        = 0.0f;

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

  sa_sonic_maximizer_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_sonic_maximizer_plugin() {
  }

//------------------------------
private:
//------------------------------

  void precalc() {
    param1 = getParameterValue(0);
    param2 = getParameterValue(1);
    param3 = getParameterValue(2);
    band1 = expf( (param1+3) / c_ampdB );
    band3 = expf( (param2+3) / c_ampdB );
    amp = expf( param3 / c_ampdB );
  }

  //----------

  void recalc(float srate) {
    xHP  = expf(freqHP_p2/srate);
    a0HP = 1.0 - xHP;
    b1HP = -xHP;
    xLP  = expf(freqLP_p2/srate);
    a0LP = 1.0 - xLP;
    b1LP = -xLP;
  }

//------------------------------
private:
//------------------------------

  void processParamValue/*Event*/(const clap_event_param_value_t* param_value) final {
    //MIP_Plugin::processParamValueEvent(param_value);
    uint32_t index = param_value->param_id;
    float value = param_value->value;
    switch (index) {
      case 0:
        param1 = value;
        band1 = expf( (param1+3) / c_ampdB );
        break;
      case 1:
        param2 = value;
        band3 = expf( (param2+3) / c_ampdB );
        break;
      case 2:
        param3 = value;
        amp = expf( param3 / c_ampdB );
        break;
    }
    //band2 := 1; // exp(0/c_ampdB);
  }

  //----------

  void processAudioBlock(MIP_ProcessContext* AContext) final {
    const clap_process_t *process = AContext->process;
    if (need_recalc) {
      recalc(MSampleRate);
      need_recalc = false;
    }
    float*    input0  = process->audio_inputs[0].data32[0];
    float*    input1  = process->audio_inputs[0].data32[1];
    float*    output0 = process->audio_outputs[0].data32[0];
    float*    output1 = process->audio_outputs[0].data32[1];
    uint32_t  length  = process->frames_count;
    //float     scale   = getParameterValue(0); // MGain;
    //MIP_CopyStereoBuffer(outputs,inputs,length);
    //MIP_ScaleStereoBuffer(outputs,scale,length);
    for (uint32_t i=0; i<length; i++) {
      //*out0++ = *in0++ * MLeft;
      //*out1++ = *in1++ * MRight;
      float s0  = *input0++;
      float s1  = *input1++;
      tmplLP    = a0LP*s0 - b1LP*tmplLP;// + cDenorm;
      tmprLP    = a0LP*s1 - b1LP*tmprLP;// + cDenorm;
      float sp0 = tmplLP;
      float sp1 = tmprLP;
      tmplHP    = a0HP*s0 - b1HP*tmplHP;// + cDenorm;
      tmprHP    = a0HP*s1 - b1HP*tmprHP;// + cDenorm;
      float sp4 = s0 - tmplHP;
      float sp5 = s1 - tmprHP;
      float sp2 = s0 - sp0 - sp4;
      float sp3 = s1 - sp1 - sp5;
      *output0++ = (sp0 * band1 + sp2 /* * band2 */ + sp4 * band3) * amp;
      *output1++ = (sp1 * band1 + sp3 /* * band2 */ + sp5 * band3) * amp;
    }
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    //setupParameters(myParameters,NUM_PARAMS);

    appendAudioInputPort(  &myAudioInputPorts[0]  );
    appendAudioOutputPort( &myAudioOutputPorts[0] );

    appendParameter(new MIP_Parameter( 0, "Low Cont", "Params",   0.0, 10.0,  1.0, CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter(new MIP_Parameter( 1, "Process",  "Params",   0.0, 10.0,  1.0, CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter(new MIP_Parameter( 2, "Output",   "Params", -30.0,  0.0, -3.0, CLAP_PARAM_IS_AUTOMATABLE ));
    bool result = MIP_Plugin::init();
    if (result) {
      setDefaultParameterValues();
      precalc();
    }
    return result;
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

//#undef cDenorm
#undef c_ampdB
#undef freqHP
#undef freqLP
#undef freqHP_p2
#undef freqLP_p2

//----------------------------------------------------------------------
#endif
