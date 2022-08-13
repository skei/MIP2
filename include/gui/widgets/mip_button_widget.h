#ifndef mip_button_widget_included
#define mip_button_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets/mip_value_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ButtonWidget
: public MIP_ValueWidget {

//------------------------------
protected:
//------------------------------

  const char* MOnText   = "on";
  const char* MOffText  = "off";
  bool        MIsToggle = true;

//------------------------------
public:
//------------------------------

  MIP_ButtonWidget(MIP_DRect ARect, const char* AOnText, const char* AOffText, double AValue)
  : MIP_ValueWidget(ARect,"",AValue) {
    MOnText = AOnText;
    MOffText = AOffText;
    MTextAlignment = MIP_TEXT_ALIGN_CENTER;
    MDrawValue = false;
    MDrawModulation = false;
    if (AValue >= 0.5) setText(AOnText);
    else setText(AOffText);
    MMouseCursor = MIP_CURSOR_FINGER;
  }

  //----------

  virtual ~MIP_ButtonWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setIsToggle(bool AToggle=true) { MIsToggle = AToggle; }

  virtual void setTexts(const char* AOnText, const char* AOffText) {
    MOnText = AOnText;
    MOffText = AOffText;
  }

  virtual void setOn() {
    setValue(1.0);
    setText(MOnText);
  }

  virtual void setOff() {
    setValue(0.0);
    setText(MOffText);
  }

//------------------------------
public: // parent to child
//------------------------------

//  void on_widget_paint(MIP_PaintContext* AContext) override { paintChildWidgets(AContext); }

  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    Flags.interactive = true;
    if (MIsToggle) {
      if (getValue() >= 0.5) {
        setValue(0.0);
        setText(MOffText);
      }
      else {
        setValue(1.0);
        setText(MOnText);
      }
      do_widget_update(this);
      do_widget_redraw(this);
    }
    else {
      setValue(1.0);
      setText(MOnText);
      do_widget_update(this);
      do_widget_redraw(this);
    }
  }

  //----------

  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    Flags.interactive = false;
    if (MIsToggle) {
    }
    else {
      setValue(0.0);
      setText(MOffText);
      do_widget_update(this);
      do_widget_redraw(this);
    }
  }

//  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, MIP_DPoint APos, uint32_t ATime) override {}
//  void on_widget_mouse_move(uint32_t AState, MIP_DPoint APos, uint32_t ATime) override {}
//  void on_widget_enter(MIP_DPoint APos, uint32_t ATime) override {}
//  void on_widget_leave(MIP_DPoint APos, uint32_t ATime) override {}
//  //void on_widget_connect(MIP_Parameter* AParameter) override {}

};

//----------------------------------------------------------------------
#endif


