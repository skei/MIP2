#ifndef kode_int_parameter_included
#define kode_int_parameter_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_parameter.h"
#include "plugin/parameters/kode_float_parameter.h"

class KODE_IntParameter
: public KODE_FloatParameter {

public:

//  KODE_IntParameter(const char* AName, float AVal, float AMin, float AMax)
//  : KODE_FloatParameter(AName,AVal,AMin,AMax,1) {
//  }
//
//  KODE_IntParameter(const char* AName, const char* ALabel, float AVal, float AMin, float AMax)
//  : KODE_FloatParameter(AName,ALabel,AVal,AMin,AMax,1) {
//  }

  KODE_IntParameter(const char* AName, int32_t AVal, int32_t AMin, int32_t AMax, int32_t AStep=1)
  : KODE_FloatParameter(AName,AVal,AMin,AMax,AStep) {
  }

  KODE_IntParameter(const char* AName, const char* ALabel, int32_t AVal, int32_t AMin, int32_t AMax, int32_t AStep=1)
  : KODE_FloatParameter(AName,ALabel,AVal,AMin,AMax,AStep) {
  }

public:

  // AValue = 0..1

  char* getDisplayText(float AValue, char* ABuffer, uint32_t ANumDigits=0) override {
    //KODE_PRINT;
    sprintf(ABuffer,"%i",(int)from01(AValue));
    return ABuffer;
  }

};

//----------------------------------------------------------------------
#endif
