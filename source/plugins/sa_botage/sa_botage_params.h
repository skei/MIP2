#ifndef sa_botage_params_included
#define sa_botage_params_included
//----------------------------------------------------------------------

#include "mip.h"

enum sa_botage_params_e {
  PAR_NUM_BEATS = 0,
  PAR_NUM_SLICES,
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
  }

};

//----------------------------------------------------------------------
#endif
