#ifndef mip_voice_manager_included
#define mip_voice_manager_included
//----------------------------------------------------------------------

#include "mip.h"
#include "audio/mip_audio_utils.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define MIP_VOICE_MAX_CHANNELS    16
#define MIP_VOICE_MAX_NOTES       128
#define MIP_VOICE_NOTES           (MIP_VOICE_MAX_CHANNELS * MIP_VOICE_MAX_NOTES)
#define MIP_MASTER_BEND_RANGE     2.0f
#define MIP_VOICE_BEND_RANGE      48.0f
#define MIP_INV_MASTER_BEND_RANGE (1.0 / MIP_MASTER_BEND_RANGE)
#define MIP_INV_VOICE_BEND_RANGE  (1.0 / MIP_VOICE_BEND_RANGE)

#define MIP_VOICE_SLICE_SIZE      16
#define MIP_VOICE_MAX_OVERSAMPLE  1
#define MIP_VOICE_BUFFERSIZE      (MIP_VOICE_SLICE_SIZE * MIP_VOICE_MAX_OVERSAMPLE)
#define MIP_VOICE_MAX_EVENTS      1024

//----------------------------------------------------------------------

struct MIP_VoiceContext {
  const clap_process_t* process     = nullptr;
  double                samplerate  = 0.0;
};

//----------

typedef MIP_Queue<uint32_t,256> MIP_NoteEndQueue;

//----------------------------------------------------------------------
//
// slice buffer
//
//----------------------------------------------------------------------

__MIP_ALIGNED(MIP_ALIGNMENT_CACHE)
float MIP_VoiceSliceBuffer[MIP_VOICE_BUFFERSIZE] = {0};

//----------

void MIP_ClearVoiceSliceBuffer(uint32_t ASize) {
  memset(MIP_VoiceSliceBuffer,0,ASize*sizeof(float));
}

void MIP_CopyVoiceSliceBuffer(float* ADst, uint32_t ASize) {
  memcpy(ADst,MIP_VoiceSliceBuffer,ASize*sizeof(float));
}

void MIP_ClearVoiceSliceBuffer() {
  MIP_ClearVoiceSliceBuffer(MIP_VOICE_SLICE_SIZE);
}

void MIP_CopyVoiceSliceBuffer(float* ADst) {
  MIP_CopyVoiceSliceBuffer(ADst,MIP_VOICE_SLICE_SIZE);
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class VOICE, int NUM>
class MIP_VoiceManager {

//------------------------------
private:
//------------------------------

  MIP_VoiceContext  MVoiceContext                 = {};
  VOICE             MVoices[NUM]                  = {};
  uint32_t          MVoiceStates[NUM]             = {};
  int32_t           MNoteToVoice[MIP_VOICE_NOTES] = {}; // 128*16, chan/key -> voice


//------------------------------
public:
//------------------------------

  MIP_VoiceManager() {
  }

  //----------

  virtual ~MIP_VoiceManager() {
  }

//------------------------------
public:
//------------------------------

  void setVoiceParameter(uint32_t AIndex, float AValue) {
  }

  //----------

  uint32_t getVoiceState(uint32_t AIndex) {
    return 0;
  }

  //----------

  //uint32_t getMaxVoices() {
  //  return NUM;
  //}

  //----------

  void prepareVoices(double samplerate) {
    MVoiceContext.samplerate = samplerate;
  }

  //----------

  void startVoice(uint32_t AChannel, uint32_t AKey, float AVelocity) {
    MIP_Print("start voice\n");
    uint32_t index = (AChannel * MIP_VOICE_MAX_NOTES) + AKey;
    int32_t voice = MNoteToVoice[index];
    // did a voice alreay play for this chan/key
    if (voice >= 0) {
      // steal it
    }
    else {
      // find a non-playing, or released voice
      voice = findVoice();
    }
    // do we have a voice?
    if (voice >= 0) {
      MNoteToVoice[index] = voice;
    }
    else {
    }
  }

  //----------

  void releaseVoice(uint32_t AChannel, uint32_t AKey, float AVelocity) {
    MIP_Print("release voice\n");
    uint32_t index = (AChannel * MIP_VOICE_MAX_NOTES) + AKey;
    MNoteToVoice[index] = 0;
  }

  //----------

  void endVoice(uint32_t AChannel, uint32_t AKey) {
    MIP_Print("end voice\n");
    uint32_t index = (AChannel * MIP_VOICE_MAX_NOTES) + AKey;
    MNoteToVoice[index] = 0;
  }

  //----------

  void chokeVoice(uint32_t AChannel, uint32_t AKey, float AVelocity) {
    MIP_Print("choke voice\n");
    uint32_t index = (AChannel * MIP_VOICE_MAX_NOTES) + AKey;
    MNoteToVoice[index] = 0;
  }

  //----------

  //void handle_master_param(uint32_t AIndex, float AValue) {
  //}

  //uint32_t  getVoiceState(uint32_t v) {
  //  return 0;
  //}

  //----------

  void process(const clap_process_t *process) {
  }

//------------------------------
private:
//------------------------------

  int32_t findVoice(bool ATryReleased=true) {
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceStates[i] == MIP_VOICE_OFF) return i;
      if (MVoiceStates[i] == MIP_VOICE_FINISHED) return i;
    }
    if (ATryReleased) {
      for (uint32_t i=0; i<NUM; i++) {
        if (MVoiceStates[i] == MIP_VOICE_RELEASED) return i;
      }
    }
    return -1;
  }

};

//----------------------------------------------------------------------
#endif
