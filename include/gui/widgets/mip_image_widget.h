#ifndef mip_image_widget_included
#define mip_image_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gfx/mip_bitmap.h"
#include "gui/widgets/mip_panel_widget.h"
#include "gui/mip_surface.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ImageWidget
: public MIP_PanelWidget {

//------------------------------
protected:
//------------------------------

  bool          MDrawImage    = true;
  //MIP_Bitmap*   MBitmap       = nullptr;
  uint32_t*     MImageBuffer  = nullptr;
  uint32_t      MImageWidth   = 0;
  uint32_t      MImageHeight  = 0;

  bool          MImageValid   = false;
  int           MImage        = -1;

  MIP_DRect     MImageRect    = {};
  uint32_t      MTileXCount   = 1;
  uint32_t      MTileYCount   = 1;
  float         MTileWidth    = 0.0;
  float         MTileHeight   = 0.0;
  uint32_t      MTile         = 0;

//------------------------------
public:
//------------------------------

  MIP_ImageWidget(MIP_DRect ARect, uint32_t* buffer, uint32_t width, uint32_t height)
  : MIP_PanelWidget(ARect) {
    MName = "MIP_TextWidget";
    //setDrawRoundedCorners(false);
    //MBitmap = ABitmap;
    MImageBuffer = buffer;
    MImageWidth  = width;
    MImageHeight = height;
    MImageValid  = false;
  }

  //----------

  MIP_ImageWidget(MIP_DRect ARect, MIP_Bitmap* ABitmap)
  : MIP_PanelWidget(ARect) {
    //MBitmap = ABitmap;
    MImageBuffer = ABitmap->getBuffer();
    MImageWidth  = ABitmap->getWidth();
    MImageHeight = ABitmap->getHeight();
    MImageValid  = false;
  }


  //----------

  virtual ~MIP_ImageWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void  setDrawImage(bool ADraw=true) {
    MDrawImage = ADraw;
  }

//  virtual void invalidateImage() {
//    MImageValue = false;
//  }

  //----------

  virtual void setTiles(uint32_t AXcount, uint32_t AYcount) {
    MTileXCount   = AXcount;
    MTileYCount   = AYcount;
    MTileWidth    = MImageWidth / AXcount;
    MTileHeight   = MImageHeight / AYcount;
  }

  virtual MIP_DRect getTileRect(uint32_t AIndex) {
    float x = /*MImageRect.x*/ 0  + (floorf(AIndex % MTileXCount) * MTileWidth);
    float y = /*MImageRect.y*/ 0  + (floorf(AIndex / MTileXCount) * MTileHeight);
    float w = MTileWidth;
    float h = MTileHeight;
    return MIP_DRect(x,y,w,h);
  }

//------------------------------
public: // parent to child
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    //fillBackground(AContext);
    //paintChildWidgets(AContext);
    drawImage(AContext);
    //drawBorder(AContext);
  }

  void on_widget_config(MIP_Widget* AOwnerWindow) override {
    //MIP_PRINT;
    MImageValid = false;
    MIP_PanelWidget::on_widget_config(AOwnerWindow);

//    if (MImageHandle < 0) {
//      MIP_PRINT;
//      MIP_Window* window = (MIP_Window*)AOwnerWindow;
//      if (window) {
//        MIP_PRINT;
//        MIP_Painter* painter = window->getPainter();
//        if (painter) {
//          MIP_Print("MImageWidth %i MImageeHeight %i MImageBuffer %p\n",MImageWidth,MImageHeight,MImageBuffer);
//          MImageHandle = painter->createImageRGBA(MImageWidth,MImageHeight,0,(uint8_t*)MImageBuffer);
//        }
//      }
//    }
//    MIP_PRINT;

  }

//------------------------------
public:
//------------------------------

  virtual void drawImage(MIP_PaintContext* AContext) {
    if (MDrawImage) {
      MIP_Painter* painter = AContext->painter;
      MIP_DRect rect = MRect;

      if (!MImageValid) {
        MImage = painter->createImageRGBA(MImageWidth,MImageHeight,0,(uint8_t*)MImageBuffer);
        MImageValid = true;
      }

//      if (MImageHandle != -1) {
//        painter->updateImage(MImageHandle,(uint8_t*)MImageBuffer);
//        //painter->deleteImage(image);
//      }
//      else {
//        //int image = painter->createImageRGBA(MImageWidth,MImageHeight,0,(uint8_t*)MImageBuffer);
//        MImageHandle = painter->createImageRGBA(MImageWidth,MImageHeight,0,(uint8_t*)MImageBuffer);
//      }

//      if (MImageHandle < 0) {
//        MImageHandle = painter->createImageRGBA(MImageWidth,MImageHeight,0,(uint8_t*)MImageBuffer);
//      }

      float ox = rect.x;        // 160
      float oy = rect.y;        // 0
      float ex = MImageWidth;   // 320
      float ey = MImageHeight;  // 240
      MIP_PaintSource paint = painter->imagePattern(ox,oy,ex,ey,0,MImage,1.0);

      painter->beginPath();
      painter->rect(rect.x,rect.y,rect.w,rect.h);
      painter->fillPaint(paint);
      //painter->fillPaint(0);
      painter->fill();

      //painter->deleteImage(image);

    }
  }

};

//----------------------------------------------------------------------
#endif



