#ifndef mip_host_proxy_included
#define mip_host_proxy_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_queue.h"

//----------------------------------------------------------------------

struct MIP_HostEvent {
  uint32_t  time;
  uint8_t   type;
  uint8_t   port;
  uint8_t   chan;   // msg2
  uint8_t   key;    // msg3
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
