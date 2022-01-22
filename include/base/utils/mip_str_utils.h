#ifndef mip_str_utils_included
#define mip_str_utils_included
//----------------------------------------------------------------------

#include "mip.h"

//----------------------------------------------------------------------

// /home/skei/test.so -> test.so
// returns ptr to first character after last /

const char* MIP_GetFilenameFromPath(const char* APath) {
  if (APath) {
    const char* slash     = strrchr(APath,'/');
    const char* backslash = strrchr(APath,'\\');
    if (slash) {
      return slash + 1;
    }
    else if (backslash) {
      return backslash + 1;
    }
  }
  return nullptr;
}

//----------

// /home/skei/test.so -> /home/skei/
// makes a copy of dst, inserts a 0 after the last /

char* MIP_GetPathOnly(char* ADst, const char* ASrc) {
  if (ADst && ASrc) {
    strcpy(ADst,ASrc);
    char* slash     = strrchr(ADst,'/');
    char* backslash = strrchr(ADst,'\\');
    if (slash) {
      slash[1] = 0;
      return ADst;
    }
    else if (backslash) {
      backslash[1] = 0;
      return ADst;
    }
  }
  return nullptr;
}



//----------------------------------------------------------------------
#endif
