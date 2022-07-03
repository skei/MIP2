#ifndef mip_knob4_widget_included
#define mip_knob4_widget_included
//----------------------------------------------------------------------

/*
    label O value

*/

#define TEXT_HEIGHT 16

#include "gui/mip_widget.h"

class MIP_Knob4Widget
: public MIP_Widget {

//------------------------------
protected:
//------------------------------

    MIP_KnobWidget*  wdg_knob        = nullptr;
    MIP_TextWidget*  wdg_label       = nullptr;
    MIP_TextWidget*  wdg_value       = nullptr;
    char              value_text[32]  = {0};

//------------------------------
public:
//------------------------------

  MIP_Knob4Widget(MIP_FRect ARect)
  : MIP_Widget(ARect) {
    setName("MIP_Knob4Widget");
    setHint("knob4");
    //setValue(AValue);
    //sprintf(value_text,"%.3f",AValue);

    #define B 5.0f
    float s  = getRect().h;
    float tw = 0.5f * (getRect().w - s - (B*2.0f));
    wdg_label = new MIP_TextWidget( MIP_FRect( 0,   0, tw, s ) );
    wdg_knob  = new MIP_KnobWidget( MIP_FRect( tw,  0, s,  s ) );
    wdg_value = new MIP_TextWidget( MIP_FRect( tw+s,0, tw, s ) );
    appendWidget(wdg_knob);
    appendWidget(wdg_label);
    appendWidget(wdg_value);
    #undef B

    //wdg_knob->setValue(AValue);
    wdg_knob->setFillBackground(true);
    wdg_knob->setBackgroundColor(MIP_Color(0.55));
    //wdg_knob->setBipolar(ABipolar);

    wdg_label->setText("knob4");
    //wdg_label->setTextSize(10);
    wdg_label->setFillBackground(true);
    wdg_label->setBackgroundColor(MIP_Color(0.55));
    wdg_label->setDrawBorder(false);
    wdg_label->setTextColor(MIP_COLOR_BLACK);
    wdg_label->setTextAlignment(MIP_TEXT_ALIGN_RIGHT);

    wdg_value->setText(value_text);
    //wdg_value->setTextSize(10);
    wdg_value->setFillBackground(true);
    wdg_value->setBackgroundColor(MIP_Color(0.55));
    wdg_value->setDrawBorder(false);
    wdg_value->setTextColor(MIP_COLOR_BLACK);
    wdg_value->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    //wdg_value->setValue(AValue);
    sprintf(value_text,"%.3f",MValue);

  }

  //----------

  virtual ~MIP_Knob4Widget() {
  }

//------------------------------
public:
//------------------------------

  virtual MIP_KnobWidget* getKnobWidget()  { return wdg_knob; }
  virtual MIP_TextWidget* getLabelWidget() { return wdg_label; }
  virtual MIP_TextWidget* getValueWidget() { return wdg_value; }

//------------------------------
public:
//------------------------------

  void setValue(float AValue) override {
    MIP_Widget::setValue(AValue);
    //MIP_Assert(wdg_knob != MIP_NULL);
    if (wdg_knob) {
      wdg_knob->setValue(AValue);
    }
    if (wdg_value) {
      //if (MParameter) {
      //  MIP_Parameter* par = (MIP_Parameter*)MParameter;
//      MIP_Parameter* par = (MIP_Parameter*)getParameter();
//      if (par) {
//        //MIP_String txt = par->getDisplayText(AValue);
//        //wdg_value->setText(txt);
//        par->displayText(value_text,AValue);
//        wdg_value->setText(value_text);
//      }
//      else {
        //MIP_FloatToString(value_text,AValue);
        sprintf(value_text,"%.3f",AValue);
        wdg_value->setText(value_text);
//      }
    }
  }

//------------------------------
public:
//------------------------------

//  void on_widget_connect(MIP_Parameter* AParameter, uint32_t ASubIndex) override {
//    wdg_knob->setParameter(AParameter);
//    if (AParameter) {
//      wdg_label->setText( AParameter->getName() );
//      float value = 0.0f;
//      //const char* txt = AParameter->getDisplayText(value);
//      //MIP_FloatToString(value_text,AValue);
//      sprintf(value_text,"%.3f",value);
//      wdg_value->setText(value_text);
//    }
//  }

//------------------------------
public:
//------------------------------

  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) final {
    //MIP_Widget::do_widget_update(ASender);
    //if (MParent) MParent->do_update(this);
    if (ASender==wdg_knob) {
      setValue( ASender->getValue() );
      //if (MParameter) {
      //  MIP_Parameter* par = (MIP_Parameter*)MParameter;
//      MIP_Parameter* par = (MIP_Parameter*)getParameter();
//      if (par) {
//        par->displayText(value_text,getValue());
//        wdg_value->setText(value_text);
//      }
      do_widget_redraw(wdg_value,wdg_value->getRect(),0);
    }
    else {
      //if (MParent) MParent->do_update(this);
      MIP_Widget::do_widget_update(ASender);

    }

  }

};

#undef TEXT_HEIGHT

//----------------------------------------------------------------------
#endif
