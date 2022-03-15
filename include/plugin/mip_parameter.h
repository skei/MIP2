#ifndef mip_parameter_included
#define mip_parameter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"

class MIP_Parameter;
typedef MIP_Array<MIP_Parameter*> MIP_ParameterArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Parameter {

//------------------------------
public:
//------------------------------

  clap_param_info_t info  = {0};
  int32_t          index  = -1;

//------------------------------
public:
//------------------------------

  MIP_Parameter(clap_id AId, clap_param_info_flags AFlags=CLAP_PARAM_IS_AUTOMATABLE, const char* AName="", const char* AModule="", double AMinValue=0, double AMaxValue=1, double ADefValue=0) {
    index = -1;
    info.id = AId;
    info.flags = AFlags;
    info.cookie = this;
    strncpy(info.name,AName,CLAP_NAME_SIZE);
    strncpy(info.module,AModule,CLAP_MODULE_SIZE);
    info.min_value = AMinValue;
    info.max_value = AMaxValue;
    info.default_value = ADefValue;
  }

  //----------

  virtual ~MIP_Parameter() {
  }

//------------------------------
public:
//------------------------------

  void setIndex(int32_t AIndex)               { index = AIndex; }

  void setId(clap_id id)                      { info.id = id; }
  void setFlags(clap_param_info_flags flags)  { info.flags = flags; }
  void setCookie(void * cookie)               { info.cookie = cookie; }
  void setNeme(const char* name)              { strncpy(info.name,name,CLAP_NAME_SIZE); }
  void setModule(const char* module)          { strncpy(info.module,module,CLAP_MODULE_SIZE); }
  void setMinValue(double min_value)          { info.min_value = min_value; }
  void setMaxValue(double max_value)          { info.max_value = max_value; }
  void setDefaultValue(double default_value)  { info.default_value = default_value; }

  int32_t               getIndex()          { return index; }
  clap_param_info_t*    getParamInfo()      { return &info; }

  clap_id               getId()             { return info.id; }
  clap_param_info_flags getFlags()          { return info.flags; }
  void*                 getCookie()         { return info.cookie; }
  char*                 getName()           { return info.name; }
  char*                 getModule()         { return info.module; }
  double                getMinValue()       { return info.min_value; }
  double                getMaxValue()       { return info.max_value; }
  double                getDefaultValue()   { return info.default_value; }


//------------------------------
public:
//------------------------------

  virtual double normalize(double value) {
    return value;
  }

  //----------

  virtual double denormalize(double value) {
    return value;
  }

  //----------

  virtual bool value_to_text(double value, char *text, uint32_t size) {
    return false;
  }

  //----------

  virtual bool text_to_value(const char* text, double* value) {
    return false;
  }


};

//----------------------------------------------------------------------
#endif
