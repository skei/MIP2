
/*
  TODO:
  - the voices are still in mono..
  - proper db and freq (params/mods)
  - scalable editor

  - parameter modulation..
  don't draw anything until we receives the first modulation event?

*/

//----------------------------------------------------------------------

#define MIP_GUI_XCB
#define MIP_PAINTER_CAIRO

#define MIP_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/mip.socket
//tail -F ~/.BitwigStudio/log/engine.log | pv --rate --bytes > /dev/null

//#define MIP_DEBUG_PRINT_TIME
//#define MIP_DEBUG_WATCHES
//#define MIP_DEBUG_CALLSTACK
//#define MIP_DEBUG_CRASH_HANDLER

//#define MIP_DEBUG_CLAP

//#define MIP_VST2
//#define MIP_VST3

//----------------------------------------------------------------------

#define PAR_MASTER_VOL      0
#define PAR_MASTER_PAN      1
#define PAR_MASTER_OSC1_OUT 2
#define PAR_MASTER_OSC2_OUT 3
#define PAR_MASTER_RES1_OUT 4
#define PAR_MASTER_RES2_OUT 5

// osc 1

#define PAR_OSC1_IN_O1      6
#define PAR_OSC1_IN_O2      7
#define PAR_OSC1_IN_R1      8
#define PAR_OSC1_IN_R2      9
#define PAR_OSC1_IN_N       10

#define PAR_OSC1_TYPE       11
#define PAR_OSC1_SHAPE      12
#define PAR_OSC1_WIDTH      13

#define PAR_OSC1_PM_TYPE    14
#define PAR_OSC1_PM_AMOUNT  15
#define PAR_OSC1_WM_TYPE    16
#define PAR_OSC1_WM_AMOUNT  17

#define PAR_OSC1_OCT        18
#define PAR_OSC1_SEMI       19
#define PAR_OSC1_CENT       20

// osc 2

#define PAR_OSC2_IN_O1      21
#define PAR_OSC2_IN_O2      22
#define PAR_OSC2_IN_R1      23
#define PAR_OSC2_IN_R2      24
#define PAR_OSC2_IN_N       25

#define PAR_OSC2_TYPE       26
#define PAR_OSC2_SHAPE      27
#define PAR_OSC2_WIDTH      28

#define PAR_OSC2_PM_TYPE    29
#define PAR_OSC2_PM_AMOUNT  30
#define PAR_OSC2_WM_TYPE    31
#define PAR_OSC2_WM_AMOUNT  32

#define PAR_OSC2_OCT        33
#define PAR_OSC2_SEMI       34
#define PAR_OSC2_CENT       35

// res 1

#define PAR_RES1_IN_O1      36
#define PAR_RES1_IN_O2      37
#define PAR_RES1_IN_R1      38
#define PAR_RES1_IN_R2      39
#define PAR_RES1_IN_N       40

#define PAR_RES1_TYPE       41
#define PAR_RES1_SHAPE      42
#define PAR_RES1_FB         43
#define PAR_RES1_DAMP       44
#define PAR_RES1_ROUGH      45
#define PAR_RES1_OCT        46
#define PAR_RES1_SEMI       47
#define PAR_RES1_CENT       48

// res 2

#define PAR_RES2_IN_O1      49
#define PAR_RES2_IN_O2      50
#define PAR_RES2_IN_R1      51
#define PAR_RES2_IN_R2      52
#define PAR_RES2_IN_N       53

#define PAR_RES2_TYPE       54
#define PAR_RES2_SHAPE      55
#define PAR_RES2_FB         56
#define PAR_RES2_DAMP       57
#define PAR_RES2_ROUGH      58
#define PAR_RES2_OCT        59
#define PAR_RES2_SEMI       60
#define PAR_RES2_CENT       61

// flt1

#define PAR_FLT1_TYPE       62
#define PAR_FLT1_FREQ       63
#define PAR_FLT1_RES        64

// env1

#define PAR_ENV1_ATT        65
#define PAR_ENV1_DEC        66
#define PAR_ENV1_SUS        67
#define PAR_ENV1_REL        68

//----------

#define NUM_PARAMS          69
#define NUM_VOICES          256

//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_voice_manager.h"

#include "sa_ts1/sa_ts1_editor.h"
#include "sa_ts1/sa_ts1_voice.h"

typedef MIP_VoiceManager<sa_ts1_Voice<double>,NUM_VOICES> sa_ts1_VoiceManager;

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

//----------

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "skei.audio/sa_ts1",
  #ifdef MIP_DEBUG
    .name       = "sa_ts1 (debug)",
  #else
    .name       = "sa_ts1",
  #endif
  .vendor       = "skei.audio",
  .url          = "https://torhelgeskei.com",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.6",
  //myFeatures
  .features     = (const char*[]){
    CLAP_PLUGIN_FEATURE_INSTRUMENT,
    CLAP_PLUGIN_FEATURE_SYNTHESIZER,
    nullptr
  }
};

//----------------------------------------------------------------------
//
// processor
//
//----------------------------------------------------------------------

//class myProcessor {
//
//public:
//
//  //myProcessor(MIP_AudioProcessorListener* AListener)
//  //: MIP_AudioProcessor(AListener) {
//  //}
//
//public:
//
//  clap_process_status process(const clap_process_t *process) {
//    //MIP_PRINT;
//    return CLAP_PROCESS_CONTINUE;
//  }
//
//  void handle_note_on_event(clap_event_note_t* event) { // final {
//    MIP_Print("TODO\n");
//  }
//
//  void handle_note_off_event(clap_event_note_t* event) { // final {
//    MIP_Print("TODO\n");
//  }
//
//  void handle_note_end_event(clap_event_note_t* event) { // final {
//    MIP_Print("ERROR\n");
//  }
//
//  void handle_note_choke_event(clap_event_note_t* event) { // final {
//    MIP_Print("TODO\n");
//  }
//
//  void handle_note_expression_event(clap_event_note_expression_t* event) { // final {
//    MIP_Print("TODO\n");
//  }
//
//};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin
//: public MIP_Plugin<myProcessor> {
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  clap_param_info_t myParameters[NUM_PARAMS] = {

    //---------- global ----------

    { PAR_MASTER_VOL,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE,
      nullptr,
      "Vol",
      "",
      0.0,
      1.0,
      0.5
    },
    { PAR_MASTER_PAN,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE,
      nullptr,
      "Pan",
      "",
      0.0,
      1.0,
      0.5
    },

    { PAR_MASTER_OSC1_OUT,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "O1",
      "",
      0.0,
      1.0,
      0.5
    },
    { PAR_MASTER_OSC2_OUT,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "O2",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_MASTER_RES1_OUT,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "R1",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_MASTER_RES2_OUT,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "R2",
      "",
      0.0,
      1.0,
      0.0
    },

    //---------- osc1 ----------

    // inputs

    { PAR_OSC1_IN_O1,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "O1",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_OSC1_IN_O2,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "O2",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_OSC1_IN_R1,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "R1",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_OSC1_IN_R2,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "R2",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_OSC1_IN_N,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "N",
      "",
      0.0,
      1.0,
      0.0
    },

    //

    { PAR_OSC1_TYPE,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_STEPPED,
      nullptr,
      "Type",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_OSC1_SHAPE,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Shape",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_OSC1_WIDTH,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Width",
      "",
      0.0,
      1.0,
      0.5
    },

    // pm, wm

    { PAR_OSC1_PM_TYPE,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_STEPPED,
      nullptr,
      "Phase",
      "",
      0,
      5,
      0
    },
    { PAR_OSC1_PM_AMOUNT,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Phase",
      "",
      0.0,
      1.0,
      0.0
    },

    { PAR_OSC1_WM_TYPE,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_STEPPED,
      nullptr,
      "Wave",
      "",
      0,
      3,
      0
    },
    { PAR_OSC1_WM_AMOUNT,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Wave",
      "",
      0.0,
      1.0,
      0.0
    },

    { PAR_OSC1_OCT,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_STEPPED
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Oct",
      "",
     -4,
      4,
      0
    },
    { PAR_OSC1_SEMI,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_STEPPED
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Semi",
      "",
     -12,
      12,
      0
    },
    { PAR_OSC1_CENT,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Cent",
      "",
     -1.0,
      1.0,
      0.
    },

    //---------- osc2 ----------

    // inputs

    { PAR_OSC2_IN_O1,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "O1",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_OSC2_IN_O2,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "O2",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_OSC2_IN_R1,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "R1",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_OSC2_IN_R2,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "R2",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_OSC2_IN_N,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "N",
      "",
      0.0,
      1.0,
      0.0
    },

    //

    { PAR_OSC2_TYPE,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_STEPPED,
      nullptr,
      "Type",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_OSC2_SHAPE,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Shape",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_OSC2_WIDTH,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Width",
      "",
      0.0,
      1.0,
      0.5
    },

    // pm, wm

    { PAR_OSC2_PM_TYPE,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_STEPPED,
      nullptr,
      "Phase",
      "",
      0,
      5,
      0
    },
    { PAR_OSC2_PM_AMOUNT,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Phase",
      "",
      0.0,
      1.0,
      0.0
    },

    { PAR_OSC2_WM_TYPE,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_STEPPED,
      nullptr,
      "Wave",
      "",
      0,
      3,
      0
    },
    { PAR_OSC2_WM_AMOUNT,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Wave",
      "",
      0.0,
      1.0,
      0.0
    },

    { PAR_OSC2_OCT,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_STEPPED
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Oct",
      "",
     -4,
      4,
      0
    },
    { PAR_OSC2_SEMI,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_STEPPED
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Semi",
      "",
     -12,
      12,
      0
    },
    { PAR_OSC2_CENT,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Cent",
      "",
     -1.0,
      1.0,
      0.
    },

    //---------- res1 ----------

    // inputs

    { PAR_RES1_IN_O1,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "O1",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_RES1_IN_O2,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "O2",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_RES1_IN_R1,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "R1",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_RES1_IN_R2,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "R2",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_RES1_IN_N,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "N",
      "",
      0.0,
      1.0,
      1.0
    },

    //

    { PAR_RES1_TYPE,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_STEPPED,
      nullptr,
      "Type",
      "",
      0.0,
      2.0,
      0.0
    },
    { PAR_RES1_SHAPE,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Shape",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_RES1_FB,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "FB",
      "",
      0.0,
      1.0,
      0.9
    },
    { PAR_RES1_DAMP,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Damp",
      "",
      0.0,
      1.0,
      0.5
    },

    { PAR_RES1_ROUGH,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Rough",
      "",
      0.0,
      1.0,
      0.0
    },

    { PAR_RES1_OCT,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_STEPPED
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Oct",
      "",
     -4,
      4,
      0
    },
    { PAR_RES1_SEMI,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_STEPPED
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Semi",
      "",
     -12,
      12,
      0
    },
    { PAR_RES1_CENT,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Cent",
      "",
     -1.0,
      1.0,
      0.0
    },

    //---------- res2 ----------

    // inputs

    { PAR_RES2_IN_O1,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "O1",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_RES2_IN_O2,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "O2",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_RES2_IN_R1,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "R1",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_RES2_IN_R2,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "R2",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_RES2_IN_N,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "N",
      "",
      0.0,
      1.0,
      1.0
    },

    //

    { PAR_RES2_TYPE,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_STEPPED,
      nullptr,
      "Type",
      "",
      0.0,
      2.0,
      0.0
    },
    { PAR_RES2_SHAPE,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Shape",
      "",
      0.0,
      1.0,
      0.0
    },
    { PAR_RES2_FB,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "FB",
      "",
      0.0,
      1.0,
      0.9
    },
    { PAR_RES2_DAMP,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Damp",
      "",
      0.0,
      1.0,
      0.5
    },

    { PAR_RES2_ROUGH,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Rough",
      "",
      0.0,
      1.0,
      0.0
    },

    { PAR_RES2_OCT,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_STEPPED
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Oct",
      "",
     -4,
      4,
      0
    },
    { PAR_RES2_SEMI,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_STEPPED
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,

      nullptr,
      "Semi",
      "",
     -12,
      12,
      0
    },
    { PAR_RES2_CENT,
      CLAP_PARAM_IS_AUTOMATABLE
        | CLAP_PARAM_IS_MODULATABLE
        | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Cent",
      "",
     -1.0,
      1.0,
      0.0
    },

    //---------- flt1 ----------

    { PAR_FLT1_TYPE,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Type",
      "",
      0.0,
      4.0,
      1.0
    },

    { PAR_FLT1_FREQ,
      CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Freq",
      "",
      0.0,
      1.0,
      1.0
    },

    { PAR_FLT1_RES,
      CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
      nullptr,
      "Res",
      "",
      0.0,
      1.0,
      0.0
    },

    //---------- env1 ----------

    { PAR_ENV1_ATT,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Att",
      "",
      0.0,
      1.0,
      0.05
    },

    { PAR_ENV1_DEC,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Dec",
      "",
      0.0,
      1.0,
      0.0
    },

    { PAR_ENV1_SUS,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Sus",
      "",
      0.0,
      1.0,
      1.0
    },

    { PAR_ENV1_REL,
      CLAP_PARAM_IS_AUTOMATABLE,
      nullptr,
      "Rel",
      "",
      0.0,
      1.0,
      0.5
    }

  };

  //----------

  #define NUM_AUDIO_OUTPUTS 1

  clap_audio_port_info_t myAudioOutputs[NUM_AUDIO_OUTPUTS] = {
    { 0,
      "Audio Out",
      CLAP_AUDIO_PORT_IS_MAIN,
      2,
      CLAP_PORT_STEREO,
      CLAP_INVALID_ID
    }
  };

  //----------

  #define NUM_NOTE_INPUTS 1

  clap_note_port_info_t myNoteInputs[NUM_NOTE_INPUTS] = {
    { 0,
      CLAP_NOTE_DIALECT_CLAP,
      CLAP_NOTE_DIALECT_CLAP,
      "Note In"
    }
  };

//  #define NUM_NOTE_OUTPUTS 1
//
//  clap_note_port_info_t myNoteOutputs[NUM_NOTE_OUTPUTS] = {
//    { 0,
//      CLAP_NOTE_DIALECT_CLAP,
//      CLAP_NOTE_DIALECT_CLAP,
//      "Note Out"
//    }
//  };

  //----------

  sa_ts1_VoiceManager MVoiceManager = {};

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  //: MIP_Plugin<myProcessor>(ADescriptor,AHost) {
  : MIP_Plugin(ADescriptor,AHost) {
    MIP_Print("Hello world!\n");
  }

//------------------------------
public: // clap
//------------------------------

  bool init() final {
    //clap_version_t ver = MHost->host->clap_version;
    //MIP_Print("host name: %s\n",MHost->host->name);
    //MIP_Print("host version: %s\n",MHost->host->version);
    //MIP_Print("host clap version: %i.%i.%i\n",ver.major,ver.minor,ver.revision);
    //MHost->printSupportedExtensions();

    setupParameters(myParameters,NUM_PARAMS);

//    appendParameter( new MIP_Parameter() );

    setupAudioOutputs(myAudioOutputs,NUM_AUDIO_OUTPUTS);
    setupNoteInputs(myNoteInputs,NUM_NOTE_INPUTS);
//    setupNoteOutputs(myNoteOutputs,NUM_NOTE_OUTPUTS);
    return MIP_Plugin::init();
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MVoiceManager.prepareVoices(sample_rate,&MParameters);
    // send initial parameter values to the voices
    for (uint32_t i=0; i<NUM_PARAMS; i++) {
      //float v = MParameterValues[i];
      float v = MParameters[i]->getValue();
      MVoiceManager.setParameter(i,v);
    }
    return MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  //TODO: make more automatic..
  // uint32_t MSupportedExtensions in MIP_Plugin ?
  // set bits in constructor, cgeck and return ptr in MIP_Plugin::getextension..

  const void* get_extension(const char *id) final {
    //MIP_Print("host asks for: %s\n",id);
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0) return &MAudioPorts;
    if (strcmp(id,CLAP_EXT_GUI) == 0)         return &MGui;
    if (strcmp(id,CLAP_EXT_NOTE_PORTS) == 0)  return &MNotePorts;
    if (strcmp(id,CLAP_EXT_THREAD_POOL) == 0) return &MThreadPool;
    if (strcmp(id,CLAP_EXT_VOICE_INFO) == 0)  return &MVoiceInfo;
    return MIP_Plugin::get_extension(id);
  }

  //----------

  clap_process_status process(const clap_process_t *process) final {
    flushAudioParams();

    //handle_input_events(process->in_events,process->out_events);
    preProcessEvents(process->in_events,process->out_events);

    //handle_process(process);
    float** outputs = process->audio_outputs[0].data32;
    uint32_t length = process->frames_count;
    #ifdef MIP_VOICE_PREPARE_EVENTS
      MVoiceManager.processPrepared(process,MHost);
    #else
      MVoiceManager.processBlock(process);
    #endif
    float v = MParameters[PAR_MASTER_VOL]->getValue();  // vol
    float p = MParameters[PAR_MASTER_PAN]->getValue();  // pan
    float l = v * (1.0 - p);
    float r = v * (      p);
    MIP_ScaleStereoBuffer(outputs,l,r,length);

    //handle_output_events(process->in_events,process->out_events);
    postProcessEvents(process->in_events,process->out_events);
    flushHostParams(process->out_events);

    // hack!!!!!
    // update gui (state only) in process!
    if (MEditor && MIsEditorOpen) {
      uint32_t num_playing = 0;
      uint32_t num_released = 0;
      for (uint32_t i=0; i<NUM_VOICES; i++) {
        uint32_t state = MVoiceManager.getVoiceState(i);
        ((sa_ts1_Editor*)MEditor)->MVoiceWidget->voice_state[i] = state;
        if (state == MIP_VOICE_PLAYING) num_playing += 1;
        if (state == MIP_VOICE_RELEASED) num_released += 1;
      }
      ((sa_ts1_Editor*)MEditor)->MPlayingVoicesWidget->setValue(num_playing);
      ((sa_ts1_Editor*)MEditor)->MReleasedVoicesWidget->setValue(num_released);
      ((sa_ts1_Editor*)MEditor)->MTotalVoicesWidget->setValue(num_playing + num_released);
    }
    return CLAP_PROCESS_CONTINUE;
  }

  //----------
  // gui
  //----------

  bool gui_create(const char *api, bool is_floating) final {
    MEditor = new sa_ts1_Editor(this,this,EDITOR_WIDTH,EDITOR_HEIGHT,true,&myDescriptor);
    if (!MEditor) return false;
    return true;
  }

  //----------
  // voice-info
  //----------

  bool voice_info_get(clap_voice_info_t *info) final {
    info->voice_count     = NUM_VOICES;
    info->voice_capacity  = NUM_VOICES;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }

  //----------
  // thread-pool
  //----------

  void thread_pool_exec(uint32_t task_index) final {
    MVoiceManager.processVoiceThread(task_index);
  }

//------------------------------
protected:
//------------------------------

  /*
    parameter has changed via gui
    we need to tell the voices about it
  */

  //void handle_editor_parameter(uint32_t AIndex, float AValue) final {
  //  //MIP_Print("%i = %.3f\n",AIndex,AValue);
  //  MVoiceManager.setParameter(AIndex,AValue);
  //}

  void on_editor_listener_parameter(uint32_t AIndex, double AValue) override {
    MIP_Plugin::on_editor_listener_parameter(AIndex,AValue);
    MVoiceManager.setParameter(AIndex,AValue);
  }

};

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

void MIP_Register(MIP_Registry* ARegistry) {
  ARegistry->appendDescriptor(&myDescriptor);
}

//----------

MIP_ClapPlugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  switch (AIndex) {
    case 0: return new myPlugin(ADescriptor,AHost);
  }
  return nullptr;
}

