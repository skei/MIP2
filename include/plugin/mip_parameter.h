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
protected:
//------------------------------

  //const clap_plugin_t*    MPlugin
  clap_param_info_t MInfo = {0};

//------------------------------
public:
//------------------------------

  MIP_Parameter() {
  }

  //----------

  virtual ~MIP_Parameter() {
  }

//------------------------------
public:
//------------------------------

  clap_param_info_t*    getParamInfo()      { return &MInfo; }

  clap_id               getId()             { return MInfo.id; }
  clap_param_info_flags getFlags()          { return MInfo.flags; }
  void*                 getCookie()         { return MInfo.cookie; }
  char*                 getName()           { return MInfo.name; }
  char*                 getModule()         { return MInfo.module; }
  double                getMinValue()       { return MInfo.min_value; }
  double                getMaxValue()       { return MInfo.max_value; }
  double                getDefaultValue()   { return MInfo.default_value; }

  void setId(clap_id id)                      { MInfo.id = id; }
  void setFlags(clap_param_info_flags flags)  { MInfo.flags = flags; }
  void setCookie(void * cookie)               { MInfo.cookie = cookie; }
  void setNeme(const char* name)              { strncpy(MInfo.name,name,CLAP_NAME_SIZE); }
  void setModule(const char* module)          { strncpy(MInfo.module,module,CLAP_MODULE_SIZE); }
  void setMinValue(double min_value)          { MInfo.min_value = min_value; }
  void setMaxValue(double max_value)          { MInfo.max_value = max_value; }
  void setDefaultValue(double default_value)  { MInfo.default_value = default_value; }

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
