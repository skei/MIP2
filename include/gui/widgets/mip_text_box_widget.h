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

  float     MTextHeight = 15.0;
  uint32_t  MNumLines   = 0;
  uint32_t  MMaxLines   = 100;

//------------------------------
public:
//------------------------------

  MIP_TextBoxWidget(MIP_FRect ARect)
  : MIP_ScrollBoxWidget(ARect,true,false) {
    setName("MIP_TextBoxWidget");
    setHint("textbox");
    //MContent->layout.innerBorder = 0;
    MContent->setDrawBorder(true);
  }

  //----------

  //virtual ~MIP_TextBoxWidget() {
  //}

//------------------------------
public:
//------------------------------

  virtual void setMaxLines(uint32_t ANum) { MMaxLines = ANum; }
  virtual void setTextHeight(float ASize) { MTextHeight = ASize; }

//------------------------------
public:
//------------------------------

  virtual void appendLine(const char* AText, bool ARedraw=true) {
    if (MNumLines >= MMaxLines) { removeOldestLine(); }
    //while (MNumLines >= MMaxLines) { removeOldestLine(); }
    MIP_TextWidget* textwidget = new MIP_TextWidget( MIP_FRect(MTextHeight),AText);
    textwidget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    //textwidget->flags.autoSize = true;
    textwidget->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    MContent->appendWidget(textwidget);
    MNumLines += 1;
    MContent->alignWidgets();
    do_widget_redraw(MContent,MContent->getRect(),0);

  }

  //----------

  void removeOldestLine() {
    if (MNumLines > 0) {
      MIP_Widgets* content = MContent->getChildren();
      MIP_Widget* wdg = MContent->getChild(0);
      content->remove(0);
      delete wdg;
      MNumLines -= 1;
    }
  }

};

//----------------------------------------------------------------------
#endif


