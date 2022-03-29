#ifndef sa_botage_descriptor_included
#define sa_botage_descriptor_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_parameters.h"

//----------------------------------------------------------------------

enum myEParameters {

  P_BUFFER_NUM_BEATS = 0,
  P_BUFFER_NUM_SLICES,

  P_ENV_LOOP_ATT,
  P_ENV_LOOP_DEC,
  P_ENV_SLICE_ATT,
  P_ENV_SLICE_DEC,

  P_REPEAT_PROB,
  P_REPEAT_SLICE_BITS,
  P_REPEAT_SPLIT_BITS,

  P_LOOPSIZE_RANGE_PROB,
  P_LOOPSIZE_RANGE_MIN,
  P_LOOPSIZE_RANGE_MAX,
  P_LOOPSIZE_LOOP_PROB,
  P_LOOPSIZE_LOOP_MIN,
  P_LOOPSIZE_LOOP_MAX,

  P_LOOPSPEED_RANGE_PROB,
  P_LOOPSPEED_RANGE_MIN,
  P_LOOPSPEED_RANGE_MAX,
  P_LOOPSPEED_LOOP_PROB,
  P_LOOPSPEED_LOOP_MIN,
  P_LOOPSPEED_LOOP_MAX,

  P_OFFSET_RANGE_PROB,
  P_OFFSET_RANGE_MIN,
  P_OFFSET_RANGE_MAX,
  P_OFFSET_LOOP_PROB,
  P_OFFSET_LOOP_MIN,
  P_OFFSET_LOOP_MAX,

  P_REVERSE_RANGE_PROB,
  P_REVERSE_LOOP_PROB,

  P_FX_MULTI,
  P_FX_RANGE_PROB,
  P_FX_RANGE_MIN,
  P_FX_RANGE_MAX,
  P_FX_LOOP_PROB,
  P_FX_LOOP_MIN,
  P_FX_LOOP_MAX,

  P_FX_FILTER_PROB,
  P_FX_FILTER_FREQ,
  P_FX_FILTER_BW,
  P_FX_FILTER_TYPE,

  P_NUM_PARAMETERS
};

const char* txt_filter[5] = {
  "Off",
  "Lowpass",
  "Highpass",
  "Bandpass",
  "Notch"
};

//----------------------------------------------------------------------

class myDescriptor
: public KODE_Descriptor {

//------------------------------
public:
//------------------------------

  myDescriptor() {

    #ifdef KODE_DEBUG
      setName("sa_botage_debug");
    #else
      setName("sa_botage");
    #endif

    setVersion( 0x0000000a );
    setAuthor("skei.audio");
    setUrl("https://torhelgeskei.com");
    setEmail("tor.helge.skei@gmail.com");

    //setIsSynth();
    //setCanSendMidi();
    //setCanReceiveMidi();

    #ifndef KODE_NO_GUI
      setHasEditor(true);
      setEditorSize(546 /*+ 136*/,626);
    #endif

    // in/out

    appendInput(  new KODE_PluginPort("input_1")  );
    appendInput(  new KODE_PluginPort("input_2")  );
    appendOutput( new KODE_PluginPort("output_1") );
    appendOutput( new KODE_PluginPort("output_2") );

    // parameters

    appendParameter( new KODE_IntParameter(   "Beats",              4,    1, 8)); // P_BUFFER_NUM_BEATS
    appendParameter( new KODE_IntParameter(   "Slices",             2,    1, 8)); // P_BUFFER_NUM_SLICES

    appendParameter( new KODE_PowParameter(   "att", "ms",          1,    3,    true, 0, 100 )); // P_ENV_LOOP_ATT
    appendParameter( new KODE_PowParameter(   "dec", "ms",          1,    3,    true, 0, 100 )); // P_ENV_LOOP_DEC
    appendParameter( new KODE_PowParameter(   "att", "%",           0,    3,    true, 0, 100 )); // P_ENV_SLICE_ATT
    appendParameter( new KODE_PowParameter(   "dec", "%",           0,    3,    true, 0, 100 )); // P_ENV_SLICE_DEC

    appendParameter( new KODE_FloatParameter( "Repeat Prob",        0.2   )); // P_REPEAT_PROB
    appendParameter( new KODE_IntParameter(   "Repeat Slice Bits",  0b00001111,    0, 255 )); // P_REPEAT_SLICE_BITS
    appendParameter( new KODE_IntParameter(   "Repeat Split Bits",  0b10001011,    0, 255 )); // P_REPEAT_SPLIT_BITS

    appendParameter( new KODE_FloatParameter( "Size",               0.2   )); // P_LOOPSIZE_RANGE_PROB
    appendParameter( new KODE_PowParameter(   "min", "%",           75,   2, true, 50, 200 )); // P_LOOPSIZE_RANGE_MIN
    appendParameter( new KODE_PowParameter(   "max", "%",           150,  2, true, 50, 200 )); // P_LOOPSIZE_RANGE_MAX
    appendParameter( new KODE_FloatParameter( "",                   0.2   )); // P_LOOPSIZE_LOOP_PROB
    appendParameter( new KODE_PowParameter(   "min", "%",            75,  2, true, 50, 200 )); // P_LOOPSIZE_LOOP_MIN
    appendParameter( new KODE_PowParameter(   "max", "%",           150,  2, true, 50, 200 )); // P_LOOPSIZE_LOOP_MAX

    appendParameter( new KODE_FloatParameter( "Speed",              0.2   )); // P_LOOPSPEED_RANGE_PROB
    appendParameter( new KODE_PowParameter(   "min", "%",            75,  2, true, 50, 200 )); // P_LOOPSPEED_RANGE_MIN
    appendParameter( new KODE_PowParameter(   "max", "%",           150,  2, true, 50, 200 )); // P_LOOPSPEED_RANGE_MAX
    appendParameter( new KODE_FloatParameter( "",                   0.2   )); // P_LOOPSPEED_LOOP_PROB
    appendParameter( new KODE_PowParameter(   "min", "%",            75,  2, true, 50, 200 )); // P_LOOPSPEED_LOOP_MIN
    appendParameter( new KODE_PowParameter(   "max", "%",           150,  2, true, 50, 200 )); // P_LOOPSPEED_LOOP_MAX

    appendParameter( new KODE_FloatParameter( "Offset",             0.2   )); // P_OFFSET_RANGE_PROB
    appendParameter( new KODE_IntParameter(   "min", "sl",         -4,    -16, 16 )); // P_OFFSET_RANGE_MIN
    appendParameter( new KODE_IntParameter(   "max", "sl",          4,    -16, 16 )); // P_OFFSET_RANGE_MAX
    appendParameter( new KODE_FloatParameter( "",                   0.2   )); // P_OFFSET_LOOP_PROB
    appendParameter( new KODE_IntParameter(   "min", "sl",         -4,    -16, 16 )); // P_OFFSET_LOOP_MIN
    appendParameter( new KODE_IntParameter(   "max",  "sl",         4,    -16, 16 )); // P_OFFSET_LOOP_MAX

    appendParameter( new KODE_FloatParameter( "Reverse 1",          0.2   )); // P_REVERSE_RANGE_PROB
    appendParameter( new KODE_FloatParameter( "Reverse",            0.2   )); // P_REVERSE_LOOP_PROB

    appendParameter( new KODE_IntParameter(   "FX multi",           1,    0, 1 )); // P_FX_MULTI
    appendParameter( new KODE_FloatParameter( "FX 1",               0.2   )); // P_FX_RANGE_PROB
    appendParameter( new KODE_PowParameter(   "min", "%",            75,  2, true, 50, 200 )); // P_FX_RANGE_MIN
    appendParameter( new KODE_PowParameter(   "max", "%",           150,  2, true, 50, 200 )); // P_FX_RANGE_MAX
    appendParameter( new KODE_FloatParameter( "FX",                 0.2   )); // P_FX_LOOP_PROB
    appendParameter( new KODE_PowParameter(   "min", "%",            75,  2, true, 50, 200 )); // P_FX_LOOP_MIN
    appendParameter( new KODE_PowParameter(   "max", "%",           150,  2, true, 50, 200 )); // P_FX_LOOP_MAX

    appendParameter( new KODE_FloatParameter( "Filter",             0.2   )); // P_FX_FILTER_PROB
    appendParameter( new KODE_FloatParameter( "Freq", "",           0.5   )); // P_FX_FILTER_FREQ
    appendParameter( new KODE_FloatParameter( "BW", "",             0.5   )); // P_FX_FILTER_BW
    appendParameter( new KODE_TextParameter(  "Type",               1,    5, txt_filter )); // P_FX_FILTER_TYPE

    //-----

//    MParameters[P_LOOPSIZE_RANGE_MIN]->setNumDigits(0);
//    MParameters[P_LOOPSIZE_RANGE_MAX]->setNumDigits(0);
//    MParameters[P_LOOPSIZE_LOOP_MIN]->setNumDigits(0);
//    MParameters[P_LOOPSIZE_LOOP_MAX]->setNumDigits(0);
//    MParameters[P_LOOPSPEED_RANGE_MIN]->setNumDigits(0);
//    MParameters[P_LOOPSPEED_RANGE_MAX]->setNumDigits(0);
//    MParameters[P_LOOPSPEED_LOOP_MIN]->setNumDigits(0);
//    MParameters[P_LOOPSPEED_LOOP_MAX]->setNumDigits(0);
//    MParameters[P_FX_RANGE_MIN]->setNumDigits(0);
//    MParameters[P_FX_RANGE_MAX]->setNumDigits(0);
//    MParameters[P_FX_LOOP_MIN]->setNumDigits(0);
//    MParameters[P_FX_LOOP_MAX]->setNumDigits(0);
//    MParameters[P_ENV_LOOP_ATT]->setNumDigits(2);
//    MParameters[P_ENV_LOOP_DEC]->setNumDigits(2);
//    MParameters[P_ENV_SLICE_ATT]->setNumDigits(0);
//    MParameters[P_ENV_SLICE_DEC]->setNumDigits(0);
//    MParameters[P_FX_FILTER_FREQ]->setNumDigits(2);
//    MParameters[P_FX_FILTER_BW]->setNumDigits(2);

  }
};

//----------------------------------------------------------------------
#endif
