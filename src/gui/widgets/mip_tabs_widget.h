#ifndef kode_tabs_widget_includedbutton
#define kode_tabs_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_button_row_widget.h"
#include "gui/widgets/kode_pages_widget.h"

#define MAX_PAGES 32

class KODE_TabsWidget
: public KODE_Widget {

//------------------------------
protected:
//------------------------------

    KODE_ButtonRowWidget* MHeader;
    KODE_PagesWidget*     MPages;
    int32_t               MNumPages;

//------------------------------
public:
//------------------------------

  KODE_TabsWidget(KODE_FRect ARect, uint32_t ANum=MAX_PAGES)
  : KODE_Widget(ARect) {
    setName("KODE_TabsWidget");
    setHint("tabs");
    MHeader = new KODE_ButtonRowWidget(KODE_FRect(20,20),ANum);
    MHeader->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    MPages = new KODE_PagesWidget(KODE_FRect());
    MPages->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    MNumPages = 0;
    MHeader->setWidth(0);
    appendWidget(MHeader);
    appendWidget(MPages);
  }

  //----------

  virtual ~KODE_TabsWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual KODE_Widget*  getHeader(void)    { return MHeader; }
  virtual KODE_Widget*  getPages(void)     { return MPages; }
  virtual int32_t       getNumPages(void)  { return MNumPages; }

  //----------

  virtual int32_t appendPage(const char* ATitle, KODE_Widget* AWidget) {
    AWidget->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    int32_t result = MPages->appendPage(AWidget);
    MHeader->setButtonName(MNumPages,ATitle);
    MNumPages += 1;
    //MHeader->setWidth(MNumPages);
    MHeader->setNumButtons(MNumPages);
    return result;
  }

  //----------

  virtual void selectPage(int32_t AIndex/*, bool ARedraw=false*/) {
    if (AIndex < MNumPages) {
      MHeader->selectButton(AIndex/*,ARedraw*/);
      MPages->setPage(AIndex/*,ARedraw*/);
    }
  }

//------------------------------
public:
//------------------------------

  void do_widget_update(KODE_Widget* ASender) final {
    if (ASender == MHeader) {
      int32_t sel = MHeader->getSelected();
      MPages->setPage(sel/*,true*/);
//      MPages->on_realign();
      MPages->alignChildren();
      do_widget_redraw(MPages,MPages->getRect(),0);
    }
    KODE_Widget::do_widget_update(ASender);
    KODE_Widget::do_widget_redraw(MPages,MPages->getRect(),0);
  }

};

#undef MAX_PAGES


//----------------------------------------------------------------------
#endif
