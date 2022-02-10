#ifndef mip_vst2_included
#define mip_vst2_included
//----------------------------------------------------------------------

#include "mip.h"

//#ifdef MIP_PLUGIN_VST2_VESTIGE
//  typedef int32_t   VstInt32;
//  typedef intptr_t  VstIntPtr;
//  struct  ERect { int16_t top, left, bottom, right; };
//  #include "extern/vst2/vestige.h"
//#else

  #define __cdecl
  #include "extern/vst2/aeffect.h"
  #include "extern/vst2/aeffectx.h"

//#endif


//----------------------------------------------------------------------
#endif



