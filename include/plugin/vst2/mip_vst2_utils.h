#ifndef mip_vst2_utils_included
#define mip_vst2_utils_included
//----------------------------------------------------------------------

#include "plugin/vst2/mip_vst2.h"

uint32_t getUniqueId(const clap_plugin_descriptor_t* descriptor) {
  //uint32_t* id = (uint32_t*)MPluginId;
  //id[0] = MIP_MAGIC_M_PL;
  //id[1] = MIP_HashString(descriptor->name);
  //id[2] = MIP_HashString(descriptor->vendor);
  //id[3] = MIP_HashString(descriptor->version);
  //return (const char*)id;
  char buffer [1024];
  buffer[0] = 0;
  //strcat(buffer,"MIP_MAGIC_M_PL");
  strcat(buffer,descriptor->name);
  strcat(buffer,descriptor->vendor);
  strcat(buffer,descriptor->version);
  return MIP_HashString(buffer);
}

//----------------------------------------------------------------------
#endif
