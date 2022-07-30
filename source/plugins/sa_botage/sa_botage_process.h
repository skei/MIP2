#ifndef sa_botage_process_included
#define sa_botage_process_included
//----------------------------------------------------------------------

#include "mip.h"

//----------------------------------------------------------------------

/*
  30 bpm, 192khz, 8 beats (2 bars)
  60 bpm = 1 sec per beat
  30 bpm = 2 sec..
  8 beats = 16 sec
  @48k = 768
  @192 = 3072
  stereo = 6144
*/

#define BUFFER_SIZE   (1024 * 1024 * 32)

//----------------------------------------------------------------------

class sa_botage_process {

  friend class sa_botage_plugin;
  friend class sa_botage_editor;

//------------------------------
private:
//------------------------------

  bool      MIsPlaying            = false;
  bool      MWasPlaying           = false;
  double    MSampleRate           = 0.0;
  float     MBuffer[BUFFER_SIZE]  = {0};
  uint32_t  MBufferLength         = 0;

  uint32_t  MReadPos              = 0;
  uint32_t  MWritePos             = 0;

  uint32_t  MCurrentSlice         = 0;
  uint32_t  MCurrentSliceStart    = 0;
  uint32_t  MCurrentSliceLength   = 0;

  uint32_t  par_num_beats         = 4;
  uint32_t  par_num_slices        = 2;

//------------------------------
public:
//------------------------------

  sa_botage_process() {
  }

  ~sa_botage_process() {
  }

//------------------------------
public:
//------------------------------

  void activate(double sample_rate) {
    MSampleRate = sample_rate;
  }


  void setParamValue(uint32_t param_id,double value) {
    switch (param_id) {
      case PAR_NUM_BEATS  : par_num_beats   = value; break;
      case PAR_NUM_SLICES : par_num_slices = value; break;
    }
  }

  void transport(uint32_t flags) {
    if (flags & CLAP_TRANSPORT_IS_PLAYING) MIsPlaying = true;
    else MIsPlaying = false;
    if (MIsPlaying && !MWasPlaying) {
      MIP_Print("play\n");
      MReadPos = 0;
      MWritePos = 0;
    }
    else if (MWasPlaying && !MIsPlaying) {
      MIP_Print("stop\n");
    }
    MWasPlaying = MIsPlaying;
  }

  void processAudioBlock(const clap_process_t* process) {
    double seconds_per_beat = 60.0 / process->transport->tempo;
    double samples_per_beat = seconds_per_beat * MSampleRate;
    MBufferLength = par_num_beats * samples_per_beat;

    float* audioin0 = process->audio_inputs[0].data32[0];
    float* audioin1 = process->audio_inputs[0].data32[1];
    float* audioout0 = process->audio_outputs[0].data32[0];
    float* audioout1 = process->audio_outputs[0].data32[1];

    uint32_t len = process->frames_count;
    for (uint32_t i=0; i<len; i++) {
      float in0 = *audioin0++;
      float in1 = *audioin1++;

      if (MIsPlaying) {

        double slice_length = MBufferLength / (par_num_beats * par_num_slices);
        double slice_pos = (double)MWritePos / slice_length;
        MCurrentSlice = (uint32_t)slice_pos;
        //MIP_Print("MCurrentSlice %i\n",MCurrentSlice);

        //float slice_sub_pos = slice_pos - MCurrentSlice;

        MCurrentSliceStart = MCurrentSlice * slice_length;
        MCurrentSliceLength = slice_length;

        //float spl = MBuffer[MReadPos];
        MReadPos += 1;
        MReadPos %= MBufferLength;

        MBuffer[MWritePos] = in0; // left..

        MWritePos += 1;
        MWritePos %= MBufferLength;


      }

      *audioout0++ = in0;
      *audioout1++ = in1;
    }

  }


};

//----------------------------------------------------------------------
#endif
