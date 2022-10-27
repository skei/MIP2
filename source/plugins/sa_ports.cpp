
#ifndef MIP_EXE
  #define MIP_DEBUG_PRINT_SOCKET
  //nc -U -l -k /tmp/mip.socket
#endif

//----------

#define MIP_GUI_XCB
#define MIP_PAINTER_NANOVG
#define MIP_PLUGIN_GENERIC_EDITOR
#define MIP_NO_ENTRY

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

#include "plugin/mip_registry.h"
#include "plugin/clap/mip_clap_entry.h"
#include "plugin/exe/mip_exe_entry.h"
#include "plugin/vst2/mip_vst2_entry.h"
#include "plugin/vst3/mip_vst3_entry.h"

//----------

void MIP_Register() {
  MIP_REGISTRY.appendDescriptor(&sa_compciter_descriptor);
  MIP_REGISTRY.appendDescriptor(&sa_event_horizon_descriptor);
  MIP_REGISTRY.appendDescriptor(&sa_exciter_descriptor);
  MIP_REGISTRY.appendDescriptor(&sa_exciter2_descriptor);
  MIP_REGISTRY.appendDescriptor(&sa_hall_reverb_descriptor);
  MIP_REGISTRY.appendDescriptor(&sa_hrtf_descriptor);
  MIP_REGISTRY.appendDescriptor(&sa_large_room_reverb_descriptor);
  MIP_REGISTRY.appendDescriptor(&sa_limiter_descriptor);
  MIP_REGISTRY.appendDescriptor(&sa_saturation_descriptor);
  MIP_REGISTRY.appendDescriptor(&sa_small_room_reverb_descriptor);
  MIP_REGISTRY.appendDescriptor(&sa_sonic_maximizer_descriptor);
  MIP_REGISTRY.appendDescriptor(&sa_tilteq_descriptor);
  MIP_REGISTRY.appendDescriptor(&sa_transient_killer_descriptor);
}

//----------

//void MIP_Unregister() {
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
