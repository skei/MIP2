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

    setFillBackground(true);
    setFillGradient(true);

    setDrawBorder(true);
    setDrawRoundedCorners(true);
    setCornerRadius(4);

    Layout.border = MIP_DRect(5,5,5,5);

    MLabelWidget  = new MIP_TextWidget( MIP_DRect( 0.8, (1.0/6.0) ), AText );
    MValueWidget  = new MIP_TextWidget( MIP_DRect( 0.8, (1.0/6.0) ), AText );
    MKnobWidget   = new MIP_KnobWidget( MIP_DRect(), "", 0.0 );

    appendChildWidget(MLabelWidget);
    appendChildWidget(MValueWidget);
    appendChildWidget(MKnobWidget);

    //MLabelWidget->setFillBackground(false);
    //MLabelWidget->setDrawBorder(false);
    MLabelWidget->setTextSize(-1);
    MLabelWidget->setTextColor(MIP_COLOR_BLACK);
    MLabelWidget->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    MLabelWidget->Layout.sizeModeX = MIP_WIDGET_SIZE_MODE_RATIO;
    MLabelWidget->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_RATIO;

    //MValueWidget->setFillBackground(false);
    //MValueWidget->setDrawBorder(false);
    MValueWidget->setTextSize(-1);
    MValueWidget->setTextColor(MIP_COLOR_BLACK);
    MValueWidget->Layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;
    MValueWidget->Layout.sizeModeX = MIP_WIDGET_SIZE_MODE_RATIO;
    MValueWidget->Layout.sizeModeY = MIP_WIDGET_SIZE_MODE_RATIO;

    sprintf(MKnobValueText,"%.2f",getValue());
    MValueWidget->setText(MKnobValueText);

    //MKnobWidget->setFillBackground(false);
    //MKnobWidget->setDrawBorder(false);
    MKnobWidget->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;

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

