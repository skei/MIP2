#ifndef mip_widget_included
#define mip_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_rect.h"

//----------------------------------------------------------------------

//#include <vector>
class MIP_Widget;
typedef std::vector<MIP_Widget*> MIP_Widgets;

//----------------------------------------------------------------------

class MIP_Widget {

//------------------------------
protected:
//------------------------------

  MIP_Widget* MParent         = nullptr;
  MIP_Widgets MChildren       = {};

  MIP_FRect   MRect           = {};
  int32_t     MParameterIndex = -1;
  float       MValue          = 0.0;

//------------------------------
public:
//------------------------------

  MIP_Widget(MIP_FRect ARect) {
    MRect = ARect;
  }

  //----------

  virtual ~MIP_Widget() {
  }

//------------------------------
public:
//------------------------------

  float   getValue()                        { return 0.0; }
  void    setValue()                        { MValue = 0.0; }

  int32_t getParameterIndex()               { return -1; }
  void    setParameterIndex(int32_t AIndex) { MParameterIndex = AIndex; }


//------------------------------
public:
//------------------------------

  virtual void updateValue(float AValue) {}
  virtual void redraw() {}

//------------------------------
public:
//------------------------------

  /*
    these 'events' tricke down the widget hierarchy,
    from the parent to the childdren..
  */

  virtual void on_widget_move() {}
  virtual void on_widget_resize() {}
  virtual void on_widget_paint() {}
  virtual void on_widget_mouseClick() {}
  virtual void on_widget_mouseMove() {}
  virtual void on_widget_mouseRelease() {}
  virtual void on_widget_keyPress() {}
  virtual void on_widget_keyRelease() {}
  virtual void on_widget_connect(int32_t AParameterIndex) {}
  //virtual void on_widget_timer() {}

//------------------------------
public:
//------------------------------

  /*
    these 'events' bubble upwards from a child widget to its parent,
    and upwards.. until it reaches either a widget that responds to it,
    or it reaches the window (which is-a widget)..
  */

  //------------------------------

  // widget value has updated..

  virtual void do_widget_update(MIP_Widget* AWidget) {
    if (MParent) MParent->do_widget_update(AWidget);
  }

  // widget needs to be redrawn

  virtual void do_widget_redraw(MIP_Widget* AWidget) {
    if (MParent) MParent->do_widget_redraw(AWidget);
  }

  // widget has changed position or size..
  // parent might need to realign its ch8ld widgets

  virtual void do_widget_realign(MIP_Widget* AWidget) {
    if (MParent) MParent->do_widget_realign(AWidget);
  }

  //

  virtual void do_widget_setMouseCursor(MIP_Widget* AWidget, int32_t ACursor) {
    if (MParent) MParent->do_widget_setMouseCursor(AWidget,ACursor);
  }

  //

  virtual void do_widget_setHint(MIP_Widget* AWidget, const char* AHint) {
    if (MParent) MParent->do_widget_setHint(AWidget,AHint);
  }

};

//----------------------------------------------------------------------
#endif
