#ifndef kode_event_horizon_included
#define kode_event_horizon_included
//----------------------------------------------------------------------

#include "kode.h"

//----------------------------------------------------------------------

//#define KODE_PI   M_PI
//#define KODE_PI2  (M_PI * 2.0)

//#define _log2db 8.6858896380650365530225783783321   /* 20 / ln(10) */
//#define _db2log 0.11512925464970228420089957273422  /* ln(10) / 20 */

#define _log2db (20.0f / logf(10.0f))
#define _db2log (logf(10.0f) / 20.0f)

template <typename T> int sign(T val) {
  return (T(0) < val) - (val < T(0));
}
//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class KODE_EventHorizon {

  private:

    //float _log2db, _db2log;
    float slider1, slider2, slider3, slider4;

    float thresh, threshdb;
    float ceiling, ceildb, makeupdb;
    float makeup;
    float sc, scv, sccomp, peakdb, peaklvl;
    float scratio, scmult;

  public:

    KODE_EventHorizon() {
      slider1   = 0.0f;
      slider2   = 0.0f;
      slider3   = 0.0f;
      slider4   = 0.0f; //10.0;
      reset();
    }

    ~KODE_EventHorizon() {
    }

  public:

    void setSlider1(float v) { slider1 = v; }
    void setSlider2(float v) { slider2 = v; }
    void setSlider3(float v) { slider3 = v; }
    void setSlider4(float v) { slider4 = v; }

    void reset() {
      thresh    = 0.0f;
      threshdb  = 0.0f;
      ceiling   = 0.0f;
      ceildb    = 0.0f;
      makeupdb  = 0.0f;
      makeup    = 0.0f;
      sc        = 0.0f;
      scv       = 0.0f;
      sccomp    = 0.0f;
      peakdb    = 0.0f;
      peaklvl   = 0.0f;
      scratio   = 0.0f;
      scmult    = 0.0f;
    }

    void recalcAll(void) {

      thresh    = expf(slider1 * _db2log);
      threshdb  = slider1;

      ceiling   = expf(slider2 * _db2log);
      ceildb    = slider2;

      makeup    = expf((ceildb-threshdb) * _db2log);
      makeupdb  = ceildb - threshdb;

      sc        = -slider3;
      scv       = expf( sc * _db2log);
      sccomp    = expf(-sc * _db2log);

      peakdb    = ceildb + 25.0f;
      peaklvl   = expf(peakdb * _db2log);
      scratio   = slider4;
      scmult    = fabs((ceildb - sc) / (peakdb - sc));
    }

    void calc_coeffs(float samplerate) {
    }

    void process_mono(float input, float* output) {
    }

    void process_stereo(float input0, float input1, float* output0, float* output1) {
      float spl0 = input0;
      float spl1 = input1;
      //float peak = SMax(abs(spl0),abs(spl1));
      spl0 *= makeup;
      spl1 *= makeup;
      float sign0 = sign(spl0);
      float sign1 = sign(spl1);
      float abs0 = fabs(spl0);
      float abs1 = fabs(spl1);
      float overdb0 = 2.08136898 * logf(abs0) * _log2db - ceildb; // c++ Log = pascal Ln ?????
      float overdb1 = 2.08136898 * logf(abs1) * _log2db - ceildb;
      if (abs0 > scv) spl0 = sign0 * (scv + expf(overdb0*scmult)*_db2log);
      if (abs1 > scv) spl1 = sign1 * (scv + expf(overdb1*scmult)*_db2log);
      spl0 = min(ceiling,fabs(spl0)) * sign(spl0);
      spl1 = min(ceiling,fabs(spl1)) * sign(spl1);
      *output0 = spl0;
      *output1 = spl1;
    }

};
//----------------------------------------------------------------------
#endif
