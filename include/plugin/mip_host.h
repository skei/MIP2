#ifndef mip_host_included
#define mip_host_included
//----------------------------------------------------------------------

// host from the plugins perspective..

#include "plugin/clap/mip_clap_host.h"

class MIP_Host
: public MIP_ClapHost {

//------------------------------
public:
//------------------------------

  MIP_Host(const clap_host_t* AHost)
  : MIP_ClapHost(AHost) {
  }

  //----------

  virtual ~MIP_Host() {
  }

//------------------------------
public:
//------------------------------

  virtual const char* getName() {
    return "unknown";
  }

  //----------

  virtual const char* getVersion() {
    return "0.0.0";
  }

  //----------

  virtual void updateParameter(uint32_t AIndex, double AValue) {
  }

  //----------

  virtual void resizeWindow(uint32_t AWidth, uint32_t AHeight) {
  }

};

*/

//----------------------------------------------------------------------
#endif
