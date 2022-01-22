#ifndef mip_float_parameter_included
#define mip_float_parameter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_parameter.h"

class MIP_FloatParameter
: public MIP_Parameter {

//------------------------------
protected:
//------------------------------

  float     MStep     = 0.0f;
  float     MRange    = 0.0f;
  float     MInvRange = 0.0f;
  float     MHalfStep = 0.0f;

  //bool      MStepped  = false;
  //int32_t   MNumSteps = 0;
  float     MStepSize = 0.0f;

//------------------------------
public:
//------------------------------

  MIP_FloatParameter()
  : MIP_Parameter("") {
  }

  //----------

  MIP_FloatParameter(const char* AName, float AVal, float AMin=0.0f, float AMax=1.0f, float AStep=0.0f)
  : MIP_Parameter(AName,AVal) {
    MIndex    = -1;
    MName     = AName;
    MLabel    = "";
    MMinValue = AMin;
    MMaxValue = AMax;
    MStep     = AStep;
    setup(AVal);
  }

  MIP_FloatParameter(const char* AName, const char* ALabel, float AVal, float AMin=0.0f, float AMax=1.0f, float AStep=0.0f)
  : MIP_Parameter(AName,AVal) {
    MIndex    = -1;
    MName     = AName;
    MLabel    = ALabel;
    MMinValue = AMin;
    MMaxValue = AMax;
    MStep     = AStep;
    setup(AVal);
  }

//------------------------------
private:
//------------------------------

  void setup(float AVal=0.0f) {
    MRange    = MMaxValue - MMinValue;
    MInvRange = 1 / MRange;
    if (MStep > 0) {
      //MIsStepped  = true;
      MNumSteps   = 1 + round( MRange / MStep ); // was: MIP_Round
      MStepSize = 1.0f / (float)(MNumSteps - 1);
      //MIP_Trace("%i MStep %f MStepSize %f\n",MStep,MStepSize);
      MHalfStep = 0.5 * MStepSize;
    } else {
      //MIsStepped  = false;
      //MNumSteps = 1;
      MNumSteps = 0;
      MStepSize = 1;
      MHalfStep = 0;
    }
    //MDefValue = to01(MDefValue);
    MDefValue = ((AVal - MMinValue) * MInvRange);
    //MIP_Trace("%f\n",MDefValue);
  }

//------------------------------
public:
//------------------------------

  //bool isStepped(void) override {
  //  return MIsStepped;
  //}

  //----------

  //uint32_t getNumSteps(void) override {
  //  return MNumSteps;
  //}

  //----------

  float to01(float AValue) override {
    return ((AValue - MMinValue) * MInvRange);
  }

  //----------

  float from01(float AValue) override {
    //if (MIsStepped) {
    if (MNumSteps > 0) {
      float n = AValue * MNumSteps;
      uint32_t st = floorf(n);
      if (st > (MNumSteps-1)) st = (MNumSteps-1);
      return  MMinValue + (st * MStep);
    } else {
      return MMinValue + (AValue * MRange);
    }
  }

};

//----------------------------------------------------------------------
#endif
