#ifndef sa_ts1_osc_included
#define sa_ts1_osc_included
//----------------------------------------------------------------------

#include "base/utils/mip_interpolation.h"

template <class T>
class sa_ts1_osc {

//------------------------------
private:
//------------------------------

  bool          MWrapped        = false;
  T             z1              = 0.0;

//------------------------------
protected:
//------------------------------

  T             MSampleRate     = 0.0;
  T             MInvSampleRate  = 0.0;

  T             MPhase          = 0.0;
  T             MPhaseAdd       = 0.0;
  T             MShape          = 0.0;
  T             MWidth          = 0.5;
  uint32_t      MWaveModType    = 0;
  T             MWaveModAmount  = 0;
  uint32_t      MPhaseModType   = 0;
  T             MPhaseModAmount = 0;

//------------------------------
public:
//------------------------------

  sa_ts1_osc() {
  }

  //----------

  ~sa_ts1_osc() {
  }

//------------------------------
public:
//------------------------------

  void setFrequency(T hz) {
    //MIP_Print("hz %.2f\n",hz);
    MIP_Assert( hz >= 1.0 );
    MIP_Assert( hz < (MSampleRate * 0.5) );
    MPhaseAdd = hz * MInvSampleRate;
  }

  void setSampleRate(T ARate) {
    MSampleRate = ARate;
    MIP_Assert(ARate > 0);
    MInvSampleRate = 1.0 / ARate;
  }

  void setPh(T ph)                      { MPhase = ph; }
  void setPhAdd(T phadd)                { MPhaseAdd = phadd; }
  void setShape(T shape)                { MShape = shape; }
  void setWidth(T width)                { MWidth = width; }

  void setWaveModType(uint32_t AType)   { MWaveModType = AType; }
  void setWaveModAmount(T AAmount)      { MWaveModAmount = AAmount; }
  void setPhaseModType(uint32_t AType)  { MPhaseModType = AType; }
  void setPhaseModAmount(T AAmount)     { MPhaseModAmount = AAmount; }

  T getPh()     { return MPhase; }
  T getPhAdd()  { return MPhaseAdd; }

//------------------------------
private:
//------------------------------

T curve(T in, T wm) {
  T sign_in = MIP_Sign(in);
  T abs_in = abs(in);
  return MIP_Curve(abs_in,wm) * sign_in;
}

//------------------------------
public:
//------------------------------

  void restart() {
    z1        = 0.0;
    MPhase    = 0.0;
    MWrapped  = false;
  }

  //----------

  T process(T mod) {

    T phase_add_mod = 0.0;

    T t  = MPhase;
    T dt = MPhaseAdd;

    // shape

    T shape = MShape * 3.0;
    T _squ = 0.0;
    T _tri = 0.0;
    T _sin = 0.0;

    if (shape < 1.0) {
      _squ = shape;
      _tri = 0.0;
      _sin = 0.0;
    }
    else if (shape < 2.0) {
      _squ = 1.0;
      _tri = shape - 1.0;
      _sin = 0.0;
    }
    else {
      _squ = 1.0;
      _tri = 1.0;
      _sin = shape - 2.0;
    }

    // phase mod

    switch (MPhaseModType) {
      case 0: { // off
        }
        break;
      case 1: { // curve
        t = curve(t,MPhaseModAmount);
        break;
      }
      case 2: { // reso
        t *= 1.0 + (MPhaseModAmount * 12);
        t = MIP_Fract(t);
        break;
      }
      case 3: { // pulse
        t *= 1.0 + (MPhaseModAmount * 12);
        t = MIP_Clamp(t,0,1);
        t = MIP_Fract(t);
        break;
      }
      case 4: { // PM
        //t += (dt * pm); // try..
        T f = mod * (MPhaseModAmount * 12);
        t += (t * f);
        t = MIP_Fract(t);
        break;
      }
      case 5: { // FM
        T f = mod * (MPhaseModAmount * 48);
        phase_add_mod = (dt * f);
        //phase_add_mod = (dt * pm);
        break;
      }
      case 6: { // Flip
        if (t >= MPhaseModAmount) {
          t = 1.0 - t;
          //t = MIP_Fract(t);
        }
        break;
      }
    }

    // saw 1

    T t1 = t;// + 0.5;

    t1 = MIP_Fract(t1);
    T saw1 = ((2.0f * t1) - 1.0);
    saw1 -= MIP_PolyBlep(t1,dt);

    // saw 2

    T t2 = t1 + MWidth;
    t2 = MIP_Fract(t2);
    T saw2 = ((2.0 * t2) - 1.0);
    saw2 -= MIP_PolyBlep(t2,dt);

    // squ

    T squ1 = saw1 - (saw2 * _squ);

    // tri

    z1 = (dt * squ1) + ((1.0 - dt) * z1);
    //T tri1 = squ1 * (1.0 - _tri) + (z1 * 4.0) * _tri;
    T tri1 = z1 * 4.0;

    // sin

    T sin1 = sin(t * MIP_PI2);

    T out = MIP_Interpolate_Linear(_tri,squ1,tri1);
    out   = MIP_Interpolate_Linear(_sin,out,sin1);

    switch (MWaveModType) {
      case 0: { // off
        break;
      }
      case 1: { // curve
        out = curve(out,MWaveModAmount);
        break;
      }
      case 2: { // AM
        T am = out * abs(mod);
        out = MIP_Interpolate_Linear(MWaveModAmount,out,am);
        break;
      }
      case 3: { // RM
        T rm = out * mod;
        out = MIP_Interpolate_Linear(MWaveModAmount,out,rm);
        break;
      }
      case 4: { // Replace
        //T rm = out * mod;
        //out = MIP_Interpolate_Linear(MWaveModAmount,out,rm);
        if (abs(out) > MWaveModAmount) {
          out = mod;
        }
        break;
      }
      case 5: { // Neg
        out = MIP_Interpolate_Linear(MWaveModAmount,out,-mod);
        break;
      }
      case 6: { // Neg
        T sign = MIP_Sign(out);
        out = MIP_Interpolate_Linear(MWaveModAmount,out,mod*sign);
        break;
      }

    }

    MPhase += MPhaseAdd + phase_add_mod;
    if ((MPhase < 0.0) || (MPhase >= 1.0)) { MWrapped = true; }
    MPhase = MIP_Fract(MPhase);

    return out;

  }

};

//----------------------------------------------------------------------
#endif
