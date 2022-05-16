#ifndef mip_parameter_manager_included
#define mip_parameter_manager_included
//----------------------------------------------------------------------

// TODO:

#include "mip.h"
#include "base/types/mip_queue.h"
#include "plugin/mip_parameter.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ParameterManager {

//------------------------------
protected:
//------------------------------

  MIP_ParameterArray  MParameters             = {};
  float*              MParameterValues        = nullptr;
  float*              MParameterModulations   = nullptr;


//------------------------------
public:
//------------------------------

  MIP_ParameterManager() {
  };

  //----------

  ~MIP_ParameterManager() {
    #ifndef MIP_NO_AUTODELETE
    deleteParameters();
    #endif
    free(MParameterValues);
    free(MParameterModulations);
  }

//------------------------------
public:
//------------------------------

  //----------
  // get
  //----------

  float getParameterModulation(uint32_t AIndex) {
    return MParameterModulations[AIndex];
  }

  float getParameterValue(uint32_t AIndex) {
    return MParameterValues[AIndex];
  }

  const clap_param_info_t* getParameterInfo(uint32_t AIndex) {
    return MParameters[AIndex]->getParamInfo();
  }

  //----------
  // set
  //----------

  void setParameterModulation(uint32_t AIndex, float AValue) {
    MParameterModulations[AIndex] = AValue;
  }

  void setParameterValue(uint32_t AIndex, float AValue) {
    MParameterValues[AIndex] = AValue;
  }

//------------------------------
public:
//------------------------------

  void setupParameters(clap_param_info_t* params, uint32_t num) {
    //MIP_PRINT;
    for (uint32_t i=0; i<num; i++) {
      MIP_Parameter* parameter = new MIP_Parameter(&params[i]);
      appendParameter(parameter);
    }
  }

  //----------

  MIP_Parameter* appendParameter(MIP_Parameter* AParameter) {
    uint32_t index = MParameters.size();
    AParameter->setIndex(index);
    MParameters.append(AParameter);
    return AParameter;
  }

  //----------

  void deleteParameters() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      delete MParameters[i];
    }
  }

  //----------

  /*
    called from MIP_Plugin.init()
    plugin overrides init(), and use addParamter()
    befomre calling this
  */

  void initParameters() {
    uint32_t num = MParameters.size();
    uint32_t size = num * sizeof(float);
    MParameterValues = (float*)malloc(size);
    MParameterModulations = (float*)malloc(size);
    setDefaultParameterValues();
  }

  //----------

  void setDefaultParameterValues() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      //const clap_param_info_t* info = &params[i];
      MParameterValues[i] = MParameters[i]->info.default_value;
      MParameterModulations[i] = 0.0;
    }
  }

//------------------------------
public:
//------------------------------


};


//----------------------------------------------------------------------
#endif
