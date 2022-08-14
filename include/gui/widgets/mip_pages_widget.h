#ifndef mip_pages_widget_included
#define mip_pages_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widget.h"
#include "gui/mip_paint_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_PagesWidget
: public MIP_Widget {

//------------------------------
protected:
//------------------------------

  uint32_t MActivePage = 0;

//------------------------------
public:
//------------------------------

  MIP_PagesWidget(MIP_DRect ARect)
  : MIP_Widget(ARect) {
    MName = "MIP_PagesWidget";

    //Flags.clipChildren = true;

  }

  //----------

  virtual ~MIP_PagesWidget() {
  }

//------------------------------
public:
//------------------------------

  int32_t appendPage(MIP_Widget* APage) {
    APage->Flags.active = false;
    APage->Flags.visible = false;
    return appendChildWidget(APage)->getWidgetIndex();
  }

  //----------

  void setPage(int32_t APage) {
    int num = MChildren.size();
    if (APage >= num) APage = num - 1;
    for (int32_t i=0; i<num; i++) {
      MIP_Widget* wdg = MChildren[i];
      if (i == APage) {
        wdg->Flags.active = true;
        wdg->Flags.visible = true;
      }
      else {
        wdg->Flags.active = false;
        wdg->Flags.visible = false;
      }
    }
    MActivePage = APage;
  }

};

//----------------------------------------------------------------------
#endif


