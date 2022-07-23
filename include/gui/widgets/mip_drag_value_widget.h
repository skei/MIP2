#ifndef mip_drag_value_widget_included
#define mip_drag_value_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets/mip_value_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_DragValueWidget
: public MIP_ValueWidget {

//------------------------------
protected:
//------------------------------

  bool      MIsDragging           = false;
  double    MDragSensitivity      = 0.002;
  double    MDragSensitivity2     = 0.02;

  double    MDragValue            = 0.0;
  double    MMousePrevX           = 0.0;
  double    MMousePrevY           = 0.0;

  //double    MClickedX             = 0.0;
  //double    MClickedY             = 0.0;
  //double    MClickedValue         = 0.0;

//------------------------------
public:
//------------------------------

  MIP_DragValueWidget(MIP_DRect ARect, const char* AText, double AValue)
  : MIP_ValueWidget(ARect,AText,AValue) {
    MName = "MIP_DragValueWidget";
    MMouseCursor = MIP_CURSOR_ARROW_UP_DOWN;
    Options.autoHideCursor = false;//true;
    Options.autoLockCursor = true;
  }

  //----------

  virtual ~MIP_DragValueWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDragSensitivity(double ASen)      { MDragSensitivity = ASen; }
  virtual void setDragSensitivity2(double ASen)     { MDragSensitivity2 = ASen; }

  virtual void scaleDragSensitivity(double AScale)  { MDragSensitivity *= AScale; }
  virtual void scaleDragSensitivity2(double AScale) { MDragSensitivity2 *= AScale; }


//------------------------------
public: // parent to child
//------------------------------

  void on_widget_mouse_press(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    switch (AButton) {
      case MIP_BUTTON_LEFT: {
        if (Options.autoHideCursor) do_widget_cursor(this,MIP_CURSOR_HIDE);
        if (Options.autoLockCursor) do_widget_cursor(this,MIP_CURSOR_LOCK);
        MMousePrevX = AXpos;
        MMousePrevY = AYpos;
        MDragValue  = MValue;
        MIsDragging = true;
        break;
      }
    }
  }

  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    switch (AButton) {
      case MIP_BUTTON_LEFT: {
        if (Options.autoHideCursor) do_widget_cursor(this,MIP_CURSOR_SHOW);
        if (Options.autoLockCursor) do_widget_cursor(this,MIP_CURSOR_UNLOCK);
        MIsDragging = false;
        break;
      }
    }
  }

  void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    if (MIsDragging) {
      //double xdiff = AXpos - MMousePrevX;
      double ydiff = MMousePrevY - AYpos;
      MDragValue += (ydiff * MDragSensitivity);
      MMousePrevX = AXpos;
      MMousePrevY = AYpos;
      MDragValue = MIP_Clamp(MDragValue,MMinValue,MMaxValue);
      setValue(MDragValue);
      do_widget_update(this);
      do_widget_redraw(this);
    }
  }

};

//----------------------------------------------------------------------
#endif



