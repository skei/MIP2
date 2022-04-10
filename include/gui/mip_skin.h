#ifndef mip_skin_included
#define mip_skin_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_painter.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class MIP_Skin {

//------------------------------
public:
//------------------------------

  struct color {
    MIP_Color   background = MIP_COLOR_GRAY;
    MIP_Color   border     = MIP_COLOR_DARK_GRAY;
    MIP_Color   text       = MIP_COLOR_DARK_GRAY;
    MIP_Color   highlight  = MIP_COLOR_DARK_GRAY;
  };

  float roundedRadius = 6.0;

//------------------------------
public:
//------------------------------

  MIP_Skin() {
  }

  //----------

  virtual ~MIP_Skin() {
  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------

MIP_Skin MIP_DEFAULT_SKIN = {};

//----------------------------------------------------------------------
#endif
