#ifndef mip_parameter_included
#define mip_parameter_included
//----------------------------------------------------------------------

// TODO:
//   setSmoothingFactor()
//   getSmoothedValue()

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

  double  MValue          = 0.0;
  double  MModulation     = 0.0;
  int32_t MIndex          = -1;

  bool    MModulated      = false;
  bool    MPolyModulated  = false;

  const char** MStrings = nullptr;

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


  void    setCookie(void* ACookie)                { MParamInfo.cookie = ACookie; }
  void    setDefaultValue(double AValue)          { MParamInfo.default_value = AValue; }
  void    setFlags(clap_param_info_flags AFlags)  { MParamInfo.flags = AFlags; }
  void    setId(clap_id AId)                      { MParamInfo.id = AId; }
  void    setIndex(int32_t AIndex)                { MIndex = AIndex; }
  void    setMaxValue(double AValue)              { MParamInfo.max_value = AValue; }
  void    setMinValue(double AValue)              { MParamInfo.min_value = AValue; }
  void    setModulated(bool AMod)                 { MModulated = AMod; }
  void    setModulation(double AValue)            { MModulation = AValue; }
  void    setModule(const char* AModule)          { strcpy(MParamInfo.module,AModule); }
  void    setName(const char* AName)              { strcpy(MParamInfo.name,AName); }
  void    setPolyModulated(bool AMod)             { MPolyModulated = AMod; }
  void    setValue(double AValue)                 { MValue = AValue; }

  void    setFlag(clap_param_info_flags AFlag)    { MParamInfo.flags |= AFlag; }
  void    clearFlag(clap_param_info_flags AFlag)  { MParamInfo.flags &= ~AFlag; }
  bool    hasFlag(clap_param_info_flags AFlag)    { return (MParamInfo.flags & AFlag); }

  void*                     getCookie()           { return MParamInfo.cookie; }
  double                    getDefaultValue()     { return MParamInfo.default_value; }
  clap_param_info_flags     getFlags()            { return MParamInfo.flags; }
  clap_id                   getId()               { return MParamInfo.id; }
  int32_t                   getIndex()            { return MIndex; }
  double                    getMaxValue()         { return MParamInfo.max_value; }
  double                    getMinValue()         { return MParamInfo.min_value; }
  bool                      getModulated()        { return MModulated; }
  double                    getModulation()       { return MModulation; }
  const char*               getModule()           { return MParamInfo.module; }
  const char*               getName()             { return MParamInfo.name; }
  const clap_param_info_t*  getParamInfo()        { return &MParamInfo; }
  bool                      getPolyModulated()    { return MPolyModulated; }
  double                    getValue()            { return MValue; }

//------------------------------
public:
//------------------------------

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

  const char* valueToText(double AValue, char* AText, uint32_t ASize) {
    if (hasFlag(CLAP_PARAM_IS_STEPPED)) {
      snprintf(AText,ASize,"%i",(int)AValue);
    }
    else {
      snprintf(AText,ASize,"%.2f",AValue);
    }
    return AText;
  }

  //----------

  double textToValue(const char* AText) {
    double f = atof(AText);
    return f;
  }

  //----------

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_IntParameter
: public MIP_Parameter {

public:

  MIP_IntParameter()
  : MIP_Parameter() {
  }

  MIP_IntParameter(const clap_param_info_t* AInfo)
  : MIP_Parameter(AInfo) {
  }

  MIP_IntParameter(clap_id AId, const char* AName, const char* AModule="", double AMinValue=0.0, double AMaxValue=1.0, double ADefValue=0.0, clap_param_info_flags AFlags=0)
  : MIP_Parameter(AId,AName,AModule,AMinValue,AMaxValue,ADefValue,AFlags) {
  }

  //----------

  virtual ~MIP_IntParameter() {
  }

public:

  const char* valueToText(double AValue, char* AText, uint32_t ASize) {
    if (hasFlag(CLAP_PARAM_IS_STEPPED)) {
      snprintf(AText,ASize,"%i",(int)AValue);
    }
    else {
      snprintf(AText,ASize,"%.2f",AValue);
    }
    return AText;
  }

  //----------

  double textToValue(const char* AText) {
    double f = atof(AText);
    return f;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_TextParameter
: public MIP_IntParameter {

public:

  MIP_TextParameter()
  : MIP_IntParameter() {
  }

  //----------

  MIP_TextParameter(const clap_param_info_t* AInfo)
  : MIP_IntParameter(AInfo) {
  }

  //----------

  MIP_TextParameter(clap_id AId, const char* AName, const char* AModule="", double AMinValue=0.0, double AMaxValue=1.0, double ADefValue=0.0, clap_param_info_flags AFlags=0, const char** AText=nullptr)
  : MIP_IntParameter(AId,AName,AModule,AMinValue,AMaxValue,ADefValue,AFlags) {
  }

  //----------

  virtual ~MIP_TextParameter() {
  }

};

//----------------------------------------------------------------------
#endif
