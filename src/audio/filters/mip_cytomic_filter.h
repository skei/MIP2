#ifndef kode_filter_cytomic_included
#define kode_filter_cytomic_included
//----------------------------------------------------------------------

/*
  https://github.com/FredAntonCorvest/Common-DSP
  see bottom of this file for license (mit)
*/

//----------

/*
  A ready to use C++ port of Andy Simper's implementation of State Variable
  Filters described in the technical paper provided at
  http://www.cytomic.com/files/dsp/SvfLinearTrapOptimised2.pdf
  Thank you Andy for the time you spent on making those technical papers.
*/

//----------------------------------------------------------------------

enum KODE_CytomicFilterTypes {
  kft_none      = 0,
  kft_lowpass   = 1,
  kft_bandpass  = 2,
  kft_highpass  = 3,
  kft_notch     = 4,
  kft_peak      = 5,
  kft_allpass   = 6,
  kft_bell      = 7,
  kft_lowshelf  = 8,
  kft_highshelf = 9
};

//----------------------------------------------------------------------

class KODE_CytomicFilter {

  private:

    double _ic1eq;
    double _ic2eq;
    double _v1, _v2, _v3;

  //----------------------------------------
  //
  //----------------------------------------

  public:

    KODE_CytomicFilter() {
      _ic1eq = _ic2eq = _v1 = _v2 = _v3 = 0;
    }

  //----------------------------------------
  //
  //----------------------------------------

  private:

    struct Coefficients {

      double _a1, _a2, _a3;
      double _m0, _m1, _m2;
      double _A;
      double _ASqrt;

      Coefficients() {
        _a1 = _a2 = _a3 = _m0 = _m1 = _m2 = 0;
        _A = _ASqrt = 1;
      }

      void update(double cutoff, double q = 0.5, uint32_t type = kft_lowpass, double sampleRate = 44100) {
        double g = tan((cutoff / sampleRate) * M_PI);
        double k = computeK(q, type == kft_bell /*USE GAIN FOR BELL FILTER ONLY*/);
        switch (type) {
          case kft_none:
            // nothing todo
            break;
          case kft_lowpass:
            computeA(g, k);
            _m0 = 0;
            _m1 = 0;
            _m2 = 1;
            break;
          case kft_bandpass:
            computeA(g, k);
            _m0 = 0;
            _m1 = 1;
            _m2 = 0;
            break;
          case kft_highpass:
            computeA(g, k);
            _m0 = 1;
            _m1 = -k;
            _m2 = -1;
            break;
          case kft_notch:
            computeA(g, k);
            _m0 = 1;
            _m1 = -k;
            _m2 = 0;
            break;
          case kft_peak:
            computeA(g, k);
            _m0 = 1;
            _m1 = -k;
            _m2 = -2;
            break;
          case kft_allpass:
            computeA(g, k);
            _m0 = 1;
            _m1 = -2*k;
            _m2 = 0;
            break;
          case kft_bell:
            computeA(g, k);
            _m0 = 1;
            _m1 = k*(_A*_A - 1);
            _m2 = 0;
            break;
          case kft_lowshelf:
            computeA(g /= _ASqrt, k);
            _m0 = 1;
            _m1 = k*(_A-1);
            _m2 = _A*_A - 1;
            break;
          case kft_highshelf:
            computeA(g *= _ASqrt, k);
            _m0 = _A*_A;
            _m1 = k*(1-_A)*_A;
            _m2 = 1-_A*_A;
            break;
          default:
            KODE_Assert(false);
        }
      }

      void setGain(double gainDb) {
        _A = pow(10.0, gainDb / 40.0);
        _ASqrt = sqrt(_A);
      }

      double computeK(double q, bool useGain=false) {
        return 1.f / (useGain ? (q*_A) : q);
      }

      void computeA(double g, double k) {
        _a1 = 1/(1 + g*(g + k));
        _a2 = g*_a1;
        _a3 = g*_a2;
      }

    } _coef;

  //----------------------------------------
  //
  //----------------------------------------

  public:

    /*
      Gain in dB to boost or cut the cutoff point of the Low shelf filter
    */

    void setGain(double gainDb) {
      _coef.setGain(gainDb);
    }

    /*
      Updates the coefficients of the filter for the given cutoff, q, type and
      sample rate
      cutoff     = cutoff frequency in Hz that should be clamped into the range
                   [16hz, NYQUIST].
      Q          = Q factor that should be clamped into the range
                   [0.025f, 40.f]. Default value is 0.5
      sampleRate = Sample rate. Default value is 44100hz. Do not forget to call
                   resetState before changing the sample rate
    */

    void updateCoefficients(double cutoff, double q = 0.5, uint32_t type = kft_lowpass, double sampleRate = 44100) {
      _coef.update(cutoff,q,type,sampleRate);
    }

    /*
      Resets the state of the filter
      Do not forget to call resetState before changing the sample rate
    */

    void resetState(void) {
      _ic1eq = _ic2eq = 0;
    }

    /*
      Tick method. Apply the filter on the provided material
    */

    double tick(double v0) {
      _v3 = v0 - _ic2eq;
      _v1 = _coef._a1*_ic1eq + _coef._a2*_v3;
      _v2 = _ic2eq + _coef._a2*_ic1eq + _coef._a3*_v3;
      _ic1eq = 2*_v1 - _ic1eq;
      _ic2eq = 2*_v2 - _ic2eq;
      return _coef._m0*v0 + _coef._m1*_v1 + _coef._m2*_v2;
    }

};

//----------------------------------------------------------------------
#endif

//----------

/*
  MIT License

  Copyright (c) 2016 Fred Anton Corvest (FAC)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
