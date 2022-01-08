#ifndef kode_text_parameter_included
#define kode_text_parameter_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_parameter.h"
#include "plugin/parameters/kode_int_parameter.h"


class KODE_TextParameter
: public KODE_IntParameter {

private:

  const char** MStrings;

public:

  KODE_TextParameter(const char* AName, uint32_t AIndex, uint32_t ANum, const char** AText)
  : KODE_IntParameter(AName,AIndex,0,ANum-1) {
    MStrings = AText;
  }

public:

  virtual char* getDisplayText(float AValue, char* ABuffer, uint32_t ANumDigits=0) override {
    int32_t i = floorf(from01(AValue));
    strcpy(ABuffer,MStrings[i]);
    return ABuffer;
  }

};

//----------------------------------------------------------------------
#endif
