#ifndef mip_menu_widget_included
#define mip_menu_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets/mip_panel_widget.h"
#include "gui/widgets/mip_menu_item_widget.h"
#include "gui/mip_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_MenuListener {
public:
  virtual void on_menu_selected(int32_t AIndex) {}
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_MenuWidget
: public MIP_PanelWidget {

//------------------------------
protected:
//------------------------------

  MIP_MenuListener* MListener = nullptr;

//------------------------------
public:
//------------------------------

  MIP_MenuWidget(MIP_DRect ARect, MIP_MenuListener* AListener/*=nullptr*/)
  : MIP_PanelWidget(ARect) {
    MName = "MIP_MenuWidget";
    MListener = AListener;
    State.active = false;
    State.visible = false;
  }

  //----------

  virtual ~MIP_MenuWidget() {
  }

//------------------------------
public: // parent to child
//------------------------------

  virtual void setListener(MIP_MenuListener* AListener) {
    MListener = AListener;
  }

//------------------------------
public: // parent to child
//------------------------------

  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    if (!MRect.contains(AXpos,AYpos)) {
      if (MListener) MListener->on_menu_selected(-1);
      close(true);
    }
    else {
      MIP_PanelWidget::on_widget_mouse_click(AButton,AState,AXpos,AYpos,ATime);
    }
  }

  //----------

  void on_widget_unmodal() override {
    //MIP_Print("unmodal\n");
    close(true);
  }

//------------------------------
public: // child to parent
//------------------------------

  void do_widget_notify(MIP_Widget* ASender, uint32_t AMode, int32_t AValue) override {
    //if (MParent) MParent->do_widget_update(ASender,AMode);
    int32_t index = AValue;//ASender->getWidgetIndex();
    if (index < 0) {
      if (MListener) MListener->on_menu_selected(-1);
    }
    else {
      if (MListener) MListener->on_menu_selected(index);
    }
    close(true);
    //do_widget_modal(nullptr);
  }



//------------------------------
public:
//------------------------------

  virtual void open(double AXpos, double AYpos, bool AModal/*=true*/) {
    double x = AXpos;
    double y = AYpos;
    MIP_Window* window = (MIP_Window*)MOwnerWindow;
    if (window) {
      uint32_t winw = window->getWindowWidth();
      uint32_t winh = window->getWindowHeight();
      if ((x + MRect.w) > winw) { x = winw - MRect.w; }
      if ((y + MRect.h) > winh) { y = winh - MRect.h; }
    }
    setWidgetPos(x,y);
    alignChildWidgets();
    State.visible = true;
    State.active = true;
    do_widget_redraw(this);
    if (AModal) do_widget_modal(this);
  }

  //----------

  virtual void close(bool AModal/*=true*/) {
    State.visible = false;
    State.active = false;
    do_widget_redraw(this);
    if (AModal) do_widget_modal(nullptr);
  }

};

//----------------------------------------------------------------------
#endif



