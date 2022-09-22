#ifndef mip_debug_included
#define mip_debug_included
//----------------------------------------------------------------------

#include "mip.h"

//#define MIP_DEBUG_ASSERT
//#define MIP_DEBUG_CALLSTACK
//#define MIP_DEBUG_CRASH_HANDLER
////#define MIP_DEBUG_MEMORY
//#define MIP_DEBUG_PRINT
//#define MIP_DEBUG_PRINT_THREAD
//#define MIP_DEBUG_PRINT_TIME
////#define MIP_DEBUG_PRINT_SOCKET
//#define MIP_DEBUG_WATCHES

//----------------------------------------------------------------------

#ifdef MIP_WIN32
  #undef MIP_DEBUG_ASSER
  #undef MIP_DEBUG_CALLSTACK
  #undef MIP_DEBUG_CRASH_HANDLER
  #undef MIP_DEBUG_MEMORY
  #undef MIP_DEBUG_PRINT_SOCKET
  #undef MIP_DEBUG_PRINT_THREAD
  #undef MIP_DEBUG_PRINT_TIME

#endif

//----------

#ifndef MIP_DEBUG
  #undef MIP_DEBUG_ASSERT
  #undef MIP_DEBUG_CALLSTACK
  #undef MIP_DEBUG_CRASH_HANDLER
  #undef MIP_DEBUG_MEMORY
  #undef MIP_DEBUG_PRINT
  #undef MIP_DEBUG_PRINT_SOCKET
  #undef MIP_DEBUG_PRINT_THREAD
  #undef MIP_DEBUG_PRINT_TIME
#endif // MIP_DEBUG

//----------------------------------------------------------------------

#include "base/debug/mip_debug_print.h"
#include "base/debug/mip_debug_assert.h"
#include "base/debug/mip_debug_watch.h"
#include "base/debug/mip_debug_callstack.h"
#include "base/debug/mip_debug_crash_handler.h"
//#include "base/debug/mip_debug_memory.h"

#include "base/utils/mip_strutils.h"

#ifdef MIP_DEBUG_MEMORY

  #include <list>

  typedef struct {
    void*     addr      = nullptr;
    uint32_t  size      = 0;
    char      file[128] = {0};
    char      func[128] = {0};
    uint32_t  line      = 0;
  } MIP_DebugMemoryNode;

  typedef std::list<MIP_DebugMemoryNode*> MIP_DebugMemoryNodes;

#endif // memory

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

  #ifdef MIP_DEBUG_MEMORY
  MIP_DebugMemoryNodes MMemoryNodes;
  #endif

//------------------------------
public:
//------------------------------

  MIP_GlobalDebug() {
    //printf("MIP_GlobalDebug() (printf)\n");
    //MIP_PRINT;
    #ifdef MIP_DEBUG_PRINT_TIME
      _mip_debug_time_start();
    #endif
    #ifdef MIP_DEBUG_PRINT_SOCKET
      _mip_debug_socket_init();
    #endif
  }

  //----------

  ~MIP_GlobalDebug() {
    //MIP_PRINT;
    //#ifdef MIP_DEBUG_PRINT_TIME
    //#endif
    #ifdef MIP_DEBUG_PRINT_SOCKET
      _mip_debug_socket_close();
    #endif
  }

//------------------------------
public:
//------------------------------

  #ifdef MIP_DEBUG_MEMORY

  void addMemoryNode(void* AAddress,  uint32_t ASize,  const char* AFile, const char *AFunc, uint32_t ALine) {
    MIP_DebugMemoryNode *info;
    //if (!allocList) {
    //  allocList = new(MIP_DebugMemoryNodes);
    //}
    info = new(MIP_DebugMemoryNode);
    info->addr = AAddress;
    const char* filename = MIP_GetFilenameFromPath(AFile);
    strncpy(info->file,filename,127);
    strncpy(info->func,AFunc,   127);
    info->line = ALine;
    info->size = ASize;
    MMemoryNodes.insert(MMemoryNodes.begin(), info);
  };

  //

  void removeMemoryNode(void* AAddress) {
    MIP_DebugMemoryNodes::iterator i;
    //if (!allocList) return;
    for (i = MMemoryNodes.begin(); i != MMemoryNodes.end(); i++) {
      if ((*i)->addr == AAddress) {
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
      MIP_DPrint("file:%s func:%s line:%i addr:0x%08p size: %i\n", (*i)->file, (*i)->func, (*i)->line, (*i)->addr, (*i)->size);
      totalSize += (*i)->size;
    }
    MIP_DPrint("-----------------------------------------------------------\n");
    MIP_Print("Total Unfreed: %d bytes\n", totalSize);
  };

  #endif // MIP_DEBUG_MEMORY


};


//----------

MIP_GlobalDebug MIP_GLOBAL_DEBUG;

#endif // MIP_DEBUG

//----------------------------------------------------------------------

#include "base/debug/mip_debug_memory.h"
//#include "base/debug/mip_debug_watch.h"

//----------------------------------------------------------------------
#endif
