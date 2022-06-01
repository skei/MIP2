#ifndef mip_process_handler_included
#define mip_process_handler_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
//#include "plugin/mip_parameter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ProcessListener {
  int dummy = 0;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ProcessHandler {

//------------------------------
private:
//------------------------------

  MIP_ProcessListener* MListener = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ProcessHandler(MIP_ProcessListener* AListener) {
    MListener = AListener;
  }

  //----------

  virtual ~MIP_ProcessHandler() {
  }

//------------------------------
public:
//------------------------------

  void processBlock(const clap_process_t* AProcess) {
  }

  void processTicks(const clap_process_t* AProcess) {
  }


};

//----------------------------------------------------------------------
#endif

