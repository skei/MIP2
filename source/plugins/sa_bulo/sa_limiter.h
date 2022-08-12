#ifndef sa_limiter_included
#define sa_limiter_included
//----------------------------------------------------------------------



//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#define MIP_NO_GUI

//----------

#include "plugin/mip_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* sa_limiter_features[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t sa_limiter_descriptor = {
  CLAP_VERSION,
  "skei.audio/sa_limiter",
  "sa_limiter",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.1",
  "sa_limiter",
  sa_limiter_features
};

//----------

//#define

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_limiter_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  bool need_recalc = true;//false;
  float MSampleRate = 0.0;

  float     C1 = 0.0f;
  float     C2 = 0.0f;
  float     C3 = 0.0f;
  uint32_t  len = 0;
  uint32_t  bufl = 0;
  uint32_t  bufr = 0;
  uint32_t  pos = 0;
  float     thresh = 0.0f;
  float     make_gain = 0.0f;
  float     ceiling = 0.0f;
  float     BUFFER[1024*1024] = {0};

  //float     slider1 = 0.0f;
  //float     slider2 = 0.0f;

  float out1 = 0.0f;
  float out2 = 0.0f;
  float out3 = 0.0f;

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

  sa_limiter_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_limiter_plugin() {
  }

//------------------------------
private:
//------------------------------

  //void precalc() {
  //}

  //----------

  void recalc(float srate) {
    C1    = exp(-1/(25/1000*srate));
    C2    = exp(-1/(25/1000*srate));
    C3    = exp(-1/(5/1000*srate));
    len   = (srate * 10) / 1000; //10/1000*srate;
    bufl  = 0;
    bufr  = len;
    pos   = 0;

    thresh    = powf(10,(MParameters[0]->getValue()/20));
    make_gain = powf(10,((-MParameters[0]->getValue()+MParameters[1]->getValue())/20));
    ceiling   = powf(10,(MParameters[1]->getValue()/20));

    need_recalc = false;
  }

//------------------------------
private:
//------------------------------

  void processParamValue/*Event*/(const clap_event_param_value_t* param_value) final {
    //MIP_Plugin::processParamValueEvent(param_value);
    need_recalc = true;
  }

  //----------

  void processAudioBlock(const clap_process_t *process) final {
    if (need_recalc) recalc(MSampleRate);
    uint32_t len = process->frames_count;
    float* input0  = process->audio_inputs[0].data32[0];
    float* input1  = process->audio_inputs[0].data32[1];
    float* output0 = process->audio_outputs[0].data32[0];
    float* output1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<len; i++) {
      float spl0 = *input0++;
      float spl1 = *input1++;
      float in = MIP_Max(fabs(spl0),fabs(spl1));
      // smooth via a rc low pass
      out1 = (in   > out1) ? in   : in   + C1 * (out1-in);
      out2 = (out1 > out2) ? out1 : out1 + C2 * (out2-out1);
      out3 = out2 + C3 * (out3-out2);
      float out = out3;
      BUFFER[bufl+pos] = spl0;
      BUFFER[bufr+pos] = spl1;
      pos = (pos+1) % len;
      float gain = ((out > thresh) ? thresh/out : 1) * make_gain;
      spl0 = BUFFER[bufl+pos] * gain;
      spl0 = MIP_Max(MIP_Min(spl0,ceiling),-ceiling);
      spl1 = BUFFER[bufr+pos] * gain;
      spl1 = MIP_Max(MIP_Min(spl1,ceiling),-ceiling);
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

    appendParameter(new MIP_Parameter( 0, "Threshold", "",  -30, 0, 0, CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter(new MIP_Parameter( 1, "Ceiling",   "",  -30, 0, 0, CLAP_PARAM_IS_AUTOMATABLE ));
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

/* Copyright (c) 2012, Michael Gruhn
 * All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
 * THIS SOFTWARE.
 */

desc:Limiter
slider1:0<-30,0,1>Threshold (dB)
slider2:0<-30,0,1>Ceiling (dB)

@init
/* rc smoothing filters */
C1 = exp(-1/(25/1000*srate));
C2 = exp(-1/(25/1000*srate));
C3 = exp(-1/(5/1000*srate));

len = 10/1000*srate;
bufl = 0;
bufr = len;
pos = 0;

pdc_delay = len;
pdc_bot_ch = 0;
pdc_top_ch = 2;

@slider
thresh = 10^(slider1/20);
make_gain = 10^((-slider1+slider2)/20);
ceiling = 10^(slider2/20);

@sample
in = max(abs(spl0),abs(spl1));

/* smooth via a rc low pass */
out1 = in  >out1 ? in   : in   + C1 * (out1-in);
out2 = out1>out2 ? out1 : out1 + C2 * (out2-out1);
out3 = out2 + C3 * (out3-out2);

out = out3;

bufl[pos] = spl0;
bufr[pos] = spl1;

pos = (pos+1)%len;

gain = (out > thresh ? thresh/out : 1)*make_gain;

spl0 = bufl[pos] * gain;
spl0 = max(min(spl0,ceiling),-ceiling);
spl1 = bufr[pos] * gain;
spl1 = max(min(spl1,ceiling),-ceiling);


#endif // 0
