

//#define MIP_GUI_XCB
//#define MIP_PAINTER_NANOVG
#define MIP_NO_GUI_WARNINGS
#define MIP_LOG

#include "mip.h"
#include "base/utils/mip_logfile.h"

uint32_t* ptr = nullptr;

int main() {
  LOG.print("hello world!\n");
  LOG.print_if(true,"true\n");
  LOG.print_if(false,"false\n");
  return 0;
}
