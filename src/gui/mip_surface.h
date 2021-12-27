#ifndef mip_surface_included
#define mip_surface_included
//----------------------------------------------------------------------

#include "mip.h"
//#include "base/types/mip_rect.h"
//#include "gui/mip_widget.h"
#include "gui/base/mip_base_surface.h"

//----------------------------------------------------------------------

#ifdef MIP_NO_GUI
  typedef MIP_BaseSurface MIP_ImplementedSurface;
#endif

#ifdef MIP_GUI_XCB
  #include "gui/xcb/mip_xcb_surface.h"
  typedef MIP_XcbSurface MIP_ImplementedSurface;
#endif

//----------------------------------------------------------------------

class MIP_Surface
: public MIP_ImplementedSurface {


//------------------------------
public:
//------------------------------

//  MIP_BaseSurface(MIP_Drawable* ATarget) {
//    // window
//  }
//
//  //----------
//
//  MIP_BaseSurface(MIP_Drawable* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=32) {


  MIP_Surface(MIP_Drawable* ATarget)
  : MIP_ImplementedSurface(ATarget) {
    MIP_PRINT;
  }

  MIP_Surface(MIP_Drawable* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=32)
  : MIP_ImplementedSurface(ATarget,AWidth,AHeight,ADepth) {
  }

  //----------

  virtual ~MIP_Surface() {
    MIP_PRINT;
  }


};

//----------------------------------------------------------------------
#endif
