#ifndef mip_gdi_painter_included
#define mip_gdi_painter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/base/mip_base_painter.h"
#include "gui/gdi/mip_gdi.h"

class MIP_GdiPainter
: public MIP_BasePainter {

public:

  MIP_GdiPainter(MIP_Drawable* ATarget, MIP_Drawable* ASource)
  : MIP_BasePainter(ATarget,ASource) {
  }

  virtual ~MIP_GdiPainter() {
  }

};

//----------------------------------------------------------------------
#endif
