#ifndef kode_rms_included
#define kode_rms_included
//----------------------------------------------------------------------

// original author: lubomir i ivanov (for axonlib)
// used with permission

//----------------------------------------------------------------------

/*
  one pole filter for RMS approximation
  usage example:
    dspRMS rmsfilter;
    rmsfilter.setup(300, 44100);              // 300ms, samplerate (can be updateSampleRate())
    float rmsout = rmsfilter.process(input);  // process input sample
    rmsfilter.reset();                        // to reset the rms value
*/

class KODE_Rms {

  private:

    float win,fout,a0,b1;

  public:

    KODE_Rms() {
      reset();
      setup(300, 44100);
    }

    //----------

    /*
      set filter coeff
      winlen  : window length (ms)
      srate   : sample rate
    */

    void setup(float winlen, float srate) {
      b1 = exp(-1/(winlen*srate*0.001));
      a0 = 1 - b1;
    }

    //----------

    void reset() {
      win = 0;
      fout = 0;
      a0 = 0;
      b1 = 0;
    }

    //----------

    float process(float input) {
      fout = a0*input*input + b1*fout + KODE_FLOAT_DENORM;
      return sqrt(fout);
    }

};

//----------------------------------------------------------------------
#endif
