#ifndef mip_int_parameter_included
#define mip_int_parameter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_parameter.h"
#include "plugin/parameters/mip_float_parameter.h"

class MIP_IntParameter
: public MIP_FloatParameter {

public:

//  MIP_IntParameter(const char* AName, float AVal, float AMin, float AMax)
//  : MIP_FloatParameter(AName,AVal,AMin,AMax,1) {
//  }
//
//  MIP_IntParameter(const char* AName, const char* ALabel, float AVal, float AMin, float AMax)
//  : MIP_FloatParameter(AName,ALabel,AVal,AMin,AMax,1) {
//  }

  MIP_IntParameter(const char* AName, int32_t AVal, int32_t AMin, int32_t AMax, int32_t AStep=1)
  : MIP_FloatParameter(AName,AVal,AMin,AMax,AStep) {
  }

  MIP_IntParameter(const char* AName, const char* ALabel, int32_t AVal, int32_t AMin, int32_t AMax, int32_t AStep=1)
  : MIP_FloatParameter(AName,ALabel,AVal,AMin,AMax,AStep) {
  }

public:

  // AValue = 0..1

  const char* displayText(char* ABuffer, float AValue, uint32_t ANumDigits=0) override {
    //MIP_PRINT;
    sprintf(ABuffer,"%i",(int)from01(AValue));
    return ABuffer;
  }

};

//----------------------------------------------------------------------
#endif
