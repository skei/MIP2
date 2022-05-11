
#define MIP_GUI_XCB
#define MIP_PAINTER_CAIRO

//#define MIP_NO_WINDOW_BUFFERING

//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_painter.h"
#include "gui/mip_window.h"
#include "gui/mip_widgets.h"

//----------------------------------------------------------------------
//
// widget
//
//----------------------------------------------------------------------

class myBitmapWidget
: public MIP_BitmapWidget {

//------------------------------
public:
//------------------------------

  myBitmapWidget(MIP_FRect ARect, MIP_Bitmap* ABitmap=nullptr)
  : MIP_BitmapWidget(ARect,ABitmap) {
  }

  //----------

  virtual ~myBitmapWidget() {
  }

//------------------------------
public:
//------------------------------

  void drawBitmap(MIP_Bitmap* ABitmap, MIP_FRect ARect) final {
    ABitmap->fill(0xff000000);
    //ABitmap->drawLineAA(10,10,90,40,MIP_COLOR_WHITE);
    ABitmap->drawLineAA(10,10,150,123,MIP_COLOR_WHITE);

  }


};


//----------------------------------------------------------------------
//
// window
//
//----------------------------------------------------------------------

class myWindow
: public MIP_Window {

//------------------------------
private:
//------------------------------


//------------------------------
public:
//------------------------------

  myWindow(uint32_t AWidth, uint32_t AHeight, MIP_WindowListener* AListener=nullptr)
  : MIP_Window(AWidth,AHeight,AListener,false) {
    setFillWindowBackground();
    //MIP_BitmapWidget* bitmap = new MIP_BitmapWidget(MIP_FRect(10,30, 256,256));
    MIP_BitmapWidget* bitmap = new myBitmapWidget(MIP_FRect(10,30, 256,256));
    appendWidget(bitmap);
  }

  //----------

  virtual ~myWindow() {
  }

};

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

int main() {
  myWindow* window = new myWindow(640,480);
  //window->setFillWindowBackground();
  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}

