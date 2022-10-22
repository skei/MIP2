
#define MIP_GUI_WIN32
#define MIP_PAINTER_GDI
//#define MIP_PAINTER_NANOVG

//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_plugin.h"
#include "gui/widgets/mip_widgets.h"

//----------------------------------------------------------------------

int main() {

  MIP_Window* window = new MIP_Window(640,480);

  window->setWindowFillBackground(true);
  window->setWindowBackgroundColor(0x000088);

    MIP_PanelWidget* panel1 = new MIP_PanelWidget(MIP_DRect(10,10,200,100));
    panel1->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    panel1->setFillBackground(true);
    panel1->setBackgroundColor(MIP_COLOR_GREEN);
    window->appendChildWidget(panel1);

  window->openWindow();
  window->eventLoop();
  window->closeWindow();
  delete window;
  return 0;
}
