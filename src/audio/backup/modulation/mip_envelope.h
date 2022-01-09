#ifndef kode_envelope_included
#define kode_envelope_included
//----------------------------------------------------------------------

// exponential decay
// https://en.wikipedia.org/wiki/Exponential_decay

//----------

/*

coeff = exp(-2 * pi * fc/fs)
coeff = exp(-1 / (tau * fs))
..where "tau" is the decay length in seconds

----------

delta = input - buffer
buffer = buffer + scale * delta

expr(n,r) = exp(ln(ln(r))-ln(ln(n)))
nroot(n,r) = n^(1/r)
pos(spd,smp) = (1-spd)^smp
spd(pos,smp) = 1-nroot(pos,smp)
smp(spd,pos) = expr(1-spd,pos)

expr()  = anti-power
nroot() = nth root
pos()   = position arrived at speed in samples
spd()   = speed required to arrive at position in samples
smp()   = samples required to arrive at position with speed

->

delta         = input - buffer
buffer        = buffer + scale * delta
pos(spd,smp)  = (1-spd)^smp
pos()         = position arrived at speed in samples

*/

//#define env_numstages 5

#include <math.h>

#define kode_env_rate_scale     10.0f // 30.0f
#define kode_env_threshold      KODE_TINY

#define KODE_ENVELOPE_OFF       0
#define KODE_ENVELOPE_ATTACK    1
#define KODE_ENVELOPE_DECAY     2
#define KODE_ENVELOPE_SUSTAIN   3
#define KODE_ENVELOPE_RELEASE   4
#define KODE_ENVELOPE_FINISHED  5

//----------------------------------------------------------------------

struct KODE_EnvelopeStage {
  float target;
  float rate;
};

//----------------------------------------------------------------------

// We calculate the coefficients using the time-constant equation:
// g = e ^ ( -1 / (time * sample rate) ),
// out = in + g * (out  in),

//----------------------------------------------------------------------

class KODE_ExponentialAdsrEnvelope {

  private:

    float               MSampleRate = 0;// = 44100;
    float               MValue = 0;
    int32_t             MStage = 0;
    KODE_EnvelopeStage  MStages[5]; // -,a,d,s,r

  public:

    KODE_ExponentialAdsrEnvelope() {
      //MScale = 50.0f;//6.0f;
      MStage = KODE_ENVELOPE_OFF;
      MValue = 0.0f;
    }

    //----------

    void    setSampleRate(float ARate) { MSampleRate = ARate; }
    float   getValue(void)             { return MValue; }
    int32_t getStage(void)             { return MStage; }

  public:

    /*
      ???
      we cheat a little, and multiply the ms value by 5..
      ..easier to make log/exp ish curves (x*x*x)..
      ???
    */

    float calcRate(float ms) {
      //return expf(-1 / (ms * MSampleRate));
      //ms = (ms*ms); // a little hack to make the knob more 'sensitive' to short durations..
      //ms *= 2.0f;
//      float rate = 1.0f - expf(-1.0f / (ms*MSampleRate));
      //KTrace("ms %.3f rate %f\n",ms,rate);

      //float rate = 1.0f;
      //if (ms > 0) rate = 1.0f - expf(-1.0f / ms*1000.0f);
      //else rate = 0;

      //return rate;

      //return 1.0f / (ms*1000.0f)

      //return 1.0f - expf(-2.0f * KODE_PI * ms / MSampleRate);

      float a = ms * kode_env_rate_scale; // 0..1 -> 0..25
      a = (a*a*a);  // 0..25 -> 0..625 (a*a*a = 15625)
      a += 1.0f;
      //if (a > 0) return 1.0f / a;
      //else return 1.0f;
      return 1.0f / a;
    }

    //----------

    void setAttack(float AValue) {
      //float r1 = AValue * kode_env_rate_scale;
      //float r2 = (r1*r1*r1) + 1;
      MStages[KODE_ENVELOPE_ATTACK].target = 1.0f;
      //MStages[KODE_ENVELOPE_ATTACK].rate   = 1.0f / r2;
      MStages[KODE_ENVELOPE_ATTACK].rate   = calcRate(AValue);
    }

    void setDecay(float AValue) {
      //float r1 = AValue * kode_env_rate_scale;
      //float r2 = (r1*r1*r1) + 1;
      //MStages[KODE_ENVELOPE_DECAY].target = ATarget; // set in setSustain
      //MStages[KODE_ENVELOPE_DECAY].rate = 1.0f / r2;
      MStages[KODE_ENVELOPE_DECAY].rate = calcRate(AValue);
    }

    void setSustain(float AValue) {
      //float r1 = AValue;                  // * env_rate_scale;
      //float r2 = (r1*r1*r1);              // + 1;
      //MStages[KODE_ENVELOPE_DECAY].target = r2;   // set in setSustain
      //MStages[KODE_ENVELOPE_SUSTAIN].target = r2; // set in setSustain
      MStages[KODE_ENVELOPE_DECAY].target = AValue;
      MStages[KODE_ENVELOPE_SUSTAIN].target = AValue;
      MStages[KODE_ENVELOPE_SUSTAIN].rate = 1.0f;
    }

    void setRelease(float AValue) {
      //float r1 = AValue * kode_env_rate_scale;
      //float r2 = (r1*r1*r1) + 1;
      MStages[KODE_ENVELOPE_RELEASE].target = 0.0f;
      //MStages[KODE_ENVELOPE_RELEASE].rate   = 1.0f / r2;
      MStages[KODE_ENVELOPE_RELEASE].rate = calcRate(AValue);
    }

    //----------

    //void setStage(int32 AStage, float ATarget, float ARate) {
    //  float r1 = ARate * env_rate_scale;
    //  float r2 = (r1*r1*r1) + 1;
    //  //float r2 = r1*r1 + 1;
    //  STrace("r2: %f\n",r2);
    //  MStages[AStage].target = ATarget;
    //  MStages[AStage].rate   = 1.0f / r2;
    //}

    //----------

    void setADSR(float a, float d, float s, float r) {
      //setStage(kes_attack,1.0f,a);
      //setStage(kes_decay,s,d);
      //setStage(kes_sustain,s,1,1);
      ////MStages[kes_sustain].target = s*s*s;
      ////MStages[kes_sustain].rate   = 1.0f;
      //setStage(kes_release,0.0f,r);
      setAttack(a);
      setDecay(d);
      setSustain(s);
      setRelease(r);
    }

    //----------

    void noteOn(void) {
      MStage = KODE_ENVELOPE_ATTACK;
      MValue = 0.0f;

    }

    //----------

    void noteOff(void) {
      MStage = KODE_ENVELOPE_RELEASE;
    }

    //----------

    float process(void) {
      if (MStage == KODE_ENVELOPE_OFF) return 0.0f;
      if (MStage == KODE_ENVELOPE_FINISHED) return 0.0f;
      if (MStage == KODE_ENVELOPE_SUSTAIN) return MValue;
      float target = MStages[MStage].target;
      float rate   = MStages[MStage].rate;
      MValue += ( (target-MValue) * rate );
      if (fabs(target-MValue) <= kode_env_threshold) {
        MStage += 1;
      }
      return MValue;
    }

    //----------

    // http://www.kvraudio.com/forum/viewtopic.php?p=6515525#p6515525

    float processSteps(int32_t ASteps) {
      if (ASteps==0) return MValue;

      float result = process();

      //if (ASteps>1) for (int32 i=1; i<ASteps; i++) process();

      float target = MStages[MStage].target;
      float rate   = MStages[MStage].rate;
      MValue += (target - MValue) * (1 - pow(1 - rate, ASteps));

      //if (fabs(target-MValue) <= kode_env_threshold) {
      //  MStage += 1;
      //}

      return result;
    }

    //----------

};

//----------------------------------------------------------------------
#endif
