#ifndef mip_clap_included
#define mip_clap_included
//----------------------------------------------------------------------

#include "extern/clap/clap.h"
#include "extern/clap/ext/draft/check-for-update.h"

//#include "plugin/clap/mip_clap_entry.h"

// max number of events per process block
// (per plugin instance)
#define MIP_CLAP_MESSAGE_QUEUE_SIZE 16384
#include "base/types/mip_queue.h"
typedef MIP_Queue<uint32_t,MIP_CLAP_MESSAGE_QUEUE_SIZE> MIP_ClapIntQueue;

//----------------------------------------------------------------------
#endif
