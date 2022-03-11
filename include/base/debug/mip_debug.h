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

#include "base/utils/mip_strutils.h"

#include <list>

typedef struct {
  void*     address;
  uint32_t  size;
  char      file[64];
  uint32_t  line;
} MIP_DebugMemoryNode;

typedef std::list<MIP_DebugMemoryNode*> MIP_DebugMemoryNodes;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifdef MIP_DEBUG

class MIP_GlobalDebug {

//------------------------------
private:
//------------------------------

  MIP_DebugMemoryNodes MMemoryNodes;

//------------------------------
public:
//------------------------------

  MIP_GlobalDebug() {
    #ifdef MIP_DEBUG_PRINT_TIME
      _mip_debug_time_start();
    #endif
    #ifdef MIP_DEBUG_PRINT_SOCKET
      _mip_debug_socket_init();
    #endif
  }

  //----------

  ~MIP_GlobalDebug() {
    //#ifdef MIP_DEBUG_PRINT_TIME
    //#endif
    #ifdef MIP_DEBUG_PRINT_SOCKET
      _mip_debug_socket_close();
    #endif
  }

//------------------------------
public:
//------------------------------

  void addMemoryNode(void* addr,  uint32_t asize,  const char *fname, uint32_t lnum) {
    MIP_DebugMemoryNode *info;
    //if (!allocList) {
    //  allocList = new(MIP_DebugMemoryNodes);
    //}
    info = new(MIP_DebugMemoryNode);
    info->address = addr;
    const char* filename = MIP_GetFilenameFromPath(fname);
    strncpy(info->file, /*fname*/filename, 63);
    info->line = lnum;
    info->size = asize;
    MMemoryNodes.insert(MMemoryNodes.begin(), info);
  };

  //

  void removeMemoryNode(void* addr) {
    MIP_DebugMemoryNodes::iterator i;
    //if (!allocList) return;
    for (i = MMemoryNodes.begin(); i != MMemoryNodes.end(); i++) {
      if ((*i)->address == addr) {
        MMemoryNodes.remove((*i));
        break;
      }
    }
  };

  //----------

  void dumpMemoryNodes() {
    MIP_DebugMemoryNodes::iterator i;
    uint32_t totalSize = 0;
    for (i = MMemoryNodes.begin(); i != MMemoryNodes.end(); i++) {
      MIP_DPrint("%-50s:\t\tLINE %d,\t\tADDRESS %d\t%d unfreed\n", (*i)->file, (*i)->line, (*i)->address, (*i)->size);
      totalSize += (*i)->size;
    }
    MIP_DPrint("-----------------------------------------------------------\n");
    MIP_Print("Total Unfreed: %d bytes\n", totalSize);
  };

};


//----------

MIP_GlobalDebug MIP_GLOBAL_DEBUG;

#endif // MIP_DEBUG

//----------------------------------------------------------------------
#endif
