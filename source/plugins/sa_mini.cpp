#define MIP_NO_GUI
#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

//----------

#include "plugin/mip_plugin.h"

#include "sa_mini/sa_grains.h"
#include "sa_mini/sa_pitch.h"
#include "sa_mini/sa_fibonacci.h"

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

void MIP_Register(MIP_ClapRegistry* ARegistry) {
  ARegistry->appendPlugin(&sa_grains_descriptor);
  ARegistry->appendPlugin(&sa_pitch_descriptor);
  ARegistry->appendPlugin(&sa_fibonacci_descriptor);
}

//----------

//void MIP_Unregister(MIP_ClapRegistry* ARegistry) {
//}

//----------------------------------------------------------------------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    case 0:  return new sa_grains_plugin(ADescriptor,AHost);
    case 1:  return new sa_pitch_plugin(ADescriptor,AHost);
    case 2:  return new sa_fibonacci_plugin(ADescriptor,AHost);
  }
  return nullptr;
}
