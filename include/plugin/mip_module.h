#ifndef mip_module_included
#define mip_module_included
//----------------------------------------------------------------------

#include "mip.h"

//----------------------------------------------------------------------

class MIP_ModuleInfo {
public:
};

//----------------------------------------------------------------------

template <class INFO>
class MIP_ModuleState {

public:

  INFO*   info  = nullptr;

public:

  MIP_ModuleState(INFO* AInfo) {
    info = AInfo;
  }

};

//----------------------------------------------------------------------

template <class INFO, class STATE>
class MIP_ModuleProc {

public:

  INFO*   info  = nullptr;
  STATE*  state = nullptr;

public:

  MIP_ModuleProc(INFO* AInfo, STATE* AState) {
    info = AInfo;
    state = AState;
  }

};


//----------------------------------------------------------------------

template <class INFO, class STATE, class PROC>
class MIP_Module {

//------------------------------
public:
//------------------------------

  INFO  info      = {};
  STATE state     = {};
  PROC  processor = {};

//------------------------------
public:
//------------------------------

  MIP_Module() {
  };

  //----------

  ~MIP_Module() {
  };

};

//----------------------------------------------------------------------
#endif
