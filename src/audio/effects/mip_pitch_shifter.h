#ifndef kode_pitch_shifter_included
#define kode_pitch_shifter_included
//----------------------------------------------------------------------

class KODE_PitchShifter {

  private:

    float in_pos;
    float out_pos;
    float base;
    float old_base;
    float fade_base;
    float inc_;
    float len_;
    float fade_;
    float buffer[1024*1024*8];

  public:

    KODE_PitchShifter() {
      inc_      = 0;
      len_      = 0;
      fade_     = 0;
      in_pos    = 0;
      out_pos   = 0;
      base      = 0;
      old_base  = 0;
      fade_base = 0;
      KODE_Memset(buffer,0,sizeof(buffer));
    }

    ~KODE_PitchShifter() {
    }

  public:

    // -1..1

    void setPitch(float APitch) {
      //inc_  = APitch/100;
      if (APitch >= 0)      inc_ = 1 + APitch;
      else if (APitch < 0)  inc_ = 1 + (APitch*0.5); // -1 -> 1+(-1*0.5) = 1 + -0.5 = 0.5
      //else inc_ = 1;
    }

    //----------

    // 1..200 ms

    void setWindow(float AWindow, float ASampleRate) {
      len_  = floorf(AWindow/1000*ASampleRate/2)*2;
    }

    //----------

    // 0..100 %
    // req: len_

    void setCrossFade(float APercent) {
      fade_ = APercent/100 * len_/2;
    }

    //----------

    void process(float* spl0, float* spl1) {
      uint32_t p = (uint32_t)floorf(in_pos);
      buffer[ p*2] = *spl0;
      buffer[ p*2+1] = *spl1;
      in_pos = (uint32_t)floorf(in_pos+1) % (uint32_t)floorf(len_);
      float gain = min(out_pos/fade_,1.0f);
      uint32_t p0 = (uint32_t)floorf(base+out_pos) % (uint32_t)floorf(len_);
      uint32_t p1 = (uint32_t)floorf(fade_base+out_pos) % (uint32_t)floorf(len_);
      *spl0 = buffer[ p0*2] * gain
            + buffer[ p1*2] * (1-gain);
      *spl1 = buffer[ p0*2+1] * gain
            + buffer[ p1*2+1] * (1-gain);
      out_pos += inc_;
      if (out_pos >= (len_-1-fade_)) {
        fade_base = base + len_ - 1 - fade_;
        out_pos = 0;
        base = in_pos;
      }
    }

};

//----------------------------------------------------------------------
#endif
