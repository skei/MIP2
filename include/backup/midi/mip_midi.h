#ifndef mip_midi_included
#define mip_midi_included
//----------------------------------------------------------------------

/*
  TODO:
    MIP_MdiEvent -> MIP_MidiEventExt (and other structs..)
    MIP_MdiMsg -> Midi_Event
    MIP_Array<MIP_Event,1024> MIP_MidiEvents
*/

//#include <vector>
#include "mip.h"
#include "base/types/mip_array.h"

//----------------------------------------------------------------------
//
// midi_msg (small)
//
//----------------------------------------------------------------------

struct MIP_MidiMsg {
  uint32_t  time    = 0;
  uint8_t   data[4] = {0};
};



//----------------------------------------------------------------------
//
// event
//
//----------------------------------------------------------------------

struct MIP_MidiEvent {

  uint8_t   type    = 0; // 0:midi, 1:sysex, 2:meta
  uint8_t   msg1    = 0;
  uint8_t   msg2    = 0;
  uint8_t   msg3    = 0;
  uint32_t  delta   = 0;
  float     time    = 0.0;
  uint32_t  offset  = 0;
  uint32_t  datalen = 0;
  uint8_t*  data    = nullptr;

  MIP_MidiEvent() {
    type    = 0;
    msg1    = 0;
    msg2    = 0;
    msg3    = 0;
    delta   = 0;
    datalen = 0;
    data    = nullptr;
  }

  MIP_MidiEvent(uint32_t d, uint8_t m1, uint8_t m2, uint8_t m3)  {
    type    = 0;
    msg1    = m1;
    msg2    = m2;
    msg3    = m3;
    delta   = d;
    delta   = d;
    datalen = 0;
    data    = nullptr;
  }

  ~MIP_MidiEvent() {
    if (data) free(data);
  }

  void setData(uint8_t* ptr, uint32_t length) {
    if (data) free(data);
    data = (uint8_t*)malloc(length);
    datalen = length;
    memcpy(data,ptr,length);
  }

  void setText(uint8_t* ptr, uint32_t length) {
    if (data) free(data);
    data = (uint8_t*)malloc(length+1);
    datalen = length;
    memcpy(data,ptr,length);
    data[length] = 0;
  }

  void setData(uint8_t v1,uint8_t v2=0,uint8_t v3=0,uint8_t v4=0,uint8_t v5=0) {
    if (data) free(data);
    data = (uint8_t*)malloc(5);
    datalen = 5;
    data[0] = v1;
    data[1] = v2;
    data[2] = v3;
    data[3] = v4;
    data[4] = v5;
  }

};

//----------

typedef std::vector<MIP_MidiEvent*> MIP_MidiEvents;

//----------------------------------------------------------------------
//
// track
//
//----------------------------------------------------------------------

struct MIP_MidiTrack {

  char*           name        = nullptr;
  uint32_t        num_events  = 0;
  MIP_MidiEvents  events      = {};
  //uint32_t        next_event  = 0;
  //bool            active      = 0;
  float           length      = 0.0;

  //----------

  MIP_MidiTrack() {
    name = nullptr;
    num_events = 0;
    events = {};
  }

  //----------

  ~MIP_MidiTrack() {
    if (name) free(name);
    for (uint32_t i=0; i<events.size(); i++) {
      delete events[i];
    }
  }

  //----------

  // assume non-zero terminated
  void setName(const char* ptr, uint32_t length) {
    if (name) free(name);
    name = (char*)malloc(length+1);
    memcpy(name,ptr,length);
    name[length] = 0;
  }

};

//----------

typedef std::vector<MIP_MidiTrack*> MIP_MidiTracks;

//----------------------------------------------------------------------
//
// sequence
//
//----------------------------------------------------------------------

struct MIP_MidiSequence {

  char*           name          = nullptr;
  uint32_t        format        = 0;
  uint32_t        num_tracks    = 0;
  uint32_t        tpq           = 0;
  MIP_MidiTracks  tracks        = {};
  float           length        = 0.0;

  uint32_t        tempo         = 0.0;
  uint32_t        timesig_num   = 0;
  uint32_t        timesig_denom = 0;

  //----------

  MIP_MidiSequence() {
    name = nullptr;
    format      = 0;
    num_tracks  = 0;
    tpq         = 0;
    tracks      = {};
  }

  //----------

  ~MIP_MidiSequence() {
    if (name) free(name);
    for (uint32_t i=0; i<tracks.size(); i++) {
      delete tracks[i];
    }
  }

  //----------

  void setName(const char* ptr, uint32_t length) {
    if (name) free(name);
    name = (char*)malloc(length+1);
    memcpy(name,ptr,length);
    name[length] = 0;
  }

  //----------

  void calc_time() {
    uint32_t  us_per_qnote    = tempo; // <Tempo in latest Set Tempo event>
    uint32_t  ticks_per_qnote = tpq; // <PPQ from the header>
    float     us_per_tick     = (float)us_per_qnote / (float)ticks_per_qnote;
    float     s_per_tick      = us_per_tick / 1000000.0;
    length = 0.0;
    for (uint32_t t=0; t<num_tracks; t++) {
      MIP_MidiTrack* track = tracks[t];
      track->length = 0.0;
      uint32_t num_events = track->num_events;
      uint32_t delta = 0;
      for (uint32_t e=0; e<num_events; e++) {
        MIP_MidiEvent* event = track->events[e];
        delta += event->delta;
        float time = (float)delta * (float)s_per_tick;
        event->time = time;
        if (time > track->length) track->length = time;
        if (time > length) length = time;
      }
    }
  }

};

//----------------------------------------------------------------------
#endif

