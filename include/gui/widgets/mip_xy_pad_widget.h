#ifndef mip_xy_pad_widget_included
#define mip_xy_pad_widget_included
//----------------------------------------------------------------------

//#include "gui/mip_widgets.h"
#include "mip.h"
#include "gui/widgets/mip_panel_widget.h"

//----------------------------------------------------------------------

class MIP_XYPadWidget
: public MIP_PanelWidget {

//------------------------------
protected:
//------------------------------

  float MValue2 = 0.0;
  bool  MIsDragging = false;
  float MMouseX = 0.0;
  float MMouseY = 0.0;
  float MSensitivity = 0.005;

  //const char* MText           = "";
  //float       MTextSize       = 12;
  //uint32_t    MTextAlignment  = MIP_TEXT_ALIGN_LEFT;
  //MIP_Color   MTextColor      = MIP_COLOR_BLACK;
  //MIP_FRect   MTextOffset     = MIP_FRect(2,2,2,2);
  //bool        MDrawText       = true;
  //bool        MDrawParamName  = true;

//------------------------------
public:
//------------------------------

  MIP_XYPadWidget(MIP_FRect ARect/*, const char* AText=""*/)
  : MIP_PanelWidget(ARect) {
    setName("MIP_XYPadWidget");
    setHint("xypad");
    //MText             = AText;
    MDrawBorder       = false;
    MFillBackground   = true;
    MBackgroundColor  = MIP_COLOR_DARK_GRAY;

    MValue = 0.3;
    MValue2 = 0.7;

  }

  virtual ~MIP_XYPadWidget() {
  }

//------------------------------
public:
//------------------------------

  //virtual void setText(const char* AText)             { MText = AText; }
  //virtual void setDrawText(bool ADraw)                { MDrawText = ADraw; }
  //virtual void setDrawParamName(bool ADraw)           { MDrawParamName = ADraw; }
  //virtual void setTextAlignment(uint32_t AAlignment)  { MTextAlignment = AAlignment; }
  //virtual void setTextColor(MIP_Color AColor)         { MTextColor = AColor; }
  //virtual void setTextOffset(MIP_FRect AOffset)       { MTextOffset = AOffset; }
  //virtual void setTextOffset(float x)                 { MTextOffset = MIP_FRect(x,x,x,x); }
  //virtual void setTextOffset(float x, float y)        { MTextOffset = MIP_FRect(x,y); }
  //virtual void setTextSize(float ASize)               { MTextSize = ASize; }

  //----------

  //virtual void drawText(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
  //  MIP_FRect rect = getRect();
  //  rect.shrink(MTextOffset);
  //  if (MDrawText) {
  //    const char* txt = MText;
  //    if (MDrawParamName && (MParamIndex >= 0)) txt = MParamName;
  //   APainter->setFontSize(MTextSize);
  //    APainter->setColor(MTextColor);
  //    APainter->drawText(rect,txt,MTextAlignment);
  //  }
  //}

  virtual void drawPad(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
    float x = MRect.x + (MValue * MRect.w);
    float y = MRect.y + (MValue2 * MRect.h);
    APainter->moveTo(x,MRect.y);
    APainter->lineTo(x,MRect.y2());
    APainter->moveTo(MRect.x,y);
    APainter->lineTo(MRect.x2(),y);
    APainter->setColor(MIP_COLOR_LIGHT_GRAY);
    APainter->setLineWidth(2);
    APainter->strokePath();
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    drawPad(APainter,ARect,AMode);
    drawBorder(APainter,ARect,AMode);
  }

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    MIsDragging = true;
    MMouseX = AXpos;
    MMouseY = AYpos;
  }

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    MIsDragging = false;
  }

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp) override {
    if (MIsDragging) {
      float deltax = AXpos - MMouseX;
      float deltay = AYpos - MMouseY;
      MValue += (deltax * MSensitivity);
      MValue2 += (deltay * MSensitivity);
      MValue = MIP_Clamp(MValue,0,1);
      MValue2 = MIP_Clamp(MValue2,0,1);
      MMouseX = AXpos;
      MMouseY = AYpos;
      do_widget_update(this,0);
      do_widget_redraw(this,MRect,0);
    }
  }



//------------------------------

};

//----------------------------------------------------------------------
#endif


