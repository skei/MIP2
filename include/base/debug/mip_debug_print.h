#ifndef mip_debug_print_included
#define mip_debug_print_included
//----------------------------------------------------------------------

/*
  see the socket output by using this in a console:
  nc -U -l -k /tmp/mip.socket
*/

#include <stdarg.h>   // va_
#include "mip.h"

//----------

void MIP_NoPrint(const char*,...) {}

//----------------------------------------------------------------------

#ifdef MIP_DEBUG

  #ifdef MIP_DEBUG_PRINT_TIME
    //#include <time.h>     // timer
    #include <sys/time.h> // gettimeofday
    double _mip_debug_start_time = 0.0;
  #endif

  //#ifdef MIP_DEBUG_PRINT_THREAD
  //  #include <sys/syscall.h>
  //  #include <sys/unistd.h>
  //  #define gettid() syscall(SYS_gettid)
  //#endif

  #ifdef MIP_DEBUG_PRINT_SOCKET
    #include <unistd.h>
    #include <sys/socket.h>
    #include <sys/un.h>
    int _mip_debug_socket_handle = 0;
  #endif

  char _mip_debug_print_buffer[256]  = {0};
  char _mip_debug_prefix_buffer[256] = {0};

  //----------------------------------------------------------------------
  //
  //----------------------------------------------------------------------

  #ifdef MIP_DEBUG_PRINT_TIME

  void _mip_debug_time_start() {
    //mip_debug_start_time = clock();
    struct timeval time;
    gettimeofday(&time,NULL);
    _mip_debug_start_time = (double)time.tv_sec + (double)time.tv_usec * .000001;
  }

  //----------

  double _mip_debug_time_elapsed() {
    //clock_t currenttime = clock();
    //double elapsed = currenttime - mip_debug_start_time;
    //return elapsed / CLOCKS_PER_SEC;
    struct timeval time;
    gettimeofday(&time,NULL);
    double currenttime = (double)time.tv_sec + (double)time.tv_usec * .000001;
    double elapsed = currenttime - _mip_debug_start_time;
    return elapsed;
  }

  #endif

  //----------------------------------------------------------------------
  // thread
  //----------------------------------------------------------------------

  //#ifdef MIP_DEBUG_PRINT_THREAD
  //
  //pid_t _mip_debug_get_process_id() {
  //  return getpid();
  //}
  //
  ////----------
  //
  //pid_t _mip_debug_get_thread_id() {
  //  return gettid();
  //}
  //
  //#endif

  //----------------------------------------------------------------------
  // thread
  //----------------------------------------------------------------------

  #ifdef MIP_DEBUG_PRINT_SOCKET

  void _mip_debug_socket_init() {
    //open_socket();
    _mip_debug_socket_handle = socket(PF_UNIX,SOCK_STREAM,0);
    sockaddr_un address;
    memset(&address,0,sizeof(sockaddr_un));
    address.sun_family = AF_UNIX;
    snprintf(address.sun_path,108,"/tmp/mip.socket"); // max 108?
    connect(_mip_debug_socket_handle,reinterpret_cast<sockaddr*>(&address),sizeof(sockaddr_un));
  }

  void _mip_debug_socket_close() {
    close(_mip_debug_socket_handle);
  }

  void _mip_debug_socket_print(char* buffer) {
    dprintf(_mip_debug_socket_handle,"%s",buffer);
  }

  #endif

  //----------------------------------------------------------------------
  // path
  //----------------------------------------------------------------------

  // /home/skei/test -> test
  // returns everything after the last /

  const char* _mip_debug_strip_path(const char* buffer) {
    const char *slash;
    slash = strrchr(buffer,'/');
    if (slash) {
      return slash + 1;
    }
    return buffer;
  }

  //----------------------------------------------------------------------
  //
  //----------------------------------------------------------------------

  void _mip_print_prefix(const char* file, const char* func, int line) {
    char buffer[256];
    const char* filename  = _mip_debug_strip_path(file);
    strcat(_mip_debug_prefix_buffer,"[");
    #ifdef MIP_DEBUG_PRINT_TIME
      double time = _mip_debug_time_elapsed();
      sprintf(buffer,"%.3f:",time);
      strcat(_mip_debug_prefix_buffer,buffer);
    #endif
    #ifdef MIP_DEBUG_PRINT_THREAD
      uint32_t thread_id = MIP_GetThreadId(); // _mip_debug_get_thread_id();
      sprintf(buffer,"%i:",thread_id);
      strcat(_mip_debug_prefix_buffer,buffer);
    #endif
    sprintf(buffer,"%s:%s:%i",filename,func,line);
    strcat(_mip_debug_prefix_buffer,buffer);
    strcat(_mip_debug_prefix_buffer,"] ");
  }

  //----------

  void _mip_print(const char* format, ...) {
    va_list args;
    va_start(args,format);
    vsprintf(_mip_debug_print_buffer,format,args);
    strcat(_mip_debug_prefix_buffer,_mip_debug_print_buffer);

    #if defined MIP_DEBUG_PRINT_SOCKET
      _mip_debug_socket_print(_mip_debug_prefix_buffer);
    #else
      printf("%s",_mip_debug_prefix_buffer);
    #endif

    _mip_debug_prefix_buffer[0] = 0;
    va_end(args); // move this up?
  }

  //----------

  void _mip_dprint(const char* format, ...) {
    va_list args;
    va_start(args,format);
    vsprintf(_mip_debug_print_buffer,format,args);

    #if defined MIP_DEBUG_PRINT_SOCKET
      _mip_debug_socket_print(_mip_debug_print_buffer);
    #else
      printf("%s",_mip_debug_print_buffer);
    #endif

    va_end(args); // again, move this up?
  }

  //----------------------------------------------------------------------
  //
  //----------------------------------------------------------------------

  #define MIP_Print \
    _mip_print_prefix( __FILE__, __FUNCTION__, __LINE__ ); \
    _mip_print

  #define MIP_DPrint \
    _mip_dprint

  #define MIP_PRINT \
    _mip_print_prefix( __FILE__, __FUNCTION__, __LINE__ ); \
    _mip_print("\n");

#else // MIP_DEBUG

  #define MIP_Print  MIP_NoPrint
  #define MIP_DPrint MIP_NoPrint
  #define MIP_PRINT  {}

#endif // MIP_DEBUG

//----------------------------------------------------------------------
#endif











//#include <iostream>
//#include <fstream>
//using namespace std;
//
//#include "src/h_BasePath.h"
//
//#ifdef H_DEBUG_LOG_UNIQUE
//  #include "src/h_Timestamp.h"
//#endif
//
////----------------------------------------------------------------------
//
//#ifndef H_DEBUG_LOG_APPEND
//  #define H_DEBUG_LOG_APPEND
//#else
//  #undef H_DEBUG_LOG_APPEND
//  #define H_DEBUG_LOG_APPEND | std::fstream::app
//#endif
//
////----------------------------------------------------------------------
//
//class MIP_LogFile {
//
//private:
//
//  char          m_LogFileName[H_MAX_PATHSIZE];
//  fstream       m_LogFile; // static?
//  #ifdef H_DEBUG_LOG_UNIQUE
//  h_Timestamp m_TS;
//  #endif
//
//public:
//
//  inline fstream& _trace(void) { return m_LogFile; }
//
//public:
//
//  MIP_LogFile(const char* a_FileName)
//    {
//      #ifndef H_DEBUG_LOG_HOME
//        // try writing to base path
//
//        m_LogFileName[0] = '\0';
//        h_GetBasePath(m_LogFileName);
//        #ifdef H_DEBUG_LOG_UNIQUE
//          h_Strcat(m_LogFileName, m_TS.str());
//          h_Strcat(m_LogFileName, (char*)"_");
//        #endif
//
//        h_Strcat(m_LogFileName, a_FileName);
//        m_LogFile.open(m_LogFileName, std::fstream::out H_DEBUG_LOG_APPEND);
//
//        if (m_LogFile.fail())
//        {
//          printf( "ERROR: cannot write log to base path: %s\n",
//                m_LogFileName);
//      #endif
//          // write to home path instead
//
//          m_LogFileName[0] = '\0';
//          h_GetHomePath(m_LogFileName);
//          #ifdef H_DEBUG_LOG_UNIQUE
//            h_Strcat(m_LogFileName, m_TS.str());
//            h_Strcat(m_LogFileName, (char*)"_");
//          #endif
//
//          h_Strcat(m_LogFileName, a_FileName);
//          m_LogFile.open(m_LogFileName, std::fstream::out H_DEBUG_LOG_APPEND);
//
//          if (m_LogFile.fail())
//          {
//            printf( "ERROR: cannot write log to home path: %s\n",
//                  m_LogFileName);
//            m_LogFile.clear();
//          }
//      #ifndef H_DEBUG_LOG_HOME
//        }
//      #endif
//    }
//
//  ~MIP_LogFile()
//    {
//      m_LogFile.close();
//    }
//
//  //void print(const char* x)
//  //  {
//  //    m_LogFile << x << endl;
//  //  }
//
//};
