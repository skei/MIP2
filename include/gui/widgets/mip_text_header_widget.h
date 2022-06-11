#ifndef mip_text_header_widget_included
#define mip_text_header_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widget.h"
#include "gui/widgets/mip_panel_widget.h"

//----------------------------------------------------------------------

class MIP_TextHeaderWidget
: public MIP_TextWidget {

//------------------------------
private:
//------------------------------


//------------------------------
public:
//------------------------------

  MIP_TextHeaderWidget(MIP_FRect ARect, const char* AText="")
  : MIP_TextWidget(ARect,AText) {
    setName("MIP_TextWidget");
    setHint("text");
  }

  virtual ~MIP_TextHeaderWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setText(const char* AText) {
  }

  virtual void setTextAlignment(uint32_t AAlignment) {
  }

  virtual void setTextColor(MIP_Color AColor) {
  }

};

//----------------------------------------------------------------------
#endif
