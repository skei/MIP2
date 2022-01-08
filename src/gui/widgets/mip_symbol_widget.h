#ifndef kode_symbol_widget_included
#define kode_symbol_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widgets.h"

//

class KODE_SymbolWidget
: public KODE_PanelWidget {

//------------------------------
private:
//------------------------------

    int32_t     MSymbol = KODE_SYMBOL_FILL_TRI_DOWN;
    KODE_Color  MColor  = KODE_COLOR_LIGHT_GRAY;

//------------------------------
public:
//------------------------------

  KODE_SymbolWidget(KODE_FRect ARect, int32_t ASymbol = KODE_SYMBOL_NONE)
  : KODE_PanelWidget(ARect) {
    setName("KODE_SymbolWidget");
    setHint("symbol");
    MSymbol = ASymbol;
    setFillBackground(false);
    setDrawBorder(false);
  }

  virtual ~KODE_SymbolWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setSymbol(int32_t ASymbol) { MSymbol = ASymbol; }
  virtual void setColor(KODE_Color AColor) { MColor = AColor; }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    KODE_FRect mrect = getRect();
    switch(MSymbol) {
      case KODE_SYMBOL_NONE:
        break;
      case KODE_SYMBOL_HLINE:
        APainter->drawLine(mrect.x,mrect.y,mrect.x2(),mrect.y,MColor);
        break;
      case KODE_SYMBOL_VLINE:
        APainter->drawLine(mrect.x,mrect.y,mrect.x,mrect.y2(),MColor);
        break;
      case KODE_SYMBOL_LINE:
        APainter->drawLine(mrect.x,mrect.y,mrect.x2(),mrect.y2(),MColor);
        break;
      case KODE_SYMBOL_RECT:
        APainter->drawRectangle(mrect,MColor);
        break;
      case KODE_SYMBOL_FILL_RECT:
        APainter->fillRectangle(mrect,MColor);
        break;
      case KODE_SYMBOL_TRI_DOWN:
        APainter->drawTriangle( mrect.x, mrect.y, mrect.x2(), mrect.y, mrect.x + (mrect.w/2), mrect.y2(), MColor );
        break;
      case KODE_SYMBOL_FILL_TRI_DOWN:
        APainter->fillTriangle( mrect.x, mrect.y, mrect.x2(), mrect.y, mrect.x + (mrect.w/2), mrect.y2(), MColor );
        break;
      case KODE_SYMBOL_ELLIPSE:
        APainter->drawEllipse(mrect, MColor);
        break;
      case KODE_SYMBOL_FILL_ELLIPSE:
        APainter->fillEllipse(mrect,MColor);
        break;
    }
    drawBorder(APainter,ARect,AMode);
  }

};

//----------------------------------------------------------------------
#endif
