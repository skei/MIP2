#ifndef mip_symbol_widget_included
#define mip_symbol_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets/mip_panel_widget.h"

//

class MIP_SymbolWidget
: public MIP_PanelWidget {

//------------------------------
private:
//------------------------------

    int32_t   MSymbol     = MIP_SYMBOL_FILL_TRI_DOWN;
    MIP_Color MColor      = MIP_Color(0.75);//MIP_COLOR_LIGHT_GRAY;
    float     MLineWidth  = 1.0;

//------------------------------
public:
//------------------------------

  MIP_SymbolWidget(MIP_DRect ARect, int32_t ASymbol = MIP_SYMBOL_NONE)
  : MIP_PanelWidget(ARect) {
    setWidgetName("MIP_SymbolWidget");
    //setHint("symbol");
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

  void on_widget_paint(MIP_PaintContext* AContext) override {
    fillBackground(AContext);
    MIP_Painter* painter = AContext->painter;
    MIP_DRect mrect = getRect();
    float midx = mrect.x + (mrect.w * 0.5);
    float midy = mrect.y + (mrect.h * 0.5);
    painter->beginPath();
    switch(MSymbol) {
      case MIP_SYMBOL_NONE:
        break;
      case MIP_SYMBOL_HLINE:
        painter->moveTo(mrect.x,midy/*mrect.y*/);
        painter->lineTo(mrect.x2(),midy/*mrect.y*/);
        painter->strokeWidth(MLineWidth);
        painter->strokeColor(MColor);
        painter->stroke();
        break;
      case MIP_SYMBOL_VLINE:
        painter->moveTo(midx/*mrect.x*/,mrect.y);
        painter->lineTo(midx/*mrect.x*/,mrect.y2());
        painter->strokeColor(MColor);
        painter->strokeWidth(MLineWidth);
        painter->stroke();
        break;
      case MIP_SYMBOL_LINE: //
        painter->moveTo(mrect.x,mrect.y);
        painter->lineTo(mrect.x2(),mrect.y2());
        painter->strokeColor(MColor);
        painter->strokeWidth(MLineWidth);
        painter->stroke();
        break;
      case MIP_SYMBOL_LINE2: //
        painter->moveTo(mrect.x2(),mrect.y);
        painter->lineTo(mrect.x,mrect.y2());
        painter->strokeColor(MColor);
        painter->strokeWidth(MLineWidth);
        painter->stroke();
        break;
      case MIP_SYMBOL_CROSS: // X
        painter->moveTo(mrect.x,mrect.y);
        painter->lineTo(mrect.x2(),mrect.y2());
        painter->moveTo(mrect.x2(),mrect.y);
        painter->lineTo(mrect.x,mrect.y2());
        painter->strokeColor(MColor);
        painter->strokeWidth(MLineWidth);
        painter->stroke();
        break;
      case MIP_SYMBOL_PLUS:  // +
        painter->moveTo(midx,mrect.y);
        painter->lineTo(midx,mrect.y2());
        painter->moveTo(mrect.x,midy);
        painter->lineTo(mrect.x2(),midy);
        painter->strokeColor(MColor);
        painter->strokeWidth(MLineWidth);
        painter->stroke();
        break;
      case MIP_SYMBOL_RECT:
        painter->rect(mrect.x,mrect.y,mrect.w,mrect.h);
        painter->strokeColor(MColor);
        painter->strokeWidth(MLineWidth);
        painter->stroke();
        break;
      case MIP_SYMBOL_FILL_RECT:
        painter->fillColor(MColor);
        painter->rect(mrect.x,mrect.y,mrect.w,mrect.h);
        painter->fill();
        break;
      case MIP_SYMBOL_DIAMOND:
        painter->moveTo(midx,mrect.y);
        painter->lineTo(mrect.x2(),midy);
        painter->lineTo(midx,mrect.y2());
        painter->lineTo(mrect.x,midy);
        painter->lineTo(midx,mrect.y);
        painter->strokeColor(MColor);
        painter->strokeWidth(MLineWidth);
        painter->stroke();
        break;
      case MIP_SYMBOL_FILL_DIAMOND:
        painter->moveTo(midx,mrect.y);
        painter->lineTo(mrect.x2(),midy);
        painter->lineTo(midx,mrect.y2());
        painter->lineTo(mrect.x,midy);
        painter->lineTo(midx,mrect.y);
        painter->fillColor(MColor);
        painter->fill();
        break;
      case MIP_SYMBOL_TRI_UP:
        painter->triangle( midx, mrect.y, mrect.x2(), mrect.y2(), mrect.x, mrect.y2() );
        painter->strokeColor(MColor);
        painter->strokeWidth(MLineWidth);
        painter->stroke();
        break;
      case MIP_SYMBOL_TRI_DOWN:
        painter->triangle( mrect.x, mrect.y, mrect.x2(), mrect.y, midx, mrect.y2() );
        painter->strokeColor(MColor);
        painter->strokeWidth(MLineWidth);
        painter->stroke();
        break;
      case MIP_SYMBOL_TRI_LEFT:
        painter->triangle( mrect.x, mrect.y, mrect.x2(), midy, mrect.x, mrect.y2() );
        painter->strokeColor(MColor);
        painter->strokeWidth(MLineWidth);
        painter->stroke();
        break;
      case MIP_SYMBOL_TRI_RIGHT:
        painter->triangle( mrect.x2(), mrect.y, mrect.x2(), mrect.y2(), mrect.x, midy );
        painter->strokeColor(MColor);
        painter->strokeWidth(MLineWidth);
        painter->stroke();
        break;
      case MIP_SYMBOL_FILL_TRI_UP:
        painter->triangle( midx, mrect.y, mrect.x2(), mrect.y2(), mrect.x, mrect.y2() );
        painter->fillColor(MColor);
        painter->fill();
        break;
      case MIP_SYMBOL_FILL_TRI_DOWN:
        painter->triangle( mrect.x, mrect.y, mrect.x2(), mrect.y, midx, mrect.y2() );
        painter->fillColor(MColor);
        painter->fill();
        break;
      case MIP_SYMBOL_FILL_TRI_LEFT:
        painter->triangle( mrect.x, mrect.y, mrect.x2(), midy, mrect.x, mrect.y2() );
        painter->fillColor(MColor);
        painter->fill();
        break;
      case MIP_SYMBOL_FILL_TRI_RIGHT:
        painter->triangle( mrect.x2(), mrect.y, mrect.x2(), mrect.y2(), mrect.x, midy );
        painter->fillColor(MColor);
        painter->fill();
        break;
      case MIP_SYMBOL_ELLIPSE:
        painter->ellipse(mrect);
        painter->strokeColor(MColor);
        painter->strokeWidth(MLineWidth);
        painter->stroke();
        break;
      case MIP_SYMBOL_FILL_ELLIPSE:
        painter->ellipse(mrect);
        painter->fillColor(MColor);
        painter->fill();
        break;
      //case MIP_SYMBOL_TRI_DOWN_RIGHT:
      //  painter->drawTriangle( mrect.x2(), mrect.y, mrect.x2(), mrect.y2(), mrect.x, mrect.y2(), MColor );
      //  break;
      //case MIP_SYMBOL_FILL_TRI_DOWN_RIGHT:
      //  painter->fillTriangle( mrect.x2(), mrect.y, mrect.x2(), mrect.y2(), mrect.x, mrect.y2(), MColor );
      //  break;

    }
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif
