#ifndef mip_win32_paths_included
#define mip_win32_paths_included
//----------------------------------------------------------------------


//----------------------------------------------------------------------
#endif




#if 0

/*
  linux exe
  MIP_DTrace("KGetHomePath %s\n",KGetHomePath(buffer)); // '/home/skei/'
  MIP_DTrace("KGetBaseFile %s\n",KGetBaseFile(buffer)); // '/home/skei/Desktop/skeilib/bin/skeilib_debug'
  MIP_DTrace("KGetBasePath %s\n",KGetBasePath(buffer)); // '/home/skei/Desktop/skeilib/bin/skeilib_debug./' !!
*/

//----------

#include <stdio.h>
#include <stdlib.h>

#ifdef MIP_LINUX
  #include <unistd.h> // readlink
  #include <dlfcn.h>
#endif

#ifdef MIP_WIN32
  #include <Shlobj.h>
#endif

#include "base/mip.h"
#include "base/mip_const.h"

//----------------------------------------------------------------------

char* MIP_GetHomePath(char* ABuffer) {
  //SStrcat(ABuffer, getenv("HOME"));
  ABuffer[0] = '\0';

  #ifdef MIP_LINUX
  char* env = getenv("HOME");
  if (env) MIP_Strcat(ABuffer,env);
  //KStrcat(ABuffer, (char*)"\\");
  MIP_Strcat(ABuffer, (char*)"/");
  #endif // MIP_LINUX

  #ifdef MIP_WIN32
  // https://msdn.microsoft.com/en-us/library/windows/desktop/bb762181(v=vs.85).aspx
  WCHAR wch[MAX_PATH];
  if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, wch))) {
    /*int l =*/ wcstombs(ABuffer,wch,MIP_MAX_STRING_LENGTH-1);
    MIP_Strcat(ABuffer, (char*)"\\");
  }
  #endif // MIP_WIN32

  return ABuffer;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

const char* MIP_GetBaseFile(char* ABuffer) {
  ABuffer[0] = '\0';

  #ifdef MIP_LINUX
    //#include <dlfcn.h>
    #if defined MIP_LIBRARY
      Dl_info dli;
      dladdr(__func__, &dli);
      //DTrace("SGetBaseFile: '" << dli.dli_fname << "'");
      //int32 len = SStrlen(dli.dli_fname);
      MIP_Strcpy(ABuffer,dli.dli_fname);
//      return ABuffer;//dli.dli_fname;
    #else //MIP_LIBRARY
      int32_t len = readlink("/proc/self/exe",ABuffer,MIP_MAX_PATH_LENGTH-1);
      if (len != -1) {
        ABuffer[len] = '\0';
//        return ABuffer;
      }
    #endif //MIP_LIBRARY
  #endif // MIP_LINUX

  #ifdef MIP_WIN32
    #if defined MIP_LIBRARY || defined MIP_PLUGIN_EXE
      //char filepath[MIP_MAX_PATH_LENGTH] = "";
      //GetModuleFileName(mip_global_WinInstance, filepath, MIP_MAX_PATH_LENGTH-1);
      //filepath[MIP_MAX_PATH_LENGTH-1] = '\0';
      GetModuleFileName(mip_global_WinInstance, ABuffer, MIP_MAX_PATH_LENGTH-1);
      //ABuffer[MIP_MAX_PATH_LENGTH-1] = '\0';
      //const char* slash = KStrrchr(filepath,'\\');
      //if (slash) {
      //  int len = (slash+1) - (char*)filepath;
      //  KStrncpy(ABuffer, filepath, len);
      //  ABuffer[len] = '\0';
      //}
      //else {
      //  KStrcat(ABuffer,(char*)".\\");
      //}
    #else
      KStrcat(ABuffer,(char*)".\\");
    #endif
  #endif // MIP_WIN32

  return ABuffer;//nullptr;
}


//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#ifdef MIP_LINUX

  #include <dlfcn.h>
  #include <unistd.h>

  const char* MIP_GetBasePath(char* ABuffer) {
    #if defined MIP_LIBRARY
      Dl_info dli;
      dladdr(__func__, &dli);
      const char* slash = MIP_Strrchr(dli.dli_fname, '/');
      if (slash) {
        int len = (slash + 1) - (char*)dli.dli_fname;
        MIP_Strncpy(ABuffer, dli.dli_fname, len);
        ABuffer[len] = '\0';
      }
      else {
        MIP_Strcat(ABuffer, (char*)"./");
      }
    #elif defined MIP_PLUGIN_EXE
      char filepath[MIP_MAX_PATH_LENGTH] = "";
      long rd_res = readlink("/proc/self/exe", filepath, MIP_MAX_PATH_LENGTH);
      filepath[MIP_MAX_PATH_LENGTH-1] = '\0';
      if (rd_res) {
        const char* slash = MIP_Strrchr(filepath, '/');
        if (slash) {
          int len = (slash + 1) - (char*)filepath;
          MIP_Strncpy(ABuffer, filepath, len);
          ABuffer[len] = '\0';
        }
        else {
          //SStrcat(ABuffer, (char*)"./");
          //SStrcat(ABuffer, (char*)"/"); // -> "/" -> root :-/
        }
      }
    #else
      MIP_Strcat(ABuffer, (char*)"./");
    #endif
    return ABuffer;
  }

#endif // MIP_LINUX

//------------------------------

#ifdef MIP_WIN32

  //#include <windows.h>

  char* MIP_GetBasePath(char* ABuffer) {
    #if defined MIP_LIBRARY || defined MIP_PLUGIN_EXE
      char filepath[MIP_MAX_PATH_LENGTH] = "";
      GetModuleFileName(mip_global_WinInstance, filepath, MIP_MAX_PATH_LENGTH-1);
      filepath[MIP_MAX_PATH_LENGTH-1] = '\0';
      const char* slash = KStrrchr(filepath,'\\');
      if (slash) {
        int len = (slash+1) - (char*)filepath;
        MIP_Strncpy(ABuffer, filepath, len);
        ABuffer[len] = '\0';
      }
      else {
        MIP_Strcat(ABuffer,(char*)".\\");
      }
    #else
      MIP_Strcat(ABuffer,(char*)".\\");
    #endif
    return ABuffer;
  }

#endif // MIP_WIN32

#endif // 0
