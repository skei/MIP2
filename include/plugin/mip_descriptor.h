#ifndef mip_descriptor_included
#define mip_descriptor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/mip_parameter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Descriptor {

public:
  clap_plugin_descriptor_t* MDescriptor = nullptr;

public:
  MParameterArray parameters;
};

//----------------------------------------------------------------------
#endif
