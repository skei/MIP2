#ifndef kode_naive_waveform_included
#define kode_naive_waveform_included
//----------------------------------------------------------------------

#include <math.h>

float KODE_NaiveSinWaveform(float t) {
  return sinf(t*KODE_PI2);
}

float KODE_NaiveRampWaveform(float t) {
  return 1.0f - (t*2.0f);
}

float KODE_NaiveSawWaveform(float t) {
  return (t*2.0f)-1.0f;
}

float KODE_NaiveSquWaveform(float t) {
  if (t < 0.5) return 1.0f;
  else return -1.0f;
}

float KODE_NaiveSquWaveform(float t, float w) {
  if (t < w) return 1.0f;
  else return -1.0f;
}

float KODE_NaiveTriWaveform(float t) {
  float a = t;    //  0.....1
  a *= 4.0f;      //  0.....4
  a -= 2.0f;      // -2..0..2
  a = fabs(a);    //  2..0..2
  a -= 1.0f;      //  1.-1..1
  return a;
}

//----------------------------------------------------------------------
#endif
