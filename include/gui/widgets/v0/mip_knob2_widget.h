#ifndef mip_knob2_widget_included
#define mip_knob2_widget_included
//----------------------------------------------------------------------

// label
//   O
// value

#define TEXT_HEIGHT 16

#include "gui/mip_widget.h"
#include "gui/widgets/v0/mip_knob_widget.h"
#include "gui/widgets/v0/mip_text_widget.h"

class MIP_Knob2Widget
: public MIP_Widget {

//------------------------------
protected:
//------------------------------

  MIP_KnobWidget*  wdg_knob        = nullptr;
  MIP_TextWidget*  wdg_label       = nullptr;
  MIP_TextWidget*  wdg_value       = nullptr;
  char             value_text[32]  = {0};

//------------------------------
public:
//------------------------------

  MIP_Knob2Widget(MIP_FRect ARect, const char* AName="")
  : MIP_Widget(ARect) {
    setName("MIP_Knob2Widget");
    setHint("knob2");
    //#define X MRect.x
    //#define Y MRect.y
    #define X 0
    #define Y 0
    #define W getRect().w
    #define H getRect().h
    #define KS W
    //sprintf(value_text,"%.3f",AValue);
    wdg_label = (MIP_TextWidget*)appendWidget( new MIP_TextWidget( MIP_FRect(X,Y,      W, 16) ));
    wdg_knob  = (MIP_KnobWidget*)appendWidget( new MIP_KnobWidget( MIP_FRect(X,Y+16,   W, KS) ));
    wdg_value = (MIP_TextWidget*)appendWidget( new MIP_TextWidget( MIP_FRect(X,Y+16+KS,W, 16) ));
    #undef X
    #undef Y
    #undef W
    #undef H
    #undef KS

    wdg_label->setText(AName);
    //wdg_label->setTextSize(10);
    wdg_label->setFillBackground(false);
    wdg_label->setBackgroundColor(MIP_Color(0.55));
    wdg_label->setDrawBorder(false);
    wdg_label->setTextColor(MIP_COLOR_BLACK);
    wdg_label->setTextAlignment(MIP_TEXT_ALIGN_CENTER);

    //wdg_knob->setValue(AValue);
    wdg_knob->setFillBackground(false);
    wdg_knob->setBackgroundColor(MIP_Color(0.55));
    //wdg_knob->setBipolar(ABipolar);

    wdg_value->setText(value_text);
    //wdg_value->setTextSize(10);
    wdg_value->setFillBackground(false);
    wdg_value->setBackgroundColor(MIP_Color(0.55));
    wdg_value->setDrawBorder(false);
    wdg_value->setTextColor(MIP_COLOR_BLACK);
    wdg_value->setTextAlignment(MIP_TEXT_ALIGN_CENTER);
    //wdg_value->setValue(AValue);

  }

  virtual ~MIP_Knob2Widget() {
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

//  void on_widget_connect(MIP_Parameter* AParameter, uint32_t ASubIndex=0) override {
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
    //if (MParent) MParent->do_widget_update(this);
    if (ASender == wdg_knob) {
      setValue( ASender->getValue() );
//      MIP_Parameter* par = (MIP_Parameter*)getParameter();
//      if (par) {
//        par->displayText(value_text,getValue());
//        wdg_value->setText(value_text);
//      }
      do_widget_redraw(wdg_value,wdg_value->getRect(),0);
    }
//    else {
      //if (MParent) MParent->do_widget_update(this);
      MIP_Widget::do_widget_update(this,AMode);
//    }
  }

};

#undef TEXT_HEIGHT

//----------------------------------------------------------------------
#endif

