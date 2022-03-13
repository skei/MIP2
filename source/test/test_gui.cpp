
//#define MIP_EXE
#define MIP_GUI_XCB
//#define MIP_PAINTER_XCB
#define MIP_USE_CAIRO
#define MIP_PAINTER_CAIRO

//#define MIP_NO_WINDOW_BUFFERING
//#define MIP_XCB_WAIT_FOR_MAPNOTIFY

//#define MIP_NO_PLUGIN

#include "mip.h"
#include "gui/mip_window.h"
#include "gui/mip_widgets.h"

#include "../data/img/knob4_60x60_131.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myWindow
: public MIP_Window {

private:

  MIP_Bitmap*   MKnobBitmap   = nullptr;
  MIP_Surface*  MKnobSurface  = nullptr;

public:

  myWindow(uint32_t AWidth, uint32_t AHeight, MIP_WindowListener* AListener=nullptr)
  : MIP_Window(AWidth,AHeight,AListener,false) {
    init();
  }

  //----------

  virtual ~myWindow() {
    cleanup();
  }

  //----------

  void init() {
    setFillWindowBackground(true);
    MKnobBitmap = new MIP_Bitmap(knob4_60x60_131,knob4_60x60_131_size);
    //knob_bitmap->convertRgbaToBgra();
    MKnobBitmap->premultAlpha(0x808080);
    MKnobSurface = new MIP_Surface(this,MKnobBitmap->getWidth(),MKnobBitmap->getHeight());
    MIP_Painter* painter = new MIP_Painter(MKnobSurface);
    painter->uploadBitmap(0,0,MKnobBitmap);
    painter->flush();
    delete painter;
    MIP_ImageStripWidget* imagestrip = new MIP_ImageStripWidget( MIP_FRect(10,10, 60,60) );
    imagestrip->setup(1,131,MKnobSurface);
    appendWidget(imagestrip);
  }

  //----------

  void cleanup() {
    delete MKnobSurface;
    delete MKnobBitmap;
  }

public:

//  void on_window_resize(int32_t AWidth, int32_t AHeight) final {
//    MIP_Print("%i,%i\n",AWidth,AHeight);
//    int32_t w = AWidth & 0xff70;
//    int32_t h = AHeight & 0xff70;
//    if ((AWidth != w) || (AHeight != h)) {
//      MIP_Print("setWindowSize(%i,%i)\n",w,h);
//      setWindowSize(w,h);
//    }
//    else {
//      MIP_Print("on_window_resize(%i,%i)\n",AWidth,AHeight);
//      MIP_Window::on_window_resize(AWidth,AHeight);
//    }
//  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

int main() {
  myWindow* window = new myWindow(400,400);
  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}
