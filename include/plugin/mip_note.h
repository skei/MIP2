#ifndef mip_note_included
#define mip_note_included
//----------------------------------------------------------------------

#include "mip.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct MIP_Note {
  int32_t  port_index = -1;
  int32_t  channel    = -1;
  int32_t  key        = -1;
  int32_t  note_id    = -1;
  //MIP_Note() {}
  void set(int32_t p, int32_t c, int32_t k, int32_t n) {
    port_index  = p;
    channel     = c;
    key         = k;
    note_id     = n;
  }
};

//----------------------------------------------------------------------
#endif
