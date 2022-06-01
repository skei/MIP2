#ifndef mip_parameter_included
#define mip_parameter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"
#include "base/types/mip_queue.h"
#include "base/utils/mip_math.h"
#include "extern/clap/clap.h"

class MIP_Parameter;
typedef MIP_Array<MIP_Parameter*> MIP_ParameterArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

/*
struct clap_param_info_t {
  clap_id               id;
  clap_param_info_flags flags;
  void*                 cookie;
  char                  name[CLAP_NAME_SIZE];
  char                  module[CLAP_MODULE_SIZE];
  double                min_value;
  double                max_value;
  double                default_value;
};
*/

//----------------------------------------------------------------------

class MIP_Parameter {

//------------------------------
private:
//------------------------------

  clap_param_info_t MInfo  = {
    .id             = 0,    // set this in appendParameter
    .flags          = CLAP_PARAM_IS_AUTOMATABLE,
    .cookie         = this,
    .name           = {0},
    .module         = {0},
    .min_value      = 0.0,
    .max_value      = 1.0,
    .default_value  = 0.0,
  };

  char    MDisplayText[32]    = {0};

  double  MValue              = 0.0;
  double  MModulation         = 0.0;
  double  MSmoothedValue      = 0.0;
  double  MSmoothedModulation = 0.0;
  double  MSmoothingFactor    = 0.001;
  bool    MIsSmoothed         = false;
  bool    MIsModulated        = false;


//------------------------------
public:
//------------------------------

  MIP_Parameter() {
    MValue = MInfo.default_value;
  }

  //----------

  MIP_Parameter(const char* AName, const char* AModule="") {
    strncpy(MInfo.name,AName,CLAP_NAME_SIZE);
    strncpy(MInfo.module,AModule,CLAP_PATH_SIZE);
    MValue = MInfo.default_value;
  }

  //----------

  MIP_Parameter(const char* AName, const char* AModule="", float AValue=0.0) {
    strncpy(MInfo.name,AName,CLAP_NAME_SIZE);
    strncpy(MInfo.module,AModule,CLAP_PATH_SIZE);
    MValue = AValue;
    MInfo.default_value = AValue;
  }

  //----------

  MIP_Parameter(const clap_param_info_t* AInfo) {
    memcpy(&MInfo,AInfo,sizeof(clap_param_info_t));
    MInfo.cookie = this;
  }

  //----------

  virtual ~MIP_Parameter() {
  }

//------------------------------
public:
//------------------------------

  void setValue(double AValue) {
    //MIP_Print("%.3f\n",AValue);
    MValue = AValue;
  }

  void setModulation(double AValue) {
    //MIP_Print("%.3f\n",AValue);
    MModulation = AValue;
  }

  //----------

  void setIsModulated(bool AState=true)       { MIsModulated = AState; }
  void setIsSmoothed(bool AState=true)        { MIsSmoothed = AState; }
  void setSmoothingFactor(double AFactor)     { MSmoothingFactor = AFactor; }

  void setId(uint32_t AId)                    { MInfo.id = AId; }
  void setFlags(clap_param_info_flags AFlags) { MInfo.flags = AFlags; }
  void setCookie(void* ACookie)               { MInfo.cookie = ACookie; }
  void setName(const char* AName)             { strcpy(MInfo.name,AName); }
  void setModule(const char* AModule)         { strcpy(MInfo.module,AModule); }
  void setMinValue(double AMinValue)          { MInfo.min_value = AMinValue; }
  void setMaxValue(double AMaxValue)          { MInfo.max_value = AMaxValue; }
  void setDefaultValue(double ADefaultValue)  { MInfo.default_value = ADefaultValue; }

//------------------------------
public:
//------------------------------

  clap_param_info_t*    getInfo()             { return &MInfo; }
  double                getValue()            { return MValue; }
  double                getModulation()       { return MModulation; }
  double                getSmoothingFactor()  { return MSmoothingFactor; }
  bool                  isSmoothed()          { return MIsSmoothed; }
  bool                  isModulated()         { return MIsModulated; }

  bool                  isStepped()           { return MInfo.flags & CLAP_PARAM_IS_STEPPED; }

  uint32_t              getId()               { return MInfo.id; }
  clap_param_info_flags getFlags()            { return MInfo.flags; }
  void*                 getCookie()           { return MInfo.cookie; }
  const char*           getName()             { return &MInfo.name[0]; }
  const char*           getModule()           { return &MInfo.module[0]; }
  double                getMinValue()         { return MInfo.min_value; }
  double                getMaxValue()         { return MInfo.max_value; }
  double                getDefaultValue()     { return MInfo.default_value; }

//------------------------------
public:
//------------------------------

  virtual bool valueToText(double AValue, char* AText, uint32_t ASize) {
    snprintf(AText,ASize,"%.3f",AValue);
    return true;
  }

  //----------

  virtual bool textToValue(const char* AText, double* AValue) {
    double f = atof(AText);
    *AValue = f;
    return true;
  }

//------------------------------
public:
//------------------------------

  double getNormalizedValue() {
    double range = (MInfo.max_value - MInfo.min_value);
    MIP_Assert(range > 0);
    return (MValue - MInfo.min_value) / range;
  }

  double normalizeValue(double AValue) {
    double range = (MInfo.max_value - MInfo.min_value);
    MIP_Assert(range > 0);
    return (AValue - MInfo.min_value) / range;
  }

  //----------

  void setNormalizedValue(double AValue) {
    double range = (MInfo.max_value - MInfo.min_value);
    MValue = (AValue * range) + MInfo.min_value;
    MValue = MIP_Clamp(MValue,MInfo.min_value,MInfo.max_value);
  }

  double denormalizeValue(double AValue) {
    double range = (MInfo.max_value - MInfo.min_value);
    double value = (AValue * range) + MInfo.min_value;
    return MIP_Clamp(value,MInfo.min_value,MInfo.max_value);
  }

  //----------

  double updateAndGetSmoothedValue(uint32_t ASteps=1) {
    if (!MIsSmoothed) return MValue;
    MSmoothedValue += (MValue - MSmoothedValue) * MSmoothingFactor;
    return MSmoothedValue;
  }

  //----------

  double getSmoothedValue() {
    if (!MIsSmoothed) return MValue;
    return MSmoothedValue;
  }

  //----------

  double updateAndGetSmoothedModulation(uint32_t ASteps=1) {
    if (!MIsSmoothed) return MModulation;
    MSmoothedModulation += (MModulation - MSmoothedModulation) * MSmoothingFactor;
    return MSmoothedModulation;
  }

  //----------

  double getSmoothedModulation() {
    if (!MIsSmoothed) return MModulation;
    return MSmoothedModulation;
  }

  //----------

//------------------------------
private:
//------------------------------

};

//----------------------------------------------------------------------
#endif
