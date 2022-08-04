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

  MIP_Knob2Widget(MIP_DRect ARect, const char* AText, double AValue)
  : MIP_PanelWidget(ARect) {

    //setFillBackground(false);
    setFillGradient(false);
    //setDrawBorder(false);
    //setDrawRoundedCorners(false);
    //setCornerRadius(4);

    MLabelWidget  = new MIP_TextWidget( MIP_DRect( 5, 3,      32,16 ), AText );
    MKnobWidget   = new MIP_KnobWidget( MIP_DRect( 5, 5 + 16, 32,32 ), "", 0.0 );
    MValueWidget  = new MIP_TextWidget( MIP_DRect( 5, 7 + 48, 32,16 ), "0.00" );

    appendChildWidget(MLabelWidget);
    appendChildWidget(MValueWidget);
    appendChildWidget(MKnobWidget);

    MLabelWidget->setFillBackground(false);
    MLabelWidget->setDrawBorder(false);
    MLabelWidget->setTextSize(10);
    MLabelWidget->setTextColor(MIP_COLOR_BLACK);

    MKnobWidget->setFillBackground(false);
    MKnobWidget->setDrawBorder(false);

    MValueWidget->setFillBackground(false);
    MValueWidget->setDrawBorder(false);
    MValueWidget->setTextSize(10);
    MValueWidget->setTextColor(MIP_COLOR_BLACK);

  }

  //----------

  virtual ~MIP_Knob2Widget() {
  }

//------------------------------
public:
//------------------------------

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

