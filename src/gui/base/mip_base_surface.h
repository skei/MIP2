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

  MIP_BaseSurface(MIP_Drawable* ATarget) {
    // window
  }

  //----------

  MIP_BaseSurface(MIP_Drawable* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=32) {
    // pixmap
  }

  //----------

  virtual ~MIP_BaseSurface() {
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




#if 0

//#include "kode.h"
//#include "gfx/kode_bitmap.h"
#include "gui/kode_drawable.h"
//#include "gui/kode_gui_base.h"
//#include "gui/base/kode_base_image.h"
#include "gui/base/kode_base_surface.h"
//#include "gui/base/kode_base_painter.h"
//#include "gui/base/kode_base_window.h"

#ifdef KODE_USE_CAIRO
#include "gui/cairo/kode_cairo.h"
#endif

//----------------------------------------------------------------------

class KODE_XcbSurface
: public KODE_BaseSurface
, public KODE_Drawable {

//------------------------------
private:
//------------------------------

  KODE_Drawable*    MTarget           = KODE_NULL;
  xcb_connection_t* MConnection       = KODE_NULL;
  xcb_visualid_t    MTargetVisual     = XCB_NONE;
  xcb_drawable_t    MTargetDrawable   = XCB_NONE;
  xcb_pixmap_t      MPixmap           = XCB_NONE;
  int32_t           MWidth            = 0;
  int32_t           MHeight           = 0;
  int32_t           MDepth            = 0;
  bool              MIsWindow         = false;
  xcb_window_t      MWindow           = XCB_NONE;

//------------------------------
public:
//------------------------------

  //KODE_XcbSurface(KODE_Drawable* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0)
  //: KODE_ISurface(ATarget,AWidth,AHeight,ADepth) {

  KODE_XcbSurface(KODE_Drawable* ATarget)
  : KODE_BaseSurface(ATarget) {
    MConnection = ATarget->getXcbConnection();
    MTargetDrawable   = ATarget->getXcbDrawable();
    MTargetVisual     = ATarget->getXcbVisual();
    MWidth      = ATarget->getWidth();
    MHeight     = ATarget->getHeight();
    MDepth      = ATarget->getDepth();
    MWindow     = ATarget->getXcbWindow();
    MIsWindow   = true;
    xcb_flush(MConnection);
  }

  //----------

  KODE_XcbSurface(KODE_Drawable* AOwner, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0)
  : KODE_BaseSurface(AOwner,AWidth,AHeight,ADepth) {
    MConnection = AOwner->getXcbConnection();
    MTargetDrawable   = AOwner->getXcbDrawable();
    MTargetVisual     = AOwner->getXcbVisual();
    MWidth      = AWidth;
    MHeight     = AHeight;
    if (ADepth  == 0) MDepth  = AOwner->getDepth();
    else MDepth  = ADepth;
    //if (AOwner->isWindow()) {
    //  MIsWindow = true;
    //  MWindow = AOwner->getWindow();
    //}
    //else {
      MPixmap = xcb_generate_id(MConnection);
      xcb_create_pixmap(
        MConnection,
        MDepth,
        MPixmap,
        MTargetDrawable,
        AWidth,
        AHeight
      );
    //}
    xcb_flush(MConnection);
  }

  //----------

  virtual ~KODE_XcbSurface() {
    if (!MIsWindow) {
      xcb_free_pixmap(MConnection,MPixmap);
    }
  }

//------------------------------
public: // drawable
//------------------------------

  bool                isSurface()         final { return true; }
  bool                isDrawable()        final { return true; }

  uint32_t            getWidth()          final { return MWidth; }
  uint32_t            getHeight()         final { return MHeight; }
  uint32_t            getDepth()          final { return MDepth; }

  xcb_pixmap_t        getXcbPixmap()      final { return MPixmap; }
  xcb_connection_t*   getXcbConnection()  final { return MConnection; }
  xcb_visualid_t      getXcbVisual()      final { return MTargetVisual; }
  xcb_drawable_t      getXcbDrawable()    final { return MPixmap; } //MTargetDrawable; }

  #ifdef KODE_USE_CAIRO

  bool isCairo() final {
    return true;
  }

  cairo_surface_t* createCairoSurface() final {
    cairo_surface_t* surface = cairo_xcb_surface_create(
      MConnection,
      MPixmap,
      kode_xcb_find_visual(MConnection,MTargetVisual),
      MWidth,
      MHeight
    );
    return surface;
  }

  #endif

//------------------------------
public:
//------------------------------


//------------------------------
public:
//------------------------------

  //void fill(KODE_Color AColor) final {
  //}

  //void upload(KODE_IImage* AImage) final {
  //}

  //void resize(uint32_t AWidth, uint32_t AHeight) final {
  //}

};

#endif // 0
