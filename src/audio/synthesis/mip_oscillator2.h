#ifndef kode_oscillator2_included
#define kode_oscillator2_included
//----------------------------------------------------------------------

#include "base/kode_math.h"
#include "audio/waveforms/kode_polyblep_waveform.h"

class KODE_Oscillator2 {

  private:
    float MPulseWidth = 0.5f;
    float MSawSqu     = 0.0f;
    float MSquTri     = 0.0f;
    float MTriSin     = 0.0f;
    float ph          = 0.0f;
    float phadd       = 0.0f;
    float z1          = 0.0f;
    float MSampleRate = 0.0f;
    float MIrate      = 0.0f;

  public:

    //KODE_Oscillator2() {
    //}

  //--------------------
  // get
  //--------------------

  public:

    float getPhase(void) { return ph; }
    float getPhaseAdd(void) { return phadd; }

  //--------------------
  // set
  //--------------------

  public:

    void setSampleRate(float rate) {
      KODE_Assert( rate > 0 );
      MSampleRate = rate;
      MIrate = 1.0f / MSampleRate;
    }

    void setFrequency(float hz) {
      //KTrace("hz %.2f\n",hz);
      KODE_Assert( hz >= 1 );
      KODE_Assert( hz < (MSampleRate*0.5f) );
      //phadd = hz / srate;
      phadd = hz * MIrate;
    }

    void setPhase(float a) {
      //KODE_Assert( a >= 0 );
      //KODE_Assert( a <  1 );
      ph = KODE_Fract(a);
    }

    void setPhaseAdd(float a) {
      KODE_Assert( a >= 0 );
      KODE_Assert( a <= 1 );
      phadd = a;
    }

    void reset(void) {
      ph = 0;
      phadd = 0;
      z1 = 0;
    }

    void setPulseWidth(float pw) {
      KODE_Assert( pw >= 0 );
      KODE_Assert( pw <= 1 );
      MPulseWidth = pw;
    }

    void setSawSqu(float sawsqu) {
      KODE_Assert( sawsqu >= 0 );
      KODE_Assert( sawsqu <= 1 );
      MSawSqu = sawsqu;
    }

    void setSquTri(float squtri) {
      KODE_Assert( squtri >= 0 );
      KODE_Assert( squtri <= 1 );
      MSquTri = squtri;
    }

    void setTriSin(float trisin) {
      KODE_Assert( trisin >= 0 );
      KODE_Assert( trisin <= 1 );
      MTriSin = trisin;
    }

  //--------------------
  // process
  //--------------------

  public:

    float process() {
      
      KODE_Assert( MSampleRate > 0.0f );
      
      ph += phadd;
      ph = KODE_Fract(ph);
      float t1 = ph + 0.5f;
      t1 = KODE_Fract(t1);
      float saw1 = 2.0f * t1 - 1.0f;
      saw1 -= KODE_PolyBlep(t1,phadd);
      float t2 = t1 + MPulseWidth;
      t2 = KODE_Fract(t2);
      float saw2 = 2.0f * t2 - 1.0f;
      saw2 -= KODE_PolyBlep(t2,phadd);
      float squ = saw1 - (saw2*MSawSqu);
      z1 = (phadd * squ) + ((1.0f - phadd) * z1);
      //TODO: KInterpolate_Linear
      float tri  = squ * (1-MSquTri) + (z1*4.0f)        * MSquTri;
      float sine = tri * (1-MTriSin) + sin(ph*KODE_PI2) * MTriSin;
      float out  = tri * (1-MTriSin) + sine             * MTriSin;
      return out;
    }

    float process_mod(float mod, float ofs=0.0f) {
      
      KODE_Assert( MSampleRate > 0.0f );
      
      //ph += phadd;
      ph += (phadd + ofs);
      ph = KODE_Fract(ph);
      float phm = KODE_Fract(ph+mod);
      float t1 = phm + 0.5f;
      t1 = KODE_Fract(t1);
      float saw1 = 2.0f * t1 - 1.0f;
      saw1 -= KODE_PolyBlep(t1,phadd);
      float t2 = t1 + MPulseWidth;
      t2 = KODE_Fract(t2);
      float saw2 = 2.0f * t2 - 1.0f;
      saw2 -= KODE_PolyBlep(t2,phadd);
      float squ = saw1 - (saw2*MSawSqu);
      z1 = (phadd * squ) + ((1.0f - phadd) * z1);
      float tri  = squ * (1-MSquTri) + (z1*4.0f)          * MSquTri;
      float sine = tri * (1-MTriSin) + sin(phm*KODE_PI2)  * MTriSin;
      float out  = tri * (1-MTriSin) + sine               * MTriSin;
      return out;
    }

};

//----------------------------------------------------------------------
#endif
