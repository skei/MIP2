
#define MIP_NO_GUI
#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

//----------

#include "mip.h"

#include "plugin/mip_plugin.h"

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
  "torhelgeskei/gain/v0.0.1",
  "gain",
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
    { 0, 0, nullptr, "Gain", "Params", 0.0, 1.0, 0.5 }
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
  //  float value = param_value->value;
  //  switch (index) {
  //    case 0 : MGain = value; break;
  //  }
  //}

  //----------

  void handle_process(const clap_process_t *process) final {
    float**   inputs  = process->audio_inputs[0].data32;
    float**   outputs = process->audio_outputs[0].data32;
    uint32_t  length  = process->frames_count;
    float     scale   = getParameterValue(0); // MGain;
    MIP_CopyStereoBuffer(outputs,inputs,length);
    MIP_ScaleStereoBuffer(outputs,scale,length);
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    setupParameters(myParameters,NUM_PARAMS);
    bool result = MIP_Plugin::init();
    if (result) {
      setDefaultParameterValues(myParameters,NUM_PARAMS);
    }
    return result;
  }

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
