#ifndef template_included
#define template_included
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

const char* template_features[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t template_descriptor = {
  CLAP_VERSION,
  "skei.audio/template",
  "template",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.1",
  "template",
  template_features
};

//----------

//#define

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class template_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  bool need_recalc = false;
  float MSampleRate = 0.0;

//------------------------------
public:
//------------------------------

  template_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~template_plugin() {
  }

//------------------------------
private:
//------------------------------

  //void precalc() {
  //}

  //----------

  void recalc(float srate) {
    need_recalc = false;
  }

//------------------------------
private:
//------------------------------

  void handle_parameter_event(const clap_event_param_value_t* param_value) final {
    MIP_Plugin::handle_parameter_event(param_value);
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
    }
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    appendParameter(new MIP_Parameter( 0, CLAP_PARAM_IS_AUTOMATABLE, "param1", "",  0,   1,    0.5    ));
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
