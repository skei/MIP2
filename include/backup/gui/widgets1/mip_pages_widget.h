#ifndef mip_pages_widget_included
#define mip_pages_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widget.h"

class MIP_PagesWidget
: public MIP_Widget {

//------------------------------
private:
//------------------------------

  uint32_t MActivePage = 0;


//------------------------------
public:
//------------------------------

  MIP_PagesWidget(MIP_FRect ARect)
  : MIP_Widget(ARect) {
    setName("MIP_PagesWidget");
    setHint("pages");
  }

  virtual ~MIP_PagesWidget() {
  }

//------------------------------
public:
//------------------------------

  int32_t appendPage(MIP_Widget* APage) {
    APage->state.active = false;
    APage->state.visible = false;
    return appendWidget(APage)->getWidgetIndex();
  }

  //----------

  void setPage(int32_t APage) {
    int num = getNumChildren();
    if (APage >= num) APage = num - 1;
    for (int32_t i=0; i<num; i++) {
      MIP_Widget* wdg = getChild(i);
      if (i == APage) {
        wdg->state.active = true;
        wdg->state.visible = true;
      }
      else {
        wdg->state.active = false;
        wdg->state.visible = false;
      }
    }
    MActivePage = APage;
  }

};

//----------------------------------------------------------------------
#endif
