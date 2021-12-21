#ifndef mip_drawable_included
#define mip_drawable_included
//----------------------------------------------------------------------

#ifdef MIP_USE_XCB
  #include "gui/xcb/mip_xcb.h"
#endif

#ifdef MIP_USE_CAIRO
  #include "gui/cairo/mip_cairo.h"
#endif

#include "gui/mip_bitmap.h"

//----------------------------------------------------------------------

struct MIP_Drawable {

  virtual bool              isDrawable()          { return false; }
  virtual bool              isImage()             { return false; }
  virtual bool              isSurface()           { return false; }
  virtual bool              isWindow()            { return false; }
  virtual bool              isCairo()             { return false; }

  virtual uint32_t          getWidth()            { return 0; }
  virtual uint32_t          getHeight()           { return 0; }
  virtual uint32_t          getDepth()            { return 0; }
  virtual uint32_t          getStride()           { return 0; }
  virtual MIP_Bitmap*       getBitmap()           { return nullptr; }

  #ifdef MIP_USE_XCB
  virtual xcb_connection_t* getXcbConnection()    { return nullptr; }
  virtual xcb_visualid_t    getXcbVisual()        { return XCB_NONE; }
  virtual xcb_drawable_t    getXcbDrawable()      { return XCB_NONE; }
  virtual xcb_image_t*      getXcbImage()         { return nullptr; }
  virtual xcb_pixmap_t      getXcbPixmap()        { return XCB_NONE; }
  virtual xcb_window_t      getXcbWindow()        { return XCB_NONE; }
  #endif

  #ifdef MIP_USE_CAIRO
  virtual cairo_surface_t*  createCairoSurface()  { return nullptr; }
  #endif

  //#ifdef MIP_USE_CTX
  //virtual Ctx*              getCtxContext()     { return nullptr; }
  //#endif

  //#ifdef MIP_USE_SDL
  //virtual SDL_Window*       getSdlWindow()      { return nullptr; }
  //irtual SDL_Renderer*     getSdlRenderer()    { return nullptr; }
  //virtual SDL_Texture*      getSdlTexture()     { return nullptr; }
  //#endif

};

//----------------------------------------------------------------------
#endif
