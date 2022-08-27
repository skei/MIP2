#ifndef mip_bitmap_widget_included
#define mip_bitmap_widget_included
//----------------------------------------------------------------------


#define MAXSIZE


#include "gui/mip_widget.h"

class MIP_BitmapWidget
: public MIP_Widget {

//------------------------------
private:
//------------------------------

  MIP_Bitmap* MBitmap = nullptr;

//------------------------------
public:
//------------------------------

  MIP_BitmapWidget(MIP_FRect ARect, MIP_Bitmap* ABitmap=nullptr)
  : MIP_Widget(ARect) {
    setName("MIP_BitmapWidget");
    setHint("bitmap");
    MBitmap = ABitmap;
  }

  //----------

  virtual ~MIP_BitmapWidget() {
  }

  //----------

  virtual void drawBitmap(MIP_Bitmap* ABitmap, MIP_FRect ARect) {
  }

//------------------------------
public:
//------------------------------

  virtual void setBitmap(MIP_Bitmap* ABitmap) {
    MBitmap = ABitmap;
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    if (MBitmap) {
      MIP_FRect src_rect = MIP_FRect(0,0,MBitmap->getWidth(),MBitmap->getHeight());
      APainter->drawBitmap(MRect.x,MRect.y,MBitmap,src_rect);
    }
    else {
      MIP_Bitmap* bitmap = new MIP_Bitmap(MRect.w,MRect.h);
      drawBitmap(bitmap,ARect);
      //MIP_FRect src_rect = MIP_FRect(0,0,MRect.w,MRect.h);
      //APainter->drawBitmap(MRect.x,MRect.y,bitmap,src_rect);
      APainter->drawBitmap(MRect.x,MRect.y,bitmap);
      APainter->flush();
      delete bitmap;
    }
  }

};

//----------------------------------------------------------------------
#endif

