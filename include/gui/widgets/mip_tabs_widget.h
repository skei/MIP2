#ifndef mip_tabs_widget_included
#define mip_tabs_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widget.h"
#include "gui/widgets/mip_button_row_widget.h"
#include "gui/widgets/mip_pages_widget.h"
#include "gui/mip_paint_context.h"

#define MAX_PAGES 32

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_TabsWidget
: public MIP_Widget {

//------------------------------
protected:
//------------------------------

    MIP_ButtonRowWidget*  MHeader;
    MIP_PagesWidget*      MPages;
    int32_t               MNumPages;

//------------------------------
public:
//------------------------------

  MIP_TabsWidget(MIP_DRect ARect, uint32_t ANum=MAX_PAGES)
  : MIP_Widget(ARect) {
    MName = "MIP_TabsWidget";
    //Layout.spacing = 5;
    MHeader = new MIP_ButtonRowWidget(MIP_DRect(20),ANum);
    MHeader->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    MHeader->setDrawRoundedBottom(false);
    MPages = new MIP_PagesWidget(MIP_DRect());
    MPages->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    MNumPages = 0;
    MHeader->setWidgetWidth(0);
    appendChildWidget(MHeader);
    appendChildWidget(MPages);


  }

  //----------

  virtual ~MIP_TabsWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual MIP_Widget* getHeader(void)    { return MHeader; }
  virtual MIP_Widget* getPages(void)     { return MPages; }
  virtual int32_t     getNumPages(void)  { return MNumPages; }

  //----------

  virtual int32_t appendPage(const char* ATitle, MIP_Widget* AWidget) {
    AWidget->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
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
public: // parent to child
//------------------------------

//  void on_widget_move(double AXpos, double AYpos)  override {}
//  void on_widget_resize(double AWidth, double AHeight) override {}
//  void on_widget_align(bool ARecursive=true) override {}
//  void on_widget_paint(MIP_PaintContext* AContext) override { paintChildWidgets(AContext); }
//  void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) override {}
//  void on_widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) override {}
//  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_enter(MIP_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_leave(MIP_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) override {}
//  //void on_widget_connect(MIP_Parameter* AParameter) override {}

//  virtual void on_widget_modal(bool AModal) {}


//------------------------------
public: // child to parent
//------------------------------

  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) override {
    if (ASender == MHeader) {
      int32_t sel = MHeader->getSelected();
      MPages->setPage(sel/*,true*/);
//      MPages->on_realign();
      MPages->alignChildWidgets();
      do_widget_redraw(MPages,0);
    }
    MIP_Widget::do_widget_update(ASender);
    MIP_Widget::do_widget_redraw(MPages,0);
  }

//  void do_widget_redraw(MIP_Widget* ASender, uint32_t AMode=0) override {}
//  void do_widget_modal(MIP_Widget* ASender, uint32_t AMode=0) override {}
//  void do_widget_cursor(MIP_Widget* ASender, uint32_t ACursor) override {}
//  void do_widget_hint(MIP_Widget* ASender, const char* AHint) override {}
//  void do_widget_notify(MIP_Widget* ASender, uint32_t AMode, int32_t AValue) override {}

};

//----------------------------------------------------------------------
#endif



