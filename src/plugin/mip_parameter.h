#ifndef mip_parameter_included
#define mip_parameter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"

//----------------------------------------------------------------------

struct MIP_Parameter;
typedef MIP_Array<MIP_Parameter*> MIP_ParameterArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct MIP_Parameter {

//------------------------------

  const char* name        = "parameter";
  const char* label       = "";
  float       def_value   = 0.0;
  float       min_value   = 0.0;
  float       max_value   = 0.0;
  uint32_t    num_steps   = 0;

//------------------------------

  MIP_Parameter(const char* AName, float AValue=0.0) {
    name      = AName;
    def_value = AValue;
  }

//------------------------------

  virtual float from01(float AValue) {
    return AValue;
  }

  //----------

  virtual float to01(float AValue) {
    return AValue;
  }

  //----------

  virtual const char* displayText(char* ABuffer, float AValue) {
    sprintf(ABuffer,"%.3f",AValue);
    return ABuffer;
  }

  //----------

};

//----------------------------------------------------------------------
#endif
