#ifndef mip_audio_note_included
#define mip_audio_note_included
//----------------------------------------------------------------------

#include "mip.h"
//#include "base/types/mip_array.h"
//#include "base/types/mip_queue.h"
//#include "base/utils/mip_math.h"
#include "extern/clap/clap.h"

class MIP_NotePort;
typedef MIP_Array<MIP_NotePort*> MIP_NotePortArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_NotePort {

//------------------------------
private:
//------------------------------

  clap_note_port_info_t MInfo  = {
   .id                  = 0, // set this in appendNotePort
   .supported_dialects  = CLAP_NOTE_DIALECT_CLAP,
   .preferred_dialect   = CLAP_NOTE_DIALECT_CLAP,
   .name               = {0},
  };

//------------------------------
public:
//------------------------------

  MIP_NotePort() {
  }

  //----------

  MIP_NotePort(const char* AName, uint32_t ASupportedDialects=CLAP_NOTE_DIALECT_CLAP, uint32_t APreferredDialect=CLAP_NOTE_DIALECT_CLAP) {
    //MInfo.id = 0;
    strncpy(MInfo.name,AName,CLAP_NAME_SIZE);
    MInfo.supported_dialects = ASupportedDialects;
    MInfo.preferred_dialect = APreferredDialect;
  }

  //----------

  ~MIP_NotePort() {
  }

//------------------------------
public:
//------------------------------

  // set/get

  void setId(uint32_t AId)                      { MInfo.id = AId; }
  void setName(const char* AName)               { strcpy(MInfo.name,AName); }
  void setSupportedDialects(uint32_t ADialects) { MInfo.supported_dialects = ADialects; }
  void setPreferredDialect(uint32_t ADialect)   { MInfo.preferred_dialect = ADialect; }

  uint32_t    getId()                 { return MInfo.id; }
  const char* getName()               { return MInfo.name; }
  uint32_t    getSupportedDialects()  { return MInfo.supported_dialects; }
  uint32_t    getPreferredDialects()  { return MInfo.preferred_dialect; }

  clap_note_port_info_t* getInfo() {
    return &MInfo;
  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif


