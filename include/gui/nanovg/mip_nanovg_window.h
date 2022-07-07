#ifndef mip_nanovg_window_included
#define mip_nanovg_window_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/opengl/mip_opengl_window.h"
#include "gui/nanovg/mip_nanovg.h"

//----------------------------------------------------------------------

class MIP_NanoVGWindow
: public MIP_OpenGLWindow {
//: public MIP_OpenGLWindow {

//------------------------------
protected:
//------------------------------

  NVGcontext* MNvgContext;

//------------------------------
public:
//------------------------------

  MIP_NanoVGWindow(uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false)
  : MIP_OpenGLWindow(AWidth,AHeight,AEmbedded) {
    initNanoVG();
  }

  //----------

  virtual ~MIP_NanoVGWindow() {
    cleanupNanoVG();
  }

//------------------------------
public:
//------------------------------

  bool initNanoVG() {
    MNvgContext = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    nvgCreateFont(MNvgContext,"font1","/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf");
    return true;
  }

  //----------

  void cleanupNanoVG() {
  }

//------------------------------
public: // frame buffer
//------------------------------

  NVGLUframebuffer* createFrameBuffer(uint32_t AWidth, uint32_t AHeight) {
    return nvgluCreateFramebuffer(MNvgContext, AWidth,AHeight, NVG_IMAGE_REPEATX | NVG_IMAGE_REPEATY);
  }

  //----------

  void deleteFrameBuffer(NVGLUframebuffer* ABuffer) {
    nvgluDeleteFramebuffer(ABuffer);
  }

  //----------

  void bindFrameBuffer(NVGLUframebuffer* ABuffer) {
    nvgluBindFramebuffer(ABuffer);
  }

  //----------

  void unbindFrameBuffer() {
    nvgluBindFramebuffer(nullptr);
  }



};


//----------------------------------------------------------------------
#endif
