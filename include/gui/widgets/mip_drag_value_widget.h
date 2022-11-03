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

  bool      MIsDragging       = false;
  double    MDragSensitivity  = 0.005;
  double    MDragSensitivity2 = 0.05;

  double    MDragValue        = 0.0;
  double    MMousePrevX       = 0.0;
  double    MMousePrevY       = 0.0;

  //double    MClickedX         = 0.0;
  //double    MClickedY         = 0.0;
  //double    MClickedValue     = 0.0;

  bool        MSnap           = false;
  float       MSnapPos        = 0.5;
  float       MSnapDist       = 0.1;
  //uint32_t    MSnapMode       = 1;        // 0: always snap, 1: shift disables snapping

  bool        MQuantize       = false;
  bool        MBipolar        = false;

//------------------------------
public:
//------------------------------

  MIP_DragValueWidget(MIP_DRect ARect, const char* AText, double AValue=0.0)
  : MIP_ValueWidget(ARect,AText,AValue) {
    MName = "MIP_DragValueWidget";
    MMouseCursor = MIP_CURSOR_ARROW_UP_DOWN;
    Options.autoHideCursor = true;
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

  virtual void setSnap(bool ASnap=true)             { MSnap = ASnap; }
  virtual void setSnapPos(float APos)               { MSnapPos = APos; }
  virtual void setSnapDist(float ADist)             { MSnapDist = ADist; }

  virtual void setQuantize(bool AQuantize=true)     { MQuantize = AQuantize; }
  virtual void setBipolar(bool ABipolar=true)       { MBipolar = ABipolar; }

  virtual void scaleDragSensitivity(double AScale)  { MDragSensitivity *= AScale; }
  virtual void scaleDragSensitivity2(double AScale) { MDragSensitivity2 *= AScale; }

//------------------------------
public:
//------------------------------

  virtual float snapValue(float AValue) {
    //float minval = getMinValue();
    float maxval = getMaxValue();
    float v = AValue;
    float s = 1.0f; // scale factor
    float dist = fabs( AValue - MSnapPos );
    if (dist < MSnapDist) {
      v = MSnapPos;
    }
    else {
      // scale left
      if (AValue < MSnapPos) {
        float sp_sd = MSnapPos - MSnapDist;
        if (sp_sd > 0) s = MSnapPos / sp_sd;
        v = AValue * s;
      }
      // scale right
      else if (AValue > MSnapPos) {
        float iv = maxval - AValue; // 1.0f - AValue;
        float isp = (maxval - MSnapPos); // (1.0f - MSnapPos);
        float isp_sd = isp - MSnapDist;
        if (isp_sd > 0) s = isp / isp_sd;
        v = iv * s;
        v = maxval - v; // 1.0f - v;
      }
    }
    //v = MIP_Clamp(v,minval,maxval); // (v,0.0f,1.0f);
    //MIP_Print("%.3f -> %.3f\n",AValue,v);
    return v;
  }

//------------------------------
public: // parent to child
//------------------------------

  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    //MIP_PRINT;
    switch (AButton) {
      case MIP_BUTTON_LEFT: {
        if (Options.autoHideCursor) do_widget_cursor(this,MIP_CURSOR_HIDE);
        if (Options.autoLockCursor) do_widget_cursor(this,MIP_CURSOR_LOCK);
        MMousePrevX = AXpos;
        MMousePrevY = AYpos;
        MDragValue  = getValue();
        MIsDragging = true;
        break;
      }
      case MIP_BUTTON_RIGHT: {
        if (AState & MIP_KEY_ALT) {
          float v = getDefaultValue();
          setValue(v);
          do_widget_update(this);
          do_widget_redraw(this);
        }
        break;
      }
    } // switch
  }

  //----------

  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    //MIP_PRINT;
    switch (AButton) {
      case MIP_BUTTON_LEFT: {
        if (Options.autoHideCursor) do_widget_cursor(this,MIP_CURSOR_SHOW);
        if (Options.autoLockCursor) do_widget_cursor(this,MIP_CURSOR_UNLOCK);
        MIsDragging = false;
        break;
      }
    }
  }

  //----------

  void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    if (MIsDragging) {

      //double xdiff = AXpos - MMousePrevX;
      double ydiff = MMousePrevY - AYpos;
      MMousePrevX = AXpos;
      MMousePrevY = AYpos;

      double sens = MDragSensitivity * (getMaxValue() - getMinValue());
      if (AState & MIP_KEY_SHIFT) sens *= MDragSensitivity2;
      MDragValue += (ydiff * sens);
      MDragValue = MIP_Clamp(MDragValue,getMinValue(),getMaxValue());

      float v = MDragValue;
      if (MSnap && !(AState & MIP_KEY_SHIFT)) v = snapValue(v);
      v = MIP_Clamp(v,getMinValue(),getMaxValue());
      setValue(v);

      do_widget_update(this);
      do_widget_redraw(this);
    }
  }



};

//----------------------------------------------------------------------
#endif



