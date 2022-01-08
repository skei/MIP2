#ifndef mip_text_box_widget_included
#define mip_text_box_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widgets.h"

//----------------------------------------------------------------------

class MIP_TextBoxWidget
: public MIP_ScrollBoxWidget {

//------------------------------
protected:
//------------------------------

  uint32_t  MTextHeight = 12;

//------------------------------
public:
//------------------------------

  MIP_TextBoxWidget(MIP_FRect ARect)
  : MIP_ScrollBoxWidget(ARect) {
    setName("MIP_TextBoxWidget");
    setHint("textbox");
    //MContent->layout.innerBorder = 0;
  }

  //----------

  //virtual ~MIP_TextBoxWidget() {
  //}

//------------------------------
public:
//------------------------------

  virtual void appendText(const char* AText) {
    MIP_TextWidget* textwidget = new MIP_TextWidget( MIP_FRect(0,0,250,MTextHeight),AText);
    textwidget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP_LEFT;
    textwidget->flags.autoSize = true;
    textwidget->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    MContent->appendWidget(textwidget);
  }

};

//----------------------------------------------------------------------
#endif


