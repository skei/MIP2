
#define MIP_GUI_XCB
#define MIP_PAINTER_XCB
//#define MIP_USE_CAIRO
//#define MIP_PAINTER_CAIRO

#define MIP_NO_PLUGIN
//#define MIP_NO_WINDOW_BUFFERING

#include "mip.h"
#include "gui/mip_window.h"
#include "gui/mip_widgets.h"

#include "../data/img/knob4_60x60_131.h"

//

int main() {
  MIP_PRINT;

  MIP_Window* window = new MIP_Window(640,480);
  window->setFillWindowBackground(true);

  MIP_Bitmap* knob_bitmap = new MIP_Bitmap(knob4_60x60_131,knob4_60x60_131_size);
  //knob_bitmap->convertRgbaToBgra();
  knob_bitmap->premultAlpha(0x808080);

  MIP_Surface* knob_surface = new MIP_Surface(window,knob_bitmap->getWidth(),knob_bitmap->getHeight());
  MIP_Painter* knob_painter = new MIP_Painter(knob_surface);
  knob_painter->uploadBitmap(0,0,knob_bitmap);
  knob_painter->flush();
  delete knob_painter;

  MIP_ImageStripWidget* imagestrip = new MIP_ImageStripWidget(  MIP_FRect(10,10, 60,60) );
  imagestrip->setup(1,131,knob_surface);

  window->appendWidget(imagestrip);

  window->open();
  window->eventLoop();
  window->close();
  delete window;

  delete knob_surface;
  delete knob_bitmap;

  return 0;
}
