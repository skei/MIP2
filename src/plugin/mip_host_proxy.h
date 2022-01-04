#ifndef mip_host_proxy_included
#define mip_host_proxy_included
//----------------------------------------------------------------------

#include "mip.h"

//----------------------------------------------------------------------

class MIP_HostProxy {

//------------------------------
public:
//------------------------------

  MIP_HostProxy() {}
  virtual ~MIP_HostProxy() {}

//------------------------------
public:
//------------------------------

  virtual void updateParameter(uint32_t AIndex, float AValue) {}
  virtual void resizeEditor(uint32_t AWidth, uint32_t AHeight) {}
  virtual void sendMidi(uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {}

};

//----------------------------------------------------------------------
#endif
