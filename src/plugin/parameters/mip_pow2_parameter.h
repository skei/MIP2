#ifndef mip_pow2_parameter_included
#define mip_pow2_parameter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_parameter.h"
#include "plugin/parameters/mip_float_parameter.h"

// 2^n

// appendParameter( new KParamPow2("pow2", 4, 1, 16, 1 )); // (1..16)^2, default = 4^2

class MIP_Pow2Parameter
: public MIP_FloatParameter {

private:

  float   MPower  = 1.0f;
  //bool    MPost   = true;

public:

  MIP_Pow2Parameter(const char* AName, float AVal, /*bool APost,*/ float AMin, float AMax, float AStep)
  : MIP_FloatParameter(AName,AVal,AMin,AMax,AStep) {
    //MPost = APost;
    //if (APost) {
    //    float n = ((AVal - MMin) * MInvRange);
    //    MDefault = powf(n,0.5f);
    //}
    //else {
      MDefValue = ((AVal - MMinValue) * MInvRange);
      //MDefValue = to01(MDefValue);
    //}
  }

public:

    float to01(float AValue) override {
      float result = 0.0f;
      //if (MPost) {
      //    float n = KParamFloat::to01(AValue);
      //    result = powf(n,0.5f);
      //}
      //else {
        AValue = powf(AValue,0.5f);
        result = MIP_FloatParameter::to01(AValue);
      //}
      return result;
    }

    //----------

    float from01(float AValue) override {
      float result = 0.0f;
      //if (MPost) {
      //    float n = powf(2.0f,AValue);
      //    result = KParamFloat::from01(n);
      //}
      //else {
        float n = MIP_FloatParameter::from01(AValue);
        result = powf(2.0f,n);
      //}
      return result;
    }

};

//----------------------------------------------------------------------
#endif
