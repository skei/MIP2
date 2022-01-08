
#define MIP_NO_PLUGIN
#define MIP_GUI_XCB
//#define MIP_USE_CAIRO

//#define MIP_NO_WINDOW_BUFFERING

#include "mip.h"
#include "gui/cairo/mip_cairo.h"
#include "gui/mip_window.h"

#include "gui/mip_widgets.h"

int main() {
  //MIP_Print("Hello world!\n");

  MIP_Window* window = new MIP_Window(640,480,"MIP_Window");
  //window->setFillBackground(false);

  window->appendWidget( new MIP_PanelWidget(  MIP_FRect(      10,10, 150,20 )  ));
  window->appendWidget( new MIP_TextWidget(   MIP_FRect(      10,35, 150,20 ), "Text" ));
  window->appendWidget( new MIP_ValueWidget(  MIP_FRect(      10,60, 150,20 ), "Value" ));
  window->appendWidget( new MIP_DragValueWidget(  MIP_FRect(  10,85, 150,20 )));

  //window->alignWidgets();
  window->open();
  window->eventLoop();
  window->close();
  delete window;

  return 0;
}
