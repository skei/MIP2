#ifndef mip_process_context_included
#define mip_process_context_included
//----------------------------------------------------------------------

#include "mip.h"

//----------

struct MIP_ProcessContext {
  float     samplerate    = 0.0;
  float     tempo         = 0.0;
  uint32_t  num_inputs    = 0;
  uint32_t  num_outputs   = 0;
  uint32_t  num_samples   = 0;
  float**   inputs        = nullptr;
  float**   outputs       = nullptr;
};

//----------------------------------------------------------------------
#endif
