#ifndef mip_text_box_widget_included
#define mip_text_box_widget_included
//----------------------------------------------------------------------

// untested

#include "gui/widgets/mip_widgets.h"

//----------------------------------------------------------------------

class MIP_TextBoxWidget
: public MIP_ScrollBoxWidget {

//------------------------------
protected:
//------------------------------

  float     MTextHeight = 15.0;
  float     MTextSize   = 13.0;
  uint32_t  MNumLines   = 0;
  uint32_t  MMaxLines   = 100;

//------------------------------
public:
//------------------------------

  MIP_TextBoxWidget(MIP_DRect ARect)
  : MIP_ScrollBoxWidget(ARect,true,true) {
    MName = "MIP_TextBoxWidget";
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
    MIP_TextWidget* textwidget = new MIP_TextWidget( MIP_DRect(MTextHeight),AText);

    /*
      this means we can't scroll..
      todo: some kind of autosize..
      keep track of the widest line (appendLine
    */

    textwidget->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    textwidget->setFillBackground(false);
    textwidget->setDrawBorder(false);
    textwidget->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    textwidget->setTextColor(MIP_COLOR_BLACK);
    textwidget->setTextSize(MTextSize);
    //textwidget->flags.autoSize = true;
    MContent->appendChildWidget(textwidget);
    MNumLines += 1;
    MContent->alignChildWidgets();
    do_widget_redraw(MContent,0);

  }

  //----------

  void removeOldestLine() {
    if (MNumLines > 0) {
      MIP_WidgetArray* content = MContent->getChildWidgets();
      MIP_Widget* wdg = MContent->getChildWidget(0);
      content->remove(0);
      delete wdg;
      MNumLines -= 1;
    }
  }

};

//----------------------------------------------------------------------
#endif


