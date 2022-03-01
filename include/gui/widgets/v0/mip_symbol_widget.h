#ifndef mip_symbol_widget_included
#define mip_symbol_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widgets.h"

//

class MIP_SymbolWidget
: public MIP_PanelWidget {

//------------------------------
private:
//------------------------------

    int32_t     MSymbol = MIP_SYMBOL_FILL_TRI_DOWN;
    MIP_Color  MColor  = MIP_COLOR_LIGHT_GRAY;

//------------------------------
public:
//------------------------------

  MIP_SymbolWidget(MIP_FRect ARect, int32_t ASymbol = MIP_SYMBOL_NONE)
  : MIP_PanelWidget(ARect) {
    setName("MIP_SymbolWidget");
    setHint("symbol");
    MSymbol = ASymbol;
    setFillBackground(false);
    setDrawBorder(false);
  }

  virtual ~MIP_SymbolWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setSymbol(int32_t ASymbol) { MSymbol = ASymbol; }
  virtual void setColor(MIP_Color AColor) { MColor = AColor; }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    MIP_FRect mrect = getRect();
    switch(MSymbol) {
      case MIP_SYMBOL_NONE:
        break;
      case MIP_SYMBOL_HLINE:
        APainter->drawLine(mrect.x,mrect.y,mrect.x2(),mrect.y,MColor);
        break;
      case MIP_SYMBOL_VLINE:
        APainter->drawLine(mrect.x,mrect.y,mrect.x,mrect.y2(),MColor);
        break;
      case MIP_SYMBOL_LINE:
        APainter->drawLine(mrect.x,mrect.y,mrect.x2(),mrect.y2(),MColor);
        break;
      case MIP_SYMBOL_RECT:
        APainter->drawRectangle(mrect,MColor);
        break;
      case MIP_SYMBOL_FILL_RECT:
        APainter->fillRectangle(mrect,MColor);
        break;
      case MIP_SYMBOL_TRI_DOWN:
        APainter->drawTriangle( mrect.x, mrect.y, mrect.x2(), mrect.y, mrect.x + (mrect.w/2), mrect.y2(), MColor );
        break;
      case MIP_SYMBOL_FILL_TRI_DOWN:
        APainter->fillTriangle( mrect.x, mrect.y, mrect.x2(), mrect.y, mrect.x + (mrect.w/2), mrect.y2(), MColor );
        break;
      case MIP_SYMBOL_ELLIPSE:
        APainter->drawEllipse(mrect, MColor);
        break;
      case MIP_SYMBOL_FILL_ELLIPSE:
        APainter->fillEllipse(mrect,MColor);
        break;
      //case MIP_SYMBOL_TRI_DOWN_RIGHT:
      //  APainter->drawTriangle( mrect.x2(), mrect.y, mrect.x2(), mrect.y2(), mrect.x, mrect.y2(), MColor );
      //  break;
      //case MIP_SYMBOL_FILL_TRI_DOWN_RIGHT:
      //  APainter->fillTriangle( mrect.x2(), mrect.y, mrect.x2(), mrect.y2(), mrect.x, mrect.y2(), MColor );
      //  break;

    }
    drawBorder(APainter,ARect,AMode);
  }

};

//----------------------------------------------------------------------
#endif
