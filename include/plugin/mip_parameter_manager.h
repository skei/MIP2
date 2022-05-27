#ifndef mip_parameter_manager_included
#define mip_parameter_manager_included
//----------------------------------------------------------------------

// TODO...

//----------------------------------------------------------------------

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
private:
//------------------------------

  MIP_ParameterArray  MParameters           = {};
  float*              MParameterValues      = nullptr;
  float*              MParameterModulations = nullptr;

  float               MSmoothFactor         = 0.001;
  float*              MSmoothedValues       = nullptr;
  float*              MSmoothedModulations  = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ParameterManager() {
  }

  //----------

  ~MIP_ParameterManager() {
    #ifndef MIP_NO_AUTODELETE
      deleteParameters();
    #endif
  }

//------------------------------
public:
//------------------------------

  void setParameterValue(uint32_t AIndex, float AValue) {
    MParameterValues[AIndex] = AValue;
  }

  //----------

  void setParameterModulation(uint32_t AIndex, float AValue) {
    MParameterModulations[AIndex] = AValue;
  }

//------------------------------
public:
//------------------------------

  float getParameterValue(uint32_t AIndex) {
    if (MParameters[AIndex]->info.flags & CLAP_PARAM_IS_AUTOMATABLE) {
      return MSmoothedValues[AIndex];
    }
    else {
      return MParameterValues[AIndex];
    }
  }

  //----------

  float getParameterModulation(uint32_t AIndex) {
    if (MParameters[AIndex]->info.flags & CLAP_PARAM_IS_MODULATABLE) {
      return MSmoothedModulations[AIndex];
    }
    else {
      return MParameterModulations[AIndex];
    }
  }

  //----------

  float getRawParameterValue(uint32_t AIndex) {
    return MParameterValues[AIndex];
  }

  //----------

  float getRawParameterModulation(uint32_t AIndex) {
    return MParameterModulations[AIndex];
  }

//------------------------------
public:
//------------------------------

  // assumes MParameters array has been set up

  void init() {
    uint32_t num = MParameters.size();
    uint32_t size = num * sizeof(float);
    MParameterValues = (float*)malloc(size);
    MParameterModulations = (float*)malloc(size);
    for (uint32_t i=0; i<num; i++) {
      float v = MParameters[i]->info.default_value;
      setParameterValue(i,v);
      setParameterModulation(i,0);
    }
  }

  //----------

  void destroy() {
    free(MParameterValues);
    free(MParameterModulations);
  }

  //----------

  void process(uint32_t ASteps=1) {
    uint32_t num = MParameters.size();
    if (ASteps == 1) {
      for (uint32_t i=0; i<num; i++) {
        // MValue += (MTarget - MValue) * MWeight;
        if (MParameters[i]->info.flags & CLAP_PARAM_IS_AUTOMATABLE) {
          MSmoothedValues[i] += ( MParameterValues[i] - MSmoothedValues[i] ) * MSmoothFactor;
        }
        if (MParameters[i]->info.flags & CLAP_PARAM_IS_MODULATABLE) {
          MSmoothedModulations[i] += ( MParameterModulations[i] - MSmoothedModulations[i] ) * MSmoothFactor;
        }
      }
    }
    else {
      // MValue += (MTarget - MValue) * MWeight;
      // MValue += (MTarget - MValue) * (1 - pow(1 - MWeight, ASteps));
      float factor = (1 - pow(1 - MSmoothFactor, ASteps));
      for (uint32_t i=0; i<num; i++) {
        if (MParameters[i]->info.flags & CLAP_PARAM_IS_AUTOMATABLE) {
          MSmoothedValues[i] += ( MParameterValues[i] - MSmoothedValues[i] ) * factor;
        }
        if (MParameters[i]->info.flags & CLAP_PARAM_IS_MODULATABLE) {
          MSmoothedModulations[i] += ( MParameterModulations[i] - MSmoothedModulations[i] ) * factor;
        }
      }
    }
  }

//------------------------------
public:
//------------------------------

  MIP_Parameter* appendParameter(clap_param_info_t* param_info) {
    MIP_Parameter* parameter = new MIP_Parameter(param_info);
    uint32_t index = MParameters.size();
    parameter->setIndex(index);
    MParameters.append(parameter);
    return parameter;
  }

  //----------

  void deleteParameters() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      if (MParameters[i]) delete MParameters[i];
      MParameters[i] = nullptr;
    }
  }

  //----------

  void setupParameters(clap_param_info_t* params, uint32_t num) {
    for (uint32_t i=0; i<num; i++) {
      appendParameter(&params[i]);
    }
  }

  //----------

  void setDefaultParameterValues() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      MParameterValues[i] = MParameters[i]->info.default_value;
      MParameterModulations[i] = 0.0;
    }
  }

};

//----------------------------------------------------------------------
#endif
