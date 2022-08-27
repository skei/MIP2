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

    MLabelWidget = new MIP_TextWidget( MIP_DRect( 1, 0.20 ), AText );
    MLabelWidget->setFillBackground(true);
    MLabelWidget->setDrawBorder(false);
    MLabelWidget->setTextSize(-1);
    MLabelWidget->setTextColor(MIP_COLOR_BLACK);
    MLabelWidget->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    MLabelWidget->Layout.horizScale = MIP_WIDGET_SCALE_PARENT_RATIO;
    MLabelWidget->Layout.vertScale = MIP_WIDGET_SCALE_PARENT_RATIO;
    appendChildWidget(MLabelWidget);

    MValueWidget = new MIP_TextWidget( MIP_DRect( 1, 0.20 ), AText );
    MValueWidget->Layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;
    MValueWidget->Layout.horizScale = MIP_WIDGET_SCALE_PARENT_RATIO;
    MValueWidget->Layout.vertScale = MIP_WIDGET_SCALE_PARENT_RATIO;
    MValueWidget->setFillBackground(true);
    MValueWidget->setDrawBorder(false);
    MValueWidget->setText(MKnobValueText);
    MValueWidget->setTextSize(-1);
    MValueWidget->setTextColor(MIP_COLOR_BLACK);
    appendChildWidget(MValueWidget);

    MKnobWidget = new MIP_KnobWidget( MIP_DRect(), "", 0.0 );
    MKnobWidget->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
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

