#ifndef mip_xcb_surface_included
#define mip_base_surface_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_drawable.h"

//----------------------------------------------------------------------

class MIP_XcbSurface {

//------------------------------
public:
//------------------------------

  MIP_XcbSurface(MIP_Drawable* ATarget) {
    // window
  }

  //----------

  MIP_XcbSurface(MIP_Drawable* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=32) {
    // pixmap
  }

  //----------

  virtual ~MIP_XcbSurface() {
  }

//------------------------------
public:
//------------------------------

  //void fill(MIP_Color AColor) override {}
  //void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, MIP_Color AColor) override {}
  //void blit(int32_t ADstX, int32_t ADstY, MIP_Drawable* ASource) override {}
  //void blit(int32_t ADstX, int32_t ADstY, MIP_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) override {}

};

//----------------------------------------------------------------------
#endif
