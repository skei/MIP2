#ifndef mip_win32_window_included
#define mip_win32_window_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/base/mip_base_window.h"
#include "gui/win32/mip_win32.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Win32Window
: public MIP_BaseWindow
, public MIP_Drawable {

private:

  int32_t   MWindowWidth        = 0;
  int32_t   MWindowHeight       = 0;
  uint32_t  MScreenDepth        = 0;
  double    MWindowWidthScale   = 1.0;
  double    MWindowHeightScale  = 1.0;
  bool      MFillBackground     = false;
  uint32_t  MBackgroundColor    = 0x808080;

public:

  MIP_Win32Window(uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false)
  : MIP_BaseWindow() {
  }

  virtual ~MIP_Win32Window() {
  }


//------------------------------
public:
//------------------------------

  virtual int32_t getWindowWidth()  { return MWindowWidth; }
  virtual int32_t getWindowHeight() { return MWindowHeight; }
  virtual double  getWindowWidthScale()  { return MWindowWidthScale; }
  virtual double  getWindowHeightScale() { return MWindowHeightScale; }

  virtual void    setWindowFillBackground(bool AFill=true)          { MFillBackground = AFill; }
  virtual void    setWindowBackgroundColor(uint32_t AColor)         { MBackgroundColor = AColor; }

  //virtual bool    isWindowExposed() { return MWindowExposed; }
  //virtual bool    isWindowMapped()  { return MWindowMapped; }



  //virtual void paint() {
  //  if (MWindowListener) MWindowListener->on_window_paint(0,0,MWindowWidth,MWindowHeight);
  //}

//------------------------------
public: // drawable
//------------------------------

  bool                drawable_isWindow()          final { return true; }
  bool                drawable_isDrawable()        final { return true; }

  uint32_t            drawable_getWidth()          final { return MWindowWidth; }
  uint32_t            drawable_getHeight()         final { return MWindowHeight; }
  uint32_t            drawable_getDepth()          final { return MScreenDepth; }

  //xcb_connection_t*   drawable_getXcbConnection()  final { return MConnection; }
  //xcb_visualid_t      drawable_getXcbVisual()      final { return MScreenVisual; }
  //xcb_drawable_t      drawable_getXcbDrawable()    final { return MWindow; }
  //xcb_window_t        drawable_getXcbWindow()      final { return MWindow; }

  //Display*            drawable_getXlibDisplay()    final { return MDisplay; }

  //#ifdef MIP_USE_CAIRO
  //bool                drawable_isCairo()           final { return true; }
  //cairo_surface_t*    drawable_getCairoSurface()   final { return MCairoSurface; }
  //#endif

};

//----------------------------------------------------------------------
#endif
