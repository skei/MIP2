#ifndef mip_filter_svf_included
#define mip_filter_svf_included
//----------------------------------------------------------------------

#include "base/utils/mip_math.h"

/*
  http://www.kvraudio.com/forum/viewtopic.php?p=6371172#p6371172
  Take a look at the State Variable Filters described here:
  http://www.cytomic.com/technical-papers
  Those have the same phase/frequency response as Direct Form filters, but
  coefficients are easier to calculate, sweep without side effects and are
  generally much more useful for musical applications.
*/

//----------------------------------------------------------------------

#define kft_none  0
#define kft_lp    1
#define kft_hp    2
#define kft_bp    3
#define kft_n     4

class MIP_SvfFilter {

  private:

    uint32_t  MMode = kft_none;
    float     MFreq = 1;
    float     MBW   = 0;
    float     z1    = 0;
    float     z2    = 0;

  public:

    MIP_SvfFilter() {
      //MMode = kft_none;
      //MFreq = 1;
      //MBW   = 0;
      reset();
      //z1    = 0;
      //z2    = 0;
    }

    void reset(void) {
      z1 = 0;
      z2 = 0;
    }

    void setMode(uint32_t AMode) {
      //MIP_Print("%i\n",AMode);
      MMode = AMode;
      //z1 = 0;
      //z2 = 0;
      //reset();
    }

    void setFreq(float AFreq) {
      MFreq = AFreq;
      //z1 = 0;
      //z2 = 0;
    }

    void setBW(float ABW) {
      MBW = ABW;
      //z1 = 0;
      //z2 = 0;
    }

    float process(float AValue) {
      //result := 0;
      if (MMode==kft_none) return AValue;
      else {
        float L   = z2 + MFreq * z1;
        float H   = AValue - L - MBW   * z1;
        float B   = MFreq * H + z1;
        float N   = H + L;
        z1 = B;
        z2 = L;
        switch(MMode) {
          case kft_lp:  return L; //break;
          case kft_hp:  return H; //break;
          case kft_bp:  return B; //break;
          case kft_n:   return N; //break;
        }
      }
      return 0;
    }

    float process(float AValue, uint32_t AType) {
      MMode = AType;
      return process(AValue);
    }

    float processMod(float AValue, float AFreq, float ABW) {
      MFreq += AFreq;
      //MBW   += ABW;
      MIP_Clamp(MFreq,0.0f,1.0f);
      //KClamp(MBW,0,1);
      float result = process(AValue);
      MFreq -= AFreq;
      //MBW   -= ABW;
      return result;
    }

};


//----------------------------------------------------------------------
#endif
