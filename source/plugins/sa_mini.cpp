#define MIP_NO_GUI
#define MIP_NO_PAINTER

#ifndef MIP_EXE
  #define MIP_DEBUG_PRINT_SOCKET
  //nc -U -l -k /tmp/mip.socket
#endif

//----------

#include "plugin/mip_plugin.h"

#define MIP_NO_ENTRY

#include "sa_mini/sa_fibonacci.h"
#include "sa_mini/sa_grains.h"
#include "sa_mini/sa_pitch.h"
#include "sa_mini/sa_transpose.h"

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

#include "plugin/mip_registry.h"
#include "plugin/clap/mip_clap_entry.h"
#include "plugin/exe/mip_exe_entry.h"
#include "plugin/vst2/mip_vst2_entry.h"
#include "plugin/vst3/mip_vst3_entry.h"

void MIP_Register() {
  MIP_REGISTRY.appendDescriptor(&sa_fibonacci_descriptor);
  MIP_REGISTRY.appendDescriptor(&sa_grains_descriptor);
  MIP_REGISTRY.appendDescriptor(&sa_pitch_descriptor);
  MIP_REGISTRY.appendDescriptor(&sa_transpose_descriptor);
}

//----------

//void MIP_Unregister() {
//}

//----------------------------------------------------------------------

MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    case 0:  return new sa_fibonacci_plugin(ADescriptor,AHost);
    case 1:  return new sa_grains_plugin(ADescriptor,AHost);
    case 2:  return new sa_pitch_descriptor(ADescriptor,AHost);
    case 3:  return new sa_transpose_plugin(ADescriptor,AHost);
  }
  return nullptr;
}
