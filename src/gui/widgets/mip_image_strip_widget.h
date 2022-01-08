#ifndef kode_template_widget_includedbutton
#define kode_template_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_drag_value_widget.h"

//----------

class KODE_ImageStripWidget
: public KODE_DragValueWidget {

//------------------------------
public:
//------------------------------

  KODE_ImageStripWidget(KODE_FRect ARect)
  : KODE_DragValueWidget(ARect) {
    setName("KODE_ImageStripWidget");
    setHint("imagestrip");
    setFillBackground(false);
    setDrawBorder(false);
  }

//------------------------------
public:
//------------------------------

  virtual void drawTile(KODE_BasePainter* APainter) {
    KODE_Surface* surface = getImageSurface();
    if (surface) {
    uint32_t num_tiles = getTileXCount() * getTileYCount();
    if (num_tiles > 0) {
      float v = (float)getValue() * (float)num_tiles;
      uint32_t tile = KODE_MinI( num_tiles - 1, floorf(v) );
      KODE_FRect rect = getTileRect(tile);
      APainter->drawBitmap(getRect().x,getRect().y,surface,rect);
    }
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    drawTile(APainter);
    drawBorder(APainter,ARect,AMode);
  }

};

//----------------------------------------------------------------------
#endif
