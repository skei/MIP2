#ifndef mip_test_included
#define mip_test_included
//----------------------------------------------------------------------

#include "mip.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

typedef int (*MIP_TestFunc)();

//----------

class MIP_Test {

//------------------------------
private:
//------------------------------

  MIP_Array<const char*>  MTestNames = {};
  MIP_Array<MIP_TestFunc> MTestFuncs = {};

//------------------------------
public:
//------------------------------

  MIP_Test() {
  }

  //----------

  ~MIP_Test() {
  }

//------------------------------
public:
//------------------------------

  int run(int argc, char** argv) {
    for (uint32_t i=0; i<MTestFuncs.size(); i++) {
      const char*   name  = MTestNames[i];
      MIP_TestFunc  func  = MTestFuncs[i];
      int result = func();
      MIP_DPrint("%s : %i\n",name,result);
    }
    return 0;
  }

  //----------

  bool appendTest(const char* name, MIP_TestFunc func) {
    MTestNames.append(name);
    MTestFuncs.append(func);
    return true;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

MIP_Test MIP_TEST = {};

//----------

#define MIP_APPEND_TEST(name,func) \
  const bool func##_test_result = MIP_TEST.appendTest(name,func);

//----------------------------------------------------------------------
#endif
