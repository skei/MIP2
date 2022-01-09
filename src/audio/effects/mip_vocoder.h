#ifndef mip_vocoder_included
#define mip_vocoder_included
//----------------------------------------------------------------------

//#define AMPLIFIER         16.0
//#define PRE_CARRIER       1
//#define PRE_FORMANT       1
//#define POST_CARRIER      2
//#define POST_FORMANT      2
//#define CLAMP(x,low,high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

#define MAX_BANDS 16

//----------

struct MIP_VocoderBandpass {
  float c, f, att;
  float freq;
  float low1, low2;
  float mid1, mid2;
  float high1, high2;
  float y;
};

struct MIP_VocoderBandsOut {
  float decay;
  float oldval;
  float level;		// 0.0 - 1.0 level of this output band
};

const float MIP_VocoderDecayTable[] = {
  1/100.0,
  1/100.0, 1/100.0, 1/100.0,
  1/125.0, 1/125.0, 1/125.0,
  1/166.0, 1/166.0, 1/166.0,
  1/200.0, 1/200.0, 1/200.0,
  1/250.0, 1/250.0, 1/250.0
};

//----------------------------------------------------------------------

class MIP_Vocoder {

  private:

    float                   MSampleRate               = 0.0f;
    uint32_t                MBandsCount               = 0;
    MIP_VocoderBandpass   MBandsFormant[MAX_BANDS]  = {};
    MIP_VocoderBandpass   MBandsCarrier[MAX_BANDS]  = {};
    MIP_VocoderBandsOut  MBandsOut[MAX_BANDS]      = {};
    float                   MBandsLevel[MAX_BANDS]    = {};

    float MPreCarrierGain   = 1.0f;
    float MPreFormantGain   = 1.0f;
    //float MPostCarrierGain  = 2.0f;
    //float MPostFormantGain  = 2.0f;

  public:

    MIP_Vocoder() {
    }

    //----------

    ~MIP_Vocoder() {
    }

  public:

    void setSampleRate(float ARate)       { MSampleRate = ARate; }
    void setPreCarrierGain(float AGain)   { MPreCarrierGain = AGain; }
    void setPreFormantGain(float AGain)   { MPreFormantGain = AGain; }
    //void setPostCarrierGain(float AGain)  { MPostCarrierGain = AGain; }
    //oid setPostFormantGain(float AGain)   { MPostFormantGain = AGain; }

    //----------

    /*
      skei: where does all these constants come from?
    */

    void setNumBands(uint32_t ANumBands) {
      MBandsCount = ANumBands;
      for (uint32_t i=0; i<ANumBands; i++) {
        MIP_Memset(&MBandsFormant[i], 0, sizeof(MIP_VocoderBandpass));
        // stretch existing bands
        float a = 16.0 * i/(float)ANumBands;
        if (a < 4.0) MBandsFormant[i].freq = 150 + 420 * a / 4.0;
        else MBandsFormant[i].freq = 600 * pow (1.23, a - 4.0);

        float c = MBandsFormant[i].freq * MIP_PI2 / MSampleRate;
        MBandsFormant[i].c = c * c;
        MBandsFormant[i].f = 0.4/c;
        MBandsFormant[i].att = 1/(6.0 + ((exp (MBandsFormant[i].freq / MSampleRate) - 1) * 10));
        MIP_Memcpy(&MBandsCarrier[i], &MBandsFormant[i], sizeof(MIP_VocoderBandpass));
        MBandsOut[i].decay = MIP_VocoderDecayTable[(int32_t)a];
        //MBandsOut[i].level = CLAMP(MBandsLevel[i], 0.0, 1.0);
      }
    }

  private:

    void processBandpasses(MIP_VocoderBandpass* bands, float sample) {
      for (uint32_t i=0; i < MBandsCount; i++) {
        bands[i].high1 = sample - bands[i].f * bands[i].mid1 - bands[i].low1;
        bands[i].mid1 += bands[i].high1 * bands[i].c;
        bands[i].low1 += bands[i].mid1;
        bands[i].high2 = bands[i].low1 - bands[i].f * bands[i].mid2 - bands[i].low2;
        bands[i].mid2 += bands[i].high2 * bands[i].c;
        bands[i].low2 += bands[i].mid2;
        bands[i].y = bands[i].high2 * bands[i].att;
      }
    }

  public:

    void process(float** AInputs, float** AOutputs, uint32_t ANumSamples) {

      for (uint32_t i=0; i<MBandsCount; i++) {
        MBandsOut[i].level = MIP_Clamp(MBandsLevel[i], 0.0, 1.0);
        //MBandsOut[i].level = CLAMP(MBandsLevel[i], 0.0, 1.0);
      }

      float* input0 = AInputs[0];
      float* input1 = AInputs[1];
      float* output0 = AOutputs[0];
      float* output1 = AOutputs[1];
      for (uint32_t i=0; i < ANumSamples; i++)  {
        float spl0 = *input0++;
        float spl1 = *input1++;
        spl0 *= MPreCarrierGain;
        spl1 *= MPreFormantGain;

        processBandpasses(MBandsCarrier,spl0);
        processBandpasses(MBandsFormant,spl1);
        spl0 = 0.0;
        for (uint32_t j=0; j < MBandsCount; j++) {
          MBandsOut[j].oldval = MBandsOut[j].oldval
                              + (fabs (MBandsFormant[j].y)
                              - MBandsOut[j].oldval)
                              * MBandsOut[j].decay;
          float x = MBandsCarrier[j].y * MBandsOut[j].oldval;
          spl0 += x * MBandsOut[j].level;
        }
        //spl0 *= POST_CARRIER;
        //spl1 *= POST_FORMANT;

        *output0++ = spl0;
        *output1++ = spl0;
      }
    }

    //----------

};

//----------------------------------------------------------------------

#undef MAX_BANDS

//#undef CLAMP
//#undef PRE_CARRIER
//#undef PRE_FORMANT
//#undef POST_CARRIER
//#undef POST_FORMANT

//----------------------------------------------------------------------
#endif
