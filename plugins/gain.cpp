
// nc -U -l -k /tmp/mip.socket
#define MIP_DEBUG_PRINT_SOCKET
#define MIP_PLUGIN_CLAP
#define MIP_NO_GUI

//----------

#include "mip.h"
#include "plugin/mip_descriptor.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_plugin_entry.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class Gain_Descriptor
: public MIP_Descriptor {

//------------------------------
public:
//------------------------------

  Gain_Descriptor() {
    MName = "Gain";
    MAuthor = "Tor-HElge Skei";
    appendParameter( new MIP_Parameter("Gain",1) );
  }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class Gain_Plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  MIP_Descriptor* MDescriptor = nullptr;
  float           MGain       = 0.0;

//------------------------------
public:
//------------------------------

  Gain_Plugin(MIP_Descriptor* ADescriptor)
  : MIP_Plugin(ADescriptor) {
    MDescriptor = ADescriptor;
  }

  //----------

  virtual ~Gain_Plugin() {
  }

//------------------------------
public:
//------------------------------

  void on_plugin_parameter(uint32_t AIndex, float AValue) final {
    switch(AIndex) {
      case 0: MGain = AValue; break;
    }
  }

  //----------

  void on_plugin_process(MIP_ProcessContext* AContext) final {
    uint32_t  num   = AContext->num_samples;
    float*    in0   = AContext->inputs[0];
    float*    in1   = AContext->inputs[0];
    float*    out0  = AContext->outputs[0];
    float*    out1  = AContext->outputs[0];
    for (uint32_t i=0; i<num; i++) {
      *out0++ = *in0++ * MGain;
      *out1++ = *in1++ * MGain;
    }
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

void MIP_RegisterPlugins(MIP_PluginList* AList) {
  MIP_PRINT;
  MIP_Descriptor* descriptor = new Gain_Descriptor();
  AList->appendPlugin(descriptor);
}

//----------

MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, MIP_Descriptor* ADescriptor) {
  MIP_PRINT;
  return new Gain_Plugin(ADescriptor);
}

