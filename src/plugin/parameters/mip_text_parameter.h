#ifndef mip_text_parameter_included
#define mip_text_parameter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_parameter.h"
#include "plugin/parameters/mip_int_parameter.h"


class MIP_TextParameter
: public MIP_IntParameter {

private:

  const char** MStrings;

public:

  MIP_TextParameter(const char* AName, uint32_t AIndex, uint32_t ANum, const char** AText)
  : MIP_IntParameter(AName,AIndex,0,ANum-1) {
    MStrings = AText;
  }

public:

  const char* displayText(char* ABuffer, float AValue, uint32_t ANumDigits=0) override {
    int32_t i = floorf(from01(AValue));
    strcpy(ABuffer,MStrings[i]);
    return ABuffer;
  }

};

//----------------------------------------------------------------------
#endif
