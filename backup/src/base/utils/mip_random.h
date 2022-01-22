#ifndef mip_random_included
#define mip_random_included
//----------------------------------------------------------------------

#include "mip.h"

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

/*
float   MIP_Random(void);
float   MIP_RandomSigned(void);
float   MIP_RandomRange(float minval, float maxval);
int32   MIP_RandomInt(void);
int32   MIP_RandomSignedInt(void);
int32   MIP_RandomRangeInt(int32 minval, int32 maxval);
*/


//#define MIP_RANDOM_SYSTEM
//#define MIP_RANDOM_XORSHIFT
// etc..


//#ifdef MIP_RANDOM_SYSTEM
  #define MIP_Random           MIP_Random_System
  #define MIP_RandomSigned     MIP_RandomSigned_System
  #define MIP_RandomRange      MIP_RandomRange_System
  #define MIP_RandomInt        MIP_RandomInt_System
  #define MIP_RandomSignedInt  MIP_RandomSignedInt_System
  #define MIP_RandomRangeInt   MIP_RandomRangeInt_System
//#endif

//----------------------------------------------------------------------
// MIP_Random
//----------------------------------------------------------------------

#include <stdlib.h> // rand

/*
class MIP_Random_Rand {
  public:

    MIP_Random_Rand() {
    }

    virtual ~MIP_Random_Rand() {
    }

    inline void  randomice(void) {
    }

    inline float random(void) {
      return (float)rand() / (float)RAND_MAX;
    }

    inline float randomSigned(void) {
      float r = (float)rand() / (float)RAND_MAX;
      return r * 2 - 1;
    }

};
*/

//----------

//static MIP_Random_Rand MIP_RANDOM;
//inline float MIP_Random(void) { return MIP_RANDOM.random(); }
//inline float MIP_RandomSigned(void) { return MIP_RANDOM.randomSigned(); }

//----------------------------------------------------------------------
// system
//----------------------------------------------------------------------

/*
  is there something weird with this?
    1 / RAND_MAX is very small -> rounding errors?
    todo: double?
*/

float MIP_Random_System(void) {
  float rnd = (float)rand();
  float result = rnd * (float)MIP_INVRANDMAX;
  return result;
}

//----------

float MIP_RandomSigned_System(void) {
  float r = (float)rand() * (float)MIP_INVRANDMAX;// / (float)RAND_MAX;
  return r * 2.0f - 1.0f;
}

// inclusive

float MIP_RandomRange_System(float minval, float maxval) {
  return minval + (MIP_Random() * (maxval - minval) );
}

//----------

int32_t MIP_RandomInt_System(void) {
  return rand();// & 0x7fffffff;
}

// -MIP_RandomMax..MIP_RandomMax

int32_t MIP_RandomSignedInt_System(void) {
  return rand();
}

// inclusive

int32_t MIP_RandomRangeInt_System(int32_t minval, int32_t maxval) {
  int32_t range = maxval-minval+1;
  if (range <= 0) return minval;
  //else return minval + SModulo(MIP_RandomInt(),range);
  else return minval + ( MIP_RandomInt() % range );
}

//----------------------------------------------------------------------
// http://iquilezles.org/www/articles/sfrand/sfrand.htm
// returns a random float within the range of -1 to 1
//----------------------------------------------------------------------

/*
float SRandFloat(int* a_Seed) {
  float res;
  *a_Seed *= 16807;
  // warning: dereferencing type-punned pointer will break strict-aliasing rules|
  // warning: dereferencing pointer \91res.89\92 does break strict-aliasing rules|
  *((unsigned int *)&res) = ( ( (unsigned int)a_Seed[0])>>9 ) | 0x40000000;
  // warning: \91res\92 is used uninitialized in this function|
  return (res-3.0f);
}
*/

//----------------------------------------------------------------------
// http://forum.cockos.com/showpost.php?p=348182&postcount=6
//----------------------------------------------------------------------

/*
  desc:noise/random
  @init
    value   = 19; // initialize
    b_noise = 19.1919191919191919191919191919191919191919;
  @sample
  // -1..1
    b_noise = b_noise * b_noise;
    i_noise = floor(b_noise); // |0;
    b_noise = b_noise - i_noise;
    r0      = b_noise - 0.5;
    b_noise = b_noise + value;
  // 0..1
    b_noiselast = b_noise;
    b_noise = b_noise + value;
    b_noise = b_noise * b_noise;
    b_noise = (b_noise+b_noiselast) * 0.5;
    i_noise = floor(b_noise);
    b_noise = b_noise - i_noise;
    r1      = b_noise;
  //
    spl0 = r0;
    spl1 = r1;
*/

//----------------------------------------------------------------------
// Lehmer (Park-Miller)
// http://en.wikipedia.org/wiki/Park%E2%80%93Miller_random_number_generator
//----------------------------------------------------------------------

/*
  uint32_t lcg_rand(uint32_t a) {
    return ((uint64_t)a * 279470273UL) % 4294967291UL;
  }
*/

//----------------------------------------------------------------------
// linear feedback shift register
// http://en.wikipedia.org/wiki/Linear_feedback_shift_register
//----------------------------------------------------------------------

/*
  # include <stdint.h>
  int main(void) {
    uint16_t lfsr = 0xACE1u;
    unsigned bit;
    unsigned period = 0;
    do {
      // taps: 16 14 13 11; feedback polynomial: x^16 + x^14 + x^13 + x^11 + 1
      bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
      lfsr =  (lfsr >> 1) | (bit << 15);
      ++period;
    } while(lfsr != 0xACE1u);
    return 0;
  }
*/

//----------------------------------------------------------------------
// mersenne twister
// http://en.wikipedia.org/wiki/Mersenne_twister
//----------------------------------------------------------------------

/*
  // Create a length 624 array to store the state of the generator
  int[0..623] MT
  int index = 0

  // Initialize the generator from a seed
  function initialize_generator(int seed) {
    index := 0
    MT[0] := seed
    for i from 1 to 623 { // loop over each other element
      MT[i] := last 32 bits of(1812433253 * (MT[i-1] xor (right shift by 30 bits(MT[i-1]))) + i) // 0x6c078965
    }
}

//----------

// Extract a tempered pseudorandom number based on the index-th value,
// calling generate_numbers() every 624 numbers
function extract_number() {
  if index == 0 {
    generate_numbers()
  }
  int y := MT[index]
  y := y xor (right shift by 11 bits(y))
  y := y xor (left shift by 7 bits(y) and (2636928640))   // 0x9d2c5680
  y := y xor (left shift by 15 bits(y) and (4022730752))  // 0xefc60000
  y := y xor (right shift by 18 bits(y))
  index := (index + 1) mod 624
  return y
}

//----------

// Generate an array of 624 untempered numbers
function generate_numbers() {
  for i from 0 to 623 {
    int y := (MT[i] and 0x80000000)               // bit 31 (32nd bit) of MT[i]
           + (MT[(i+1) mod 624] and 0x7fffffff)   // bits 0-30 (first 31 bits) of MT[...]
    MT[i] := MT[(i + 397) mod 624] xor (right shift by 1 bit(y))
    if (y mod 2) != 0 { // y is odd
      MT[i] := MT[i] xor (2567483615) // 0x9908b0df
    }
  }
}
*/

//----------------------------------------------------------------------
// multiply-with-carry
// http://en.wikipedia.org/wiki/Multiply-with-carry
//----------------------------------------------------------------------

/*
#include <stdint.h>
#define PHI 0x9e3779b9
static uint32_t Q[4096], c = 362436;

//----------

void init_rand(uint32_t x) {
  int i;
  Q[0] = x;
  Q[1] = x + PHI;
  Q[2] = x + PHI + PHI;
  for (i = 3; i < 4096; i++)
    Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
}

//----------

uint32_t rand_cmwc(void) {
  static uint32_t i = 4095;
  uint64_t t;
  i = (i + 1) & 4095;
  t = (18705ULL * Q[i]) + c;
  c = t >> 32;
  Q[i] = 0xfffffffe - t;
  return Q[i];
}
*/

//----------------------------------------------------------------------
// xorshift
// http://en.wikipedia.org/wiki/Xorshift
//----------------------------------------------------------------------

/*
#include <stdint.h>

// These state variables must be initialized so that they are not all zero.
uint32_t x, y, z, w;

//----------

uint32_t xor128(void) {
  uint32_t t;
  t = x ^ (x << 11);
  x = y; y = z; z = w;
  return w = w ^ (w >> 19) ^ t ^ (t >> 8);
}
*/

// see variants..

//----------------------------------------------------------------------
// "minimal standard"
// http://www.firstpr.com.au/dsp/rand31/
//----------------------------------------------------------------------

/*
  Park-Miller "minimal standard" 31 bit pseudo-random number generator,
  implemented with David G. Carta's optimisation: with 32 bit math and
  without division.
*/

//----------

/*
long unsigned int rand31_next() {
  long unsigned int hi,lo;
  lo = 16807 * (seed & 0xFFFF);
  hi = 16807 * (seed >> 16);
  lo += (hi & 0x7FFF) << 16;
  lo += hi >> 15;
  if (lo>0x7FFFFFFF) lo -= 0x7FFFFFFF;
  return ( seed = (long)lo );
}
*/

//----------------------------------------------------------------------
// middle square method
// http://en.wikipedia.org/wiki/Middle-square_method
//----------------------------------------------------------------------

/*
for (int i=0; i<samplesToProduce; ++i) {
  //select the middle six digits for the next seed
  //the seed has been selected
  newseed = (x / 1000) % 1000000;
  storageArray[i] = newseed;
  //The sequence will almost always go to zero, so an improvement
  //to the algorithm would be reseeding with the time as seen below.
  //
  //if (newseed < threshHold) {
  //  newseed = time(NULL);
  //}
  //
  x = newseed * newseed;
}
*/

//----------------------------------------------------------------------
// rc4
// http://en.wikipedia.org/wiki/RC4
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// fast float random numbers
// http://www.musicdsp.org/archive.php?classid=5#273
//----------------------------------------------------------------------

/*
  a small and fast implementation for random float numbers in the range [-1,1],
  usable as white noise oscillator.
  compared to the naive usage of the rand() function it gives a speedup factor
  of 9-10. compared to a direct implementation of the rand() function (visual
  studio implementation) it still gives a speedup by a factor of 2-3.
  apart from beeing faster it also provides more precision for the resulting
  floats since its base values use full 32bit precision.
*/

//----------

/*
  There is no doubt that implementation 3 is fast, but the problem I had with it
  is that there's no obvious way to limit the amplitude of the generated signal.

  So instead I tried implementation 2 and ran into a different problem. The code
  is written such that it assumes that RAND_MAX is equal to 0x7FFF, which was
  not true on my system (it was 0x7FFFFFFF). Fortunately, this was easy to fix.
  I simply removed the >> 16 and worked fine for me. My final implementation was:
  return (float)(RandSeed = RandSeed * 214013L + 2531011L) / 0x7FFFFFFF * 2.0f * amp - amp;
  where "amp" is the desired amplitude.

  -----

  It should be noted in the code that for method #3, you must initialize the
  seed to non-zero before using it.

  -----

  I don't understand Judahmenter's comment about 3 not limiting the amplitude.
  As it stands it returns a value -1 to 1, so just multiply by your 'amp' value.
  This turns into a handy 0-1 random number if you take off the sign bit:
  (float)(RandSeed & 0x7FFFFFFF) * 4.6566129e-010f;
*/

//----------

/*
// set the initial seed to whatever you like
static int RandSeed = 1;

//----------

// using rand() (16bit precision)
// takes about 110 seconds for 2 billion calls

float RandFloat1() {
  return ((float)rand()/RAND_MAX) * 2.0f - 1.0f;
}

//----------

// direct implementation of rand() (16 bit precision)
// takes about 32 seconds for 2 billion calls

float RandFloat2() {
  return ((float)(((RandSeed = RandSeed * 214013L + 2531011L) >> 16) & 0x7fff)/RAND_MAX) * 2.0f - 1.0f;
}

//----------

// fast rand float, using full 32bit precision
// takes about 12 seconds for 2 billion calls

float Fast_RandFloat() {
  RandSeed *= 16807;
  return (float)RandSeed * 4.6566129e-010f;
}
*/

//----------------------------------------------------------------------
// pseudo-random generator
// http://www.musicdsp.org/archive.php?classid=1#59
//----------------------------------------------------------------------

/*
  Type : Linear Congruential, 32bit
  References : Hal Chamberlain, "Musical Applications of Microprocessors"
  (Posted by Phil Burk)
  Notes : This can be used to generate random numeric sequences or to synthesise
  a white noise audio signal. If you only use some of the bits, use the most
  significant bits by shifting right. Do not just mask off the low bits.
*/

//----------

/*
  // Calculate pseudo-random 32 bit number based on linear congruential method.
  unsigned long GenerateRandomNumber(void) {
    // Change this for different random sequences.
    static unsigned long randSeed = 22222;
    randSeed = (randSeed * 196314165) + 907633515;
    return randSeed;
  }
*/

//----------------------------------------------------------------------
// easy noise generation
// http://www.musicdsp.org/archive.php?classid=1#217
//----------------------------------------------------------------------

// b_noise = 19.1919191919191919191919191919191919191919;

// alternatively, the number 19 below can be replaced with a number of your
// choice, to get that particular flavour of noise.

/*
b_noise = b_noise * b_noise;
int i_noise = b_noise;
b_noise = b_noise - i_noise;
double b_noiseout;
b_noiseout = b_noise - 0.5;
b_noise = b_noise + 19;
*/

/*
  you can change 0.5, to for instance 0.19.
  It controls the patterning. You could try even lower aswell, maybe 1e-19.
*/

//----------

/*
  // alternatively you can do:
  double b_noiselast = b_noise;
  b_noise = b_noise + 19;
  b_noise = b_noise * b_noise;
  b_noise = b_noise + ((-b_noise + b_noiselast) * 0.5);
  int i_noise = b_noise;
  b_noise = b_noise - i_noise;
  // this will remove the patterning.
*/

//----------------------------------------------------------------------
//
// http://www.kvraudio.com/forum/viewtopic.php?p=4611887#p4611887
//----------------------------------------------------------------------

/*
  Works equally well with single precision (though obviously with shorter
  period). Sounds significantly better than anything that takes comparable
  amount of work. I originally got it from http://www.number.com.pt/index.html
  (though the page seems to have changed; there's another PRNG proposed now).
*/

//----------

/*
double rand_x, rand_a;
public:
  double drand() {
    rand_a = rand_a + rand_x;
    rand_x = rand_a * rand_x + 0.5;
    rand_x -= floor(rand_x);
    return 2.0*rand_x - 1.0;
  }
*/

//----------------------------------------------------------------------
#endif
