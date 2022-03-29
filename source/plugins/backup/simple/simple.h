
// nc -U -l -k /tmp/mip.socket
#define MIP_DEBUG_PRINT_SOCKET
//#define MIP_DEBUG_PRINT_THREAD
//#define MIP_DEBUG_PRINT_TIME
#define MIP_NO_GUI
#define MIP_PLUGIN_ALL

#include "mip.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class myDescriptor
: public MIP_PluginDescriptor {

//------------------------------
public:
//------------------------------

  myDescriptor()
  : MIP_PluginDescriptor() {
    setName("myPlugin");
    setAuthor("mySelf");
    setVersion(0x00000001);
    appendInputPort("Input",2);
    appendOutputPort("Output",2);
    appendParameter("Gain",0.5);
  }

  //----------

  virtual ~myDescriptor() {
  }

};

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class myInstance
: public MIP_PluginInstance {

//------------------------------
private:
//------------------------------

  float MGain = 0.0;

//------------------------------
public:
//------------------------------

  myInstance(MIP_PluginDescriptor* ADescriptor)
  : MIP_PluginInstance(ADescriptor) {
  }

  //virtual ~myInstance() {
  //}

//------------------------------
public:
//------------------------------

  //bool on_plugin_init() final {
  //  return true;
  //}

  //void on_plugin_deinit() final {
  //}

  //bool on_plugin_activate(float ASampleRate, uint32_t AMinFrames, uint32_t AMaxFrames) final {
  //  return true;
  //}

  //void on_plugin_deactivate() final {
  //}

  //bool on_plugin_startProcessing() final {
  //  return true;
  //}

  //void on_plugin_stopProcessing() final {
  //}

  //----------

  uint32_t on_plugin_process(MIP_ProcessContext* AContext) final {
    float* in0 = AContext->inputs[0];
    float* in1 = AContext->inputs[1];
    float* out0 = AContext->outputs[0];
    float* out1 = AContext->outputs[1];
    for (uint32_t i=0; i<AContext->blocksize; i++) {
      *out0++ = *in0++ * MGain;
      *out1++ = *in1++ * MGain;
    }
    return 1;
  }

  //----------

  void on_plugin_parameter(uint32_t AIndex, float AValue) final {
    if (AIndex == 0) MGain = (AValue * AValue);
  }

  //void on_plugin_midi(uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) final {
  //}

  //void on_plugin_expression(uint32_t AKey, uint32_t AExpression, float AValue) final {
  //}

};

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

void MIP_RegisterPlugins() {
  MIP_PLUGIN_LIST.appendPlugin( new myDescriptor() );
}

//------------------------------
//
//------------------------------

MIP_PluginInstance* MIP_CreateInstance(uint32_t AIndex, MIP_PluginDescriptor* ADescriptor) {
  if (AIndex == 0) return new myInstance(ADescriptor);
  else return nullptr;
}

//------------------------------
//
//------------------------------

MIP_MAIN;

