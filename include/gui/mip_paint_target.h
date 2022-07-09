#ifndef mip_paint_target_included
#define mip_paint_target_included
//----------------------------------------------------------------------

#include "mip.h"

#ifdef MIP_USE_XCB
  #include "gui/xcb/mip_xcb.h"
#endif

#ifdef MIP_PAINTER_OPENGL
  #include "gui/opengl/mip_opengl.h"
#endif

//----------------------------------------------------------------------

class MIP_PaintTarget {

public:

  virtual bool              isBitmap()          { return false; }
  virtual bool              isSurface()         { return false; }
  virtual bool              isWindow()          { return false; }
  virtual bool              isDrawable()        { return false; }

  virtual uint32_t          getWidth()          { return 0; }
  virtual uint32_t          getHeight()         { return 0; }
  virtual uint32_t          getDepth()          { return 0; }

  #ifdef MIP_USE_XCB
  virtual xcb_connection_t* getXcbConnection()  { return nullptr; }
  virtual xcb_visualid_t    getXcbVisual()      { return XCB_NONE; }
  virtual xcb_drawable_t    getXcbDrawable()    { return XCB_NONE; }
  virtual xcb_window_t      getXcbWindow()      { return XCB_NONE; }
  virtual xcb_pixmap_t      getXcbPixmap()      { return XCB_NONE; }
  #endif

  //#ifdef MIP_USE_CAIRO
  //#endif

  #ifdef MIP_USE_OPENGL
    virtual Display* getXlibDisplay() { return nullptr; }
  #endif

  //#ifdef MIP_USE_NANOVG
  //#endif

};

//----------------------------------------------------------------------
#endif
