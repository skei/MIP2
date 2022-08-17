#ifndef sa_exciter_included
#define sa_exciter_included
//----------------------------------------------------------------------

/*

  based on the 'exciter' jesusonic plugin by 'loser'

  (C) 2007, Michael Gruhn.
  NO WARRANTY IS GRANTED. THIS PLUG-IN IS PROVIDED ON AN "AS IS" BASIS, WITHOUT
  WARRANTY OF ANY KIND. NO LIABILITY IS GRANTED, INCLUDING, BUT NOT LIMITED TO,
  ANY DIRECT OR INDIRECT,  SPECIAL,  INCIDENTAL OR CONSEQUENTIAL DAMAGE ARISING
  OUT OF  THE  USE  OR INABILITY  TO  USE  THIS PLUG-IN,  COMPUTER FAILTURE  OF
  MALFUNCTION INCLUDED.  THE USE OF THE SOURCE CODE,  EITHER  PARTIALLY  OR  IN
  TOTAL, IS ONLY GRANTED,  IF USED IN THE SENSE OF THE AUTHOR'S INTENTION,  AND
  USED WITH ACKNOWLEDGEMENT OF THE AUTHOR. FURTHERMORE IS THIS PLUG-IN A  THIRD
  PARTY CONTRIBUTION,  EVEN IF INCLUDED IN REAPER(TM),  COCKOS INCORPORATED  OR
  ITS AFFILIATES HAVE NOTHING TO DO WITH IT.  LAST BUT NOT LEAST, BY USING THIS
  PLUG-IN YOU RELINQUISH YOUR CLAIM TO SUE IT'S AUTHOR, AS WELL AS THE CLAIM TO
  ENTRUST SOMEBODY ELSE WITH DOING SO.

*/

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "plugin/mip_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* sa_exciter_features[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t sa_exciter_descriptor = {
  CLAP_VERSION,
  "skei.audio/sa_exciter",
  "sa_exciter",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.1",
  "sa_exciter",
  sa_exciter_features
};

//----------

//#define

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_exciter_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  bool need_recalc = false;
  float MSampleRate = 0.0;

  //float slider1     = 0;
  //float slider2     = 0;
  //float slider3     = 0;
  //float slider4     = 0;
  float tmplONE     = 0;
  float tmprONE     = 0;
  float tmplTWO     = 0;
  float tmprTWO     = 0;

  float c_ampDB     = 8.65617025;
  float cDenorm     = 10^-30;
  float clipBoost   = 0;
  float mixBack     = 0;
  float hdistr      = 0;
  float foo         = 0;
  float freq        = 0;
  float x           = 0;
  float a0          = 0;
  float b1          = 0;

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

  sa_exciter_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_exciter_plugin() {
  }

//------------------------------
private:
//------------------------------

  //void precalc() {
  //}

  //----------

  void recalc(float srate) {
    need_recalc = false;
    clipBoost = exp(MParameters[1]->getValue() / c_ampDB);
    mixBack   = exp(MParameters[3]->getValue() / c_ampDB);
    hdistr    = MIP_Min(MParameters[2]->getValue() / 100.0f, 0.9f);
    foo       = 2.0f * hdistr / (1.0f - hdistr);
    freq      = MIP_Min(MParameters[0]->getValue(),srate);
    x         = exp(-2.0f * MIP_PI * freq / srate);
    a0        = 1.0f - x;
    b1        = -x;
  }

//------------------------------
private:
//------------------------------

  void processParamValue/*Event*/(const clap_event_param_value_t* param_value) final {
    //MIP_Plugin::processParamValueEvent(param_value);
    need_recalc = true;
  }

  //----------

  void processAudioBlock(MIP_ProcessContext* AContext) final {
    const clap_process_t *process = AContext->process;
    if (need_recalc) recalc(MSampleRate);
    uint32_t len = process->frames_count;
    float* in0  = process->audio_inputs[0].data32[0];
    float* in1  = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<len; i++) {

      //*out0++ = *in0++ * MLeft;
      //*out1++ = *in1++ * MRight;

      float spl0 = *in0++;
      float spl1 = *in1++;

      float s0 = spl0;
      float s1 = spl1;

      tmplONE = a0*s0 - b1*tmplONE;// + cDenorm;
      tmprONE = a0*s1 - b1*tmprONE;// + cDenorm;

      //TODO: denorm

      s0 -= tmplONE;
      s1 -= tmprONE;;
      s0 = MIP_Min(MIP_Max(s0*clipBoost,-1),1);
      s1 = MIP_Min(MIP_Max(s1*clipBoost,-1),1);
      s0 = (1+foo) * s0 / (1 + foo * abs(spl0));
      s1 = (1+foo) * s1 / (1 + foo * abs(spl1));

      tmplTWO = a0*s0 - b1*tmplTWO;// + cDenorm
      tmprTWO = a0*s1 - b1*tmprTWO;// + cDenorm;

      //TODO: denorm

      s0 -= tmplTWO;
      s1 -= tmprTWO;

      spl0 += (s0 * mixBack);
      spl1 += (s1 * mixBack);

      *out0++ = spl0;
      *out1++ = spl1;

    }
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    appendAudioInputPort(  &myAudioInputPorts[0]  );
    appendAudioOutputPort( &myAudioOutputPorts[0] );
    appendParameter(new MIP_Parameter( 0, "Freq",  "",  100, 18000, 2000, CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter(new MIP_Parameter( 1, "Boost", "",  0,   6,     0,    CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter(new MIP_Parameter( 2, "Harm",  "",  0,   100,   0,    CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter(new MIP_Parameter( 3, "Mix",   "", -120, 0,     -6,   CLAP_PARAM_IS_AUTOMATABLE ));
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
