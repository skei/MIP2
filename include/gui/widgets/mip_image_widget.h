#ifndef mip_image_widget_included
#define mip_image_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets/mip_panel_widget.h"
//#include "gui/mip_bitmap.h"
//#include "gui/mip_surface.h"

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

  bool          MDrawImage  = true;
  uint32_t*     MImageBuffer = nullptr;
  uint32_t      MImageWidth  = 0;
  uint32_t      MImageHeight = 0;

  //MIP_Bitmap*   MBitmap     = nullptr;
  //MIP_Surface*  MSurface    = nullptr;

  int image = -1;

//------------------------------
public:
//------------------------------

  MIP_ImageWidget(MIP_DRect ARect, uint32_t* buffer, uint32_t width, uint32_t height)
  : MIP_PanelWidget(ARect) {
    MName = "MIP_TextWidget";
    setDrawRoundedCorners(false);
    //MBitmap = ABitmap;
    MImageBuffer = buffer;
    MImageWidth  = width;
    MImageHeight = height;

  }

  //----------

  virtual ~MIP_ImageWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void  setDrawImage(bool ADraw=true)      { MDrawImage = ADraw; }

//------------------------------
public: // parent to child
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    //fillBackground(AContext);
    //paintChildWidgets(AContext);
    drawImage(AContext);
    //drawBorder(AContext);
  }

//------------------------------
public:
//------------------------------

  virtual void drawImage(MIP_PaintContext* AContext) {
    if (MDrawImage) {
      MIP_Painter* painter = AContext->painter;
      MIP_DRect rect = MRect;

      if (image != -1) {
        painter->updateImage(image,(uint8_t*)MImageBuffer);
        //painter->deleteImage(image);
      }
      else {
        //int image = painter->createImageRGBA(MImageWidth,MImageHeight,0,(uint8_t*)MImageBuffer);
        image = painter->createImageRGBA(MImageWidth,MImageHeight,0,(uint8_t*)MImageBuffer);
      }

      MIP_PaintSource paint = painter->imagePattern(160,0,320,240,0,image,1.0);
      painter->beginPath();
      painter->rect(rect.x,rect.y,rect.w,rect.h);
      painter->fillPaint(paint);
      painter->fill();

      //painter->deleteImage(image);

    }
  }

};

//----------------------------------------------------------------------
#endif



