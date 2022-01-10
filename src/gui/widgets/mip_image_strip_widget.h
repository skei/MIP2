#ifndef mip_template_widget_includedbutton
#define mip_template_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/mip_drag_value_widget.h"

MIP_Surface*    MSurface           = nullptr;
//bool            MImageSurfaceAllocated  = false;
//uint32_t        MTileCount              = 0;
uint32_t        MTileXCount             = 0;
uint32_t        MTileYCount             = 0;
uint32_t        MTileWidth              = 0;
uint32_t        MTileHeight             = 0;


//----------

class MIP_ImageStripWidget
: public MIP_DragValueWidget {

//------------------------------
public:
//------------------------------

  MIP_ImageStripWidget(MIP_FRect ARect)
  : MIP_DragValueWidget(ARect) {
    setName("MIP_ImageStripWidget");
    setHint("imagestrip");
    setFillBackground(false);
    setDrawBorder(false);
  }

//------------------------------
public:
//------------------------------

//  virtual void setImage(MIP_Drawable* ATarget, MIP_Surface* ASurface) {
//    MImageSurface = ASurface;
//    MImageSurfaceAllocated = false;
//  }
//
//  virtual void setImage(MIP_Drawable* ATarget, MIP_Bitmap* ABitmap) {
//    MImageSurface = new MIP_Surface(ATarget,ABitmap->getWidth(),ABitmap->getHeight());
//    MImageSurfaceAllocated = true;
//    MIP_Painter* painter = new MIP_Painter(MImageSurface);
//    painter->uploadBitmap(0,0,ABitmap);
//    //painter->flush();
//    delete painter;
//  }
//
//  virtual void setImage(MIP_Drawable* ATarget, const uint8_t* ABuffer, uint32_t ASize, MIP_Color ABackground) {
//    MIP_Bitmap* bitmap = new MIP_Bitmap(ABuffer,ASize);
//    bitmap->premultAlpha( (uint32_t)ABackground );
//    setImage(ATarget,bitmap);
//    delete bitmap;
//  }
//
//  virtual void setImage(MIP_Drawable* ATarget, const char* AFilename, MIP_Color ABackground) {
//    MIP_Bitmap* bitmap = new MIP_Bitmap(AFilename);
//    bitmap->premultAlpha( (uint32_t)ABackground );
//    setImage(ATarget,bitmap);
//    delete bitmap;
//  }

  //----------

  virtual void setup(uint32_t AXcount, uint32_t AYcount, MIP_Surface* ASurface=nullptr) {
    if (ASurface) MSurface = ASurface;
    MTileXCount = AXcount;
    MTileYCount = AYcount;
    MTileWidth  = MSurface->getWidth() / AXcount;
    MTileHeight = MSurface->getHeight() / AYcount;
  }

  MIP_FRect getTileRect(uint32_t AIndex) {
    float x = floorf(AIndex % MTileXCount) * MTileWidth;
    float y = floorf(AIndex / MTileXCount) * MTileHeight;
    float w = MTileWidth - 1;
    float h = MTileHeight - 1;
    return MIP_FRect(x,y,w,h);
  }

  virtual void drawTile(MIP_BasePainter* APainter) {
    //MIP_Surface* surface = getImageSurface();
    if (MSurface) {
      uint32_t num_tiles = MTileXCount * MTileYCount;
      if (num_tiles > 0) {
        float v = (float)getValue() * (float)num_tiles;
        uint32_t tile = MIP_MinI( num_tiles - 1, floorf(v) );
        MIP_FRect rect = getTileRect(tile);
        APainter->drawBitmap(getRect().x,getRect().y,MSurface,rect);
      }
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    drawTile(APainter);
    drawBorder(APainter,ARect,AMode);
  }

};

//----------------------------------------------------------------------
#endif
