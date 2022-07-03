#ifndef kode_lv2_included
#define kode_lv2_included
//----------------------------------------------------------------------

#include "kode.h"

#include <lv2.h>
#include <lv2/atom/atom.h>
#include "lv2/atom/util.h"
#include <lv2/core/lv2.h>
#include "lv2/core/lv2_util.h"
#include "lv2/log/log.h"
#include "lv2/log/logger.h"
#include <lv2/midi/midi.h>
#include <lv2/urid/urid.h>

#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

//----------

#ifndef KODE_DEBUG
  #undef KODE_DEBUG_LV2
#endif

#ifdef KODE_DEBUG_LV2
  #define KODE_LV2PRINT  KODE_PRINT
  #define KODE_Lv2Print  KODE_Print
  #define KODE_Lv2DPrint KODE_DPrint
#else
  #define KODE_LV2PRINT
  #define KODE_Lv2Print  KODE_NoPrint
  #define KODE_Lv2DPrint KODE_NoPrint
#endif


#define KODE_LV2_MAX_URI_LENGTH     256
#define KODE_LV2_MANIFEST_TTL_SIZE  65536
#define KODE_LV2_PLUGIN_TTL_SIZE    65536

#define KODE_LV2_EXPORT_TTL

//----------------------------------------------------------------------



//----------------------------------------------------------------------
#endif
