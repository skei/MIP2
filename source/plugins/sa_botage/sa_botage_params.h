#ifndef sa_botage_params_included
#define sa_botage_params_included
//----------------------------------------------------------------------

#include "mip.h"

enum sa_botage_params_e {
  PAR_NUM_BEATS = 0,
  PAR_NUM_SLICES,
  //PAR_TEST,

  PAR_RANGE_PROB,
  PAR_RANGE_SLICE_COUNT,
  PAR_RANGE_LOOP_COUNT,

  PAR_LOOP_ENV_ATTACK,
  PAR_LOOP_ENV_DECAY,
  PAR_SLICE_ENV_ATTACK,
  PAR_SLICE_ENV_DECAY,

  PAR_FX_FILTER_PROB,
  PAR_FX_FILTER_TYPE,
  PAR_FX_FILTER_FREQ,
  PAR_FX_FILTER_BW,

  PAR_PROB_SIZE_PROB_RANGE,
  PAR_PROB_SIZE_MIN_RANGE,
  PAR_PROB_SIZE_MAX_RANGE,
  PAR_PROB_SIZE_PROB_LOOP,
  PAR_PROB_SIZE_MIN_LOOP,
  PAR_PROB_SIZE_MAX_LOOP,

  PAR_PROB_SPEED_PROB_RANGE,
  PAR_PROB_SPEED_MIN_RANGE,
  PAR_PROB_SPEED_MAX_RANGE,
  PAR_PROB_SPEED_PROB_LOOP,
  PAR_PROB_SPEED_MIN_LOOP,
  PAR_PROB_SPEED_MAX_LOOP,

  PAR_PROB_OFFSET_PROB_RANGE,
  PAR_PROB_OFFSET_MIN_RANGE,
  PAR_PROB_OFFSET_MAX_RANGE,
  PAR_PROB_OFFSET_PROB_LOOP,
  PAR_PROB_OFFSET_MIN_LOOP,
  PAR_PROB_OFFSET_MAX_LOOP,

  PAR_PROB_REVERSE_PROB_RANGE,
  PAR_PROB_REVERSE_PROB_LOOP,

  PAR_PROB_FX_PROB_RANGE,
  PAR_PROB_FX_MIN_RANGE,
  PAR_PROB_FX_MAX_RANGE,
  PAR_PROB_FX_PROB_LOOP,
  PAR_PROB_FX_MIN_LOOP,
  PAR_PROB_FX_MAX_LOOP,

  SA_BOTAGE_PARAM_COUNT
};

//----------------------------------------------------------------------

const clap_param_info_t sa_botage_params[SA_BOTAGE_PARAM_COUNT] = {

  { PAR_NUM_BEATS,
    CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
    nullptr,
    "Beats",
    "",
    1,
    8,
    4
  },

  { PAR_NUM_SLICES,
    CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
    nullptr,
    "Slices",
    "",
    1,
    8,
    2
  },

  // ----- slice

  { PAR_RANGE_PROB,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Range Prob",
    "",
    0,
    1,
    0.25
  },
  { PAR_RANGE_SLICE_COUNT,
    CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
    nullptr,
    "Range Slice Count",
    "",
    0,
    255,
    15
  },
  { PAR_RANGE_LOOP_COUNT,
    CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
    nullptr,
    "Range Loop Count",
    "",
    0,
    255,
    15
  },

  // ----- env

  { PAR_LOOP_ENV_ATTACK,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Loop Env Attack",
    "",
    0,
    1,
    0
  },
  { PAR_LOOP_ENV_DECAY,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Loop Env Decay",
    "",
    0,
    1,
    0
  },
  { PAR_SLICE_ENV_ATTACK,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Slice Env Attack",
    "",
    0,
    1,
    0
  },
  { PAR_SLICE_ENV_DECAY,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Slice Env Decay",
    "",
    0,
    1,
    0
  },

  // ----- fx filter

  { PAR_FX_FILTER_PROB,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "FX Filter Prob",
    "",
    0,
    1,
    0
  },
  { PAR_FX_FILTER_TYPE,
    CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
    nullptr,
    "FX FIlter Type",
    "",
    0,
    2,
    1
  },
  { PAR_FX_FILTER_FREQ,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "FX Filter Frequency",
    "",
    0,
    1,
    0.5
  },
  { PAR_FX_FILTER_BW,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "FX Filter BandWidth",
    "",
    0,
    1,
    0.5
  },

  // ----- loop size

  { PAR_PROB_SIZE_PROB_RANGE,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Size Prob Range",
    "",
    0,
    1,
    0.25
  },
  { PAR_PROB_SIZE_MIN_RANGE,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Size Min Range",
    "",
    0.5,
    2.0,
    0.75
  },
  { PAR_PROB_SIZE_MAX_RANGE,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Size Max Range",
    "",
    0.5,
    2.0,
    1.5
  },
  { PAR_PROB_SIZE_PROB_LOOP,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Size Prob Loop",
    "",
    0,
    1,
    0.25
  },
  { PAR_PROB_SIZE_MIN_LOOP,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Size Min Loop",
    "",
    0.5,
    2.0,
    0.75
  },
  { PAR_PROB_SIZE_MAX_LOOP,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Size Max Loop",
    "",
    0.5,
    2.0,
    1.5
  },

  // ----- loop speed

  { PAR_PROB_SPEED_PROB_RANGE,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Speed Prob Range",
    "",
    0,
    1,
    0.25
  },
  { PAR_PROB_SPEED_MIN_RANGE,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Speed Min Range",
    "",
    0.5,
    2.0,
    0.75
  },
  { PAR_PROB_SPEED_MAX_RANGE,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Speed Max Range",
    "",
    0.5,
    2.0,
    1.5
  },
  { PAR_PROB_SPEED_PROB_LOOP,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Speed Prob Loop",
    "",
    0,
    1,
    0.25
  },
  { PAR_PROB_SPEED_MIN_LOOP,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Speed Min Loop",
    "",
    0.5,
    2.0,
    0.75
  },
  { PAR_PROB_SPEED_MAX_LOOP,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Speed Max Loop",
    "",
    0.5,
    2.0,
    1.5
  },

  // ----- loop offset

  { PAR_PROB_OFFSET_PROB_RANGE,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Offset Prob Range",
    "",
    0,
    1,
    0.25
  },
  { PAR_PROB_OFFSET_MIN_RANGE,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Offset Min Range",
    "",
    0.5,
    2.0,
    0.75
  },
  { PAR_PROB_OFFSET_MAX_RANGE,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Offset Max Range",
    "",
    0.5,
    2.0,
    1.5
  },
  { PAR_PROB_OFFSET_PROB_LOOP,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Offset Prob Loop",
    "",
    0,
    1,
    0.25
  },
  { PAR_PROB_OFFSET_MIN_LOOP,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Offset Min Loop",
    "",
    0.5,
    2.0,
    0.75
  },
  { PAR_PROB_OFFSET_MAX_LOOP,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Offset Max Loop",
    "",
    0.5,
    2.0,
    1.5
  },

  // ----- loop reverse

  { PAR_PROB_REVERSE_PROB_RANGE,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Reverse Prob Range",
    "",
    0,
    1,
    0.25
  },
  { PAR_PROB_REVERSE_PROB_LOOP,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Reverse Prob Loop",
    "",
    0,
    1,
    0.25
  },

  // ----- loop fx

  { PAR_PROB_FX_PROB_RANGE,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "FX Prob Range",
    "",
    0,
    1,
    0.25
  },
  { PAR_PROB_FX_MIN_RANGE,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "FX Min Range",
    "",
    0.5,
    2.0,
    0.75
  },
  { PAR_PROB_FX_MAX_RANGE,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "FX Max Range",
    "",
    0.5,
    2.0,
    1.5
  },
  { PAR_PROB_FX_PROB_LOOP,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "FX Prob Loop",
    "",
    0,
    1,
    0.25
  },
  { PAR_PROB_FX_MIN_LOOP,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "FX Min Loop",
    "",
    0.5,
    2.0,
    0.75
  },
  { PAR_PROB_FX_MAX_LOOP,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "FX Max Loop",
    "",
    0.5,
    2.0,
    1.5
  },

};

//----------------------------------------------------------------------
#endif
