#ifndef mip_slider_widget_included
#define mip_slider_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets2/mip_value_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


class MIP_SliderWidget
: public MIP_ValueWidget {

//------------------------------
private:
//------------------------------


//------------------------------
public:
//------------------------------

  MIP_SliderWidget(MIP_FRect ARect)
  : MIP_ValueWidget(ARect) {
  }

  //----------

  virtual ~MIP_SliderWidget() {
  }

};

//----------------------------------------------------------------------
#endif

