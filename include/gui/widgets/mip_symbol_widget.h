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

    int32_t   MSymbol     = MIP_SYMBOL_FILL_TRI_DOWN;
    MIP_Color MColor      = MIP_COLOR_LIGHT_GRAY;
    float     MLineWidth  = 1.0;

//------------------------------
public:
//------------------------------

  MIP_SymbolWidget(MIP_FRect ARect, int32_t ASymbol = MIP_SYMBOL_NONE)
  : MIP_PanelWidget(ARect) {
    setName("MIP_SymbolWidget");
    setHint("symbol");
    MSymbol = ASymbol;
    //setFillBackground(false);
    setDrawBorder(false);
  }

  virtual ~MIP_SymbolWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setSymbol(int32_t ASymbol) { MSymbol = ASymbol; }
  virtual void setColor(MIP_Color AColor) { MColor = AColor; }
  virtual void setLineWidth(float AWidth) { MLineWidth = AWidth; }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    MIP_FRect mrect = getRect();
    float midx = mrect.x + (mrect.w * 0.5);
    float midy = mrect.y + (mrect.h * 0.5);
    switch(MSymbol) {
      case MIP_SYMBOL_NONE:
        break;
      case MIP_SYMBOL_HLINE:
        APainter->moveTo(mrect.x,midy/*mrect.y*/);
        APainter->lineTo(mrect.x2(),midy/*mrect.y*/);
        APainter->setColor(MColor);
        APainter->setLineWidth(MLineWidth);
        APainter->strokePath();
        break;
      case MIP_SYMBOL_VLINE:
        APainter->moveTo(midx/*mrect.x*/,mrect.y);
        APainter->lineTo(midx/*mrect.x*/,mrect.y2());
        APainter->setColor(MColor);
        APainter->setLineWidth(MLineWidth);
        APainter->strokePath();
        break;
      case MIP_SYMBOL_LINE: //
        APainter->moveTo(mrect.x,mrect.y);
        APainter->lineTo(mrect.x2(),mrect.y2());
        APainter->setColor(MColor);
        APainter->setLineWidth(MLineWidth);
        APainter->strokePath();
        break;
      case MIP_SYMBOL_LINE2: //
        APainter->moveTo(mrect.x2(),mrect.y);
        APainter->lineTo(mrect.x,mrect.y2());
        APainter->setColor(MColor);
        APainter->setLineWidth(MLineWidth);
        APainter->strokePath();
        break;
      case MIP_SYMBOL_CROSS: // X
        APainter->moveTo(mrect.x,mrect.y);
        APainter->lineTo(mrect.x2(),mrect.y2());
        APainter->moveTo(mrect.x2(),mrect.y);
        APainter->lineTo(mrect.x,mrect.y2());
        APainter->setColor(MColor);
        APainter->setLineWidth(MLineWidth);
        APainter->strokePath();
        break;
      case MIP_SYMBOL_PLUS:  // +
        APainter->moveTo(midx,mrect.y);
        APainter->lineTo(midx,mrect.y2());
        APainter->moveTo(mrect.x,midy);
        APainter->lineTo(mrect.x2(),midy);
        APainter->setColor(MColor);
        APainter->setLineWidth(MLineWidth);
        APainter->strokePath();
        break;
      case MIP_SYMBOL_RECT:
        APainter->rectangle(mrect);
        APainter->setColor(MColor);
        APainter->setLineWidth(MLineWidth);
        APainter->strokePath();
        break;
      case MIP_SYMBOL_FILL_RECT:
        APainter->setColor(MColor);
        APainter->rectangle(mrect);
        APainter->fillPath();
        break;
      case MIP_SYMBOL_DIAMOND:
        APainter->moveTo(midx,mrect.y);
        APainter->lineTo(mrect.x2(),midy);
        APainter->lineTo(midx,mrect.y2());
        APainter->lineTo(mrect.x,midy);
        APainter->lineTo(midx,mrect.y);
        APainter->setColor(MColor);
        APainter->setLineWidth(MLineWidth);
        APainter->strokePath();
        break;
      case MIP_SYMBOL_FILL_DIAMOND:
        APainter->moveTo(midx,mrect.y);
        APainter->lineTo(mrect.x2(),midy);
        APainter->lineTo(midx,mrect.y2());
        APainter->lineTo(mrect.x,midy);
        APainter->lineTo(midx,mrect.y);
        APainter->setColor(MColor);
        APainter->fillPath();
        break;
      case MIP_SYMBOL_TRI_UP:
        APainter->triangle( midx, mrect.y, mrect.x2(), mrect.y2(), mrect.x, mrect.y2() );
        APainter->setColor(MColor);
        APainter->setLineWidth(MLineWidth);
        APainter->strokePath();
        break;
      case MIP_SYMBOL_TRI_DOWN:
        APainter->triangle( mrect.x, mrect.y, mrect.x2(), mrect.y, midx, mrect.y2() );
        APainter->setColor(MColor);
        APainter->setLineWidth(MLineWidth);
        APainter->strokePath();
        break;
      case MIP_SYMBOL_TRI_LEFT:
        APainter->triangle( mrect.x, mrect.y, mrect.x2(), midy, mrect.x, mrect.y2() );
        APainter->setColor(MColor);
        APainter->setLineWidth(MLineWidth);
        APainter->strokePath();
        break;
      case MIP_SYMBOL_TRI_RIGHT:
        APainter->triangle( mrect.x2(), mrect.y, mrect.x2(), mrect.y2(), mrect.x, midy );
        APainter->setColor(MColor);
        APainter->setLineWidth(MLineWidth);
        APainter->strokePath();
        break;
      case MIP_SYMBOL_FILL_TRI_UP:
        APainter->triangle( midx, mrect.y, mrect.x2(), mrect.y2(), mrect.x, mrect.y2() );
        APainter->setColor(MColor);
        APainter->fillPath();
        break;
      case MIP_SYMBOL_FILL_TRI_DOWN:
        APainter->triangle( mrect.x, mrect.y, mrect.x2(), mrect.y, midx, mrect.y2() );
        APainter->setColor(MColor);
        APainter->fillPath();
        break;
      case MIP_SYMBOL_FILL_TRI_LEFT:
        APainter->triangle( mrect.x, mrect.y, mrect.x2(), midy, mrect.x, mrect.y2() );
        APainter->setColor(MColor);
        APainter->fillPath();
        break;
      case MIP_SYMBOL_FILL_TRI_RIGHT:
        APainter->triangle( mrect.x2(), mrect.y, mrect.x2(), mrect.y2(), mrect.x, midy );
        APainter->setColor(MColor);
        APainter->fillPath();
        break;
      case MIP_SYMBOL_ELLIPSE:
        APainter->ellipse(mrect);
        APainter->setColor(MColor);
        APainter->setLineWidth(MLineWidth);
        APainter->strokePath();
        break;
      case MIP_SYMBOL_FILL_ELLIPSE:
        APainter->fillEllipse(mrect,MColor);
        APainter->setColor(MColor);
        APainter->fillPath();
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
