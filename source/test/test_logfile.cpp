

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
  LOG.print("test... ");
  uint32_t i = 666;
  uint32_t* ptr = &i;
  LOG.print("OK : %i\n",*ptr);
  return 0;
}
