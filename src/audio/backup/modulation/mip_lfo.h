#ifndef kode_lfo_included
#define kode_lfo_included
//----------------------------------------------------------------------

#include "base/kode_random.h"
#include "audio/synthesis/kode_phasor.h"
#include "audio/waveforms/kode_naive_waveform.h"

class KODE_Lfo {
  private:
    uint32_t    MType       = 0;
    float       MSpeed      = 0.0f;       // hz
    float       MSampleRate = 0.0f;
    float       MValue      = 0.0f;
    float       prev_ph     = 0.0f;
    KODE_Phasor MPhasor;

  public:

    KODE_Lfo() {
      //MType = 0;
      //MSpeed = 0;
      //MSampleRate = 0;
      //prev_ph = 1;
    }

    //----------

    float getValue(void) {
      return MValue;
    }

    //----------

    void setSampleRate(float ARate) {
      MSampleRate = ARate;
      //MPhase.setSampleRate(ARate);
      MPhasor.setSampleRate(ARate);
    }

    void setType(uint32_t AType) {
      //KODE_Trace("AType %i\n",AType);
      MType = AType;
    }

    void setSpeed(float ASpeed) {
      //KODE_Trace("ASpeed %.3f\n",ASpeed);
      //MPhase.setFreq(ASpeed);
      MPhasor.setHz(ASpeed);
    }

    void reset(void) {
      //MPhase.resetPhase();
      MPhasor.t = 0.0f;
      MPhasor.dt = 0.0f;
      MValue = 0;
      prev_ph = 1; // force calc new value
    }

    float process(void) {
      //float ph = MPhase.process();
      float ph = MPhasor.process();
      //KODE_Trace("%.3f\n",ph);
      //float v = 0;
      switch(MType) {
        case 0: MValue = 0.0f; break; //  Off,
        case 1: MValue = KODE_NaiveSinWaveform(ph); break; //  Sine,
        case 2: MValue = KODE_NaiveRampWaveform(ph); break; //  Ramp,
        case 3: MValue = KODE_NaiveSawWaveform(ph); break; //  Saw,
        case 4: MValue = KODE_NaiveSquWaveform(ph); break; //  Squ,
        case 5: MValue = KODE_NaiveTriWaveform(ph); break; //  Tri,
        case 6: if (ph < prev_ph) MValue = KODE_RandomSigned(); prev_ph = ph; break; //  s&h / Noise
        //default: 
        //  KODE_Trace("error.. unknown type %i\n",MType);
        //  break;
      }
      //KODE_Trace("%.3f\n",MValue);
      return MValue;
    }

    //float process(float t) {
    //  return 0;
    //}

    float processSteps(int32_t ASteps) {
      if (ASteps==0) return MValue;
      float result = process();
      //if (ASteps>1) for (int32_t i=1; i<ASteps; i++) {
      //  process();

        //MPhase.skip(ASteps);
        MPhasor.skip(ASteps);

      //}
      return result;
    }

    //----------



};

//----------------------------------------------------------------------
#endif
