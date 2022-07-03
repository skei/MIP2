#ifndef mip_image_widget_included
#define mip_image_widget_included
//----------------------------------------------------------------------

#define MIP_IMAGE_WIDGET_DRAW_NONE     0
#define MIP_IMAGE_WIDGET_DRAW_SURFACE  1
#define MIP_IMAGE_WIDGET_DRAW_BITMAP   2

#include "gui/widgets/mip_panel_widget.h"
//#include "gui/mip_widgets.h"


class MIP_ImageWidget
: public MIP_PanelWidget {

//------------------------------
private:
//------------------------------

  MIP_Surface*  MImageSurface = nullptr;
  MIP_FRect     MImageRect    = {};
  uint32_t      MTileXCount   = 1;
  uint32_t      MTileYCount   = 1;
  float         MTileWidth    = 0.0;
  float         MTileHeight   = 0.0;
  uint32_t      MTile         = 0;

//------------------------------
public:
//------------------------------

  MIP_ImageWidget(MIP_FRect ARect, MIP_Surface* ASurface)
  : MIP_PanelWidget(ARect) {
    MImageRect    = MIP_FRect(0,0,ASurface->getWidth(),ASurface->getHeight());
    MImageSurface = ASurface;
    setName("MIP_ImageWidget");
    setHint("image");
    setFillBackground(false);
    setDrawBorder(false);
  }

  //----------

  MIP_ImageWidget(MIP_FRect ARect, MIP_Surface* ASurface, MIP_FRect AImageRect)
  : MIP_PanelWidget(ARect) {
    MImageRect    = AImageRect;
    MImageSurface = ASurface;
    setName("MIP_ImageWidget");
    setHint("image");
    setFillBackground(false);
    setDrawBorder(false);
  }

  //----------

  MIP_ImageWidget(MIP_FRect ARect, MIP_Surface* ASurface, uint32_t AXcount, uint32_t AYcount)
  : MIP_PanelWidget(ARect) {
    MImageRect    = ARect;
    MImageSurface = ASurface;
    MTileXCount   = AXcount;
    MTileYCount   = AYcount;
    MTileWidth    = ARect.w / AXcount;
    MTileHeight   = ARect.h / AYcount;
    setName("MIP_ImageWidget");
    setHint("image");
    setFillBackground(false);
    setDrawBorder(false);
  }

  //----------

  MIP_ImageWidget(MIP_FRect ARect, MIP_Surface* ASurface, MIP_FRect AImageRect, uint32_t AXcount, uint32_t AYcount)
  : MIP_PanelWidget(ARect) {
    MImageRect    = AImageRect;
    MImageSurface = ASurface;
    MTileXCount   = AXcount;
    MTileYCount   = AYcount;
    MTileWidth    = AImageRect.w / AXcount;
    MTileHeight   = AImageRect.h / AYcount;
    setName("MIP_ImageWidget");
    setHint("image");
    setFillBackground(false);
    setDrawBorder(false);
  }

//------------------------------
public:
//------------------------------

  virtual void setTile(uint32_t AIndex) {
    MTile = AIndex;
  }

  virtual uint32_t getTile() {
    return MTile;
  }

  virtual MIP_FRect getTileRect(uint32_t AIndex) {
    float x = MImageRect.x + (floorf(AIndex % MTileXCount) * MTileWidth);
    float y = MImageRect.y + (floorf(AIndex / MTileXCount) * MTileHeight);
    float w = MTileWidth;
    float h = MTileHeight;
    return MIP_FRect(x,y,w,h);
  }

  //----------

  virtual void drawTile(MIP_Painter* APainter, uint32_t AIndex) {
    if (MImageSurface) {
      MIP_FRect rect = getTileRect(AIndex);
      APainter->drawImage(MRect,MImageSurface,rect);
    }
  }

  virtual void drawImage(MIP_Painter* APainter) {
    if (MImageSurface) {
      APainter->drawImage(MRect,MImageSurface,MImageRect);
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    if ((MTileXCount > 1) || (MTileYCount > 1)) {
        drawTile(APainter,MTile);
    }
    else {
      drawImage(APainter);
    }
    paintWidgets(APainter,ARect,AMode);
    drawBorder(APainter,ARect,AMode);
  }

//------------------------------

};

//----------------------------------------------------------------------
#endif
