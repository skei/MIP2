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
: public MIP_ClapPlugin {

//------------------------------
protected:
//------------------------------

  const clap_plugin_descriptor_t* MDescriptor       = nullptr;
  const clap_host_t*              MHost             = nullptr;
  uint32_t                        MNumParams        = 0;
  float*                          MParamVal         = nullptr;
  float*                          MParamMod         = nullptr;

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
  }

  //----------

  virtual ~MIP_Plugin() {
    free(MParamVal);
    free(MParamMod);
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


};

//----------------------------------------------------------------------
#endif
