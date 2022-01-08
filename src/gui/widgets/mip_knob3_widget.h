#ifndef kode_knob3_widget_included
#define kode_knob3_widget_included
//----------------------------------------------------------------------

/*
      _
     / \ label
     \_/ value

*/

#define TEXT_HEIGHT 16

#include "gui/kode_widget.h"

class KODE_Knob3Widget
: public KODE_Widget {

//------------------------------
private:
//------------------------------

  KODE_KnobWidget*  wdg_knob        = nullptr;
  KODE_TextWidget*  wdg_label       = nullptr;
  KODE_TextWidget*  wdg_value       = nullptr;
  char              value_text[32]  = {0};


//------------------------------
public:
//------------------------------

  KODE_Knob3Widget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    setName("KODE_Knob3Widget");
    setHint("knob3");
    //setValue(AValue);
    //sprintf(value_text,"%.3f",AValue);
    sprintf(value_text,"000");

    #define B 5
    float s   = getRect().h;
    float s05 = s * 0.5f;
    float ss = (getRect().w - B) - s;
    wdg_knob  = new KODE_KnobWidget( KODE_FRect(     0, 0,   s,      s   ) );
    wdg_label = new KODE_TextWidget( KODE_FRect( 5 + s, 0,   B + ss, s05 ) );
    wdg_value = new KODE_TextWidget( KODE_FRect( 5 + s, s05, B + ss, s05 ) );
    appendWidget(wdg_knob);
    appendWidget(wdg_label);
    appendWidget(wdg_value);
    #undef B

    //wdg_knob->setValue(AValue);
    wdg_knob->setFillBackground(false);
    wdg_knob->setBackgroundColor(KODE_Color(0.55));
    //wdg_knob->setBipolar(ABipolar);

    wdg_label->setText("knob3");
    //wdg_label->setTextSize(10);
    wdg_label->setFillBackground(false);
    wdg_label->setBackgroundColor(KODE_Color(0.55));
    wdg_label->setDrawBorder(false);
    wdg_label->setTextColor(KODE_COLOR_BLACK);
    wdg_label->setTextAlignment(KODE_TEXT_ALIGN_LEFT);

    wdg_value->setText(value_text);
    //wdg_value->setTextSize(10);
    wdg_value->setFillBackground(false);
    wdg_value->setBackgroundColor(KODE_Color(0.55));
    wdg_value->setDrawBorder(false);
    wdg_value->setTextColor(KODE_COLOR_BLACK);
    wdg_value->setTextAlignment(KODE_TEXT_ALIGN_LEFT);
    //wdg_value->setValue(AValue);

  }

  virtual ~KODE_Knob3Widget() {
  }

//------------------------------
public:
//------------------------------

  virtual KODE_KnobWidget* getKnobWidget()  { return wdg_knob; }
  virtual KODE_TextWidget* getLabelWidget() { return wdg_label; }
  virtual KODE_TextWidget* getValueWidget() { return wdg_value; }


//------------------------------
public:
//------------------------------

    void setValue(float AValue) override {
      KODE_Widget::setValue(AValue);
      //KODE_Assert(wdg_knob != KODE_NULL);
      if (wdg_knob) {
        wdg_knob->setValue(AValue);
      }
      if (wdg_value) {

        //if (MParameter) {
        //  KODE_Parameter* par = (KODE_Parameter*)MParameter;
        KODE_Parameter* par = (KODE_Parameter*)getParameter();
        if (par) {

          //KODE_String txt = par->getDisplayText(AValue);
          //wdg_value->setText(txt);
          par->getDisplayText(AValue,value_text);
          wdg_value->setText(value_text);
        }
        else {
          //KODE_FloatToString(value_text,AValue);
          sprintf(value_text,"%.3f",AValue);
          wdg_value->setText(value_text);
        }
      }
    }

//------------------------------
public:
//------------------------------

  void on_widget_connect(KODE_Parameter* AParameter, uint32_t ASubIndex) override {
      wdg_knob->setParameter(AParameter);
      if (AParameter) {
        wdg_label->setText( AParameter->getName() );
        float value = 0.0f;
        //const char* txt = AParameter->getDisplayText(value);
        //KODE_FloatToString(value_text,AValue);
        sprintf(value_text,"%.3f",value);
        wdg_value->setText(value_text);
      }
  }

//------------------------------
public:
//------------------------------

  void do_widget_update(KODE_Widget* ASender) final {
    //KODE_Widget::do_widget_update(ASender);
    //if (MParent) MParent->do_widget_update(this);
    if (ASender == wdg_knob) {
      setValue( ASender->getValue() );
      //if (MParameter) {
      //  KODE_Parameter* par = (KODE_Parameter*)MParameter;
      KODE_Parameter* par = (KODE_Parameter*)getParameter();
      if (par) {
        par->getDisplayText(getValue(),value_text);
        wdg_value->setText(value_text);
      }
      do_widget_redraw(wdg_value,wdg_value->getRect(),0);
    }
    else {
      //if (MParent) MParent->do_update(this);
      KODE_Widget::do_widget_update(ASender);
    }
  }

};

#undef TEXT_HEIGHT

//----------------------------------------------------------------------
#endif
