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

  //MIP_Bitmap*   MBitmap       = nullptr;

  bool          MDrawImage    = true;
  uint32_t*     MImageBuffer  = nullptr;
  uint32_t      MImageWidth   = 0;
  uint32_t      MImageHeight  = 0;
  bool          MImageValid   = false;
  int           MImage        = -1;

  //MIP_DRect     MImageRect    = {};
//  uint32_t      MTileCount    = 1;
//  uint32_t      MTileXCount   = 1;
//  uint32_t      MTileYCount   = 1;
//  float         MTileWidth    = 0.0;
//  float         MTileHeight   = 0.0;
//  uint32_t      MTile         = 0;

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

  //----------

  virtual void invalidateImage() {
    MImageValid = false;
    //TODO
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


//------------------------------
public: // parent to child
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    //fillBackground(AContext);
    //paintChildWidgets(AContext);
    drawImage(AContext);
    //drawBorder(AContext);
  }

  //----------

  void on_widget_config(MIP_Widget* AOwnerWindow) override {
    MImageValid = false;
    MIP_PanelWidget::on_widget_config(AOwnerWindow);
  }

//------------------------------
public:
//------------------------------

  virtual void drawImage(MIP_PaintContext* AContext) {
    if (MDrawImage) {
      MIP_Painter* painter = AContext->painter;
      MIP_DRect rect = MRect;

      validateImage(painter);

//      float ox = rect.x;        // 160
//      float oy = rect.y;        // 0
//      float ex = MImageWidth;   // 320
//      float ey = MImageHeight;  // 240
//      MIP_PaintSource paint = painter->imagePattern(ox,oy,ex,ey,0,MImage,1.0);
//      painter->beginPath();
//      painter->rect(rect.x,rect.y,rect.w,rect.h);
//      painter->fillPaint(paint);
//      painter->fill();
//      //painter->deleteImage(image);

//      MIP_DRect R = getTileRect(iindex);
      //MIP_Print("%.2f -> i : %.0f,%.0f, %.0f,%.0f\n",getValue(),iindex,R.x,R.y,R.w,R.h);
      float xscale = rect.w / MImageWidth;
      float yscale = rect.h / MImageHeight;
      float ox = rect.x;// - (R.x * xscale);
      float oy = rect.y;// - (R.y * yscale);
      float ex = MImageWidth * xscale;
      float ey = MImageHeight * yscale;
      MIP_PaintSource paint = painter->imagePattern(ox,oy,ex,ey,0,MImage,1.0);
      painter->beginPath();
      painter->rect(rect.x,rect.y,rect.w,rect.h);
      painter->fillPaint(paint);
      painter->fill();


    }
  }

};

//----------------------------------------------------------------------
#endif



