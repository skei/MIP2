#define MIP_NO_GUI
#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

//----------

#include "plugin/mip_plugin.h"

#include "sa_mini/sa_fibonacci.h"
#include "sa_mini/sa_grains.h"
#include "sa_mini/sa_pitch.h"
#include "sa_mini/sa_transpose.h"

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

void MIP_Register(MIP_ClapRegistry* ARegistry) {
  //ARegistry->appendPlugin(&sa_fibonacci_descriptor);
  ARegistry->appendDescriptor(&sa_grains_descriptor);
  ARegistry->appendDescriptor(&sa_pitch_descriptor);
  //ARegistry->appendDescriptor(&sa_transpose_descriptor);
}

//----------

//void MIP_Unregister(MIP_ClapRegistry* ARegistry) {
//}

//----------------------------------------------------------------------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    //case 0:  return new sa_fibonacci_plugin(ADescriptor,AHost);
    //case 1:  return new sa_grains_plugin(ADescriptor,AHost);
    //case 2:  return new sa_pitch_plugin(ADescriptor,AHost);
    //case 3:  return new sa_transpose_plugin(ADescriptor,AHost);
    case 0:  return new sa_grains_plugin(ADescriptor,AHost);
    case 1:  return new sa_pitch_plugin(ADescriptor,AHost);
  }
  return nullptr;
}

