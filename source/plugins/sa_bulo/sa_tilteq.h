#ifndef sa_tilteq_included
#define sa_tilteq_included
//----------------------------------------------------------------------

// (C) lubomir i. ivanov
// used with permission

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "plugin/mip_plugin.h"

//----------------------------------------------------------------------

const char* str_proc[2] = {"Stereo","Mono" };

//----------

const char* sa_tilteq_features[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t sa_tilteq_descriptor = {
  CLAP_VERSION,
  "skei.audio/sa_tilteq",
  "sa_tilteq",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.1",
  "sa_tilteq",
  sa_tilteq_features
};

//----------

//#define

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_tilteq_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  bool need_recalc = true;
  float MSampleRate = 0.0;

  float amp = 6/log(2); //Ln
  float lp_out    = 0;
  float lp_out_r  = 0;
  float a0        = 0;
  float b1        = 0;

  float outgain   = 0;
  float lgain     = 0;
  float hgain     = 0;
  uint32_t mono   = 0;

  float sr3       = 0;

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

  sa_tilteq_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_tilteq_plugin() {
  }

//------------------------------
private:
//------------------------------

  //void precalc() {
  //}

  //----------

  //void recalc(float srate) {

  void recalc() {

    float sx,f0,omega,n,gain,gfactor,g1,g2;

    need_recalc = false;

    //switch(index) {
    //  case 0: // slider1
        mono = MIP_Trunc(MParameters[0]->getValue());
    //    break;
    //  case 1: // slider2
        sx = 16 + MParameters[1]->getValue() * 1.20103;
        f0 = floor(exp(sx*log(1.059))*8.17742);
        //f0 := trunc(Exp(sx*ln(1.059))*8.17742); // log(x), log2? orig had Logf
        omega = 2*MIP_PI*f0;
        n = 1/(sr3 + omega); // sr3 = 0 in constructor...
        a0 = 2*omega*n;
        b1 = (sr3 - omega)*n;
    //    break;
    //  case 2: // slider3
        gain = MParameters[2]->getValue();
        gfactor = 4;
        if (gain>0) {
          g1 = -gfactor*gain;
          g2 = gain;
        }
        else {
          g1 = -gain;
          g2 = gfactor*gain;
        }
        lgain = exp(g1/amp)-1;
        hgain = exp(g2/amp)-1;
    //    break;
    //  case 3: // slider4
        outgain = exp(MParameters[3]->getValue()/amp);
    //    break;
    //}
  }

//------------------------------
private:
//------------------------------

  void processParamValue/*Event*/(const clap_event_param_value_t* param_value) final {
    //MIP_Plugin::processParamValueEvent(param_value);
    //recalc();
    need_recalc = true;
    //uint32_t index = param_value->param_id;
    //float value = param_value->value;


  }

  //----------

  void processAudioBlock(const clap_process_t *process) final {

    if (need_recalc) recalc();//MSampleRate);

    sr3 = 3 * MSampleRate;

    uint32_t len = process->frames_count;
    float* input0  = process->audio_inputs[0].data32[0];
    float* input1  = process->audio_inputs[0].data32[1];
    float* output0 = process->audio_outputs[0].data32[0];
    float* output1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<len; i++) {

      float inp,outp, inp_r,outp_r;
      float spl0 = *input0++;//+ DENORM;
      float spl1 = *input1++;//+ DENORM;
      if (mono==1) {
        //process mono
        inp = (spl0+spl1)*0.5;
        lp_out = a0*inp + b1*lp_out;
        outp = inp + lgain*lp_out + hgain*(inp - lp_out);
        spl0 = outp*outgain;
        spl1 = outp*outgain;
      }
      else {
        //process stereo
        inp = spl0;
        lp_out = a0*inp + b1*lp_out;
        outp = inp + lgain*lp_out + hgain*(inp - lp_out);
        spl0 = outp*outgain;

        inp_r = spl1;
        lp_out_r = a0*inp_r + b1*lp_out_r;
        outp_r = inp_r + lgain*lp_out_r + hgain*(inp_r - lp_out_r);
        spl1 = outp_r*outgain;
      }
      *output0++ = spl0 ;//- DENORM;
      *output1++ = spl1 ;//- DENORM;



    }
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {

    appendAudioInputPort(  &myAudioInputPorts[0]  );
    appendAudioOutputPort( &myAudioOutputPorts[0] );

    appendParameter(new MIP_TextParameter( 0, "Processing",  "",  0,   1,    0,   CLAP_PARAM_IS_AUTOMATABLE, str_proc ));
    appendParameter(new MIP_Parameter(     1, "Center Freq", "",  0,   100,  50, CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter(new MIP_Parameter(     2, "Tilt",        "", -6,   6,    0,   CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter(new MIP_Parameter(     3, "Gain",        "", -25,  25,   0,   CLAP_PARAM_IS_AUTOMATABLE ));

    bool result = MIP_Plugin::init();
    if (result) {
      setDefaultParameterValues();
      //need_recalc = true;
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
