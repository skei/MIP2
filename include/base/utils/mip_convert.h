#ifndef mip_convert_included
#define mip_convert_included
//----------------------------------------------------------------------

#include <stdio.h>  // sprintf
#include <stdlib.h> // atof

//----------------------------------------------------------------------

//------------------------------
#ifdef MIP_NO_STDLIB
//------------------------------

  #error not implemented

//------------------------------
#else // !MIP_NO_STDLIB
//------------------------------

char* MIP_FloatToString(char* AString, float AValue, uint32_t ADigits=3) {
  char format_string[16];
  format_string[0] = '%';
  format_string[1] = '.';
  format_string[2] = 48 + ADigits;
  format_string[3] = 'f';
  format_string[4] = 0;
  sprintf(AString,format_string,AValue);
  return AString;
}

//----------

char* MIP_IntToString(char* AString, int32_t AValue) {
  char format_string[16];
  format_string[0] = '%';
  format_string[1] = 'i';
  format_string[2] = 0;
  sprintf(AString,format_string,AValue);
  return AString;
}

//----------

float MIP_StringToFloat(char* AString) {
  return atof(AString);
}

//----------

int32_t MIP_StringToInt(char* AString) {
  return atoi(AString);
}

//------------------------------
#endif // MIP_NO_STDLIB
//------------------------------

//----------------------------------------------------------------------
#endif
