#ifndef mip_knob_widget_included
#define mip_knob_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets2/mip_value_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


class MIP_KnobWidget
: public MIP_ValueWidget {

//------------------------------
private:
//------------------------------


//------------------------------
public:
//------------------------------

  MIP_KnobWidget(MIP_FRect ARect)
  : MIP_ValueWidget(ARect) {
  }

  //----------

  virtual ~MIP_KnobWidget() {
  }

};

//----------------------------------------------------------------------
#endif

