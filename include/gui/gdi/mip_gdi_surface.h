#ifndef mip_xcb_surface_included
#define mip_xcb_surface_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_drawable.h"
#include "gui/gdi/mip_gdi.h"

class MIP_GdiSurface
: public MIP_Drawable {

public:

  MIP_GdiSurface(MIP_Drawable* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0) {
  }

  virtual ~MIP_GdiSurface() {
  }
};

//----------------------------------------------------------------------
#endif
