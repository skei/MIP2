#define MIP_NO_GUI
#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket

//----------

#include "plugin/mip_plugin.h"
#include "sa_ports/sa_compciter.h"
#include "sa_ports/sa_event_horizon.h"
#include "sa_ports/sa_exciter.h"
#include "sa_ports/sa_exciter2.h"
#include "sa_ports/sa_hall_reverb.h"
#include "sa_ports/sa_hrtf.h"
#include "sa_ports/sa_large_room_reverb.h"
#include "sa_ports/sa_limiter.h"
#include "sa_ports/sa_saturation.h"
#include "sa_ports/sa_small_room_reverb.h"
#include "sa_ports/sa_sonic_maximizer.h"
#include "sa_ports/sa_tilteq.h"
#include "sa_ports/sa_transient_killer.h"

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

void MIP_Register(MIP_ClapRegistry* ARegistry) {
  ARegistry->appendPlugin(&sa_compciter_descriptor);
  ARegistry->appendPlugin(&sa_event_horizon_descriptor);
  ARegistry->appendPlugin(&sa_exciter_descriptor);
  ARegistry->appendPlugin(&sa_exciter2_descriptor);
  ARegistry->appendPlugin(&sa_hall_reverb_descriptor);
  ARegistry->appendPlugin(&sa_hrtf_descriptor);
  ARegistry->appendPlugin(&sa_large_room_reverb_descriptor);
  ARegistry->appendPlugin(&sa_limiter_descriptor);
  ARegistry->appendPlugin(&sa_saturation_descriptor);
  ARegistry->appendPlugin(&sa_small_room_reverb_descriptor);
  ARegistry->appendPlugin(&sa_sonic_maximizer_descriptor);
  ARegistry->appendPlugin(&sa_tilteq_descriptor);
  ARegistry->appendPlugin(&sa_transient_killer_descriptor);
}

//----------

//void MIP_Unregister(MIP_ClapRegistry* ARegistry) {
//}

//----------------------------------------------------------------------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    case 0:  return new sa_compciter_plugin(ADescriptor,AHost);
    case 1:  return new sa_event_horizon_plugin(ADescriptor,AHost);
    case 2:  return new sa_exciter_plugin(ADescriptor,AHost);
    case 3:  return new sa_exciter2_plugin(ADescriptor,AHost);
    case 4:  return new sa_hall_reverb_plugin(ADescriptor,AHost);
    case 5:  return new sa_hrtf_plugin(ADescriptor,AHost);
    case 6:  return new sa_large_room_reverb_plugin(ADescriptor,AHost);
    case 7:  return new sa_limiter_plugin(ADescriptor,AHost);
    case 8:  return new sa_saturation_plugin(ADescriptor,AHost);
    case 9:  return new sa_small_room_reverb_plugin(ADescriptor,AHost);
    case 10: return new sa_sonic_maximizer_plugin(ADescriptor,AHost);
    case 11: return new sa_tilteq_plugin(ADescriptor,AHost);
    case 12: return new sa_transient_killer_plugin(ADescriptor,AHost);
  }
  return nullptr;
}
