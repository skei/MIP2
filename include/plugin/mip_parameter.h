#ifndef mip_parameter_included
#define mip_parameter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"
#include "audio/filters//mip_rc_filter.h"
#include "plugin/clap/mip_clap.h"

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
private:
//------------------------------

//  MIP_RcFilter<double>  MDezipper = {};

//------------------------------
public:
//------------------------------

  clap_param_info_t   info            = {0};
  int32_t             index           = -1;

  bool                mono_modulated  = false;
  bool                poly_modulated  = false;

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

  MIP_Parameter(uint32_t id, uint32_t flags, const char* name, const char* module, double minval, double maxval, double defval) {

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

  // for widgets, etc..

  virtual double from01(double value) {
    double range = info.max_value - info.min_value;
    return info.min_value + (value * range);
  }

  //----------

  virtual double to01(double value) {
    double range = info.max_value - info.min_value;
    return (value - info.min_value) / range;
  }

  //----------

  virtual bool valueToText(double value, char* text, uint32_t size) {
    sprintf(text,"%.3f",value);
    return true;
  }

  //----------

  virtual bool textToValue(const char* text, double* value) {
    double f = atof(text);
    *value = f;
    return true;
  }

//------------------------------
public:
//------------------------------

//  void setDezipperTime(double ms) {
//    MDezipper.setTime(ms);
//  }
//
//  void setDezipperValue(double value) {
//    MDezipper.setTarget(value);
//  }
//
//  // call this once (only) per sample
//  double getDezipperValue() {
//    return MDezipper.process();
//  }

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
//
// pow
//
//----------------------------------------------------------------------

// value^n

// appendParameter( new MIP_ParamPow( "pow/post", 440, 2, true,  20, 20000, 1 ));  // n^2 = 20.20000, default = 440
// appendParameter( new MIP_ParamPow( "pow",      5,   2, false, 1,  10,    1 ));  // 1^2..10^2, default = 5^2

class MIP_PowParameter
: public MIP_Parameter {

//------------------------------
private:
//------------------------------

  double   MPower  = 1.0f;
  bool    MPost   = true; // if true, calc ^n after range/scale conversion, else calc before..

//------------------------------
public:
//------------------------------

  //MIP_PowParameter(clap_param_info_t* param_info)
  //: MIP_Parameter(param_info) {
  //}

  //----------

  MIP_PowParameter(uint32_t id, uint32_t flags, const char* name, const char* module, double minval, double maxval, double defval, double APower, bool APost=false)
  : MIP_Parameter(id,flags,name,module,minval,maxval,defval) {
    MPower = APower;
    MPost = APost;
    //setup(defval);
  }

//------------------------------
private:
//------------------------------

  /*
  void setup(double AVal) {
    //MDefValue = to01(MDefValue);
    if (MPost) {
      double n = ((AVal - MMinValue) * MInvRange);
      if (MPower > 0) MDefValue = powf(n,1.0f/MPower);
      else MDefValue = 0.0f;
    }
    else {
      //if (MPower > 0) AVal = powf(AVal,1.0f/MPower);
      MDefValue = ((AVal - MMinValue) * MInvRange);
    }
  }
  */

//------------------------------
public:
//------------------------------

  double to01(double AValue) override {
    double result = 0.0f;
    if (MPost) {
      double n = MIP_Parameter::to01(AValue);
      if (MPower > 0) result = powf(n,1.0f/MPower);
    }
    else {
      if (MPower > 0) AValue = powf(AValue,1.0f/MPower);
      result = MIP_Parameter::to01(AValue);
    }
    return result;
  }

  //----------

  double from01(double AValue) override {
    double result = 0.0f;
    if (MPost) {
      double n = powf(AValue,MPower);
      result = MIP_Parameter::from01(n);
    }
    else {
      double n = MIP_Parameter::from01(AValue);
      result = powf(n,MPower);
    }
    return result;
  }

};

//----------------------------------------------------------------------
//
// pow2
//
//----------------------------------------------------------------------

// 2^n

// appendParameter( new KParamPow2("pow2", 4, 1, 16, 1 )); // (1..16)^2, default = 4^2

class MIP_Pow2Parameter
: public MIP_Parameter {

//------------------------------
private:
//------------------------------

  double   MPower  = 1.0f;

//------------------------------
public:
//------------------------------

  MIP_Pow2Parameter(uint32_t id, uint32_t flags, const char* name, const char* module, double minval, double maxval, double defval)
  : MIP_Parameter(id,flags,name,module,minval,maxval,defval) {
  }

//------------------------------
public:
//------------------------------

  double to01(double AValue) override {
    double result = 0.0f;
    AValue = powf(AValue,0.5f);
    result = MIP_Parameter::to01(AValue);
    return result;
  }

  //----------

  double from01(double AValue) override {
    double result = 0.0f;
    double n = MIP_Parameter::from01(AValue);
    result = powf(2.0f,n);
    return result;
  }

};

//----------------------------------------------------------------------
//
// Db
//
//----------------------------------------------------------------------

class MIP_DbParameter
: public MIP_Parameter {

//------------------------------
public:
//------------------------------

  MIP_DbParameter(uint32_t id, uint32_t flags, const char* name, const char* module, double minval, double maxval, double defval)
  : MIP_Parameter(id,flags,name,module,minval,maxval,defval) {
  }

//------------------------------
public:
//------------------------------

  /*

    float MIP_DbToVolume(float db) {
      return powf(10.0f,0.05f*db);
    }

    float MIP_DbToAmp(float g) {
      if (g > -144.0) return exp(g * 0.115129254);
      else return 0;
    }

    double dbtoa(double db) {
      return pow(2, db/6);
    }

    //converts decibel value to linear
    float amp_db = 20.f;         // 20 decibels
    signal *= KDB2Lin(amp_db);  // *= ~10.079
    #define MIP_Db2Lin(dB) ( MIP_Expf( DB2LOG*(dB) ) )

    //

    float MIP_VolumeToDb(float volume) {
      return 20.0f * log10f(volume);
    }

    double atodb(double a) {
      return 20*log10(a);
    }

    //converts linear value to decibel
    float dc_signal = 1.f;
    float amp_half = 0.5f;
    dc_signal *= amp_half;                  // half the amplitude
    float amp_db = KLin2DB(amp_half);      // amp_db = -6dbFS
    #define MIP_Lin2DB(lin) ( LOG2DB*MIP_Logf( (lin) ) )

  */

   // with 0 < x <= 4, plain = 20 * log(x)


  //----------

  // AValue = db

  double to01(double AValue) override {
    double value = powf(10.0f, 0.05f * AValue);
    return MIP_Parameter::to01(value);
  }

  //----------

  // returns db

  double from01(double AValue) override {
    double value = MIP_Parameter::from01(AValue);
    return 20.0f * log10f(value);
  }

};

//----------------------------------------------------------------------
//
// Hz
//
//----------------------------------------------------------------------

class MIP_HzParameter
: public MIP_Parameter {

//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  MIP_HzParameter(uint32_t id, uint32_t flags, const char* name, const char* module, double minval, double maxval, double defval)
  : MIP_Parameter(id,flags,name,module,minval,maxval,defval) {
  }

//------------------------------
public:
//------------------------------

//float MIP_NoteToHz(float ANote) {
//  return 440 * pow(2.0,(ANote-69)*MIP_INV12F);
//}


  double to01(double AValue) override {
    return MIP_Parameter::to01(AValue);
  }

  //----------

  double from01(double AValue) override {
    return MIP_Parameter::from01(AValue);
  }

};


//----------------------------------------------------------------------
#endif
