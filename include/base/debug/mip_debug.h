#ifndef mip_debug_included
#define mip_debug_included
//----------------------------------------------------------------------

#define MIP_DEBUG_ASSERT
#define MIP_DEBUG_CALLSTACK
#define MIP_DEBUG_CRASH_HANDLER
#define MIP_DEBUG_PRINT
//#define MIP_DEBUG_PRINT_SOCKET
//#define MIP_DEBUG_PRINT_THREAD
//#define MIP_DEBUG_PRINT_TIME

//----------------------------------------------------------------------

#ifdef MIP_WIN32
  #undef MIP_DEBUG
#endif

#ifndef MIP_DEBUG
  #undef MIP_DEBUG_ASSER
  #undef MIP_DEBUG_CALLSTACK
  #undef MIP_DEBUG_CRASH_HANDLER
  #undef MIP_DEBUG_PRINT
  #undef MIP_DEBUG_PRINT_SOCKET
  #undef MIP_DEBUG_PRINT_THREAD
  #undef MIP_DEBUG_PRINT_TIME
#endif // MIP_DEBUG

//----------------------------------------------------------------------

#include "base/debug/mip_debug_print.h"
#include "base/debug/mip_debug_assert.h"
#include "base/debug/mip_debug_callstack.h"
#include "base/debug/mip_debug_crash_handler.h"

//----------------------------------------------------------------------

#ifdef MIP_DEBUG

  class MIP_GlobalDebug {
  public:
    MIP_GlobalDebug() {
      #ifdef MIP_DEBUG_PRINT_TIME
      _mip_debug_time_start();
      #endif
      #ifdef MIP_DEBUG_PRINT_SOCKET
      _mip_debug_socket_init();
      #endif
    }
    ~MIP_GlobalDebug() {
      //#ifdef MIP_DEBUG_PRINT_TIME
      //#endif
      #ifdef MIP_DEBUG_PRINT_SOCKET
      _mip_debug_socket_close();
      #endif
    }
  };

  //----------

  MIP_GlobalDebug MIP_GLOBAL_DEBUG;

#endif // MIP_DEBUG

//----------------------------------------------------------------------
#endif
