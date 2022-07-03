#ifndef mip_cached_widget
#define mip_cached_widget
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_CachedWidget
: public MIP_Widget {

//------------------------------
private:
//------------------------------

  MIP_Surface*  MCachedSurface  = nullptr;
  bool          MCacheDirty     = false;

//------------------------------
public:
//------------------------------

  MIP_CachedWidget(MIP_FRect ARect)
  : MIP_Widget(ARect) {
  }

  //----------

  virtual ~MIP_CachedWidget() {
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode=0) override {
  }

//------------------------------
public:
//------------------------------



};



//----------------------------------------------------------------------
#endif
