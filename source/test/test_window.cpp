
#define MIP_GUI_XCB
#define MIP_PAINTER_XCB

#include "gui/mip_window.h"

int main() {
  MIP_Window* window = new MIP_Window(600,400);
  window->setWindowTitle("MIP_Window");
  window->setMouseCursor(MIP_CURSOR_FINGER);
  window->openWindow();
  window->eventLoop();
  window->closeWindow();
  delete window;
  return 0;
}
