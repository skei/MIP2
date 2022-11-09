#ifndef mip_logfile_included
#define mip_logfile_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/system/mip_file.h"
#include "base/system/mip_paths.h"
#include "base/system/mip_time.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifdef MIP_LOG

class MIP_LogFile {

//------------------------------
private:
//------------------------------

  MIP_File  MFile = {};
  char      MHomePath[256] = {0};
  char      MFilename[MIP_MAX_PATH_LENGTH]  = {0};
  char      MTempBuffer[MIP_MAX_PATH_LENGTH] = {0};

//------------------------------
public:
//------------------------------

  MIP_LogFile() {
    MIP_GetHomePath(MHomePath);
    sprintf(MFilename,"%s/Desktop/mip2_log.txt",MHomePath);
    // TODO: if directory "skei.audio" exists, open log there, else don't open..
    #ifdef MIP_LOG_APPEND
      MFile.open(MFilename,MIP_FILE_APPEND_TEXT);
    #else
      MFile.open(MFilename,MIP_FILE_WRITE_TEXT);
    #endif
    print_header();
  }

  //----------

  ~MIP_LogFile() {
    print("\n......................................................................\n\n");
    MFile.close();
  }

//------------------------------
public:
//------------------------------

  void print_header() {
    MIP_CurrentTime time = {0};
    MIP_GetLocalTime(&time);

    print("\n======================================================================\n\n");

    // basics

    print("Time: %02i.%02i.%04i (%i.%i.%i)\n",time.day,time.month,time.year,time.hour,time.minutes,time.seconds);

    #ifdef MIP_LINUX
      print("OS: Linux\n");
    #endif

    #ifdef MIP_WIN32
      print("OS: Windows\n");
    #endif

    print("Home path: %s\n",MHomePath);

    // gui

    const char* gui = "MIP_NO_GUI";
    const char* painter = "MIP_NO_PAINTER";

    #ifdef MIP_GUI_WIN32
      gui = "MIP_GUI_WIN32";
    #endif

    #ifdef MIP_GUI_XCB
      gui = "MIP_GUI_XCB";
    #endif

    #ifdef MIP_PAINTER_GDI
      painter = "MIP_PAINER_GDI";
    #endif

    #ifdef MIP_PAINTER_GLX
      painter = "MIP_PAINER_GLX";
    #endif

    #ifdef MIP_PAINTER_NANOVG
      painter = "MIP_PAINER_NANOVG";
    #endif

    #ifdef MIP_PAINTER_WGL
      painter = "MIP_PAINER_WGL";
    #endif

    #ifdef MIP_PAINTER_XCB
      painter = "MIP_PAINER_XCB";
    #endif

    print("GUI: %s\n",gui);
    print("Painter: %s\n",painter);

    //----------

    print("\n----------------------------------------------------------------------\n\n");

  }

//------------------------------
public:
//------------------------------

  //void operator () (const char* format, ...) {}

  //----------

  void print_string(char* str) {
    uint32_t len = strlen(str);
    MFile.write(str,len);
    MFile.flush();
}

  //----------

  void print(const char* format, ...) {
    if (MFile.isOpen()) {
      va_list args;
      va_start(args,format);
      vsprintf(MTempBuffer,format,args);
      va_end(args);
      print_string(MTempBuffer);
    }
  }

  void print_if(bool cond, const char* format, ...) {
    if (cond && MFile.isOpen()) {
      va_list args;
      va_start(args,format);
      vsprintf(MTempBuffer,format,args);
      va_end(args);
      print_string(MTempBuffer);
    }
  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------

#else

  class MIP_LogFile {
  public:
    void print(const char* format, ...) {}
  };

#endif

//----------------------------------------------------------------------
#endif
