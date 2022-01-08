#ifndef mip_clap_included
#define mip_clap_included
//----------------------------------------------------------------------

//#define MIP_CLAP_VIRTUAL virtual
//#define MIP_CLAP_VIRTUAL

//----------

#include "mip.h"
#include "extern/clap/clap.h"

#include "extern/clap/ext/draft/ambisonic.h"
#include "extern/clap/ext/draft/check-for-update.h"

#ifndef MIP_NO_PLUGIN
  //#include "plugin/clap/mip_clap_plugin_list.h"
  //#include "plugin/clap/mip_clap_plugin_descriptor.h"
  //#include "plugin/clap/mip_clap_plugin.h"
  //#include "plugin/clap/mip_clap_plugin_entry.h"
#endif

//----------------------------------------------------------------------

#ifndef MIP_DEBUG
  #undef MIP_DEBUG_VST3
#endif

#ifdef MIP_DEBUG_CLAP
  #define MIP_CLAPPRINT  MIP_PRINT
  #define MIP_ClapPrint  MIP_Print
  #define MIP_ClapDPrint MIP_DPrint
#else
  #define MIP_CLAPPRINT
  #define MIP_ClapPrint  MIP_NoPrint
  #define MIP_ClapDPrint MIP_NoPrint
#endif

//----------------------------------------------------------------------
#endif
