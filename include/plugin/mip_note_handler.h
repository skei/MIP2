#ifndef mip_note_handler_included
#define mip_note_handler_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_note.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_NoteListener {
public:
  virtual void on_note_listener_note_end(MIP_Note ANote) {}
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_NoteHandler {

//------------------------------
private:
//------------------------------

  MIP_NoteListener* MListener         = nullptr;
  MIP_Note*         MNoteMap[128*16]  = {0}; // 16k

//------------------------------
public:
//------------------------------

  MIP_NoteHandler(MIP_NoteListener* AListener) {
    MListener = AListener;
  }

  //----------

  virtual ~MIP_NoteHandler() {
  }

//------------------------------
public:
//------------------------------

  void handle_note_on_event(clap_event_note_t* event) {
    MIP_PRINT;
    //MIP_Note note = {
    //  .port_index = event->port_index,
    //  .channel    = event->channel,
    //  .key        = event->key,
    //  .note_id    = event->note_id
    //};
    //uint32_t time = event->header.time;
    //double velocity = event->velocity;
  }

  //----------

  void handle_note_off_event(clap_event_note_t* event) {
    MIP_PRINT;
  }

  //----------

  void handle_note_end_event(clap_event_note_t* event) {
    MIP_PRINT;
  }

  //----------

  void handle_note_choke_event(clap_event_note_t* event) {
    MIP_PRINT;
  }

  //----------

  void handle_note_expression_event(clap_event_note_expression_t* event) {
    MIP_PRINT;
  }

//------------------------------
public:
//------------------------------

  bool isNoteOn(int32_t channel, int32_t key) {
    return false;
  }

  //----------

};

//----------------------------------------------------------------------
#endif
