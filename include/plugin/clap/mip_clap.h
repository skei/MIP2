#ifndef mip_clap_included
#define mip_clap_included
//----------------------------------------------------------------------

#include "extern/clap/clap.h"
#include "extern/clap/ext/draft/check-for-update.h"

//#include "plugin/clap/mip_clap_entry.h"

#define MIP_CLAP_MESSAGE_QUEUE_SIZE 1024

#include "base/types/mip_queue.h"
typedef MIP_Queue<uint32_t,MIP_CLAP_MESSAGE_QUEUE_SIZE> MIP_ClapIntQueue;

//----------------------------------------------------------------------
#endif
