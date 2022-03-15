#ifndef audio_file_included
#define audio_file_included
//----------------------------------------------------------------------

#define AUDIO_FILE_READ   0x10
#define AUDIO_FILE_WRITE  0x20
#define AUDIO_FILE_RDWR   0x30

#include <sndfile.h>

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class AudioFile {

//------------------------------
private:
//------------------------------

  SNDFILE*    MHandle     = nullptr;
  SF_INFO     MInfo       = {0};

  const char* MPath       = nullptr;
  uint32_t    MMode       = AUDIO_FILE_READ;
  uint32_t    MSapleRate  = 44100;
  uint32_t    MChannels   = 2;

//------------------------------
public:
//------------------------------

  AudioFile() {
  }

  //----------

  ~AudioFile() {
  }

  //----------

  SF_INFO* getInfo() {
    return &MInfo;
  }

//------------------------------
public:
//------------------------------

  bool open(const char *APath, uint32_t AMode=AUDIO_FILE_READ, uint32_t ASampleRate=44100, uint32_t AChannels=2) {

    MPath       = APath;
    MMode       = AMode;
    MSapleRate  = ASampleRate;
    MChannels   = AChannels;

    switch (MMode) {

      case AUDIO_FILE_READ:

        MHandle = sf_open(APath,SFM_READ,&MInfo);
        if (!MHandle) {
          printf("couldn't open \"%s\": %s\n", MPath, sf_strerror(MHandle));
          return false;
        }
        return true;

      case AUDIO_FILE_WRITE:

        MInfo.frames     = 0;
        MInfo.samplerate = ASampleRate;
        MInfo.channels   = AChannels;
        MInfo.format     = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
        MInfo.sections   = 0;
        MInfo.seekable   = 0;
        MHandle = sf_open(APath,SFM_WRITE,&MInfo);
        if (!MHandle) {
          printf("couldn't open \"%s\": %s\n", MPath, sf_strerror(MHandle));
          return false;
        }
        sf_command(MHandle,SFC_SET_ADD_PEAK_CHUNK,nullptr,SF_FALSE);
        return true;

      //case AUDIO_FILE_RDWR:
      //  return false;

    } // switch

    return false;
  }

  //----------

  void close() {
    if (MHandle) sf_close(MHandle);
    MHandle = nullptr;
  }

  //----------

  int32_t read(uint32_t AChannels, uint32_t ABlockSize, float **ABuffers) {
    float read_buf[AChannels];
    uint32_t f;
    for (f=0; f<ABlockSize; f++) {
      if (!sf_readf_float(MHandle,read_buf,1)) break;
      for (uint32_t c=0; c<AChannels; c++) ABuffers[c][f] = read_buf[c];
    }
    uint32_t ret = f;
    for (; f<ABlockSize; f++) {
      for (uint32_t c=0; c<AChannels; c++) {
        ABuffers[c][f] = 0.0f;
      }
    }
    return ret;
  }

  //----------

  void write(uint32_t AChannels, uint32_t ABlockSize, float **ABuffers) {
    float write_buf[AChannels];
    uint32_t f;
    for (f = 0; f < ABlockSize; f++) {
      for (uint32_t c=0; c<AChannels; c++) {
        write_buf[c] = ABuffers[c][f];
      }
      sf_writef_float(MHandle,write_buf,1);
    }
  }

  //----------

  void printInfo() {
    printf("  path:               '%s'\n",MPath);
    printf("  - MInfo.frames      %i\n",(int)MInfo.frames);
    printf("  - MInfo.samplerate  %i\n",MInfo.samplerate);
    printf("  - MInfo.channels    %i\n",MInfo.channels);
    printf("  - MInfo.format      %i\n",MInfo.format);
    printf("  - MInfo.sections    %i\n",MInfo.sections);
    printf("  - MInfo.seekable    %i\n",MInfo.seekable);
    printf("\n");
  }

};

//----------------------------------------------------------------------
#endif
