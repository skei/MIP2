
#define MIP_GUI_XCB
//#define MIP_PAINTER_XCB
#define MIP_USE_CAIRO
#define MIP_PAINTER_CAIRO

#define MIP_NO_PLUGIN
//#define MIP_NO_WINDOW_BUFFERING

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

  MIP_Bitmap*   knob_bitmap   = nullptr;
  MIP_Surface*  knob_surface  = nullptr;

public:

  myWindow(uint32_t AWidth, uint32_t AHeight, MIP_WindowListener* AListener=nullptr, bool AEmbedded=false)
  : MIP_Window(AWidth,AHeight,AListener,AEmbedded) {
    setFillWindowBackground(true);
    knob_bitmap = new MIP_Bitmap(knob4_60x60_131,knob4_60x60_131_size);
    //knob_bitmap->convertRgbaToBgra();
    knob_bitmap->premultAlpha(0x808080);
    knob_surface = new MIP_Surface(this,knob_bitmap->getWidth(),knob_bitmap->getHeight());
    MIP_Painter* knob_painter = new MIP_Painter(knob_surface);
    knob_painter->uploadBitmap(0,0,knob_bitmap);
    knob_painter->flush();
    delete knob_painter;
    MIP_ImageStripWidget* imagestrip = new MIP_ImageStripWidget(  MIP_FRect(10,10, 60,60) );
    imagestrip->setup(1,131,knob_surface);
    appendWidget(imagestrip);
  }

  //----------

  virtual ~myWindow() {
    delete knob_surface;
    delete knob_bitmap;
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
  myWindow* window = new myWindow(640,480);
  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}
