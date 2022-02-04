#ifndef mip_plugin_included
#define mip_plugin_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_editor.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_entry.h"

#include "plugin/wrapper/mip_exe_wrapper.h"
#include "plugin/wrapper/mip_lv2_wrapper.h"
#include "plugin/wrapper/mip_vst2_wrapper.h"
#include "plugin/wrapper/mip_vst3_wrapper.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Plugin
: public MIP_ClapPlugin
, public MIP_EditorListener {

//------------------------------
protected:
//------------------------------

  const clap_plugin_descriptor_t* MDescriptor       = nullptr;
  const clap_host_t*              MHost             = nullptr;
  uint32_t                        MNumParams        = 0;
  float*                          MParamVal         = nullptr;
  float*                          MParamMod         = nullptr;
//  MIP_Timer*                      MTimer            = nullptr;

//------------------------------
public:
//------------------------------

  MIP_Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost, uint32_t ANumParams)
  : MIP_ClapPlugin(ADescriptor,AHost) {
    MDescriptor       = ADescriptor;
    MHost             = AHost;
    MNumParams        = ANumParams;
    MParamVal         = (float*)malloc(ANumParams * sizeof(float));
    MParamMod         = (float*)malloc(ANumParams * sizeof(float));
//    MTimer            = new MIP_Timer(this);
  }

  //----------

  virtual ~MIP_Plugin() {
    free(MParamVal);
    free(MParamMod);
//    delete MTimer;
  }

//------------------------------
public:
//------------------------------

  float getParamMod(uint32_t AIndex) { return MParamMod[AIndex]; }
  float getParamVal(uint32_t AIndex) { return MParamVal[AIndex]; }

  void  setParamMod(uint32_t AIndex, float AValue) { MParamMod[AIndex] = AValue; }
  void  setParamVal(uint32_t AIndex, float AValue) { MParamVal[AIndex] = AValue; }

//------------------------------
private:
//------------------------------

  // MIP_TimerListener

//  void on_timerCallback(void) final {
//    //MIP_Print("timer\n");
//    //if (MEditor) MEditor->refresh();
//    //flushHostParams();
//  }

//------------------------------
private:
//------------------------------

  // editor listener

  void updateParameterFromEditor(uint32_t AIndex, float AValue) final {
    MIP_Print("index: %i value: %.3f\n",AIndex,AValue);
  }

  //----------

  void timerFromEditor() final {
    //MIP_PRINT;
  }

  //----------

//  void setEditorWidgetValues() {
//    for (uint32_t i=0; i<MNumParams; i++) {
//      float v = MParamVal[i];
//      MEditor->setParameterValue(i,v);
//    }
//  }

};

//----------------------------------------------------------------------
#endif
