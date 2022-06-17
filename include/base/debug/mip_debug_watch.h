#ifndef mip_debug_watch_included
#define mip_debug_watch_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

enum EDebugWatchType {
  MIP_WATCH_UINT32  = 0,
  MIP_WATCH_FLOAT   = 1,
  MIP_WATCH_CHARPTR = 2,
  MIP_WATCH_PTR     = 3,
  MIP_WATCH_BOOL    = 4,
  MIP_WATCH_INTPTR  = 5
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifdef MIP_DEBUG_WATCHES

struct MIP_WatchItem {

  uint32_t    type  = 0;
  const char* name  = "";
  void*       ptr   = nullptr;

  MIP_WatchItem() {
  }

  MIP_WatchItem(uint32_t AType, const char* AName, void* APtr) {
    type  = AType;
    name  = AName;
    ptr   = APtr;
  }

};

//----------

typedef MIP_Array<MIP_WatchItem* > MIP_WatchItemArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_DebugWatch {

private:

  MIP_WatchItemArray  MWatches = {};

public:

  MIP_DebugWatch() {
  }

  //----------

  ~MIP_DebugWatch() {
    for (uint32_t i=0; i<MWatches.size(); i++) {
      delete MWatches[i];
    }
  };

public:

  void addWatch(uint32_t AType, const char* AName, void* APtr) {
    MIP_WatchItem* item = new MIP_WatchItem(AType,AName,APtr);
    MWatches.append(item);
  }

  //----------

  int32_t findWatch(const char* AName) {
    for (uint32_t i=0; i<MWatches.size(); i++) {
      if (strcmp(AName,MWatches[i]->name) == 0) return i;
    }
    return -1;
  }

  //----------

  void printWatch(uint32_t AIndex, const char* APrefix="") {
    uint32_t    type  = MWatches[AIndex]->type;
    const char* name  = MWatches[AIndex]->name;
    void*       ptr   = MWatches[AIndex]->ptr;
    switch (type) {
      case MIP_WATCH_UINT32:  MIP_DPrint("%s%s : %i\n",   APrefix, name, *(uint32_t*)ptr);               break;
      case MIP_WATCH_FLOAT:   MIP_DPrint("%s%s : %f\n",   APrefix, name, *(float*)ptr);                  break;
      case MIP_WATCH_CHARPTR: MIP_DPrint("%s%s : %s\n",   APrefix, name, *(const char*)ptr);             break;
      case MIP_WATCH_PTR:     MIP_DPrint("%s%s : 0x%xp\n",APrefix, name, ptr);                           break;
      case MIP_WATCH_BOOL:    MIP_DPrint("%s%s : %s\n",   APrefix, name, (*(bool*)ptr)?"true":"false");  break;
      case MIP_WATCH_INTPTR:  MIP_DPrint("%s%s : %i\n",   APrefix, name, *(intptr_t*)ptr);               break;
    }
  }

  //----------

  void printWatch(const char* AWatch, const char* APrefix="") {
    int32_t watch = findWatch(AWatch);
    if (watch >= 0) printWatch(watch,APrefix);
  }

  //----------

  void printWatches(const char* APrefix="") {
    if (APrefix[0] != 0) MIP_DPrint("\n%s\n",APrefix);
    for (uint32_t i=0; i<MWatches.size(); i++) {
      printWatch(i,"");
    }
  }

};

//----------------------------------------------------------------------

#else // MIP_DEBUG_WATCHES

//----------------------------------------------------------------------

class MIP_DebugWatch {
public:
  MIP_DebugWatch() {}
  ~MIP_DebugWatch() {}
public:
  void    addWatch(uint32_t AType, const char* AName, void* APtr) {}
  int32_t findWatch(const char* AName) { return -1; }
  void    printWatch(uint32_t AIndex, const char* APrefix="") {}
  void    printWatch(const char* AWatch, const char* APrefix="") {}
  void    printWatches(const char* APrefix="") {}
};

#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

MIP_DebugWatch  MIP_GLOBAL_WATCHES = {};

//----------------------------------------------------------------------
#endif
