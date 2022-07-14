#ifndef mip_surface_included
#define mip_surface_included
//----------------------------------------------------------------------

#include "mip.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifdef MIP_GUI_XCB
  #include "gui/xcb/mip_xcb_surface.h"
  typedef MIP_XcbSurface MIP_BaseSurface;
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Surface
: public MIP_BaseSurface {

//------------------------------
protected:
//------------------------------


//------------------------------
public:
//------------------------------

  MIP_Surface(MIP_Drawable* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0)
  : MIP_BaseSurface(ATarget,AWidth,AHeight,ADepth) {
  }

  //----------

  virtual ~MIP_Surface() {
  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif

