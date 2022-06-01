#ifndef mip_parameter_manager_included
#define mip_parameter_manager_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_parameter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ParameterListener {
  int dummy = 0;
  //virtual void on_parameter_listener_beginGesture()  {}
  //virtual void on_parameter_listener_endGesture()    {}
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ParameterManager {

//------------------------------
private:
//------------------------------

  MIP_ParameterListener*  MListener   = nullptr;
  MIP_ParameterArray      MParameters = {};


//------------------------------
public:
//------------------------------

  MIP_ParameterManager(MIP_ParameterListener* AListener) {
    MListener = AListener;
  }

  //----------

  virtual ~MIP_ParameterManager() {
    #ifndef MIP_NO_AUTODELETE
      deleteParameters();
    #endif
  }

//------------------------------
public:
//------------------------------

  void appendParameter(MIP_Parameter* AParameter, bool AKeepIndex=false) {
    if (!AKeepIndex) {
      uint32_t index = MParameters.size();
      AParameter->setId(index);
    }
    MParameters.append(AParameter);
  }

  //----------

  void deleteParameters() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      delete MParameters[i];
    }
  }

  //----------

  uint32_t parameterCount() {
    return MParameters.size();
  }

  //----------

  MIP_Parameter* getParameter(uint32_t AIndex) {
    return MParameters[AIndex];
  }

  //----------

  void setDefaultParameterValues() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      MIP_Parameter* param = MParameters[i];
      double value = param->getDefaultValue();
      //MIP_Print("%i. %f\n",i,value);
      param->setValue(value);
    }
  }

  //----------

  double getParameterValue(uint32_t AIndex) {
    return MParameters[AIndex]->getValue();
  }

  double getParameterModulation(uint32_t AIndex) {
    return MParameters[AIndex]->getModulation();
  }

  void setParameterValue(uint32_t AIndex, double AValue) {
    MParameters[AIndex]->setValue(AValue);
  }

  void setParameterModulation(uint32_t AIndex, double AValue) {
    MParameters[AIndex]->setModulation(AValue);
  }


//------------------------------
public:
//------------------------------

  //void init() {
  //}

  //----------

  //void reset() {
  //}

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    return true;
  }

  //----------

//------------------------------
public: // events
//------------------------------

  void handle_value_event(clap_event_param_value_t* event) {
    handle_parameter(event->param_id,event->value);
  }

  //----------

  void handle_modulation_event(clap_event_param_mod_t* event) {
    handle_modulation(event->param_id,event->amount);
  }

//------------------------------
public:
//------------------------------

  void handle_parameter(uint32_t AIndex, double AValue) {
//    MIP_Print("AIndex %i AValue %.3f\n",AIndex,AValue);
    MParameters[AIndex]->setValue(AValue);
  }

  //----------

  void handle_modulation(uint32_t AIndex, double AValue) {
//    MIP_Print("AIndex %i AValue %.3f\n",AIndex,AValue);
    MParameters[AIndex]->setModulation(AValue);
  }



};

//----------------------------------------------------------------------
#endif

