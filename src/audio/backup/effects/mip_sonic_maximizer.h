#ifndef kode_sonic_maximizer_included
#define kode_sonic_maximizer_included
//----------------------------------------------------------------------

#include "kode.h"

//----------------------------------------------------------------------

#define cDenorm   10e-30
#define c_ampdB   8.65617025
#define freqHP    2243.2
#define freqLP    224.32
#define freqHP_p2 -KODE_PI2 * freqHP
#define freqLP_p2 -KODE_PI2 * freqLP

//----------------------------------------------------------------------

class KODE_SonicMaximizer {

  private:

    float band1       = 0.0f;
    float band3       = 0.0f;
    float amp         = 0.0f;
    float tmplLP      = 0.0f;
    float tmprLP      = 0.0f;
    float tmplHP      = 0.0f;
    float tmprHP      = 0.0f;
    float xLP         = 0.0f;
    float xHP         = 0.0f;
    float a0LP        = 0.0f;
    float a0HP        = 0.0f;
    float b1LP        = 0.0f;
    float b1HP        = 0.0f;
    float samplerate  = 44100.0f;

  //public:
  //
  //  KODE_SonicMaximizer() {}
  //  ~KODE_SonicMaximizer() {}

  public:

    void reset() {
      band1   = 0;
      band3   = 0;
      tmplLP  = 0;
      tmprLP  = 0;
      tmplHP  = 0;
      tmprHP  = 0;
    }

    void calc_coeffs(float samplerate) {
      xHP  = expf(freqHP_p2 / samplerate);
      a0HP = 1.0 - xHP;
      b1HP = -xHP;
      xLP  = expf(freqLP_p2 / samplerate);
      a0LP = 1.0 - xLP;
      b1LP = -xLP;
    }

    void setBand1(float f)  { band1 = f; }
    void setBand3(float f)  { band3 = f; }
    void setAmp(float f)    { amp = f; }

    void process_mono(float input, float* output) {
      float s0 = input;
      tmplLP = a0LP*s0 - b1LP*tmplLP + cDenorm;
      float sp0 = tmplLP;
      tmplHP = a0HP*s0 - b1HP*tmplHP + cDenorm;
      float sp4 = s0 - tmplHP;
      float sp2 = s0 - sp0 - sp4;
      *output = (sp0 * band1 + sp2 + sp4 * band3) * amp;
    }

    void process_stereo(float input0, float input1, float* output0, float* output1) {
      float s0 = input0;
      float s1 = input1;
      tmplLP = a0LP*s0 - b1LP*tmplLP + cDenorm;
      tmprLP = a0LP*s1 - b1LP*tmprLP + cDenorm;
      float sp0 = tmplLP;
      float sp1 = tmprLP;
      tmplHP = a0HP*s0 - b1HP*tmplHP + cDenorm;
      tmprHP = a0HP*s1 - b1HP*tmprHP + cDenorm;
      float sp4 = s0 - tmplHP;
      float sp5 = s1 - tmprHP;
      float sp2 = s0 - sp0 - sp4;
      float sp3 = s1 - sp1 - sp5;
      *output0 = (sp0 * band1 + sp2 + sp4 * band3) * amp;
      *output1 = (sp1 * band1 + sp3 + sp5 * band3) * amp;
    }

};

//----------------------------------------------------------------------
#endif
