#ifndef kode_selector_widget_included
#define kode_selector_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_menu_widget.h"
#include "gui/widgets/kode_text_widget.h"

class KODE_SelectorWidget
: public KODE_TextWidget
, public KODE_MenuListener {

//------------------------------
private:
//------------------------------

  KODE_MenuWidget*    MMenu     = KODE_NULL;
  //KODE_MenuListener*  MListener = KODE_NULL;
  bool MDrawTriangle = true;

//------------------------------
public:
//------------------------------

  KODE_SelectorWidget(KODE_FRect ARect)
  : KODE_TextWidget(ARect) {
    setName("KODE_SelectorWidget");
    setHint("selector");
    setCursor(KODE_CURSOR_FINGER);
    setFillBackground(true);
    setBackgroundColor(KODE_COLOR_DARK_GRAY);
    setTextColor(KODE_COLOR_WHITE);
    setDrawBorder(false);
    setDrawText(true);
    setText("<select>");
  }

  virtual ~KODE_SelectorWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setMenu(KODE_MenuWidget* AMenu) {
    MMenu = AMenu;
  }

  virtual void setDrawTriangle(bool ADraw) {
    MDrawTriangle = ADraw;
  }

  //virtual void setListener(KODE_MenuListener* AListener) {
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
    //KODE_Print("AIndex %i\n",AIndex);
    float v = indexToValue(AIndex);
    setValue(v);
    do_widget_update(this);
    do_widget_redraw(this,getRect(),0);
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) final {
    KODE_FRect mrect = getRect();
    if (MMenu) {
      //if (MMenu->getNumItems() > 0) {
        float val = getValue();
        int32_t sel = valueToIndex(val);
        if (sel >= 0) MText = MMenu->getItem(sel);
// !!!!!!!!
        else MText = "---";

      //}
      //else MText = "neg";
    }
    //else MText = "(no menu)";
    KODE_TextWidget::on_widget_paint(APainter,ARect,AMode);
    //APainter->setDrawColor(KODE_LightGrey);
    if (MDrawTriangle) {
      APainter->fillTriangle(
        mrect.x2() - 12,  mrect.y2() - 9,
        mrect.x2() - 5,   mrect.y2() - 9,
        mrect.x2() - 9,   mrect.y2() - 5,
        KODE_COLOR_LIGHT_GRAY
      );
    }
  }

  //----------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    //KODE_TextWidget::on_widget_mouseClick(AXpos,AYpos,AButton,AState,ATimeStamp); // Release ??
    if (AButton == KODE_BUTTON_LEFT) {
      if (MMenu) {
        MMenu->open(this,AXpos,AYpos);
      }
    }
  }

};

//----------------------------------------------------------------------
#endif
