#ifndef sa_botage_process_included
#define sa_botage_process_included
//----------------------------------------------------------------------

#include "mip.h"

#include "sa_botage_params.h"

//----------------------------------------------------------------------

#define BUFFER_SIZE   (1024 * 1024 * 16)

//----------------------------------------------------------------------

class sa_botage_process {

  friend class sa_botage_plugin;
  friend class sa_botage_editor;

//------------------------------
private:
//------------------------------

  double    MSampleRate               = 0.0;
  float     MLeftBuffer[BUFFER_SIZE]  = {0};
  float     MRightBuffer[BUFFER_SIZE] = {0};
  uint32_t  MBufferLength             = 0;
  double    MBufferLengthF            = 0;

  double    MReadPos                  = 0.0; // vari pitch
  uint32_t  MWritePos                 = 0;
  bool      MIsPlaying                = false;
  bool      MWasPlaying               = false;
  uint32_t  MPrevSlice                = -1;

  double    MReadSpeed                = 1.0;

  uint32_t  MSlice                    = 0;
  uint32_t  MSliceStart               = 0;
  uint32_t  MSliceLength              = 0;

  bool      MRange                    = false;
  uint32_t  MRangeStart               = 0;
  uint32_t  MRangeLength              = 0;

  bool      MLoop                     = false;
  uint32_t  MLoopStart                = 0;
  uint32_t  MLoopLength               = 0;

//------------------------------
private:
//------------------------------

  uint32_t  par_num_beats         = 4;
  uint32_t  par_num_slices        = 2;

  double    par_range_prob        = 0.0;
  uint32_t  par_range_slices      = 0;
  uint32_t  par_range_loops       = 0;

//------------------------------
public:
//------------------------------

  sa_botage_process() {
  }

  //----------

  ~sa_botage_process() {
  }

//------------------------------
public:
//------------------------------

  void activate(double sample_rate) {
    MSampleRate = sample_rate;
  }

  //----------

  void setParamValue(uint32_t param_id,double value) {
    switch (param_id) {
      case PAR_NUM_BEATS  : par_num_beats   = value; break;
      case PAR_NUM_SLICES : par_num_slices = value; break;
    }
  }

  //----------

  void transport(uint32_t flags) {
    if (flags & CLAP_TRANSPORT_IS_PLAYING) MIsPlaying = true;
    else MIsPlaying = false;
    if (MIsPlaying && !MWasPlaying) {
      //MIP_Print("play\n");
      MPrevSlice = -1;
      MReadPos = 0.0;
      MWritePos = 0;

    }
    else if (MWasPlaying && !MIsPlaying) {
      //MIP_Print("stop\n");
    }
    MWasPlaying = MIsPlaying;
  }

  //----------

  void processAudioBlock(const clap_process_t* process) {
    float* audioin0  = process->audio_inputs[0].data32[0];
    float* audioin1  = process->audio_inputs[0].data32[1];
    float* audioout0 = process->audio_outputs[0].data32[0];
    float* audioout1 = process->audio_outputs[0].data32[1];
    double seconds_per_beat = 60.0 / process->transport->tempo;
    double samples_per_beat = seconds_per_beat * MSampleRate;
    MBufferLengthF = (double)par_num_beats * samples_per_beat;
    MBufferLength = ceil(MBufferLengthF);
    uint32_t len = process->frames_count;

    if (MIsPlaying) {
      double slice_length = MBufferLengthF / (double)(par_num_beats * par_num_slices);
      for (uint32_t i=0; i<len; i++) {
        float in0 = *audioin0++;
        float in1 = *audioin1++;

        double slice_pos = (double)MWritePos / slice_length;
        MSlice = MIP_Trunc(slice_pos);
        //double slice_fract = slice_pos - MSlice; // 0..1
        MSliceStart = MSlice * slice_length;
        MSliceLength = slice_length;

        if (MSlice != MPrevSlice) {
          MIP_Print("New slice: %i\n",MSlice);
          // new slice
        }
        MPrevSlice = MSlice;

        uint32_t pos = MIP_Fract(MReadPos);
        float buf0 = MLeftBuffer[pos];
        float buf1 = MRightBuffer[pos];

        MLeftBuffer[MWritePos]  = in0;
        MRightBuffer[MWritePos] = in1;

        MReadPos += MReadSpeed;
        MReadPos = fmod(MReadPos,MBufferLength);

        MWritePos += 1;
        MWritePos %= MBufferLength;

        float out0 = in0 + buf0;
        float out1 = in1 + buf1;

        *audioout0++ = out0;
        *audioout1++ = out1;
      }
    }

    else {
      for (uint32_t i=0; i<len; i++) {
        float spl0 = *audioin0++;
        float spl1 = *audioin1++;
        *audioout0++ = spl0;
        *audioout1++ = spl1;
      }
    }

  }


};

//----------------------------------------------------------------------
#endif
