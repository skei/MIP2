#ifndef mip_host_proxy_included
#define mip_host_proxy_included
//----------------------------------------------------------------------

#include "mip.h"

//----------------------------------------------------------------------

class MIP_HostProxy {

//------------------------------
public:
//------------------------------

  virtual void updateParameter(uint32_t AIndex, float AValue) {}
  virtual void resizeEditor(uint32_t AWidth, uint32_t AHeight) {}

};

//----------------------------------------------------------------------
#endif
