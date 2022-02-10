
#define MIP_NO_GUI
#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

//----------

#include "mip.h"

#include "plugin/mip_plugin.h"
#include "plugin/ladspa/mip_ladspa.h"
#include "plugin/ladspa/mip_ladspa_hosted_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* myFeatures[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t myDescriptor = {
  CLAP_VERSION,
  "torhelgeskei/ladspa_wrapper/v0.0.1",
  "ladspa_wrapper",
  "torhelgeskei",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.1",
  "simple mip2 test plugin",
  myFeatures
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myPlugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  #define NUM_PARAMS 1

  clap_param_info_t myParameters[NUM_PARAMS] = {
    { 0, CLAP_PARAM_IS_MODULATABLE, nullptr, "Gain", "Params", 0.0, 1.0, 0.5 }
  };

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~myPlugin() {
  }

//------------------------------
private:
//------------------------------

  //void handle_parameter_event(const clap_event_param_value_t* param_value) final {
  //  MIP_Plugin::handle_parameter_event(param_value);
  //  uint32_t index = param_value->param_id;
  //  double value = param_value->value;
  //}

  //----------

  //void handle_modulation_event(const clap_event_param_mod_t* param_mod) final {
  //  MIP_Plugin::handle_modulation_event(param_mod);
  //}

  //----------

  void handle_process(const clap_process_t *process) final {
    float**  inputs  = process->audio_inputs[0].data32;
    float**  outputs = process->audio_outputs[0].data32;
    uint32_t length  = process->frames_count;
    float    scale   = getParameterValue(0) + getParameterModulation(0);
    MIP_CopyStereoBuffer(outputs,inputs,length);
    MIP_ScaleStereoBuffer(outputs,scale,length);
  }

  //----------

//  void handle_output_events(const clap_output_events_t* out_events) final {
//    if (MEditor && MIsEditorOpen) {
//      float v0 = MParamVal[0] + MParamMod[0];
//      v0 = MIP_Clamp(v0,0,1);
//      MEditor->send_param_mod(0,v0,out_events);
//    }
//    MIP_Plugin::handle_output_events(out_events);
//  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {

    //setupParameters(myParameters,NUM_PARAMS);
    //for (uint32_t i=0; i<num; i++) {
    //  const clap_param_info_t* info = &params[i];
    //  MParameters.append(info);
    //}

    bool result = MIP_Plugin::init();
    if (result) {
      setDefaultParameterValues(myParameters,NUM_PARAMS);
    }
    return result;
  }

  //----------

//  const void* get_extension(const char *id) final {
//    const void* ext = MIP_Plugin::get_extension(id);
//    return ext;
//    //return nullptr;
//  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void MIP_RegisterPlugins(MIP_ClapList* AList) {
  AList->appendPlugin(&myDescriptor);
}

//----------------------------------------------------------------------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    case 0: return new myPlugin(ADescriptor,AHost);
  }
  return nullptr;
}
