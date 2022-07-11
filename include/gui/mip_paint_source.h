#ifndef mip_paint_source_included
#define mip_paint_source_included
//----------------------------------------------------------------------

#include "mip.h"

#ifdef MIP_USE_XCB
  #include "gui/xcb/mip_xcb.h"
#endif

#ifdef MIP_PAINTER_OPENGL
  #include "gui/opengl/mip_opengl.h"
#endif

//----------------------------------------------------------------------

class MIP_PaintSource {

public:

  virtual bool              paint_source_isBitmap()          { return false; }
  virtual bool              paint_source_isSurface()         { return false; }
  virtual bool              paint_source_isWindow()          { return false; }
  virtual bool              paint_source_isDrawable()         { return false; }

  virtual uint32_t          paint_source_getWidth()          { return 0; }
  virtual uint32_t          paint_source_getHeight()         { return 0; }
  virtual uint32_t          paint_source_getDepth()          { return 0; }

  #ifdef MIP_USE_XCB
  virtual xcb_connection_t* paint_source_getXcbConnection()  { return nullptr; }
  virtual xcb_visualid_t    paint_source_getXcbVisual()      { return XCB_NONE; }
  virtual xcb_drawable_t    paint_source_getXcbDrawable()    { return XCB_NONE; }
  virtual xcb_window_t      paint_source_getXcbWindow()      { return XCB_NONE; }
  virtual xcb_pixmap_t      paint_source_getXcbPixmap()      { return XCB_NONE; }
  #endif

  //#ifdef MIP_USE_CAIRO
  //#endif

  #ifdef MIP_USE_OPENGL
    virtual Display* paint_source_getXlibDisplay() { return nullptr; }
  #endif

  //#ifdef MIP_USE_NANOVG
  //#endif

};

//----------------------------------------------------------------------
#endif

