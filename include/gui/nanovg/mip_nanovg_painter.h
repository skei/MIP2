#ifndef mip_nanovg_painter_included
#define mip_nanovg_painter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/opengl/mip_opengl_painter.h"
#include "gui/nanovg/mip_nanovg.h"

//----------------------------------------------------------------------


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_NanoVGPainter
: public MIP_OpenGLPainter {

//------------------------------
private:
//------------------------------

  NVGcontext* MContext    = nullptr;
  int         MFont       = 0;

//------------------------------
public:
//------------------------------

  MIP_NanoVGPainter(MIP_Drawable* ASurface, MIP_Drawable* ATarget)
  : MIP_OpenGLPainter(ASurface,ATarget) {
    MIP_OpenGLPainter::makeCurrent();
    MContext    = nvgCreateGL3(NVG_ANTIALIAS);// | NVG_STENCIL_STROKES);
    MFont       = nvgCreateFont(MContext,"font1","/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf");
    //MIconImage  = nvgCreateImage(MContext,"/DISKS/sda2/code/git/MIP2/include/extern/oui-blendish/blender_icons16.png",0); // NVG_IMAGE_PREMULTIPLIED
    //bndSetFont(MFont);
    //bndSetIconImage(MIconImage);
    MIP_OpenGLPainter::resetCurrent();
  }

  //----------

  virtual ~MIP_NanoVGPainter() {
    nvgDeleteGL3(MContext);
  }

//------------------------------
public:
//------------------------------

  NVGcontext* getNvgContext() {
    return MContext;
  }

//------------------------------
public:
//------------------------------

  void beginPaint(int32_t AWidth, int32_t AHeight) override {
    MIP_OpenGLPainter::beginPaint(AWidth,AHeight);
    nvgBeginFrame(MContext,AWidth,AHeight,1.0);
  }

  //----------

  void endPaint() override {
    nvgEndFrame(MContext);
    MIP_OpenGLPainter::endPaint();
  }

};

//----------------------------------------------------------------------
#endif
