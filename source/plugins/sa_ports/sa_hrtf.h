#ifndef sa_hrtf_included
#define sa_hrtf_included
//----------------------------------------------------------------------



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

const char* sa_hrtf_features[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t sa_hrtf_descriptor = {
  CLAP_VERSION,
  "skei.audio/sa_hrtf",
  "sa_hrtf",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.1",
  "sa_hrtf",
  sa_hrtf_features
};

//----------

//#define

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_hrtf_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  bool need_recalc = false;
  float MSampleRate = 0.0;

//  float  MRotate     = 0.0f;
//  float  MSlope      = 0.0f;
//  float  MDistance   = 1.0f;

//------------------------------
public:
//------------------------------

  sa_hrtf_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_hrtf_plugin() {
  }

//------------------------------
private:
//------------------------------

private:

  void update(float* left, float* right) {

    float MRotate = MParameters[0]->getValue();
    //float MSlope = MParameterValues[1];
    float MDistance = MParameters[2]->getValue() * MParameters[2]->getValue();

    float panl = (cosf(((MRotate + 90.0f) * MIP_PI) / 180.0f) + 1.0f) * 0.5f;
    float panr = 1.0f - panl;
    float pan_intensity = 1.0f / MDistance;
    *left  = powf(panl, pan_intensity);
    *right = powf(panr, pan_intensity);
  }

  //----------

  //void precalc() {
  //}

  //----------

  void recalc(float srate) {
    need_recalc = false;
  }

//------------------------------
private:
//------------------------------

//  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
//    switch(AIndex) {
//      case 0: MRotate = AValue;           break;
//      case 1: MSlope = AValue;            break;
//      case 2: MDistance = AValue*AValue;  break;
//    }
//  }

  //void handle_parameter_event(const clap_event_param_value_t* param_value) final {
  //  MIP_Plugin::handle_parameter_event(param_value);
  //  need_recalc = true;
  //}

  //----------

  void processAudioBlock(const clap_process_t *process) final {
    if (need_recalc) recalc(MSampleRate);
    uint32_t len = process->frames_count;
    float* in0  = process->audio_inputs[0].data32[0];
    float* in1  = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];

    float left, right;
    update(&left,&right);

    for (uint32_t i=0; i<len; i++) {
      *out0++ = *in0++ * left;
      *out1++ = *in1++ * right;
    }
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {

    appendParameter(new MIP_Parameter( 0, "Rotate",   "", -180,  180, 0, CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter(new MIP_Parameter( 1, "Slope",    "", -90,   90,  0, CLAP_PARAM_IS_AUTOMATABLE ));
    appendParameter(new MIP_Parameter( 2, "Distance", "",  0.01, 2,   1, CLAP_PARAM_IS_AUTOMATABLE ));

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




/*
  Copyright (c) 2017 Benno Straub
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
