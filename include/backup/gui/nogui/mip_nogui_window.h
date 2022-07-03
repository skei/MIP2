#ifndef mip_nogui_window_included
#define mip_nogui_window_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widget.h"

//----------------------------------------------------------------------

class MIP_BaseWindow
: public MIP_Widget {

//------------------------------
public:
//------------------------------

  MIP_BaseWindow(int32_t AWidth, int32_t AHeight, void* AParentPtr)
  : MIP_Widget(MIP_FRect(AWidth,AHeight)) {
  }

  //----------

  virtual ~MIP_BaseWindow() {
  }

//------------------------------
public: // MIP_Widget
//------------------------------

  void do_widget_update(MIP_Widget* AWidget) override {
    MIP_PRINT;
  }

  //----------

  void do_widget_redraw(MIP_Widget* AWidget) override {
    MIP_PRINT;
    //TODO: widget->on_paint_..
  }

};

//----------------------------------------------------------------------
#endif
