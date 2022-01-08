#ifndef mip_image_widget_included
#define mip_image_widget_included
//----------------------------------------------------------------------

#define MIP_IMAGE_WIDGET_DRAW_NONE     0
#define MIP_IMAGE_WIDGET_DRAW_SURFACE  1
#define MIP_IMAGE_WIDGET_DRAW_BITMAP   2

#include "gui/widgets/mip_panel_widget.h"

class MIP_ImageWidget
: public MIP_PanelWidget {

//------------------------------
private:
//------------------------------

  MIP_Surface*  MSurface = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ImageWidget(MIP_FRect ARect)
  : MIP_PanelWidget(ARect) {
    setName("MIP_ImageWidget");
    setHint("image");
    setFillBackground(false);
    setDrawBorder(false);
  }

//------------------------------
public:
//------------------------------

  virtual void drawImage(MIP_BasePainter* APainter) {
    //MIP_Surface* surface = getImageSurface();
    if (MSurface) {
      MIP_FRect rect = MIP_FRect(0,0,MSurface->getWidth(),MSurface->getHeight());
      APainter->drawBitmap(getRect().x,getRect().y,MSurface,rect);
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
