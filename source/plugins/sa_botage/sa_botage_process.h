#ifndef sa_botage_process_included
#define sa_botage_process_included
//----------------------------------------------------------------------

#include "mip.h"

#include "sa_botage_params.h"

//----------------------------------------------------------------------

#define BUFFER_SIZE       (1024 * 1024 * 16)

#define MIN_LOOP_LENGTH   64
#define MIN_READ_SPEED    (1.0 / 8.0)
#define MAX_READ_SPEED    8.0

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
  int32_t   MPrevSlice                = -1;

  double    MReadSpeed                = 1.0;

  int32_t   MSlice                    = 0;
  double    MSliceFract               = 0.0;
  uint32_t  MSliceStart               = 0;
  uint32_t  MSliceLength              = 0;

  bool      MRange                    = false;
  //double    MRangeFract               = 0.0;
  uint32_t  MRangeStart               = 0;
  uint32_t  MRangeLength              = 0;
  uint32_t  MRangeCount               = 0;

  bool      MLoop                     = false;
  //double    MLoopFract                = 0.0;
  uint32_t  MLoopStart                = 0;
  uint32_t  MLoopLength               = 0;
  uint32_t  MLoopCount                = 0;

  double    MLoopFract                = 0.0;

  bool      MLoopWrapped              = false;

//------------------------------
private:
//------------------------------

  // these are not automatically initialized..

  uint32_t  par_num_beats               = 4;
  uint32_t  par_num_slices              = 2;

  double    par_range_prob              = 0.25;
  uint32_t  par_range_slice_count       = 15;
  uint32_t  par_range_loop_count        = 15;

  double    par_loop_env_attack         = 0.0;
  double    par_loop_env_decay          = 0.0;
  double    par_slice_env_attack        = 0.0;
  double    par_slice_env_decay         = 0.0;

  double    par_fx_filter_prob          = 0.0;
  uint32_t  par_fx_filter_type          = 0;
  double    par_fx_filter_freq          = 0.0;
  double    par_fx_filter_bw            = 0.0;

  double    par_prob_size_prob_range    = 0.0;
  double    par_prob_size_min_range     = 0.0;
  double    par_prob_size_max_range     = 0.0;
  double    par_prob_size_prob_loop     = 0.0;
  double    par_prob_size_min_loop      = 0.0;
  double    par_prob_size_max_loop      = 0.0;

  double    par_prob_speed_prob_range   = 0.0;
  double    par_prob_speed_min_range    = 0.0;
  double    par_prob_speed_max_range    = 0.0;
  double    par_prob_speed_prob_loop    = 0.0;
  double    par_prob_speed_min_loop     = 0.0;
  double    par_prob_speed_max_loop     = 0.0;

  double    par_prob_offset_prob_range  = 0.0;
  double    par_prob_offset_min_range   = 0.0;
  double    par_prob_offset_max_range   = 0.0;
  double    par_prob_offset_prob_loop   = 0.0;
  double    par_prob_offset_min_loop    = 0.0;
  double    par_prob_offset_max_loop    = 0.0;

  double    par_prob_reverse_prob_range = 0.0;
  double    par_prob_reverse_prob_loop  = 0.0;

  double    par_prob_fx_prob_range      = 0.0;
  double    par_prob_fx_min_range       = 0.0;
  double    par_prob_fx_max_range       = 0.0;
  double    par_prob_fx_prob_loop       = 0.0;
  double    par_prob_fx_min_loop        = 0.0;
  double    par_prob_fx_max_loop        = 0.0;

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
      case PAR_NUM_BEATS                : par_num_beats               = value; break;
      case PAR_NUM_SLICES               : par_num_slices              = value; break;
      case PAR_RANGE_PROB               : par_range_prob              = value; break;
      case PAR_RANGE_SLICE_COUNT        : par_range_slice_count       = value; break;
      case PAR_RANGE_LOOP_COUNT         : par_range_loop_count        = value; break;
      case PAR_LOOP_ENV_ATTACK          : par_loop_env_attack         = value; break;
      case PAR_LOOP_ENV_DECAY           : par_loop_env_decay          = value; break;
      case PAR_SLICE_ENV_ATTACK         : par_slice_env_attack        = value; break;
      case PAR_SLICE_ENV_DECAY          : par_slice_env_decay         = value; break;
      case PAR_FX_FILTER_PROB           : par_fx_filter_prob          = value; break;
      case PAR_FX_FILTER_TYPE           : par_fx_filter_type          = value; break;
      case PAR_FX_FILTER_FREQ           : par_fx_filter_freq          = value; break;
      case PAR_FX_FILTER_BW             : par_fx_filter_bw            = value; break;
      case PAR_PROB_SIZE_PROB_RANGE     : par_prob_size_prob_range    = value; break;
      case PAR_PROB_SIZE_MIN_RANGE      : par_prob_size_min_range     = value; break;
      case PAR_PROB_SIZE_MAX_RANGE      : par_prob_size_max_range     = value; break;
      case PAR_PROB_SIZE_PROB_LOOP      : par_prob_size_prob_loop     = value; break;
      case PAR_PROB_SIZE_MIN_LOOP       : par_prob_size_min_loop      = value; break;
      case PAR_PROB_SIZE_MAX_LOOP       : par_prob_size_max_loop      = value; break;
      case PAR_PROB_SPEED_PROB_RANGE    : par_prob_speed_prob_range   = value; break;
      case PAR_PROB_SPEED_MIN_RANGE     : par_prob_speed_min_range    = value; break;
      case PAR_PROB_SPEED_MAX_RANGE     : par_prob_speed_max_range    = value; break;
      case PAR_PROB_SPEED_PROB_LOOP     : par_prob_speed_prob_loop    = value; break;
      case PAR_PROB_SPEED_MIN_LOOP      : par_prob_speed_min_loop     = value; break;
      case PAR_PROB_SPEED_MAX_LOOP      : par_prob_speed_max_loop     = value; break;
      case PAR_PROB_OFFSET_PROB_RANGE   : par_prob_offset_prob_range  = value; break;
      case PAR_PROB_OFFSET_MIN_RANGE    : par_prob_offset_min_range   = value; break;
      case PAR_PROB_OFFSET_MAX_RANGE    : par_prob_offset_max_range   = value; break;
      case PAR_PROB_OFFSET_PROB_LOOP    : par_prob_offset_prob_loop   = value; break;
      case PAR_PROB_OFFSET_MIN_LOOP     : par_prob_offset_min_loop    = value; break;
      case PAR_PROB_OFFSET_MAX_LOOP     : par_prob_offset_max_loop    = value; break;
      case PAR_PROB_REVERSE_PROB_RANGE  : par_prob_reverse_prob_range = value; break;
      case PAR_PROB_REVERSE_PROB_LOOP   : par_prob_reverse_prob_loop  = value; break;
      case PAR_PROB_FX_PROB_RANGE       : par_prob_fx_prob_range      = value; break;
      case PAR_PROB_FX_MIN_RANGE        : par_prob_fx_min_range       = value; break;
      case PAR_PROB_FX_MAX_RANGE        : par_prob_fx_max_range       = value; break;
      case PAR_PROB_FX_PROB_LOOP        : par_prob_fx_prob_loop       = value; break;
      case PAR_PROB_FX_MIN_LOOP         : par_prob_fx_min_loop        = value; break;
      case PAR_PROB_FX_MAX_LOOP         : par_prob_fx_max_loop        = value; break;
    }
  }

  //----------

  void transport(uint32_t flags) {

    if (flags & CLAP_TRANSPORT_IS_PLAYING) {
      MIsPlaying = true;
    }
    else {
      MIsPlaying = false;
      MRange = false;
      MLoop = false;
      MReadPos = 0.0;
      MWritePos = 0;
    }

    if (MIsPlaying && !MWasPlaying) {
      //MIP_Print("play\n");
      MPrevSlice = -1;
      MReadPos = 0.0;
      MWritePos = 0;
    }
    else if (MWasPlaying && !MIsPlaying) {
      //MIP_Print("stop\n");
      // reset waveform areas/markers..
    }
    MWasPlaying = MIsPlaying;
  }

//------------------------------
public:
//------------------------------

  void processAudioBlock(MIP_ProcessContext* AContext) {

    const clap_process_t* process = AContext->process;

    float* audioin0  = process->audio_inputs[0].data32[0];
    float* audioin1  = process->audio_inputs[0].data32[1];
    float* audioout0 = process->audio_outputs[0].data32[0];
    float* audioout1 = process->audio_outputs[0].data32[1];

    // buffer length

    MIP_Assert( process->transport->tempo > 0.0);
    MIP_Assert( par_num_beats > 0 );
    MIP_Assert( par_num_slices > 0);

    double seconds_per_beat = 60.0 / process->transport->tempo;
    double samples_per_beat = seconds_per_beat * MSampleRate;
    MBufferLengthF = (double)par_num_beats * samples_per_beat;
    MBufferLength = ceil(MBufferLengthF);

    uint32_t len = process->frames_count;

    if (MIsPlaying) {

      double slice_length = MBufferLengthF / (double)(par_num_beats * par_num_slices);
      for (uint32_t i=0; i<len; i++) {

        // get input

        float in0 = *audioin0++;
        float in1 = *audioin1++;

        // write to buffer

        MLeftBuffer[MWritePos]  = in0;
        MRightBuffer[MWritePos] = in1;
        MWritePos += 1;
        if (MWritePos >= MBufferLength) { // will wrap..
          handleBufferWrapping();
        }
        MWritePos %= MBufferLength;

        // (re-) calc slice length

        MIP_Assert( slice_length > 0.0 );

        double slice_pos = (double)MWritePos / slice_length;
        MSlice = MIP_Trunc(slice_pos);
        MSliceFract = slice_pos - MSlice; // 0..1
        MSliceStart = MSlice * slice_length;
        MSliceLength = slice_length;

        // loop fract

        if (MLoop) {
          if (MLoopLength > 0) {
            MLoopFract = (MReadPos - MLoopStart) / MLoopLength;
          }
        }
        else {
          MLoopFract = 0.0;
        }

        // count slices, samples

        if (MSlice != MPrevSlice) nextSlice();
        MPrevSlice = MSlice;
        countSamples();

        // read from buffer

        uint32_t pos = MIP_Trunc(MReadPos);
        float buf0 = MLeftBuffer[pos];
        float buf1 = MRightBuffer[pos];
        MReadPos += MReadSpeed;
        MReadPos = fmod(MReadPos,MBufferLength);

        // out

        float out0 = in0;
        float out1 = in1;

        if (MLoop) {
          //if (MLoopWrapped) {
            // only after we have wrapped around, so the content has changed
            // first time around, the buffer might be empty..
            out0 = buf0;
            out1 = buf1;
          //}
        }

        // envelopes
        float env = handleEnvelopes(MSliceFract,MLoopFract);
        out0 *= env;
        out1 *= env;

        // finat output

        *audioout0++ = out0;
        *audioout1++ = out1;
      }
    }

    else { // playing
      for (uint32_t i=0; i<len; i++) {
        float spl0 = *audioin0++;
        float spl1 = *audioin1++;
        *audioout0++ = spl0;
        *audioout1++ = spl1;
      }
    }

  }

//------------------------------
private:
//------------------------------

  // in = uint32-t bitfield
  // return = index of selected bit

  uint32_t randomBit(uint32_t AValue) {
    uint32_t numbits = 0;
    uint32_t bit = 1;
    for (uint32_t i=0; i<32; i++) {
      if (AValue & bit) numbits += 1;
      bit <<= 1;
    }
    if (numbits == 0) return 0;
    uint32_t rnd = MIP_RandomRangeInt(0,numbits-1);
    bit = 1;
    for (uint32_t i=0; i<32; i++) {
      if (AValue & bit) {
        if (rnd == 0) {
          //MIP_Print("i %i bit %i\n",i,bit);
          return (i + 1);
        }
        rnd -= 1;
      }
      bit <<= 1;
    }
    return 0;
  }

//------------------------------
private:
//------------------------------

  /*
    called every time we enter a new slice
  */

  void nextSlice() {
    if (!MRange) {
      // assume off
      MRange = false;
      MLoop = false;
      // range
      if (MIP_RandomRange(0.0, 0.999) < par_range_prob) {
        startRange();
      }
    }
  }

  //----------

  // called every sample

  void countSamples() {
    if (MRange) {
      MRangeCount += 1;
      if (MRangeCount >= MRangeLength) { // range finished?
        endRange();
      }
    }
    if (MLoop) {
      MLoopCount += 1;
      if (MLoopCount >= MLoopLength) {
        handleLoopWrapping();
      }
    }
  }

//------------------------------
private:
//------------------------------

  /*
    called from nextSlice, if prob resulted in new range
  */

  void startRange() {
    uint32_t remain = (par_num_beats * par_num_slices) - MSlice;

    uint32_t num_slices = randomBit(par_range_slice_count);
    num_slices = MIP_MaxI(num_slices,1);
    num_slices = MIP_MinI(num_slices,remain);

    uint32_t num_loops = randomBit(par_range_loop_count);
    num_loops = MIP_MaxI(num_loops,1);

    MRange        = true;
    MRangeCount   = 0;
    MRangeStart   = MSliceStart;
    MRangeLength  = MSliceLength * num_slices;

    MLoop         = true;
    MLoopCount    = 0;
    MLoopStart    = MSliceStart;
    MLoopLength   = MRangeLength / num_loops;
    MLoopWrapped  = false;

    handleLoopStarting();
  }

  //----------

  /*
    called from countSamples, after range has ended
  */

  void endRange() {
    MRange = false;
    MLoop = false;
    MPrevSlice = -1;
    MReadPos = MWritePos;
    MReadSpeed = 1.0;
  }

  //----------

  /*
    called from startRange
    (after range/loop has been defined)
  */

  void handleLoopStarting() {
    // probs
  }

  //----------

  /*
    called from countSamples, after loop has ended
    (wraparound)
  */

  void handleLoopWrapping() {
    MLoopWrapped = true;
    MLoopCount = 0;
    MReadPos = MLoopStart;

    // probs

//    MReadSpeed *= 0.8;
//    MReadSpeed = MIP_Clamp(MReadSpeed,MIN_READ_SPEED,MAX_READ_SPEED);

//    MLoopLength = MIP_Trunc((double)MLoopLength * 0.8);
//    MLoopLength = MIP_MaxI(MLoopLength,MIN_LOOP_LENGTH);

  }

  //----------

  void handleBufferWrapping() {
    //MIP_Print("buffer finished\n");
    MPrevSlice = -1;
  }

//------------------------------
private:
//------------------------------

  float handleEnvelopes(float ASliceFract, float ALoopFract) {
    float env = 1.0f;
    float sa = par_slice_env_attack;  // * MSliceLength;
    float sd = par_slice_env_decay;   // * MSliceLength;
    if ((sa > 0) && (ASliceFract <         sa))  env *= (       ASliceFract  / sa);
    if ((sd > 0) && (ASliceFract >= (1.0 - sd))) env *= ((1.0 - ASliceFract) / sd);
//    if (MLoop) {
//      float la = par_loop_env_attack * (MSampleRate / 10.0);  // 0.01 in on_parameter
//      float ld = par_loop_env_decay  * (MSampleRate / 10.0);  // 1/100 / 10 = 1/1000
//      if ((la > 0) && (ALoopFract <         la))  env *= (       MLoopFract  / la);
//      if ((ld > 0) && (ALoopFract >= (1.0 - ld))) env *= ((1.0 - ALoopFract) / ld);
//    }
    return env;
  }

};

//----------------------------------------------------------------------
#endif
