
// nc -U -l -k /tmp/mip.socket
#define MIP_DEBUG_PRINT_SOCKET
#define MIP_NO_GUI
#define MIP_PLUGIN_CLAP
//#define MIP_PLUGIN_VST2
//#define MIP_PLUGIN_VST3

#define MIP_DEBUG_CLAP

//----------

#include "mip.h"
#include "plugin/mip_descriptor.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_host_proxy.h"
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
    MName   = "Gain";
    //MName   = "mip_debug";
    MAuthor = "Tor-Helge Skei";
    MEmail  = "tor.helge.skei@gmail.com";
    MUrl    = "https://torhelgeskei.com";
    MCanReceiveMidi = true;
    MIP_Parameter* param = appendParameter( new MIP_Parameter( "Gain",   1 ));
    param->setCanModulate();
    appendInputPort( new MIP_AudioPort( "Input",  2 ));
    appendOutputPort(new MIP_AudioPort( "Output", 2 ));
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
  float           MGainMod    = 0.0;
  int32_t         MNumNotes   = 0;

//------------------------------
public:
//------------------------------

  Gain_Plugin(MIP_Descriptor* ADescriptor, MIP_HostProxy* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    //MIP_PRINT;
    MDescriptor = ADescriptor;
  }

  //----------

  virtual ~Gain_Plugin() {
  }

//------------------------------
public:
//------------------------------

  void on_plugin_midi(uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) final {
    //MIP_PRINT;
    switch (AMsg1 & 0xF0) {
      case MIP_MIDI_NOTE_ON:
        MNumNotes += 1;
        //MHost->sendMidi(AMsg1,AMsg2,AMsg3);
        break;
      case MIP_MIDI_NOTE_OFF: // note on
        MNumNotes -= 1;
        //MHost->sendMidi(AMsg1,AMsg2,AMsg3);
        break;
    }
  }

  void on_plugin_parameter(uint32_t AIndex, float AValue) final {
    switch(AIndex) {
      case 0: MGain = AValue; break;
    }
  }

  //----------

  void on_plugin_modulation(uint32_t AIndex, float AValue) final {
    switch(AIndex) {
      case 0: MGainMod = AValue; break;
    }
  }

  //----------

  void on_plugin_process(MIP_ProcessContext* AContext) final {
    uint32_t  num   = AContext->num_samples;
    float*    in0   = AContext->inputs[0];
    float*    in1   = AContext->inputs[1];
    float*    out0  = AContext->outputs[0];
    float*    out1  = AContext->outputs[1];
    for (uint32_t i=0; i<num; i++) {
      if (MNumNotes > 0) {
        *out0++ = 0.0;
        *out1++ = 0.0;
      }
      else {
        *out0++ = *in0++ * (MGain + MGainMod);
        *out1++ = *in1++ * (MGain + MGainMod);
      }
    }
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

void MIP_RegisterPlugins(MIP_PluginList* AList) {
  MIP_Descriptor* descriptor = new Gain_Descriptor();
  AList->appendPlugin(descriptor);
}

//----------

MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, MIP_Descriptor* ADescriptor, MIP_HostProxy* AHost) {
  if (AIndex == 0) return new Gain_Plugin(ADescriptor,AHost);
  return nullptr;
}

