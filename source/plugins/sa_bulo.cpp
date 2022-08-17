#define MIP_NO_GUI
#define MIP_NO_PAINTER

#ifndef MIP_EXE
  #define MIP_DEBUG_PRINT_SOCKET
  //nc -U -l -k /tmp/mip.socket
#endif

// sabulo, lat, sand/gravel

//----------

#include "plugin/mip_plugin.h"

#include "sa_bulo/sa_compciter.h"
#include "sa_bulo/sa_event_horizon.h"
#include "sa_bulo/sa_exciter.h"
#include "sa_bulo/sa_exciter2.h"
#include "sa_bulo/sa_hall_reverb.h"
#include "sa_bulo/sa_hrtf.h"
#include "sa_bulo/sa_large_room_reverb.h"
#include "sa_bulo/sa_limiter.h"
#include "sa_bulo/sa_saturation.h"
#include "sa_bulo/sa_small_room_reverb.h"
#include "sa_bulo/sa_sonic_maximizer.h"
#include "sa_bulo/sa_tilteq.h"
#include "sa_bulo/sa_transient_killer.h"

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

#include "plugin/clap/mip_clap_entry.h"
#include "plugin/mip_registry.h"


void MIP_Register(MIP_Registry* ARegistry) {
  ARegistry->appendDescriptor(&sa_compciter_descriptor);
  ARegistry->appendDescriptor(&sa_event_horizon_descriptor);
  ARegistry->appendDescriptor(&sa_exciter_descriptor);
  ARegistry->appendDescriptor(&sa_exciter2_descriptor);
  ARegistry->appendDescriptor(&sa_hall_reverb_descriptor);
  ARegistry->appendDescriptor(&sa_hrtf_descriptor);
  ARegistry->appendDescriptor(&sa_large_room_reverb_descriptor);
  ARegistry->appendDescriptor(&sa_limiter_descriptor);
  ARegistry->appendDescriptor(&sa_saturation_descriptor);
  ARegistry->appendDescriptor(&sa_small_room_reverb_descriptor);
  ARegistry->appendDescriptor(&sa_sonic_maximizer_descriptor);
  ARegistry->appendDescriptor(&sa_tilteq_descriptor);
  ARegistry->appendDescriptor(&sa_transient_killer_descriptor);
}

//----------

//void MIP_Unregister(MIP_ClapRegistry* ARegistry) {
//}

//----------------------------------------------------------------------

MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
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
