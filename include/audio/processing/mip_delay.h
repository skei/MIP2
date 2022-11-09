#ifndef mip_delay_included
#define mip_delay_included
//----------------------------------------------------------------------

#include "base/utils/mip_math.h"
#include "audio/mip_audio_math.h"
//#include "audio/filters/mip_dc_filter.h"

//----------------------------------------------------------------------

struct MIP_NoDelayFx {
  float process(float x) { return x; }
};

//----------------------------------------------------------------------
//
// interpolated delay
//
//----------------------------------------------------------------------


template <int MAX_DELAY, typename FBLOOPFX=MIP_NoDelayFx>
class MIP_InterpolatedDelay {

  private:

    float       MBuffer[MAX_DELAY]  = {0};
    int         MCounter            = 0;
    float       MPhase              = 0.0f;
    bool        MWrapped            = false;
    FBLOOPFX    MFBLoopFX           = {};
    //MIP_DcFilter  MDC;

  public:

    MIP_InterpolatedDelay() {
      clear();
    }

    ~MIP_InterpolatedDelay() {
    }

  public:

    FBLOOPFX* getFeedbackFX() {
      return &MFBLoopFX;
    }

    bool hasWrapped() {
      return MWrapped;
    }

    void reset() {
      MCounter = 0;
      MWrapped = false;
    }

    void clear() {
      MCounter = 0;
      memset(MBuffer,0,MAX_DELAY*sizeof(float));
    }

    void start() {
      MWrapped = false;
      MPhase = 0.0f;
    }

    //void restart() {
    //  MCounter = 0;
    //  MWrapped = false;
    //  //MPhase = 0.0f;
    //}

    float process(float AInput, float AFeedback, float ADelay) {

      MIP_Assert( ADelay > 0 );
      MIP_Assert( ADelay < MAX_DELAY );

      // calculate delay offset
      float back = (float)MCounter - ADelay;
      if (back < 0.0) back += MAX_DELAY;            // + back;
      int index0 = (int)back;
      int index_1 = index0-1;
      int index1 = index0+1;
      int index2 = index0+2;
      if (index_1 < 0) index_1 += MAX_DELAY;        // index_1 = MAX_DELAY - 1;
      if (index1 >= MAX_DELAY) index1 -= MAX_DELAY; // index1 = 0;
      if (index2 >= MAX_DELAY) index2 -= MAX_DELAY; // index2 = 0;

      // interpolate
      float y_1 = MBuffer[index_1];
      float y0  = MBuffer[index0];
      float y1  = MBuffer[index1];
      float y2  = MBuffer[index2];
      float x   = (float)back - (float)index0;
      float c0  = y0;
      float c1  = 0.5f * (y1 - y_1);
      float c2  = y_1 - 2.5f * y0 + 2.0f * y1 - 0.5f * y2;
      float c3  = 0.5f * (y2 - y_1) + 1.5f * (y0 - y1);
      float output = ((c3 * x + c2) * x + c1) * x + c0;

      //output = MDC.process(output);
      //output = MIP_KillDenormal(output);

      float fb = output * AFeedback;
      float flt = MFBLoopFX.process(fb);
      //float flt = fb;
      float out = AInput + flt;

      //-----

      //out = atan(out); // KClamp((AInput + flt), -1, 1);

      // hard-clip
      if (out >  1.0f) out =  1.0f;
      if (out < -1.0f) out = -1.0f;

      //-----

      MIP_Assert( MCounter >= 0 );
      MIP_Assert( MCounter < MAX_DELAY );

      // if only part of next sample 'fits' inside delay length...
      //if ((MPhase + 1.0) >= ADelay) {
      //  float diff = ADelay - MPhase;
      //  out *= diff;
      //}

      MBuffer[MCounter] = out;

      MCounter++;
      if (MCounter >= MAX_DELAY) {
        MCounter -= MAX_DELAY;// 0;
      }

      MPhase += 1.0f;
      if (MPhase >= ADelay) {
        MWrapped = true;
        while (MPhase >= ADelay) MPhase -= ADelay;
      }

      return output;
    }

};

//----------------------------------------------------------------------
//
// simple delay
//
//----------------------------------------------------------------------

//template <int MAX_DELAY, typename FBLOOPFX=MIP_NoDelayFx>
//class KSimpleDelay {
//
//  private:
//
//    float       MBuffer[MAX_DELAY];
//    int32       MCounter;
//    FBLOOPFX    MFBLoopFX;
//
//  public:
//
//    KSimpleDelay() {
//      clear();
//    }
//
//    ~KSimpleDelay() {
//    }
//
//    FBLOOPFX* getFeedbackFX(void) {
//      return &MFBLoopFX;
//    }
//
//    void restart(void) {
//      MCounter = 0;
//    }
//
//    void clear(void) {
//      MCounter = 0;
//      KMemset(MBuffer,0,MAX_DELAY*sizeof(float));
//    }
//
//    float process(float AInput, float AFeedback, int32 ADelay) {
//      int32 back = MCounter - ADelay;
//      if (back < 0) back = MAX_DELAY + back;
//      int index = (int)back;
//      float output = MBuffer[index];
//      float fb = AInput + output * AFeedback;
//      fb = MFBLoopFX.process(fb);
//      MBuffer[MCounter] = fb;
//      MCounter++;
//      if (MCounter >= MAX_DELAY) MCounter = 0;
//      return output;
//    }
//
//};

//----------------------------------------------------------------------
#endif
