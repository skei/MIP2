
#define MIP_GUI_WIN32
#define MIP_PAINTER_GDI

//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_plugin.h"
#include "gui/widgets/mip_widgets.h"


//----------------------------------------------------------------------

int main() {
  MIP_Window* window = new MIP_Window(640,480,false);
  window->setWindowFillBackground(true);

  window->appendChildWidget(new MIP_PanelWidget(MIP_DRect(0,0,100,100)));

  window->openWindow();
  window->eventLoop();
  window->closeWindow();
  delete window;
  return 0;
}
