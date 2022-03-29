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

  // clap_param_info_t
  /*
    clap_id                id;
    clap_param_info_flags  flags;
    void*                  cookie;
    char                   name[CLAP_NAME_SIZE];
    char                   module[CLAP_MODULE_SIZE];
    double                 min_value;
    double                 max_value;
    double                 default_value;
  */

  // clap_param_info_flags
  /*
    CLAP_PARAM_IS_STEPPED
    CLAP_PARAM_IS_PER_NOTE
    CLAP_PARAM_IS_PER_CHANNEL
    CLAP_PARAM_IS_PER_PORT
    CLAP_PARAM_IS_PERIODIC
    CLAP_PARAM_IS_HIDDEN
    CLAP_PARAM_IS_BYPASS
    CLAP_PARAM_IS_READONLY
    CLAP_PARAM_IS_MODULATABLE
    CLAP_PARAM_REQUIRES_PROCESS
    CLAP_PARAM_IS_AUTOMATABLE
  */

//----------------------------------------------------------------------
//
// parameter
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

  MIP_Parameter(clap_param_info_t* param_info) {
    index               = -1;
    info.id             = param_info->id;
    info.flags          = param_info->flags;
    info.cookie         = this;//param_info->cookie;
    info.min_value      = param_info->min_value;
    info.max_value      = param_info->max_value;
    info.default_value  = param_info->default_value;
    strncpy(info.name,param_info->name,CLAP_NAME_SIZE);
    strncpy(info.module,param_info->module,CLAP_MODULE_SIZE);
  }

  //----------

  MIP_Parameter(uint32_t id, uint32_t flags, const char* name, const char* module, float minval, float maxval, float defval) {

    index               = -1;

    info.id             = id;
    info.flags          = flags;
    info.cookie         = this;
    info.min_value      = minval;
    info.max_value      = maxval;
    info.default_value  = defval;
    strncpy(info.name,name,CLAP_NAME_SIZE);
    strncpy(info.module,module,CLAP_MODULE_SIZE);
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

  //----------

  int32_t               getIndex()        { return index; }
  clap_param_info_t*    getParamInfo()    { return &info; }

  clap_id               getId()           { return info.id; }
  clap_param_info_flags getFlags()        { return info.flags; }
  void*                 getCookie()       { return info.cookie; }
  char*                 getName()         { return info.name; }
  char*                 getModule()       { return info.module; }
  double                getMinValue()     { return info.min_value; }
  double                getMaxValue()     { return info.max_value; }
  double                getDefaultValue() { return info.default_value; }

//------------------------------
public:
//------------------------------

  virtual double fromInternal(double value) {
    return value;
  }

  //----------

  virtual double toInternal(double value) {
    return value;
  }

  //----------

  virtual bool valueToText(double value, char* text, uint32_t size) {
    sprintf(text,"%.3f",value);
    return true;
  }

  //----------

  virtual bool textToValue(const char* text, double* value) {
    float f = atof(text);
    *value = f;
    return true;
  }


};

//----------------------------------------------------------------------
//
// int
//
//----------------------------------------------------------------------

class MIP_IntParameter
: public MIP_Parameter {

//------------------------------
public:
//------------------------------

  MIP_IntParameter(clap_param_info_t* param_info)
  : MIP_Parameter(param_info) {
  }

  //----------

  MIP_IntParameter(uint32_t id, uint32_t flags, const char* name, const char* module, int32_t minval, int32_t maxval, int32_t defval)
  : MIP_Parameter(id,flags,name,module,minval,maxval,defval) {
  }

  //----------

  virtual ~MIP_IntParameter() {
  }

//------------------------------
public:
//------------------------------

  bool valueToText(double value, char* text, uint32_t size) override {
    sprintf(text,"%i",(int)value);
    return true;
  }

};

//----------------------------------------------------------------------
//
// text
//
//----------------------------------------------------------------------

class MIP_TextParameter
: public MIP_IntParameter {

//------------------------------
private:
//------------------------------

  const char** MText = nullptr;

//------------------------------
public:
//------------------------------

  MIP_TextParameter(uint32_t id, uint32_t flags, const char* name, const char* module, int32_t minval, int32_t maxval, int32_t defval, const char** text)
  : MIP_IntParameter(id,flags,name,module,minval,maxval,defval) {
    MText = text;
    info.flags |= CLAP_PARAM_IS_STEPPED;
  }

  //----------

  bool valueToText(double value, char* text, uint32_t size) override {
    int i = (int)value;
    strncpy(text,MText[i],size);
    return true;
  }


};

//----------------------------------------------------------------------
#endif
