#ifndef mip_windowing_included
#define mip_windowing_included
//----------------------------------------------------------------------

#include <math.h>

/*
float MIP_Windowing_Rect(float pos);
float MIP_Windowing_Hann(float pos);
float MIP_Windowing_Hamming(float pos);
float MIP_Windowing_Blackman(float pos);
float MIP_Windowing_Gaussian(float pos);
float MIP_Windowing_Welch(float pos);
float MIP_Windowing_Bartlett(float pos);
*/

//----------------------------------------------------------------------

float MIP_Windowing_Rect(float pos) {
  return 1.0;
}

float MIP_Windowing_Hann(float pos) {
  float x = cosf(pos*MIP_PI/2.0);
  return x*x;
}

float MIP_Windowing_Hamming(float pos) {
  return 0.54 + 0.46 * cosf(MIP_PI*pos);
}

float MIP_Windowing_Blackman(float pos) {
  return 0.42 + 0.5 * cos(MIP_PI*pos) + 0.08 * cosf(2.0*MIP_PI*pos);
}

float MIP_Windowing_Gaussian(float pos) {
  float a = 4.0;
  float ax = a * pos;
  return exp(-0.5 * ax*ax);
}

float MIP_Windowing_Welch(float pos) {
  return 1.0 - pos*pos;
}

float MIP_Windowing_Bartlett(float pos) {
  if (pos == 0.0) return 1.0;
  else if (pos > -1.0 && pos < 0.0) return pos + 1.0;
  else if (pos > 0.0 && pos < 1.0) return 1.0 - pos;
  else return 0.0;
}

//----------------------------------------------------------------------
#endif
