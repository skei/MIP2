
#define MIP_NO_PLUGIN
#define MIP_GUI_XCB
//#define MIP_USE_CAIRO

#define MIP_NO_WINDOW_BUFFERING

#include "mip.h"
#include "gui/cairo/mip_cairo.h"
#include "gui/mip_window.h"
#include "gui/mip_test_widget.h"

int main() {
  //MIP_Print("Hello world!\n");

  MIP_Window* window = new MIP_Window(640,480,"MIP_Window");
  //window->setFillBackground(false);
  window->appendWidget( new MIP_TestWidget(MIP_FRect(10,10,100,100),"test",0.5));
  //window->alignWidgets();
  window->open();
  window->eventLoop();
  window->close();
  delete window;

  return 0;
}
