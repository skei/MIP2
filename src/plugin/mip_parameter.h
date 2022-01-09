#ifndef mip_parameter_included
#define mip_parameter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_array.h"

//----------------------------------------------------------------------

struct MIP_Parameter;
typedef MIP_Array<MIP_Parameter*> MIP_ParameterArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Parameter {

  friend class MIP_Descriptor;

//------------------------------
//private:
protected:
//------------------------------

  int32_t     MIndex        = -1;
  const char* MName         = "parameter";
  const char* MShortName    = "param";
  const char* MLabel        = "";
  float       MDefValue     = 0.0;
  float       MMinValue     = 0.0;
  float       MMaxValue     = 0.0;
  uint32_t    MNumSteps     = 0;
  bool        MCanAutomate  = true;
  bool        MCanModulate  = false;

//------------------------------
public:
//------------------------------

  MIP_Parameter(const char* AName, float AdefValue=0.0, float AMinValue=0.0, float AMaxValue=1.0, uint32_t ANumSteps=0) {
    MName     = AName;

    MDefValue = AdefValue;
    MMinValue = AMinValue;
    MMaxValue = AMaxValue;
    MNumSteps = ANumSteps;
  }

  //----------

  virtual ~MIP_Parameter() {
  }

//------------------------------
public:
//------------------------------

  int32_t     getIndex()      { return MIndex; }
  const char* getName()       { return MName; }
  const char* getShortName()  { return MShortName; }
  const char* getLabel()      { return MLabel; }
  float       getDefValue()   { return MDefValue; }
  float       getMinValue()   { return MMinValue; }
  float       getMaxValue()   { return MMaxValue; }
  uint32_t    getNumSteps()   { return MNumSteps; }
  bool        canAutomate()   { return MCanAutomate; }
  bool        canModulate()   { return MCanModulate; }

  void        setCanAutomate(bool b=true) { MCanAutomate = b; }
  void        setCanModulate(bool b=true) { MCanModulate = b; }

//------------------------------
public:
//------------------------------

  virtual float from01(float AValue) {
    return AValue;
  }

  //----------

  virtual float to01(float AValue) {
    return AValue;
  }

  //----------

  virtual const char* displayText(char* ABuffer, float AValue, uint32_t ANumDigits=0) {
    sprintf(ABuffer,"%.3f",AValue);
    return ABuffer;
  }

  //----------

};

//----------------------------------------------------------------------
#endif
