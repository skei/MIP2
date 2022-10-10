#ifndef mip_linux_paths_included
#define mip_linux_paths_included
//----------------------------------------------------------------------

// parts of the code by lubomir i ivanov (for axonlib)
// used with permission

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // readlink
#include <dlfcn.h>

/*
  MIP_GetHomePath    '/home/<user>/'
  MIP_GetExeFilename '/home/<user>/Desktop/skeilib/bin/skeilib_debug'
  MIP_GetExePath     '/home/<user>/Desktop/skeilib/bin/'
*/

//----------------------------------------------------------------------

char* MIP_GetHomePath(char* ABuffer) {
  ABuffer[0] = '\0';
  char* env = getenv("HOME");
  if (env) strcat(ABuffer,env);
  //KStrcat(ABuffer, (char*)"\\");
  strcat(ABuffer, (char*)"/");
  return ABuffer;
}

//----------

const char* MIP_GetExeFilename(char* ABuffer) {
  ABuffer[0] = '\0';
  int32_t len = readlink("/proc/self/exe",ABuffer,MIP_MAX_PATH_LENGTH-1);
  if (len != -1) {
    ABuffer[len] = '\0';
  }
  return ABuffer;
}

//----------

const char* MIP_GetExePath(char* ABuffer) {
  char filepath[MIP_MAX_PATH_LENGTH] = "";
  long rd_res = readlink("/proc/self/exe", filepath, MIP_MAX_PATH_LENGTH);
  filepath[MIP_MAX_PATH_LENGTH-1] = '\0';
  if (rd_res) {
    const char* slash = strrchr(filepath, '/');
    if (slash) {
      int len = (slash + 1) - (char*)filepath;
      strncpy(ABuffer, filepath, len);
      ABuffer[len] = '\0';
    }
    else {
      //SStrcat(ABuffer, (char*)"./");
      //SStrcat(ABuffer, (char*)"/"); // -> "/" -> root :-/
    }
  }
  return ABuffer;
}

//----------

const char* MIP_GetLibFilename(char* ABuffer) {
  ABuffer[0] = '\0';
  Dl_info dli;
  dladdr(__func__, &dli);
  //int32 len = MIP_Strlen(dli.dli_fname);
  strcpy(ABuffer,dli.dli_fname);
  return ABuffer;//nullptr;
}

//----------

const char* MIP_GetLibPath(char* ABuffer) {
  Dl_info dli;
  dladdr(__func__, &dli);
  const char* slash = strrchr(dli.dli_fname, '/');
  if (slash) {
    int len = (slash + 1) - (char*)dli.dli_fname;
    strncpy(ABuffer, dli.dli_fname, len);
    ABuffer[len] = '\0';
  }
  else {
    strcat(ABuffer, (char*)"./");
  }
  return ABuffer;
}

//----------------------------------------------------------------------

#ifdef MIP_PLUGIN
  #define MIP_GetBaseFilename  MIP_GetLibFilename
  #define MIP_GetBasePath      MIP_GetLibPath
#else
  #define MIP_GetBaseFilename  MIP_GetExeFilename
  #define MIP_GetBasePath      MIP_GetExePath
#endif

//----------------------------------------------------------------------
#endif
