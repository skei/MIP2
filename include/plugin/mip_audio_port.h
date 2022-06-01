#ifndef mip_audio_port_included
#define mip_audio_port_included
//----------------------------------------------------------------------

#include "mip.h"
//#include "base/types/mip_array.h"
//#include "base/types/mip_queue.h"
//#include "base/utils/mip_math.h"
#include "extern/clap/clap.h"

class MIP_AudioPort;
typedef MIP_Array<MIP_AudioPort*> MIP_AudioPortArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_AudioPort {

//------------------------------
private:
//------------------------------

  clap_audio_port_info_t MInfo  = {
    .id             = 0,    // set this in appendAudioPort
    .name           = {0},
    .flags          = 0,    // CLAP_AUDIO_PORT_IS_MAIN
    .channel_count  = 0,
    .port_type      = {0},  // CLAP_PORT_STEREO
    .in_place_pair  = CLAP_INVALID_ID
  };

//------------------------------
public:
//------------------------------

  MIP_AudioPort() {  }

  //----------

  MIP_AudioPort(const char* AName, uint32_t AFlags=CLAP_AUDIO_PORT_IS_MAIN, uint32_t AChannels=2, const char* APortType=CLAP_PORT_STEREO, uint32_t AInPlacePair=CLAP_INVALID_ID) {
    //MInfo.id = 0;
    strncpy(MInfo.name,AName,CLAP_NAME_SIZE);
    MInfo.flags         = AFlags;
    MInfo.channel_count = AChannels;
    MInfo.port_type     = APortType;
    MInfo.in_place_pair = AInPlacePair;
  }

  //----------

  ~MIP_AudioPort() {  }

//------------------------------
public:
//------------------------------

  // set/get

  void setId(uint32_t AId)              { MInfo.id = AId; }
  void setName(const char* AName)       { strcpy(MInfo.name,AName); }
  void setFlags(uint32_t AFlags)        { MInfo.flags = AFlags; }
  void setChannelCount(uint32_t ACount) { MInfo.channel_count = ACount; }
  void setPortType(const char* AType)   { strcpy((char*)MInfo.port_type,AType); }
  void setInPlacePair(uint32_t APair)   { MInfo.in_place_pair = APair; }

  uint32_t    getId()           { return MInfo.id; }
  const char* getName()         { return MInfo.name; }
  uint32_t    getFlags()        { return MInfo.flags; }
  uint32_t    getChannelCount() { return MInfo.channel_count; }
  const char* getPortType()     { return MInfo.port_type; }
  uint32_t    getInPlacePair()  { return MInfo.in_place_pair; }

  clap_audio_port_info_t* getInfo() {
    return &MInfo;
  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif

