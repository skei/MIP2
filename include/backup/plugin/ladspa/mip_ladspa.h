
#include "mip.h"

#include <ladspa.h>

//#ifndef KODE_DEBUG
//  #undef KODE_DEBUG_LADSPA
//#endif
//
//#ifdef KODE_DEBUG_LADSPA
//  #define KODE_LADSPAPRINT  KODE_PRINT
//  #define KODE_LadspaPrint  KODE_Print
//  #define KODE_LadspaDPrint KODE_DPrint
//#else
//  #define KODE_LADSPAPRINT
//  #define KODE_LadspaPrint  KODE_NoPrint
//  #define KODE_LadspaDPrint KODE_NoPrint
//#endif

#define MIP_PLUGIN_LADSPA_MAX_NAME_LENGTH      256
#define MIP_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH 64

