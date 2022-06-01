#ifndef mip_clap_included
#define mip_clap_included
//----------------------------------------------------------------------

#include "extern/clap/clap.h"
//#include "extern/clap/ext/draft/check-for-update.h"
//#include "extern/clap/ext/draft/transport-control.h"

//----------------------------------------------------------------------

#ifdef MIP_DEBUG_CLAP
  #define CLAP_Print   MIP_Print
  #define CLAP_DPrint  MIP_DPrint
  #define CLAP_PRINT   MIP_PRINT
#else
  #define CLAP_Print   MIP_NoPrint
  #define CLAP_DPrint  MIP_NoPrint
  #define CLAP_PRINT   {}
#endif

//----------------------------------------------------------------------
#endif
