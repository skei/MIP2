#ifndef mip_descriptor_included
#define mip_descriptor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"
//#include "base/utils/mip_utils.h"
#include "base/utils/mip_strutils.h"
#include "plugin/mip_parameter.h"

//----------------------------------------------------------------------

#define MIP_AUDIO_PORT_INPUT  0
#define MIP_AUDIO_PORT_OUTPUT 1

//----------

class MIP_AudioPort {

//------------------------------
public:
//------------------------------

  const char* name          = "";
  uint32_t    num_channels  = 0;
  //uint32_t    direction     = 0; // 0=in, 1=out

//------------------------------
public:
//------------------------------

  MIP_AudioPort(const char* AName, uint32_t ANumChannels) {
    name = AName;
    num_channels = ANumChannels;
  }

};

typedef MIP_Array<MIP_AudioPort*> MIP_AudioPortArray;

//----------

//struct MIP_NotePort {
//}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Descriptor {

//------------------------------
private:
//------------------------------

  char                  MVersionText[16]  = {0};
  char                  MLongId[16]       = {0};
  char                  MLongEditorId[16] = {0};
  char                  MStringId[256]    = {0};

//------------------------------
protected:
//------------------------------

  char                  MRealName[256]      = {0};

  const char*           MName               = "";
  const char*           MAuthor             = "";
  const char*           MDescription        = "";
  const char*           MEmail              = "";
  const char*           MUrl                = "";
  const char*           MManualUrl          = "";
  const char*           MSupportUrl         = "";
  const char*           MKeywords           = "";
  uint32_t              MVersion            = 0x00000000;
  uint32_t              MShortId            = 0;
  MIP_ParameterArray    MParameters         = {};
  MIP_AudioPortArray    MAudioInputs        = {};
  MIP_AudioPortArray    MAudioOutputs       = {};
  bool                  MIsSynth            = false;
  bool                  MIsMPE              = false;
  bool                  MCanReceiveMidi     = false;
  bool                  MCanSendMidi        = false;
  bool                  MHasEditor          = false;
  uint32_t              MEditorWidth        = 0;
  uint32_t              MEditorHeight       = 0;

//------------------------------
public:
//------------------------------

  MIP_Descriptor() {
  }

  virtual ~MIP_Descriptor() {
    #ifndef MIP_NO_AUTODELETE
    deleteParameters();
    deleteAudioInputs();
    deleteAudioOutputs();
    #endif
  }

//------------------------------
public:
//------------------------------


  const char*           getName()                   { return MName; }
  const char*           getAuthor()                 { return MAuthor; }
  const char*           getDescription()            { return MDescription; }
  const char*           getEmail()                  { return MEmail; }
  const char*           getUrl()                    { return MUrl; }
  const char*           getManualUrl()              { return MManualUrl; }
  const char*           getSupportUrl()             { return MSupportUrl; }
  const char*           getKeywords()               { return MKeywords; }
  uint32_t              getVersion()                { return MVersion; }
  uint32_t              getShortId()                { return MShortId; }
  MIP_ParameterArray*   getParameters()             { return &MParameters; }
  uint32_t              getNumParameters()          { return MParameters.size(); }
  MIP_Parameter*        getParameter(uint32_t i)    { return MParameters[i]; }
  MIP_AudioPortArray*   getAudioInputs()            { return &MAudioInputs; }
  uint32_t              getNumAudioInputs()         { return MAudioInputs.size(); }
  MIP_AudioPort*        getAudioInput(uint32_t i)   { return MAudioInputs[i]; }
  MIP_AudioPortArray*   getAudioOutputs()           { return &MAudioOutputs; }
  uint32_t              getNumAudioOutputs()        { return MAudioOutputs.size(); }
  MIP_AudioPort*        getAudioOutput(uint32_t i)  { return MAudioOutputs[i]; }
  bool                  isSynth()                   { return MIsSynth; }
  bool                  isMPE()                     { return MIsMPE; }
  bool                  canReceiveMidi()            { return MCanReceiveMidi; }
  bool                  canSendMidi()               { return MCanSendMidi; }
  bool                  hasEditor()                 { return MHasEditor; }
  uint32_t              getEditorWidth()            { return MEditorWidth; }
  uint32_t              getEditorHeight()           { return MEditorHeight; }

  const char* getRealName() {
    strcpy(MRealName,MName);
    #ifdef MIP_DEBUG
    strcat(MRealName," (debug)");
    #endif
    return MRealName;
  }

//------------------------------
public:
//------------------------------

  MIP_Parameter* appendParameter(MIP_Parameter* AParameter) {
    int32_t index = MParameters.size();
    AParameter->MIndex = index;
    MParameters.append(AParameter);
    return AParameter;
  }

  //----------

  void deleteParameters() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      delete MParameters[i];
    }
  }

  //----------

  int32_t findParameter(const char* AName) {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      const char* name = MParameters[i]->getName();
      if (strcmp(name,AName) == 0) return i;
    }
    return -1;
  }

  //----------

  MIP_AudioPort* appendInputPort(MIP_AudioPort* APort) {
    MAudioInputs.append(APort);
    return APort;
  }

  //----------

  void deleteAudioInputs() {
    for (uint32_t i=0; i<MAudioInputs.size(); i++) {
      delete MAudioInputs[i];
    }
  }

  //----------

  MIP_AudioPort* appendOutputPort(MIP_AudioPort* APort) {
    MAudioOutputs.append(APort);
    return APort;
  }

  //----------

  void deleteAudioOutputs() {
    for (uint32_t i=0; i<MAudioOutputs.size(); i++) {
      delete MAudioOutputs[i];
    }
  }

  //----------

  const char* getVersionText() {
    sprintf(MVersionText,"%i.%i.%i",
      ((MVersion & 0xff000000) >> 24),
      ((MVersion & 0x00ff0000) >> 16),
       (MVersion & 0x0000ffff)
    );
    return MVersionText;
  }

  //----------

  const char* getLongId() {
    uint32_t* id = (uint32_t*)MLongId;
    id[0] = MIP_MAGIC_M_PL;//MShortId;
    id[1] = MIP_HashString(MName);
    id[2] = MIP_HashString(MAuthor);
    id[3] = MVersion;
    return MLongId;
  }

  const char* getLongEditorId() {
    uint32_t* id = (uint32_t*)MLongId;
    id[0] = MIP_MAGIC_M_ED;//MShortId;
    id[1] = MIP_HashString(MName);
    id[2] = MIP_HashString(MAuthor);
    id[3] = MVersion;
    return MLongId;
  }

  //----------

  const char* getStringId() {
    strcpy(MStringId,MAuthor);
    strcat(MStringId,"/");
    strcpy(MStringId,MName);
    strcat(MStringId,"/");
    strcpy(MStringId,getVersionText());
    return MStringId;
  }

};

//----------------------------------------------------------------------
#endif
