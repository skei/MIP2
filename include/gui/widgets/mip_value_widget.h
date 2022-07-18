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

  bool      MDrawValue            = true;
  double    MValue                = 0.0;
  MIP_Color MValueColor           = MIP_COLOR_BLACK;
  uint32_t  MValueAlignment       = MIP_TEXT_ALIGN_RIGHT;
  float     MValueSize            = 13.0;

  bool      MDrawModulation       = false;//true;
  double    MModulation           = 0.0;
  MIP_Color MModulationColor      = MIP_COLOR_WHITE;
  uint32_t  MModulationAlignment  = MIP_TEXT_ALIGN_CENTER;
  float     MModulationSize       = 10.0;

//------------------------------
public:
//------------------------------

  MIP_ValueWidget(MIP_DRect ARect, const char* AText, double AValue)
  : MIP_TextWidget(ARect,AText) {
    MValue = AValue;
    MTextAlignment = MIP_TEXT_ALIGN_LEFT;
  }

  //----------

  virtual ~MIP_ValueWidget() {
  }

//------------------------------
public:
//------------------------------

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

//------------------------------
public:
//------------------------------

  virtual void drawValue(MIP_PaintContext* AContext) {
    if (MDrawValue) {
      MIP_Painter* painter = AContext->painter;
      char temp[16];
      sprintf(temp,"%.3f",MValue);
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
      char temp[16];
      sprintf(temp,"%.3f",MModulation);
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



