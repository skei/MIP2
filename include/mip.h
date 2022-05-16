#ifndef mip_included
#define mip_included
//----------------------------------------------------------------------

// ordering is important!
// be careful!

//----------

#include <dirent.h>
#include <math.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

//----------

// output may be truncated copying 255 bytes from a string of length 255 [-Wstringop-truncation]
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wstringop-truncation"
//#pragma GCC diagnostic pop

//----------

#include "base/mip_config.h"
#include "base/mip_defines.h"
#include "base/mip_const.h"
#include "base/mip_enums.h"
#include "base/mip_attributes.h"

#include "base/debug/mip_debug.h"

//----------------------------------------------------------------------
#endif
