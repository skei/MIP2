#ifndef sa_ts1_res_included
#define sa_ts1_res_included
//----------------------------------------------------------------------

//#include "base/utils/mip_interpolation.h"
#include "audio/filters/mip_rc_filter.h"
#include "audio/processing/mip_delay.h"


//


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class T>
class sa_ts1_res {
public:
  struct sa_ts1_DelayFx {

    MIP_RcFilter<T> filter  = {};
    T               rough   = 0.0;

    T process(T x) {
      T r = MIP_Random();
      if (r >= rough) x = -x;
      return filter.process(x);
    }

  };

  //----------

  typedef MIP_InterpolatedDelay<4096,sa_ts1_DelayFx> sa_ts1_Delay;

//------------------------------
private:
//------------------------------

  T             MSampleRate   = 0.0;
  sa_ts1_Delay  MDelay        = {};
  MIP_SvfFilter MInputShaper  = {};

  T MHz       = 0.0;
  T MShape    = 0.0;
  T MFeedback = 0.0;
  T MDamp     = 0.0;
  T MRough    = 0.0;

  uint32_t MMode          = 0;
  uint32_t MSpeed         = 0;
  uint32_t MSpeedCounter  = 0;

//------------------------------
public:
//------------------------------

  sa_ts1_res() {
  }

  //----------

  ~sa_ts1_res() {
  }

//------------------------------
public:
//------------------------------


//------------------------------
public:
//------------------------------

  void setSampleRate(T ARate) {
    MSampleRate = ARate;
  }

  void setHz(T AHz) {
    MHz = AHz;
  }

  void setShape(T s)        { MShape = s; }
  void setFeedback(T f)     { MFeedback = f; }
  void setDamp(T d)         { MDamp = d; }
  void setRough(T r)        { MRough = r; }

  void setMode(uint32_t m)  { MMode = m; }
  void setSpeed(uint32_t s) { MSpeed = s; }

  //void setHz(T hz) {
  //}

  //----------

  void restart() {
    MDelay.clear();
    MDelay.start();
    MSpeedCounter = 0;
  }

  //----------

  T process(T in) {
    T out = in;

    T sh = 1.0 - MShape;
    MInputShaper.setMode(MIP_SVF_LP);
    MInputShaper.setFreq(sh * sh);
    MInputShaper.setBW(1);
    in = MInputShaper.process(in);

    T delay = (MSampleRate / MHz);
    //delay *= 0.5;

    T fb = MFeedback;
    //fb = 1.0 - fb;
    //fb = fb * fb * fb;
    fb = MIP_Curve(fb,0.95);
    //fb = 1.0 - fb;

    T damp = MDamp;
    damp = damp * damp;
    damp = 1.0 - damp;
    MDelay.getFeedbackFX()->filter.setWeight(damp);

    float ro = 1.0 - MRough;

    if (ro < 0.5) {
      ro = ro * 2.0; // 0..0,5 -> 0..1
      ro = MIP_Curve(ro,0.01);
    }
    else {
      ro = (ro - 0.5) * 2.0; // 0.5..1 -> 0..1
      ro = MIP_Curve(ro,0.99);
      ro += 1.0;
    }
    // 0..2 -> 0..1
    ro *= 0.5;
    MDelay.getFeedbackFX()->rough = ro;

    T _in = in;

    switch (MMode) {
      case 0: {// plucked
        if (MDelay.hasWrapped()) {
          _in = 0.0;
        }
        break;
      }
      case 1: { // bowed
        if (MDelay.hasWrapped()) {
          MSpeedCounter += 1;
          if (MSpeedCounter >= MSpeed) {
            MSpeedCounter = 0;
            MDelay.start();
          }
          else {
            _in = 0.0;
          }
        }
        break;
      }
      case 2: {
        break;
      }
    }

    out = MDelay.process(_in,fb,delay);
    return out;

  }

};

//----------------------------------------------------------------------
#endif

