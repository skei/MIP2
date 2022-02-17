#ifndef mip_base_surface_included
#define mip_base_surface_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_drawable.h"

//----------------------------------------------------------------------

class MIP_BaseSurface {

//------------------------------
public:
//------------------------------

  MIP_BaseSurface(/*MIP_Drawable* ATarget*/) {
    // window
  }

  //----------

  //MIP_BaseSurface(MIP_Drawable* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0) {
  //  // pixmap
  //}

  //----------

  virtual ~MIP_BaseSurface() {
  }

//------------------------------
public:
//------------------------------

  virtual void resize(uint32_t AWidth, uint32_t AHeight) {
  }

//------------------------------
public:
//------------------------------

  //virtual void fill(MIP_Color AColor) {}
  //virtual void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, MIP_Color AColor) {}
  //virtual void blit(int32_t ADstX, int32_t ADstY, MIP_Drawable* ASource) {}
  //virtual void blit(int32_t ADstX, int32_t ADstY, MIP_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) {}


};

//----------------------------------------------------------------------
#endif
