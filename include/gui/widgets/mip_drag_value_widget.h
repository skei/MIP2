#ifndef mip_drag_value_widget_included
#define mip_drag_value_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widget.h"
#include "gui/widgets/mip_value_widget.h"

class MIP_DragValueWidget
: public MIP_ValueWidget {

//------------------------------
private:
//------------------------------

  float MPrevXpos = 0.0;
  float MPrevYpos = 0.0;

//------------------------------
protected:
//------------------------------

  bool    MSnap               = false;
  double  MSnapPos            = 0.5;
  double  MSnapDist           = 0.01;

  float MDragSensitivity      = 0.002;
  float MDragExtraSensitivity = 0.05;

//------------------------------
public:
//------------------------------

  MIP_DragValueWidget(MIP_FRect ARect, const char* AName="")
  : MIP_ValueWidget(ARect,AName) {
    setName("MIP_TemplateWidget");
    setHint("template");
    //flags.redrawHover = true;
    //flags.autoHint = true;
    setMouseCursor(MIP_CURSOR_ARROW_UP_DOWN);
    flags.autoCursor = true;
    flags.autoMouseLock = true;
    flags.autoMouseHide = true;
    flags.redrawInteract = false;
  }

  //----------

  virtual ~MIP_DragValueWidget() {
  }

//------------------------------
public:
//------------------------------

  //bool isDragging() { return MIsDragging; }

  //----------

  //virtual void setQuantize(bool AQuantize=true) { MQuantize = AQuantize; }
  virtual void setSnap(bool ASnap=true)         { MSnap = ASnap; }
  virtual void setSnapPos(float APos)           { MSnapPos = APos; }
  virtual void setSnapDist(float ADist)         { MSnapDist = ADist; }

//------------------------------
public:
//------------------------------

  void drag(float AXDelta, float AYDelta) {
    //MIP_Print("xdelta %.2f ydelta %.2f\n",AXDelta,AYDelta);
    if (MParameter) {
      double minval = MParameter->getMinValue();
      double maxval = MParameter->getMaxValue();
      double range = maxval - minval;
      //bool stepped = MParameter->isStepped();
      //uint32_t numsteps = 1;
      //if (stepped) { numsteps = (int)range + 1; }
      float value = getValue();
      float sens = MDragSensitivity * range;
      value -= AYDelta * sens;
      value = MIP_Clamp(value,minval,maxval);

      //if (stepped) value = (double)(int)value;
      //MIP_Print("value %.3f\n",value);

      setValue(value);

      update();
      redraw();
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
    if (AButton == MIP_BUTTON_LEFT) {
      state.interacting = true;
      MPrevXpos = AXpos;
      MPrevYpos = AYpos;
      if (flags.autoMouseLock) do_widget_setMouseCursor(this,MIP_CURSOR_GRAB);
      if (flags.autoMouseHide) do_widget_setMouseCursor(this,MIP_CURSOR_HIDE);
      if (flags.redrawInteract) do_widget_redraw(this,getRect(),0);
    }
  }

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
    if (AButton == MIP_BUTTON_LEFT) {
      state.interacting = false;
      if (flags.autoMouseLock) do_widget_setMouseCursor(this,MIP_CURSOR_RELEASE);
      if (flags.autoMouseHide) do_widget_setMouseCursor(this,MIP_CURSOR_SHOW);
      if (flags.redrawInteract) do_widget_redraw(this,getRect(),0);
    }
  }

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) override {
    if (state.interacting) {
      float xdelta = AXpos - MPrevXpos;
      float ydelta = AYpos - MPrevYpos;
      drag(xdelta,ydelta);
      MPrevXpos = AXpos;
      MPrevYpos = AYpos;
    }
  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif
