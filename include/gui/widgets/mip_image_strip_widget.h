
#ifndef mip_image_strip_widget_included
#define mip_image_strip_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/mip_drag_value_widget.h"

class MIP_ImageStripWidget
: public MIP_DragValueWidget {

//------------------------------
protected:
//------------------------------

  bool          MDrawTile     = true;

  uint32_t      MTileCount    = 1;
  uint32_t      MTileXCount   = 1;
  uint32_t      MTileYCount   = 1;
  float         MTileWidth    = 0.0;
  float         MTileHeight   = 0.0;
  uint32_t      MTile         = 0;

  uint32_t*     MImageBuffer  = nullptr;
  uint32_t      MImageWidth   = 0;
  uint32_t      MImageHeight  = 0;
  bool          MImageValid   = false;
  int           MImage        = -1;

//------------------------------
public:
//------------------------------

  MIP_ImageStripWidget(MIP_DRect ARect, const char* AText, double AValue)
  : MIP_DragValueWidget(ARect,AText,AValue) {
    MName = "MIP_ImageStripWidget";
  }

  //----------

  virtual ~MIP_ImageStripWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setupImage(uint32_t* buffer, uint32_t width, uint32_t height) {
    MImageBuffer = buffer;
    MImageWidth  = width;
    MImageHeight = height;
    MImageValid  = false;
  }

  //----------

  virtual void setupImage(MIP_Bitmap* ABitmap) {
    MImageBuffer = ABitmap->getBuffer();
    MImageWidth  = ABitmap->getWidth();
    MImageHeight = ABitmap->getHeight();
    MImageValid  = false;
  }

  //----------

  virtual void validateImage(MIP_Painter* APainter) {
    if (!MImageValid) {
      MImage = APainter->createImageRGBA(MImageWidth,MImageHeight,0,(uint8_t*)MImageBuffer);
      //APainter->updateImage(MImageHandle,(uint8_t*)MImageBuffer);
      MImageValid = true;
    }
  }

  //----------

  virtual void invalidateImage() {
    MImageValid = false;
  }

  //----------


//------------------------------
public:
//------------------------------

  virtual void setupTiles(uint32_t AXcount, uint32_t AYcount) {
    MTileXCount   = AXcount;
    MTileYCount   = AYcount;
    MTileCount    = AXcount * AYcount;
    MTileWidth    = (float)MImageWidth / AXcount;
    MTileHeight   = (float)MImageHeight / AYcount;
  }

  //----------

  virtual MIP_DRect getTileRect(uint32_t AIndex) {
    float x = /*MImageRect.x*/ 0  + (floorf(AIndex % MTileXCount) * MTileWidth);
    float y = /*MImageRect.y*/ 0  + (floorf(AIndex / MTileXCount) * MTileHeight);
    float w = MTileWidth;
    float h = MTileHeight;
    return MIP_DRect(x,y,w,h);
  }

  //----------

  virtual void drawTile(MIP_PaintContext* AContext) {
    if (MDrawTile) {
      MIP_Painter* painter = AContext->painter;
      MIP_DRect rect = MRect;
      validateImage(painter);
      //float ox = rect.x;        // 160
      //float oy = rect.y;        // 0
      //float ex = MImageWidth;   // 320
      //float ey = MImageHeight;  // 240
      double index = getValue() * (double)(MTileCount - 1);
      uint32_t iindex = MIP_Trunc(index);
      MIP_DRect R = getTileRect(iindex);
      //MIP_Print("%.2f -> i : %.0f,%.0f, %.0f,%.0f\n",getValue(),iindex,R.x,R.y,R.w,R.h);
      float xscale = rect.w / MTileWidth;
      float yscale = rect.h / MTileHeight;
      float ox = rect.x - (R.x * xscale);
      float oy = rect.y - (R.y * yscale);
      float ex = MImageWidth * xscale;
      float ey = MImageHeight * yscale;
      MIP_PaintSource paint = painter->imagePattern(ox,oy,ex,ey,0,MImage,1.0);
      painter->beginPath();
      painter->rect(rect.x,rect.y,rect.w,rect.h);
      painter->fillPaint(paint);
      painter->fill();
    }
  }

//------------------------------
public: // parent to child
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    //fillBackground(AContext);
    //paintChildWidgets(AContext);
    drawTile(AContext);
    //drawBorder(AContext);
  }

  //----------

  void on_widget_config(MIP_Widget* AOwnerWindow) override {
    //MImageValid = false;
    invalidateImage();
    MIP_DragValueWidget::on_widget_config(AOwnerWindow);
  }

};

//----------------------------------------------------------------------
#endif
















#if 0

// deprecated?
// use MIP_ImageWidget and the tiling features instead

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
//    painter->drawBitmap(0,0,ABitmap);
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

  virtual void drawTile(MIP_Painter* APainter) {
    MIP_FRect mrect = getRect();
    //MIP_Surface* surface = getImageSurface();
    if (MSurface) {
      uint32_t num_tiles = MTileXCount * MTileYCount;
      if (num_tiles > 0) {
        float v = (float)getValue() * (float)num_tiles;
        uint32_t tile = MIP_MinI( num_tiles - 1, floorf(v) );
        MIP_FRect rect = getTileRect(tile);
        APainter->drawImage(mrect.x,mrect.y,MSurface,rect);
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

#endif // 0
