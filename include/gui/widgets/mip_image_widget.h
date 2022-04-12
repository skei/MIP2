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

  MIP_Surface*  MImageSurface    = nullptr;
  MIP_FRect     MImageRect  = {};

//------------------------------
public:
//------------------------------

  MIP_ImageWidget(MIP_FRect ARect, MIP_Surface* ASurface)
  : MIP_PanelWidget(ARect) {
    MImageRect = MIP_FRect(0,0,ASurface->getWidth(),ASurface->getHeight());
    MImageSurface = ASurface;
    setName("MIP_ImageWidget");
    setHint("image");
    setFillBackground(false);
    setDrawBorder(false);
  }

  MIP_ImageWidget(MIP_FRect ARect, MIP_Surface* ASurface, MIP_FRect AImageRect)
  : MIP_PanelWidget(ARect) {
    MImageRect = AImageRect;
    MImageSurface = ASurface;
    setName("MIP_ImageWidget");
    setHint("image");
    setFillBackground(false);
    setDrawBorder(false);
  }

//------------------------------
public:
//------------------------------

  virtual void drawImage(MIP_Painter* APainter) {

    //APainter->setColor(1);
    //APainter->rectangle(MRect);
    //APainter->fillPath();

    if (MImageSurface) {
      //APainter->drawImage(getRect().x,getRect().y,MImageSurface,MImageRect);
      //MIP_Print("%.2f,%.2f : %.2f,%.2f\n",MRect.x,MRect.y,MRect.w,MRect.h);
      //APainter->drawImage(MRect.x,MRect.y,MImageSurface,MImageRect);
      APainter->drawImage(MRect,MImageSurface,MImageRect);
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    drawImage(APainter);
    paintWidgets(APainter,ARect,AMode);
    drawBorder(APainter,ARect,AMode);
  }

//------------------------------

};

//----------------------------------------------------------------------
#endif
