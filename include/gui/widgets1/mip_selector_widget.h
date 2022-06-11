#ifndef mip_selector_widget_included
#define mip_selector_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/mip_menu_widget.h"
#include "gui/widgets/mip_text_widget.h"

class MIP_SelectorWidget
: public MIP_TextWidget
, public MIP_MenuListener {

//------------------------------
private:
//------------------------------

  MIP_MenuWidget*     MMenu           = nullptr;
  //MIP_MenuListener*   MListener       = nullptr;
  bool                MDrawTriangle   = true;
  MIP_Color           MTriangleColor  = MIP_COLOR_DARK_GRAY;

//------------------------------
public:
//------------------------------

  MIP_SelectorWidget(MIP_FRect ARect)
  : MIP_TextWidget(ARect) {
    setName("MIP_SelectorWidget");
    setHint("selector");
    setCursor(MIP_CURSOR_FINGER);
    setFillBackground(true);
    setBackgroundColor(0.5);
    setTextColor(MIP_COLOR_BLACK);
    setDrawBorder(true);
    setBorderColor(0.4);
    setDrawText(true);
    setText("<select>");
    setDrawParamName(false);
  }

  virtual ~MIP_SelectorWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setMenu(MIP_MenuWidget* AMenu) {
    MMenu = AMenu;
  }

  virtual void setDrawTriangle(bool ADraw) {
    MDrawTriangle = ADraw;
  }

  virtual void setTriangleColor(MIP_Color AColor) {
    MTriangleColor = AColor;
  }

  //virtual void setListener(MIP_MenuListener* AListener) {
  //  MListener = AListener;
  //}

  virtual void setSelected(int32_t AIndex) {
    float v = indexToValue(AIndex);
    setValue(v);
    //do_update(this);
    //do_redraw(this,MRect);
  }

  //----------

  virtual int32_t getSelected(void) {
    int32_t i = valueToIndex( getValue() );
    return i;
  }

  //----------

  virtual float indexToValue(int32_t AIndex) {
    float v = 0.0f;
    if (MMenu) {
      uint32_t num = MMenu->getNumItems() - 1;
      if (num > 0) {
        v = (float)AIndex / (float)num;
      }
    }
    return v;
  }

  //----------

  virtual uint32_t valueToIndex(float AValue) {
    float i = 0;
    if (MMenu) {
      uint32_t num = MMenu->getNumItems() - 1;
      if (num > 0) {
        i = AValue * (float)num;
      }
    }
    return (uint32_t)i;
  }

//------------------------------
public:
//------------------------------

//    // todo: set MValue
//
//    void on_menuEvent(int32_t AIndex) override {
//      float v = indexToValue(AIndex);
//      setValue(v);
//      do_update(this);
//      do_redraw(this,MRect);
//    }

  void on_menuEvent(int32_t AIndex) override {
    //MIP_Print("AIndex %i\n",AIndex);
    float v = indexToValue(AIndex);
    setValue(v);
    do_widget_update(this);
    do_widget_redraw(this,getRect(),0);
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) final {
    MIP_FRect mrect = getRect();
    if (MMenu) {
      //if (MMenu->getNumItems() > 0) {
        float val = getValue();
        int32_t sel = valueToIndex(val);


        if (sel >= 0) {
          //MIP_Print("sel %i MText %s\n",sel,MText);
          MText = MMenu->getItem(sel);
        }
        else {
          MText = "---";
        }

      //}
      //else MText = "neg";
    }
    //else MText = "(no menu)";
    MIP_TextWidget::on_widget_paint(APainter,ARect,AMode);
    //APainter->setDrawColor(MIP_LightGrey);
    if (MDrawTriangle) {
      APainter->fillTriangle(
        mrect.x2() - 12,  mrect.y2() - 9,
        mrect.x2() - 5,   mrect.y2() - 9,
        mrect.x2() - 9,   mrect.y2() - 5,
        MTriangleColor
      );
    }
  }

  //----------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    //MIP_TextWidget::on_widget_mouseClick(AXpos,AYpos,AButton,AState,ATimeStamp); // Release ??
    if (AButton == MIP_BUTTON_LEFT) {
      if (MMenu) {
        MMenu->open(this,AXpos,AYpos);
      }
    }
  }

};

//----------------------------------------------------------------------
#endif
