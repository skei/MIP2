
// nc -U -l -k /tmp/mip.socket
#define MIP_DEBUG_PRINT_SOCKET
#define MIP_NO_GUI
//#define MIP_PLUGIN_ALL
#define MIP_PLUGIN_CLAP
#define MIP_PLUGIN_EXE
#define MIP_PLUGIN_VST2
#define MIP_PLUGIN_VST3

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

class myDescriptor
: public MIP_Descriptor {

//------------------------------
public:
//------------------------------

  myDescriptor() {
    MName   = "Plugin";
    //MName   = "mip_debug";
    MAuthor = "Tor-Helge Skei";
    MEmail  = "tor.helge.skei@gmail.com";
    MUrl    = "https://torhelgeskei.com";
    MCanReceiveMidi = true;
    //MIP_Parameter* param = appendParameter( new MIP_Parameter( "Gain",   1 ));
    //param->setCanModulate();
    appendInputPort( new MIP_AudioPort( "Input",  2 ));
    appendOutputPort(new MIP_AudioPort( "Output", 2 ));
  }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  MIP_Descriptor* MDescriptor = nullptr;
  //float           MGain       = 0.0;
  //float           MGainMod    = 0.0;
  //int32_t         MNumNotes   = 0;

//------------------------------
public:
//------------------------------

  myPlugin(MIP_Descriptor* ADescriptor, MIP_HostProxy* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    MIP_PRINT;
    MDescriptor = ADescriptor;
  }

  //----------

  virtual ~myPlugin() {
  }

//------------------------------
public:
//------------------------------

  bool on_plugin_init() {
    return true;
  }

  void on_plugin_deinit() {
  }

  bool on_plugin_activate(float sample_rate, uint32_t min_frames, uint32_t max_frames) {
    return true;
  }

  void on_plugin_deactivate() {
  }

  bool on_plugin_start_processing() {
    return true;
  }

  void on_plugin_stop_processing() {
  }

  void on_plugin_process(MIP_ProcessContext* AContext) final {
    uint32_t  num   = AContext->num_samples;
    float*    in0   = AContext->inputs[0];
    float*    in1   = AContext->inputs[1];
    float*    out0  = AContext->outputs[0];
    float*    out1  = AContext->outputs[1];
    for (uint32_t i=0; i<num; i++) {
      *out0++ = *in0++;
      *out1++ = *in1++;
    }
  }

  void on_plugin_parameter(uint32_t AIndex, float AValue) final {
    //switch(AIndex) {
    //  case 0: MGain = AValue; break;
    //}
  }

  void on_plugin_modulation(uint32_t AIndex, float AValue) {
  }

  void on_plugin_midi(uint8_t AMsg1,uint8_t AMsg2,uint8_t AMsg3) {
  }

  void on_plugin_note_expression(uint32_t AType, uint32_t ANote, uint32_t AChannel, float AValue) {
  }

  MIP_Editor* on_plugin_open_editor(void* AParent) {
    return nullptr;
  }

  void on_plugin_close_editor() {
  }

  void on_plugin_update_editor() {
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

void MIP_RegisterPlugins(MIP_PluginList* AList) {
  MIP_Descriptor* descriptor = new myDescriptor();
  AList->appendPlugin(descriptor);
}

//----------

MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, MIP_Descriptor* ADescriptor, MIP_HostProxy* AHost) {
  if (AIndex == 0) return new myPlugin(ADescriptor,AHost);
  return nullptr;
}

