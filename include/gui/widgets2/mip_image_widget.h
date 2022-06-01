#ifndef mip_image_widget_included
#define mip_image_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets2/mip_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


class MIP_ImageWidget
: public MIP_Widget {

//------------------------------
private:
//------------------------------


//------------------------------
public:
//------------------------------

  MIP_ImageWidget(MIP_FRect ARect)
  : MIP_Widget(ARect) {
  }

  //----------

  virtual ~MIP_ImageWidget() {
  }

};

//----------------------------------------------------------------------
#endif

