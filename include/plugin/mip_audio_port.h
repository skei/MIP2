#ifndef mip_audio_port_included
#define mip_audio_port_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"
//#include "audio/filters//mip_rc_filter.h"
#include "plugin/clap/mip_clap.h"

class MIP_AudioPort;
typedef MIP_Array<const clap_audio_port_info_t*> MIP_AudioPortArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

  // clap_audio_port_info_t
  /*
  */

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_AudioPort {

//------------------------------
private:
//------------------------------


//------------------------------
public:
//------------------------------

  clap_audio_port_info_t  info  = {0};
  int32_t                 index = -1;

//------------------------------
public:
//------------------------------

  MIP_AudioPort(clap_audio_port_info_t* audio_port_info) {
  }

  //----------

  MIP_AudioPort(uint32_t id, uint32_t flags, const char* name, const char* module, double minval, double maxval, double defval) {
  }

  //----------

  virtual ~MIP_AudioPort() {
  }

//------------------------------
public:
//------------------------------

//  void setIndex(int32_t AIndex)               { index = AIndex; }
//
//  void setId(clap_id id)                      { info.id = id; }
//  void setFlags(clap_param_info_flags flags)  { info.flags = flags; }
//  void setCookie(void * cookie)               { info.cookie = cookie; }
//  void setNeme(const char* name)              { strncpy(info.name,name,CLAP_NAME_SIZE); }
//  void setModule(const char* module)          { strncpy(info.module,module,CLAP_MODULE_SIZE); }
//  void setMinValue(double min_value)          { info.min_value = min_value; }
//  void setMaxValue(double max_value)          { info.max_value = max_value; }
//  void setDefaultValue(double default_value)  { info.default_value = default_value; }
//
//  //----------
//
//  int32_t               getIndex()        { return index; }
//  clap_param_info_t*    getParamInfo()    { return &info; }
//
//  clap_id               getId()           { return info.id; }
//  clap_param_info_flags getFlags()        { return info.flags; }
//  void*                 getCookie()       { return info.cookie; }
//  char*                 getName()         { return info.name; }
//  char*                 getModule()       { return info.module; }
//  double                getMinValue()     { return info.min_value; }
//  double                getMaxValue()     { return info.max_value; }
//  double                getDefaultValue() { return info.default_value; }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif
