#ifndef mip_host_proxy_included
#define mip_host_proxy_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_queue.h"

//----------------------------------------------------------------------

#define MIP_EVENT_NONE    0
#define MIP_EVENT_PARAM   1
#define MIP_EVENT_MOD     2
#define MIP_EVENT_MIDI    3
#define MIP_EVENT_NOTE    4

//----------

struct MIP_HostEvent {
  uint32_t  time;
  uint32_t  type;
  int32_t   port;
  int32_t   chan;
  int32_t   key;
  uint8_t   data[4];
  uint32_t  index;
  float     value;
};

typedef MIP_Queue<MIP_HostEvent,1024> MIP_HostEventQueue;

//----------------------------------------------------------------------

class MIP_HostProxy {

//------------------------------
private:
//------------------------------

  // MOutEvents
  MIP_HostEventQueue  MEventQueue = {};

//------------------------------
public:
//------------------------------

  MIP_HostProxy() {}
  virtual ~MIP_HostProxy() {}

//------------------------------
public:
//------------------------------

  virtual uint32_t getPluginFormat() {
    return MIP_PLUGIN_FORMAT_NONE;
  }

  virtual void writeEvent(MIP_HostEvent AEvent) {
    MIP_PRINT;
    MEventQueue.write(AEvent);
  }

  virtual bool readEvent(MIP_HostEvent* AEvent) {
    return MEventQueue.read(AEvent);
  }

};

//----------------------------------------------------------------------
#endif
