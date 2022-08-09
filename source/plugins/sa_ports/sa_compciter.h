#ifndef sa_compciter_included
#define sa_compciter_included
//----------------------------------------------------------------------

// based on the 'compciter' jesusonic effect by 'loser'

/*
  (C) 2007, Michael Gruhn.

  NO WARRANTY IS GRANTED.  THIS PLUG-IN  IS PROVIDED FOR FREE  ON  AN  "AS  IS"
  BASIS, WITHOUT WARRANTY OF ANY KIND.  NO LIABILITY IS GRANTED, INCLUDING, BUT
  NOT LIMITED TO, ANY DIRECT OR INDIRECT,  SPECIAL, INCIDENTAL OR CONSEQUENTIAL
  DAMAGE ARISING OUT OF THE USE  OR INABILITY  TO  USE  THIS PLUG-IN,  COMPUTER
  FAILTURE  OF  MALFUNCTION  INCLUDED.  THE  USE  OF THE  SOURCE  CODE,  EITHER
  PARTIALLY OR IN TOTAL, IS ONLY GRANTED,  IF USED IN THE SENSE OF THE AUTHOR'S
  INTENTION,  AND USED WITH ACKNOWLEDGEMENT OF THE AUTHOR.  LAST BUT NOT LEAST,
  BY USING THIS  PLUG-IN YOU RELINQUISH YOUR CLAIM TO SUE IT'S AUTHOR,  AS WELL
  AS YOUR CLAIM TO ENTRUST SOMEBODY ELSE WITH DOING SO.
*/

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#define MIP_NO_GUI

//----------

//#include "mip.h"
#include "plugin/mip_plugin.h"
//#include "plugin/mip_parameter.h"
//#include <math.h>

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* sa_compciter_features[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t sa_compciter_descriptor = {
  CLAP_VERSION,
  "skei.audio/sa_compciter",
  "sa_compciter",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.1",
  "sa_ports / sa_compciter",
  sa_compciter_features
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

class sa_compciter_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  bool need_recalc = false;
  float MSampleRate = 0.0;

//  float slider1   = 0;
//  float slider2   = 0;
//  float slider3   = 0;
//  float slider4   = 0;
//  float slider5   = 0;

  float threshDB  = 0;
  float thresh    = 0;
  float ratio     = 0;
  float release   = 0;
  float t0        = 0;
  float t1        = 0;
  float blp       = 0;
  float alp       = 0;
  float wet       = 0;
  float dry       = 0;

  float t00       = 0;
  float t01       = 0;
  float t10       = 0;
  float t11       = 0;
  float t20       = 0;
  float t21       = 0;

  // @init

  float gain      = 1.0f;
  float seekgain  = 1.0f;
  float c         = 8.65617025;
  float dc        = 10^-30;

//------------------------------
public:
//------------------------------

  sa_compciter_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_compciter_plugin() {
  }

//------------------------------
private:
//------------------------------

  //void precalc() {
  //}

  //----------

  void recalc(float srate) {
    need_recalc = false;
    threshDB  = - MParameters[0]->getValue();;
    thresh    = exp(threshDB / c);
    ratio     = 1 / 20;
    release   = exp(-60 / (pow(1 - MParameters[1]->getValue() / 100, 3) * 500 * srate / 1000) / c);
    t0        = 0;
    t1        = 0;
    blp       = -exp(-2 * MIP_PI * MParameters[2]->getValue() * 3 / srate);
    alp       = 1.0 + blp;
    wet       = exp(MParameters[3]->getValue() / c) / exp((threshDB - threshDB * ratio ) / c);
    dry       = exp(MParameters[4]->getValue() / c);
    seekgain  = 1;
  }

//------------------------------
private:
//------------------------------

  void processParamValueEvent(const clap_event_param_value_t* param_value) final {
    MIP_Plugin::processParamValueEvent(param_value);
    need_recalc = true;
  }

  //----------

  void processAudioBlock(const clap_process_t *process) final {
    if (need_recalc) recalc(MSampleRate);
    uint32_t len = process->frames_count;
    float* in0  = process->audio_inputs[0].data32[0];
    float* in1  = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];

    for (uint32_t i=0; i<len; i++) {
      float spl0 = *in0++;
      float spl1 = *in1++;
      float s0,s1;
      t00 = alp * spl0 - blp * t00;
      t01 = alp * spl1 - blp * t01;
      s0 = t00;
      s1 = t01;
      t10 = alp * s0 - blp * t10;
      t11 = alp * s1 - blp * t11;
      s0 = t10;
      s1 = t11;
      t20 = alp * s0 - blp * t20;
      t21 = alp * s1 - blp * t21;
      s0 = spl0 - t20;
      s1 = spl1 - t21;
      float rms = fmax( abs(spl0) , abs(spl1) );
      //seekgain = ((rms > thresh) ? exp((threshDB + (log(rms)*c-threshDB)*ratio) /c) / rms : 1);
      if (rms > thresh) seekgain = exp((threshDB + (log(rms)*c-threshDB)*ratio) /c) / rms;
      else seekgain = 1;
      //gain = ((gain > seekGain) ? seekGain : min( gain/release , seekGain ));
      if (gain > seekgain) {
        gain = seekgain;
      }
      else {
        gain = fmin( gain/release , seekgain );
      }
      spl0 = spl0*dry + s0 *gain * wet;
      spl1 = spl1*dry + s1 *gain * wet;
      *out0++ = spl0;
      *out1++ = spl1;
    }
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    appendParameter(new MIP_Parameter( 0, "Drive", "",  0,   60,    0,    CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter(new MIP_Parameter( 1, "Dist",  "",  0,   100,   25,   CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter(new MIP_Parameter( 2, "HP",    "",  800, 12000, 5000, CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter(new MIP_Parameter( 3, "Wet",   "", -60,  24,   -6,    CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter(new MIP_Parameter( 4, "Dry",   "", -120, 0,     0,    CLAP_PARAM_IS_AUTOMATABLE ));
    bool result = MIP_Plugin::init();
    if (result) {
      setDefaultParameterValues();

      //precalc();
      need_recalc = true;

      return true;
    }
    //return result;
    return false;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MSampleRate = sample_rate;
    return MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  //const void* get_extension(const char *id) final {
  //  const void* ext = MIP_Plugin::get_extension(id);
  //  if (!ext) {
  //    if (strcmp(id,CLAP_EXT_GUI) == 0)             return &MGui;
  //    if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0)     return &MAudioPorts;
  //    if (strcmp(id,CLAP_EXT_NOTE_PORTS) == 0)      return &MNotePorts;
  //    if (strcmp(id,CLAP_EXT_QUICK_CONTROLS) == 0)  return &MQuickControls;
  //  }
  //  return ext;
  //}

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
