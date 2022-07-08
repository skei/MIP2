#ifndef mip_window_included
#define mip_window_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/nanovg/mip_nanovg_window.h"

//----------------------------------------------------------------------

class MIP_Window
: public MIP_NanoVGWindow {

//------------------------------
protected:
//------------------------------

  //NVGLUframebuffer* MBuffer = nullptr;

//------------------------------
public:
//------------------------------

  MIP_Window(/*MIP_WindowListener* AListener,*/ uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false)
  : MIP_NanoVGWindow(/*AListener,*/AWidth,AHeight,AEmbedded) {
    //MBuffer = createFrameBuffer(AWidth,AHeight);
    //bindFrameBuffer(MBuffer);
    //...
    //unbindFrameBuffer();
  }

  //----------

  virtual ~MIP_Window() {
    //delete MBuffer;
  }

//------------------------------
public:
//------------------------------

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    MIP_Print("Mwidth %i MHeight %i\n",getWidth(),getHeight());

    // crashes without this..
    makeCurrent();

//    glViewport(0,0,getWidth(),getHeight());

    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);

//    NVGcontext* nvg = getNvgContext();
//    nvgBeginFrame(nvg,getWidth(),getHeight(),1.0);
//    nvgBeginPath(nvg);
//    nvgCircle(nvg,200,200,150);
//    nvgFillColor(nvg,nvgRGB(255,255,255));
//    nvgFill(nvg);
//    nvgEndFrame(nvg);

    // screen not updated without this
//    swapBuffers();

  }

};


//----------------------------------------------------------------------
#endif

