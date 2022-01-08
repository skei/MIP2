#ifndef kode_pages_widget_included
#define kode_pages_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"

class KODE_PagesWidget
: public KODE_Widget {

//------------------------------
private:
//------------------------------

  uint32_t MActivePage = 0;


//------------------------------
public:
//------------------------------

  KODE_PagesWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    setName("KODE_PagesWidget");
    setHint("pages");
  }

  virtual ~KODE_PagesWidget() {
  }

//------------------------------
public:
//------------------------------

  int32_t appendPage(KODE_Widget* APage) {
    APage->flags.active = false;
    APage->flags.visible = false;
    return appendWidget(APage)->getIndex();
  }

  //----------

  void setPage(int32_t APage) {
    int num = getNumChildren();
    if (APage >= num) APage = num - 1;
    for (int32_t i=0; i<num; i++) {
      KODE_Widget* wdg = getChild(i);
      if (i == APage) {
        wdg->flags.active = true;
        wdg->flags.visible = true;
      }
      else {
        wdg->flags.active = false;
        wdg->flags.visible = false;
      }
    }
    MActivePage = APage;
  }

};

//----------------------------------------------------------------------
#endif
