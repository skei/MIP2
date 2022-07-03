#ifndef mip_wrapper_included
#define mip_wrapper_included
//----------------------------------------------------------------------

#include "mip.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class PLUGIN, class HOST>
class MIP_Wrapper {

public:

  MIP_Wrapper() {}
  virtual ~MIP_Wrapper() {}

private:

  PLUGIN* MPlugin = nullptr;
  HOST*   MHost   = nullptr;

};

//----------------------------------------------------------------------
#endif

