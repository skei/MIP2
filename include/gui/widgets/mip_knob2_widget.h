#ifndef mip_knob2_widget_included
#define mip_knob2_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets/mip_panel_widget.h"
#include "gui/widgets/mip_text_widget.h"
#include "gui/widgets/mip_knob_widget.h"
#include "gui/mip_paint_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Knob2Widget
: public MIP_PanelWidget {

//------------------------------
protected:
//------------------------------

  MIP_TextWidget* MLabelWidget = nullptr;
  MIP_TextWidget* MValueWidget = nullptr;
  MIP_KnobWidget* MKnobWidget = nullptr;

  char MKnobValueText[32] = {0};

//------------------------------
public:
//------------------------------

  MIP_Knob2Widget(MIP_DRect ARect, const char* AText, double AValue=0.0)
  : MIP_PanelWidget(ARect) {

    setFillBackground(true);
    setBackgroundColor(0.55);
    setFillGradient(false);
    setDrawBorder(false);
    setDrawRoundedCorners(false);
    setCornerRadius(0);
    //Layout.border = MIP_DRect(5,5,5,5);

    sprintf(MKnobValueText,"%.2f",AValue);

    double h02 = MRect.h * 0.2;
    double h06 = MRect.h * 0.6;

    //MLabelWidget = new MIP_TextWidget( MIP_DRect( 1, 0.20 ), AText );
    //MLabelWidget->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    //MLabelWidget->Layout.rectMode = MIP_WIDGET_RECT_MODE_PARENT_RATIO;
    MLabelWidget = new MIP_TextWidget( MIP_DRect( 0,0,MRect.w,h02 ), AText );
    MLabelWidget->setFillBackground(false);//(true);
    MLabelWidget->setDrawBorder(false);
    MLabelWidget->setTextSize(-1.0);
    MLabelWidget->setTextColor( MIP_Color(0)/*MIP_COLOR_BLACK*/);
    appendChildWidget(MLabelWidget);

    //MValueWidget = new MIP_TextWidget( MIP_DRect( 1, 0.20 ), AText );
    //MValueWidget->Layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;
    //MValueWidget->Layout.rectMode = MIP_WIDGET_RECT_MODE_PARENT_RATIO;
    MValueWidget = new MIP_TextWidget( MIP_DRect( 0,MRect.h-h02,MRect.w, h02 ), AText );
    MValueWidget->setFillBackground(false);//(true);
    MValueWidget->setDrawBorder(false);
    MValueWidget->setText(MKnobValueText);
    MValueWidget->setTextSize(-0.9);
    MValueWidget->setTextColor( MIP_Color(0)/*MIP_COLOR_BLACK*/);
    appendChildWidget(MValueWidget);

    //MKnobWidget = new MIP_KnobWidget( MIP_DRect(), "", 0.0 );
    //MKnobWidget->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    MKnobWidget = new MIP_KnobWidget( MIP_DRect(0,h02,MRect.w,h06), "", 0.0 );
    MKnobWidget->setFillBackground(false);
    MKnobWidget->setDrawBorder(false);
    MKnobWidget->setDrawBorder(false);
    appendChildWidget(MKnobWidget);

  }

  //----------

  virtual ~MIP_Knob2Widget() {
  }

//------------------------------
public:
//------------------------------

  MIP_TextWidget* getLabelWidget()  { return MLabelWidget; }
  MIP_TextWidget* getValueWidget()  { return MValueWidget; }
  MIP_KnobWidget* getKnobWidget()   { return MKnobWidget; }

  double getValue() override {
    //MIP_PRINT;
    return MKnobWidget->getValue();
  }

  void setValue(double v) override {
    //MIP_PRINT;
    MKnobWidget->setValue(v);
    MIP_Parameter* parameter = MKnobWidget->getParameter();
    if (parameter) {
      parameter->valueToText(v,MKnobValueText,31);
    }
    else {
      sprintf(MKnobValueText,"%.2f",v);
    }
    MValueWidget->setText(MKnobValueText);
    MValueWidget->redraw();
  }

  double getMinValue() override {
    //MIP_PRINT;
    return MKnobWidget->getMinValue();
  }

  void setMinValue(double v) override {
    //MIP_PRINT;
    MKnobWidget->setMinValue(v);
  }

  double getMaxValue() override {
    //MIP_PRINT;
    return MKnobWidget->getMaxValue();
  }

  void setMaxValue(double v) override {
    //MIP_PRINT;
    MKnobWidget->setMaxValue(v);
  }
  double getDefaultValue() override {
    //MIP_PRINT;
    return MKnobWidget->getDefaultValue();
  }

  void setDefaultValue(double v) override {
    //MIP_PRINT;
    MKnobWidget->setDefaultValue(v);
  }

  MIP_Parameter* getParameter() override {
    return MKnobWidget->getParameter();
  }

  void setParameter(MIP_Parameter* AParam) override {
    MKnobWidget->setParameter(AParam);
  }

//------------------------------
public: // child to parent
//------------------------------

  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) override {
    //MIP_PRINT;
    MIP_PanelWidget::do_widget_update(ASender,AMode);
    if (ASender == MKnobWidget) {
      double value = MKnobWidget->getValue();
      MIP_Parameter* parameter = MKnobWidget->getParameter();
      if (parameter) {
        parameter->valueToText(value,MKnobValueText,31);
      }
      else {
        sprintf(MKnobValueText,"%.2f",value);
      }
      MValueWidget->setText(MKnobValueText);
      MValueWidget->redraw();
    }
  }

};

//----------------------------------------------------------------------
#endif

