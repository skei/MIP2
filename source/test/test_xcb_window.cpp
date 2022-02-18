
#define MIP_USE_XCB
#define MIP_GUI_XCB
#define MIP_PAINTER_XCB

#include "mip.h"
#include "gui/mip_window.h"

int main() {
  MIP_Window* window = nullptr;
  window = new MIP_Window(600,400);
  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}
