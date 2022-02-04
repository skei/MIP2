#ifndef mip_drag_value_widget_included
#define mip_drag_value_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/mip_value_widget.h"

#define MIP_VALUE_WIDGET_LABEL_SPACE 4.0f

//----------------------------------------------------------------------

class MIP_DragValueWidget
: public MIP_ValueWidget {

//------------------------------
private:
//------------------------------

  bool        MIsDragging     = false;
  //bool        MIsDragging2    = false;
  float       MPrevXpos       = 0.0f;
  float       MPrevYpos       = 0.0f;
  uint32_t    MPrevClickTime  = 0;

  bool        MSnap           = false;
  float       MSnapPos        = 0.5;
  float       MSnapDist       = 0.1;
  uint32_t    MSnapMode       = 1;        // 0: always snap, 1: shift disables snapping

  bool        MQuantize       = false;
  uint32_t    MQuantizeSteps  = 0;
  uint32_t    MQuantizeMode   = 1;

//------------------------------
protected:
//------------------------------

  // drag
  bool        MCanDragValue       = true;
  //bool        MCanDragValue2      = true;
  float       MDragSensitivity    = 0.004f;
  float       MDragSensitivity2   = 0.05;
  uint32_t    MDragDirection      = MIP_UP;
  float       MDragValue          = 0.0f;
  //float       MDragValue2         = 0.0f;

  // double-click
  bool        MDblClickReset      = true;

  //float MValue2 = 0.0;
  bool MPrevShift = false;

//------------------------------
public:
//------------------------------

  MIP_DragValueWidget(MIP_FRect ARect, const char* AText="", float AValue=0.0)
  : MIP_ValueWidget(ARect) {
    setName("MIP_DragValueWidget");
    setHint("dragvalue");
    MText = AText;
    MValue = AValue;
    flags.autoMouseLock   = true;
    flags.autoMouseHide   = true;
    flags.autoMouseRedraw = true;
  }

  //----------

  virtual ~MIP_DragValueWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void      setCanDragValue(bool ADrag=true)      { MCanDragValue = ADrag; }
  //virtual void      setCanDragValue2(bool ADrag=true)     { MCanDragValue2 = ADrag; }
  virtual void      setDragSensitivity(float AValue)      { MDragSensitivity = AValue; }
  //virtual void      setDragSensitivity2(float AValue)     { MDragSensitivity2 = AValue; }
  virtual void      setDragDirection(uint32_t ADirection) { MDragDirection = ADirection; }

  virtual void      setSnap(bool ASnap=true)              { MSnap = ASnap; }
  virtual void      setSnapPos(float APos)                { MSnapPos = APos; }
  virtual void      setSnapDist(float ADist)              { MSnapDist = ADist; }
  virtual void      setSnapMode(uint32_t AMode)           { MSnapMode = AMode; }

  virtual void      setQuantize(bool AQuantize=true)      { MQuantize = AQuantize; }
  virtual void      setQuantizeSteps(uint32_t ASteps)     { MQuantizeSteps = ASteps; }
  virtual void      setQuantizeMode(uint32_t AMode)       { MQuantizeMode = AMode; }

  //----------

  virtual bool      getSnap()                             { return MSnap; }
  virtual float     getSnapPos()                          { return MSnapPos; }
  virtual float     getSnapDist()                         { return MSnapDist; }
  virtual uint32_t  getSnapMode()                         { return MSnapMode; }

  virtual bool      getQuantize()                         { return MQuantize; }
  virtual uint32_t  getQuantizeSteps()                    { return MQuantizeSteps; }
  virtual uint32_t  getQuantizeMode()                     { return MQuantizeMode; }

  //----------

  virtual bool      isDragging()                          { return MIsDragging; }
  //virtual bool      isDragging2()                         { return MIsDragging2; }

  //----------

  //virtual void  setValue2(float AValue) { MValue2 = AValue; }
  //virtual float getValue2() { return MValue2; }

//------------------------------
protected:
//------------------------------

  virtual float snapValue(float AValue) {
    float v = AValue;
    float s = 1.0f;
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
        float iv = 1.0f - AValue;
        float isp = (1.0f - MSnapPos);
        float isp_sd = isp - MSnapDist;
        if (isp_sd > 0) s = isp / isp_sd;
        v = iv * s;
        v = 1.0f - v;
      }
    }
    v = MIP_Clamp(v,0.0f,1.0f);
    return v;
  }

  //----------

  float calcValue(float value, float deltax, float deltay, bool shift) {
    float sens = MDragSensitivity;
    if (shift) sens *= MDragSensitivity2;
    switch (MDragDirection) {
      case MIP_UP:     value += (sens * deltay);   break;
      case MIP_DOWN:   value -= (sens * deltay);   break;
      case MIP_LEFT:   value -= (sens * deltax);   break;
      case MIP_RIGHT:  value += (sens * deltax);   break;
    }
    MDragValue = MIP_Clamp(value,0.0f, 1.0f);
    if (MSnap && !shift) value = snapValue(value);
    if (MQuantize && !shift) value = MIP_Quantize(value,MQuantizeSteps);
    value = MIP_Clamp(value,0.0f, 1.0f);
    return value;
  }

  //----------

  float calcValueFromMouse(float AXpos, float AYpos, uint32_t AState) {
    float value = getValue();
    if (MCanDragValue) {
      //float sens = MDragSensitivity;
      //if (AState & MIP_KEY_SHIFT) sens *= MDragSensitivity2;
      if (MIsDragging) {
        float value = MDragValue;
        float deltax = AXpos - MPrevXpos; // right is increasing
        float deltay = MPrevYpos - AYpos; // up is increasing
        bool shift = (AState & MIP_KEY_SHIFT);
        //if (shift && !MPrevShift) {
        //  //MIP_Print("shift pressed\n");
        //  if (MQuantize) MDragValue = getValue();
        //}
        //if (!shift && MPrevShift) {
        //  //MIP_Print("shift released\n");
        //  if (MQuantize) MDragValue = getValue();
        //}
        //MPrevShift = shift;
        value = calcValue(value,deltax,deltay,shift/*,sens*/);
      }
      MPrevXpos = AXpos;
      MPrevYpos = AYpos;
    }
    return value;
  }

//------------------------------
public:
//------------------------------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {

    MPrevShift = (AState & MIP_KEY_SHIFT);
    MDragValue = getValue();

    if (AButton == MIP_BUTTON_LEFT) {
//      if (MDblClickReset) {
//        if ((ATimeStamp - MPrevClickTime) < MIP_GUI_DBLCLICK_MS) {
//          if (getParameter()) setValue( getParameter()->getDefValue() );
//          else setValue( getDefaultValue() );
//          update();
//          redraw();
//        }
//      } // dbl click
//      MPrevClickTime = ATimeStamp;

      if (MCanDragValue) {
        if (flags.autoMouseLock)    do_widget_setMouseCursor(this,MIP_CURSOR_GRAB);
        if (flags.autoMouseHide)    do_widget_setMouseCursor(this,MIP_CURSOR_HIDE);
        if (flags.autoMouseRedraw)  do_widget_redraw(this,getRect(),0);
        MPrevXpos     = AXpos;
        MPrevYpos     = AYpos;
        MIsDragging   = true;
      } // can drag

    } // left

//    else if (AButton == MIP_BUTTON_RIGHT) {
//      if (MCanDragValue2) {
//        if (flags.autoMouseLock)    do_widget_setMouseCursor(this,MIP_CURSOR_GRAB);
//        if (flags.autoMouseHide)    do_widget_setMouseCursor(this,MIP_CURSOR_HIDE);
//        if (flags.autoMouseRedraw)  do_widget_redraw(this,getRect(),0);
//        MPrevXpos     = AXpos;
//        MPrevYpos     = AYpos;
//        MIsDragging2   = true;
//      } // can drag 2
//    } // right
  }

  //----------

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
    if (MCanDragValue) {
      if (AButton == MIP_BUTTON_LEFT) {
        if (flags.autoMouseHide)    do_widget_setMouseCursor(this,MIP_CURSOR_SHOW);
        if (flags.autoMouseLock)    do_widget_setMouseCursor(this,MIP_CURSOR_RELEASE);
        if (flags.autoMouseRedraw)  do_widget_redraw(this,getRect(),0);
        MIsDragging = false;
        //setValue(MDragValue);
      }
    }

//    if (MCanDragValue2) {
//      if (AButton == MIP_BUTTON_RIGHT) {
//        if (flags.autoMouseHide)    do_widget_setMouseCursor(this,MIP_CURSOR_SHOW);
//        if (flags.autoMouseLock)    do_widget_setMouseCursor(this,MIP_CURSOR_RELEASE);
//        if (flags.autoMouseRedraw)  do_widget_redraw(this,getRect(),0);
//        MIsDragging2 = false;
//        //setValue(MDragValue);
//      }
//    }
  }

  //----------

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) override {

    if (MCanDragValue) {
      //float sens = MDragSensitivity;
      //if (AState & MIP_KEY_SHIFT) sens *= MDragSensitivity2;
      if (MIsDragging) {
        float value = MDragValue;
        float deltax = AXpos - MPrevXpos; // right is increasing
        float deltay = MPrevYpos - AYpos; // up is increasing
        bool shift = (AState & MIP_KEY_SHIFT);
        /*
        if (shift && !MPrevShift) {
          //MIP_Print("shift pressed\n");
          if (MQuantize) MDragValue = getValue();
        }
        if (!shift && MPrevShift) {
          //MIP_Print("shift released\n");
          if (MQuantize) MDragValue = getValue();
        }
        MPrevShift = shift;
        */
        value = calcValue(value,deltax,deltay,shift/*,sens*/);
        //value = calcValueFromMouse(AXpos,AYpos,AState);
        setValue(value);
        update();
        redraw();
      } // is dragging
      //if (MIsDragging2) {
      //  float value = MDragValue;
      //  float deltax = AXpos - MPrevXpos; // right is increasing
      //  float deltay = MPrevYpos - AYpos; // up is increasing
      //  value = calcValue(value,deltax,deltay,(AState & MIP_KEY_SHIFT)/*,sens*/);
      //  setValue2(value);
      //  update();
      //  redraw();
      //} // is dragging

      MPrevXpos = AXpos;
      MPrevYpos = AYpos;

    }

  }

  //----------

  void on_widget_mouseEnter(float AXpos, float AYpos, MIP_Widget* AFrom/*, uint32_t ATimeStamp=0*/) override {
    if (MCanDragValue) {
      switch (MDragDirection) {
        case MIP_LEFT:
        case MIP_RIGHT:
          do_widget_setMouseCursor(this,MIP_CURSOR_ARROWLEFTRIGHT);
          break;
        case MIP_UP:
        case MIP_DOWN:
          do_widget_setMouseCursor(this,MIP_CURSOR_ARROWUPDOWN);
          break;
      }
    }
    if (flags.autoHint) do_widget_setHint(this,getHint());
  }

  //----------

  //void on_widget_leave(float AXpos, float AYpos, MIP_Widget* ATo, uint32_t ATimeStamp=0) override {
  //  do_widget_setMouseCursor(this,MIP_CURSOR_DEFAULT);
  //  do_widget_setHint(this,"");
  //}

//------------------------------

};

//----------------------------------------------------------------------
#endif
