#ifndef mip_window_included
#define mip_window_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widget.h"

//----------

#ifdef MIP_GUI_XCB
  #include "gui/xcb/mip_xcb_window.h"
  typedef MIP_XcbWindow MIP_BaseWindow;
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Window
: public MIP_BaseWindow
, public MIP_Widget {

//------------------------------
protected:
//------------------------------

//------------------------------
public:
//------------------------------

  MIP_Window(uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false)
  : MIP_BaseWindow(AWidth,AHeight,AEmbedded) {
  }

  //----------

  virtual ~MIP_Window() {
  }

//------------------------------
public:
//------------------------------

};


//----------------------------------------------------------------------
#endif

