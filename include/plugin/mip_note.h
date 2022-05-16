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

  int32_t key         =  0;   // 0..127
  int32_t channel     =  0;   // 0..15
  int32_t port_index  =  0;
  int32_t note_id     = -1;   // -1 if unspecified, otherwise > 0

  //----------

  MIP_Note() {
  }

  MIP_Note(int32_t k, int32_t c, int32_t p=0, int32_t n=-1) {
    key         = k;
    channel     = c;
    port_index  = p;
    note_id     = n;
  }

};

//----------------------------------------------------------------------
#endif
