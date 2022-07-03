#ifndef mip_knob2_widget_included
#define mip_knob2_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widget.h"
#include "gui/widgets/mip_panel_widget.h"
#include "gui/widgets/mip_knob_widget.h"
#include "gui/widgets/mip_text_widget.h"

class MIP_Knob2Widget
: public MIP_PanelWidget {

//------------------------------
private:
//------------------------------

  MIP_KnobWidget*   MKnobWidget       = nullptr;;
  MIP_TextWidget*   MLabelWidget      = nullptr;;
  MIP_TextWidget*   MValueWidget      = nullptr;;

  char              MDisplayText[32]  = {0};

  bool              MDrawLabel        = true;
  bool              MDrawValue        = true;

//------------------------------
public:
//------------------------------

  MIP_Knob2Widget(MIP_FRect ARect, const char* AName="", bool ALabel=true, bool AValue=true)
  : MIP_PanelWidget(ARect) {
    setName("MIP_Knob2Widget");
    setHint("knob2");

    setDrawBorder(false);

    MDrawLabel = ALabel;
    MDrawValue = AValue;

    if (MDrawLabel) {
      MLabelWidget = new MIP_TextWidget( MIP_FRect(15) );
      MLabelWidget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
      appendWidget(MLabelWidget);
    }

    if (MDrawValue) {
      MValueWidget = new MIP_TextWidget( MIP_FRect(15) );
      MValueWidget->layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;
      appendWidget(MValueWidget);
    }

    MKnobWidget = new MIP_KnobWidget( MIP_FRect() );
    MKnobWidget->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    appendWidget(MKnobWidget);

  }

  //----------

  virtual ~MIP_Knob2Widget() {
  }

//------------------------------
public:
//------------------------------

  MIP_KnobWidget* getKnobWidget() {
    return MKnobWidget;
  }

  // setValue

//------------------------------
public:
//------------------------------

  void setValue(float AValue) override {
    if (MKnobWidget) {
      MKnobWidget->setValue(AValue);
    }
    if (MValueWidget) {
      sprintf(MDisplayText,"%.2f",AValue);
      MValueWidget->setText(MDisplayText);
      //MValueWidget->setValue(AValue);
      //MValueWidget->setText(MDisplayText);
      MValueWidget->redraw();
    }
  }

  //----------

  void setModValue(float AValue) override {
    MIP_PanelWidget::setModValue(AValue);
    if (MKnobWidget) {
      MKnobWidget->setModValue(AValue);
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_connect(MIP_Parameter* AParameter) override {
    MKnobWidget->on_widget_connect(AParameter);
    if (MLabelWidget) {
      const char* name = AParameter->getName();
      MLabelWidget->setText(name);
    }
    //if (MValueWidget) {
    //  double value = AParameter->getValue();
    //  sprintf(MDisplayText,"%.2f",value);
    //  MValueWidget->setText(MDisplayText);
    //  //MIP_Print("%s %.3f\n",name,value);
    //}
    //MIP_PanelWidget::on_widget_connect(AParameter);
  }

  //----------

  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) override {
    if (ASender == MKnobWidget) {
      double value = ASender->getValue();
      //MIP_Print("value %.3f\n",value);
      setValue(value);
      //if (MValueWidget) {
      //  MValueWidget->setValue(value);
      //  MValueWidget->redraw();
      //}
    }
    //MIP_PanelWidget::do_widget_update(this,AMode);
    MIP_PanelWidget::do_widget_update(ASender,AMode);
  }

};

//----------------------------------------------------------------------
#endif
