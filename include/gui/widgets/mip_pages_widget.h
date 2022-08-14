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

//  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) override {}
//  void do_widget_redraw(MIP_Widget* ASender, uint32_t AMode=0) override {}
//  void do_widget_modal(MIP_Widget* ASender, uint32_t AMode=0) override {}
//  void do_widget_cursor(MIP_Widget* ASender, uint32_t ACursor) override {}
//  void do_widget_hint(MIP_Widget* ASender, const char* AHint) override {}
//  void do_widget_notify(MIP_Widget* ASender, uint32_t AMode, int32_t AValue) override {}

};

//----------------------------------------------------------------------
#endif


