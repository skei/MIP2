#ifndef mip_drawable_included
#define mip_drawable_included
//----------------------------------------------------------------------

#include "mip.h"

#ifdef MIP_USE_GLX
  #include "gui/glx/mip_glx.h"
#endif

#ifdef MIP_USE_NANOVG
  #include "gui/nanovg/mip_nanovg.h"
#endif

#ifdef MIP_USE_XCB
  #include "gui/xcb/mip_xcb.h"
  #include "gui/xlib/mip_xlib.h"
#endif


//----------------------------------------------------------------------

class MIP_Drawable {

public:

  virtual bool              drawable_isBitmap()          { return false; }
  virtual bool              drawable_isSurface()         { return false; }
  virtual bool              drawable_isWindow()          { return false; }
  virtual bool              drawable_isDrawable()         { return false; }

  virtual uint32_t          drawable_getWidth()          { return 0; }
  virtual uint32_t          drawable_getHeight()         { return 0; }
  virtual uint32_t          drawable_getDepth()          { return 0; }

  #ifdef MIP_USE_CAIRO
  #endif

  #ifdef MIP_USE_GDI
  #endif

  //#ifdef MIP_USE_GLX
  //  virtual Display* drawable_getXlibDisplay() { return nullptr; }
  //#endif

  #ifdef MIP_USE_NANOVG
  #endif

  #ifdef MIP_USE_XCB
    virtual xcb_connection_t* drawable_getXcbConnection() { return nullptr; }
    virtual xcb_visualid_t    drawable_getXcbVisual()     { return XCB_NONE; }
    virtual xcb_drawable_t    drawable_getXcbDrawable()   { return XCB_NONE; }
    virtual xcb_window_t      drawable_getXcbWindow()     { return XCB_NONE; }
    virtual xcb_pixmap_t      drawable_getXcbPixmap()     { return XCB_NONE; }
    virtual Display*          drawable_getXlibDisplay()   { return nullptr; }
  #endif

};

//----------------------------------------------------------------------
#endif

