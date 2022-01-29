
#ifndef midi_player_included
#define midi_player_included
//----------------------------------------------------------------------

// "plays" a MidiSequence
// keeps track of 'block size' window into event stream

#include "midi_file.hpp"

typedef void (*append_event_callback)(MidiEvent* event, void* ptr);

//----------------------------------------------------------------------

class MidiPlayer {

private:

  MidiFile*     MMidiFile       = nullptr;
  MidiSequence* MSequence       = nullptr;
  uint32_t      MSamplePos      = 0;
  float         MSampleRate     = 44100.0;
  float         MInvSampleRate  = (1.0 / MSampleRate);
  float         MCurrentTime    = 0.0;

//------------------------------
public:
//------------------------------

  MidiPlayer() {
  }

  //----------

  ~MidiPlayer() {
  }


//------------------------------
public:
//------------------------------

  MidiFile*     getMidiFile()     { return MMidiFile; }
  MidiSequence* getMidiSequence() { return MSequence; }

  //----------

  void initialize(MidiFile* AMidiFile, float ASampleRate=44100.0, float AStartPos=0.0) {
    MMidiFile       = AMidiFile;
    MSequence       = AMidiFile->getMidiSequence();
    MSampleRate     = ASampleRate;
    MInvSampleRate  = 1.0 / MSampleRate;
    MCurrentTime    = AStartPos;
    for (uint32_t i=0; i<MSequence->num_tracks; i++) {
      //MSequence->tracks[i]->active = true;
      //MSequence->tracks[i]->next_event = 0;
    }
  }

  //----------

  float getLength() {
    return MSequence->length;
  }

  //----------

  void setPos(float APos) {
    MCurrentTime = APos;
    //TODO: find next_event..
  }

  //----------

  /*
    appends within AFrom..(AFrom + ALength) are appended to blockevents
    (no malloc/copy)

    todo:
      keep track of last/previous event (index), and start looking from there.
      also keep track of next event time, and wait until it's time
  */

  void GetEventsForBlock(float AFrom, float ALength, MidiEvents* blockevents) {
    if (MSequence) {
      for (uint32_t t=0; t<MSequence->tracks.size(); t++) {
        MidiTrack* track = MSequence->tracks[t];
        if (track) {
          for (uint32_t e=0; e<track->events.size(); e++) {
            MidiEvent* event = track->events[e];
            if (event) {
              if ((event->time >= AFrom) && (event->time < (AFrom + ALength))) {
                if ((event->type == 0) && (event->msg1 < 0xF0)) {
                  //printf("  midiplayer.EVENT: %f : %02x %02x %02x\n",event->time,event->msg1,event->msg2,event->msg2);
                  blockevents->push_back(event);
                }
              }
            }
          }
        }
      }
    }
  }


};

//----------------------------------------------------------------------
#endif
