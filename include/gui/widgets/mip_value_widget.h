#ifndef mip_value_widget_included
#define mip_value_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets/mip_text_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ValueWidget
: public MIP_TextWidget {

//------------------------------
protected:
//------------------------------

  bool      MDrawParameterValue   = true;

  bool      MDrawValue            = true;
  double    MValue                = 0.0;
  double    MMinValue             = 0.0;
  double    MMaxValue             = 1.0;
  double    MDefValue             = 0.0;
  MIP_Color MValueColor           = MIP_COLOR_BLACK;
  uint32_t  MValueAlignment       = MIP_TEXT_ALIGN_RIGHT;
  float     MValueSize            = 13.0;

  bool      MDrawModulation       = false;//true;
  double    MModulation           = 0.0;
  MIP_Color MModulationColor      = MIP_COLOR_WHITE;
  uint32_t  MModulationAlignment  = MIP_TEXT_ALIGN_CENTER;
  float     MModulationSize       = 10.0;

  bool      MIsDragging           = false;
  double    MDragSensitivity      = 0.0005;
  double    MDragSensitivity2     = 0.025;

  double    MDragValue            = 0.0;
  double    MMousePrevX           = 0.0;
  double    MMousePrevY           = 0.0;

  //double    MClickedX             = 0.0;
  //double    MClickedY             = 0.0;
  //double    MClickedValue         = 0.0;

//------------------------------
public:
//------------------------------

  MIP_ValueWidget(MIP_DRect ARect, const char* AText, double AValue)
  : MIP_TextWidget(ARect,AText) {
    MName = "MIP_ValueWidget";
    MValue = AValue;
    MDefValue = MValue;
    MTextAlignment = MIP_TEXT_ALIGN_LEFT;
  }

  //----------

  virtual ~MIP_ValueWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void  setDrawParameterValue(bool ADraw=true)  { MDrawParameterValue = ADraw; }

  virtual void  setDrawValue(bool ADraw=true)           { MDrawValue = ADraw; }
  virtual void  setValue(double AValue)                 { MValue = AValue; }
  virtual void  setValueColor(MIP_Color AColor)         { MValueColor = AColor; }
  virtual void  setValueAlignment(uint32_t AAlign)      { MValueAlignment = AAlign; }
  virtual void  setValueSize(float ASize)               { MValueSize = ASize; }

  virtual void  setDrawModulation(bool ADraw=true)      { MDrawModulation = ADraw; }
  virtual void  setModulation(double AValue)            { MModulation = AValue; }
  virtual void  setModulationColor(MIP_Color AColor)    { MModulationColor = AColor; }
  virtual void  setModulationAlignment(uint32_t AAlign) { MModulationAlignment = AAlign; }
  virtual void  setModulationSize(float ASize)          { MModulationSize = ASize; }

//------------------------------
public: // parent to child
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    fillBackground(AContext);
    paintChildWidgets(AContext);
    drawText(AContext);
    drawValue(AContext);
    drawModulation(AContext);
    drawBorder(AContext);
  }

  void on_widget_mouse_press(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    switch (AButton) {
      case MIP_BUTTON_LEFT: {
        MMousePrevX = AXpos;
        MMousePrevY = AYpos;
        MDragValue    = MValue;
        //MIP_Print("click! MValue: %.3f\n",MValue);
        MIsDragging   = true;
        break;
      }
    }
  }

  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    switch (AButton) {
      case MIP_BUTTON_LEFT: {
        MIsDragging   = false;
        break;
      }
    }
  }

  void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    if (MIsDragging) {
      double xdiff = AXpos - MMousePrevX;
      double ydiff = MMousePrevY - AYpos;
      MDragValue += (ydiff * MDragSensitivity);
      MMousePrevX = AXpos;
      MMousePrevY = AYpos;
      MDragValue = MIP_Clamp(MDragValue,MMinValue,MMaxValue);
      setValue(MDragValue);
      do_widget_update(this);
      do_widget_redraw(this);
    }
  }

//------------------------------
public:
//------------------------------

  virtual void drawValue(MIP_PaintContext* AContext) {
    if (MDrawValue) {
      MIP_Painter* painter = AContext->painter;
      char temp[16];
      double value = MValue;
      if (MDrawParameterValue) {
        MIP_Parameter* parameter = getParameter();
        if (parameter) {
          value = parameter->getValue();
        }
      }
      sprintf(temp,"%.3f",value);
      MIP_DRect rect = MRect;
      rect.shrink(2);
      painter->fontSize(MValueSize);
      painter->beginPath();
      painter->drawTextBox(rect,temp,MValueAlignment,MValueColor);
    }
  }

  //----------

  virtual void drawModulation(MIP_PaintContext* AContext) {
    if (MDrawModulation) {
      MIP_Painter* painter = AContext->painter;
      __MIP_UNUSED double value = MValue;
      double modulation = MModulation;
      if (MDrawParameterValue) {
        MIP_Parameter* parameter = getParameter();
        if (parameter) {
          value = parameter->getValue();
          modulation = parameter->getModulation();
        }
      }
      char temp[16];
      sprintf(temp,"%.3f",modulation);
      MIP_DRect rect = MRect;
      rect.shrink(2);
      painter->fontSize(MModulationSize);
      painter->beginPath();
      painter->drawTextBox(rect,temp,MModulationAlignment,MModulationColor);
    }
  }

};

//----------------------------------------------------------------------
#endif



