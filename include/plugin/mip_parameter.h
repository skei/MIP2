#ifndef mip_parameter_included
#define mip_parameter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"
#include "plugin/clap/mip_clap.h"

class MIP_Parameter;
typedef MIP_Array<MIP_Parameter*> MIP_ParameterArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

/*
  param flags:
    CLAP_PARAM_IS_STEPPED
    CLAP_PARAM_IS_PERIODIC
    CLAP_PARAM_IS_HIDDEN
    CLAP_PARAM_IS_READONLY
    CLAP_PARAM_IS_BYPASS
    CLAP_PARAM_IS_AUTOMATABLE
    CLAP_PARAM_IS_AUTOMATABLE_PER_NOTE_ID
    CLAP_PARAM_IS_AUTOMATABLE_PER_KEY
    CLAP_PARAM_IS_AUTOMATABLE_PER_CHANNEL
    CLAP_PARAM_IS_AUTOMATABLE_PER_PORT
    CLAP_PARAM_IS_MODULATABLE
    CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID
    CLAP_PARAM_IS_MODULATABLE_PER_KEY
    CLAP_PARAM_IS_MODULATABLE_PER_CHANNEL
    CLAP_PARAM_IS_MODULATABLE_PER_PORT
    CLAP_PARAM_REQUIRES_PROCESS
*/

class MIP_Parameter {

//------------------------------
private:
//------------------------------

  clap_param_info_t MParamInfo = {
    .id             = 0,
    .flags          = CLAP_PARAM_IS_AUTOMATABLE,
    .cookie         = this,
    .name           = {},
    .module         = {},
    .min_value      = 0.0,
    .max_value      = 1.0,
    .default_value  = 0.0
  };

  double MValue       = 0.0;
  double MModulation  = 0.0;

//------------------------------
public:
//------------------------------

  MIP_Parameter() {
  }

  //----------

  MIP_Parameter(const clap_param_info_t* AInfo) {
    MParamInfo.id             = AInfo->id;
    MParamInfo.flags          = AInfo->flags;
    MParamInfo.cookie         = this;
    MParamInfo.min_value      = AInfo->min_value;
    MParamInfo.max_value      = AInfo->max_value;
    MParamInfo.default_value  = AInfo->default_value;
    strcpy(MParamInfo.name,AInfo->name);
    strcpy(MParamInfo.module,AInfo->module);
    MValue = MParamInfo.default_value;
  }

  //----------

  MIP_Parameter(clap_id AId, const char* AName, const char* AModule="", double AMinValue=0.0, double AMaxValue=1.0, double ADefValue=0.0, clap_param_info_flags AFlags=0) {
    MParamInfo.id             = AId;
    MParamInfo.flags          = AFlags;
    MParamInfo.cookie         = this;
    MParamInfo.min_value      = AMinValue;
    MParamInfo.max_value      = AMaxValue;
    MParamInfo.default_value  = ADefValue;
    strcpy(MParamInfo.name,AName);
    strcpy(MParamInfo.module,AModule);
    MValue = MParamInfo.default_value;
  }

  //----------

  virtual ~MIP_Parameter() {
  }

//------------------------------
public:
//------------------------------

  void setId(clap_id AId)                     { MParamInfo.id = AId; }
  void setFlags(clap_param_info_flags AFlags) { MParamInfo.flags = AFlags; }
  void setCookie(void* ACookie)               { MParamInfo.cookie = ACookie; }
  void setName(const char* AName)             { strcpy(MParamInfo.name,AName); }
  void setModule(const char* AModule)         { strcpy(MParamInfo.module,AModule); }
  void setMinValue(double AValue)             { MParamInfo.min_value = AValue; }
  void setMaxValue(double AValue)             { MParamInfo.max_value = AValue; }
  void setDefaultValue(double AValue)         { MParamInfo.default_value = AValue; }


  clap_id                   getId()           { return MParamInfo.id; }
  clap_param_info_flags     getFlags()        { return MParamInfo.flags; }
  void*                     getCookie()       { return MParamInfo.cookie; }
  const char*               getName()         { return MParamInfo.name; }
  const char*               getModule()       { return MParamInfo.module; }
  double                    getMinValue()     { return MParamInfo.min_value; }
  double                    getMaxValue()     { return MParamInfo.max_value; }
  double                    getDefaultValue() { return MParamInfo.default_value; }

  const clap_param_info_t*  getParamInfo()    { return &MParamInfo; }

  bool hasFlag(clap_param_info_flags AFlag)   { return (MParamInfo.flags & AFlag); }
  void setFlag(clap_param_info_flags AFlag)   { MParamInfo.flags |= AFlag; }
  void clearFlag(clap_param_info_flags AFlag) { MParamInfo.flags &= ~AFlag; }

//------------------------------
public:
//------------------------------

  void    setValue(double AValue)       { MValue = AValue; }
  void    setModulation(double AValue)  { MModulation = AValue; }

  double  getValue()                    { return MValue; }
  double  getModulation()               { return MModulation; }

  //----------

  double normalize(double AValue) {
    double range = MParamInfo.max_value - MParamInfo.min_value;
    MIP_Assert(range > 0.0);
    return (AValue - MParamInfo.min_value) / range;
  }

  //----------

  double denormalize(double AValue) {
    double range = MParamInfo.max_value - MParamInfo.min_value;
    MIP_Assert(range > 0.0);
    return MParamInfo.min_value + (AValue * range);
  }

  //----------

  //const char* getDisplayText(double value) {
  //  return "";
  //}

  const char* valueToText(double AValue, char* AText, uint32_t ASize) {
    if (hasFlag(CLAP_PARAM_IS_STEPPED)) {
      sprintf(AText,"%i\n",(int)AValue);
    }
    else {
      sprintf(AText,"%.3f\n",AValue);
    }
    return AText;
  }

  double textToValue(const char* AText) {
    double f = atof(AText);
    return f;
  }

  //----------

};

//----------------------------------------------------------------------
#endif
