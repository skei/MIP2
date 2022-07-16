
#define MIP_GUI_XCB
#define MIP_USE_OPENGL
#define MIP_PAINTER_OPENGL

#include "mip.h"
#include "gui/mip_window.h"
#include "gui/mip_surface.h"
#include "gui/nanovg/mip_nanovg.h"

//----------------------------------------------------------------------

class myWindow
: public MIP_Window {

//------------------------------
public:
//------------------------------

  MIP_Surface*        MSurface        = nullptr;
  MIP_OpenGLPainter*  MPixmapPainter  = nullptr;
  NVGcontext*         MNvgContext     = nullptr;

  MIP_OpenGLPainter*  MWindowPainter  = nullptr;

//------------------------------
public:
//------------------------------

  myWindow(uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false)
  : MIP_Window(AWidth,AHeight,AEmbedded) {
    setFillBackground();
    MSurface = new MIP_Surface(this,256,256);
    MPixmapPainter = new MIP_Painter(MSurface,this);
    MPixmapPainter->makeCurrent();
    glClearColor(1,1,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    //MPixmapPainter->swapBuffers();
    //glXWaitGL();
    MNvgContext = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    MIP_Assert(MNvgContext);
    nvgCreateFont(MNvgContext,"font1","/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf");

    //MWindowPainter = new MIP_Painter(this,this);

  }

  //----------

  virtual ~myWindow() {
    delete MPixmapPainter;
    delete MSurface;
  }

  //----------

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) final {

    //MWindowPainter->makeCurrent();
    //MWindowPainter->swapBuffers();

    MPixmapPainter->makeCurrent();

    //glViewport(0,0,getWidth(),getHeight());
    //glViewport(0,256-getHeight(),getWidth(),getHeight());

    glClearColor(1,0,1,0.5);
    glClear(GL_COLOR_BUFFER_BIT);

    nvgBeginFrame(MNvgContext,getWidth(),getHeight(),1.0);

      nvgBeginPath(MNvgContext);
      nvgCircle(MNvgContext, 128,128,127);
      nvgFillColor(MNvgContext, nvgRGBA(128,64,64,255));
      nvgFill(MNvgContext);

      nvgFontFace(MNvgContext,"font1");
      nvgFontSize(MNvgContext,32);
      nvgFillColor(MNvgContext, nvgRGBA(255,255,255,128));
      nvgText(MNvgContext,30,40,"Hello world!",0);

      nvgFillColor(MNvgContext, nvgRGBA(0,0,0,255));
      nvgFontSize(MNvgContext,8);
      nvgText(MNvgContext,30,100,"Tiny, tiny 123 æøå",0);

      nvgFontSize(MNvgContext,9);
      nvgText(MNvgContext,30,110,"Tiny, tiny 123 æøå",0);

      nvgFontSize(MNvgContext,10);
      nvgText(MNvgContext,30,120,"Tiny, tiny 123 æøå",0);

    nvgEndFrame(MNvgContext);

    //..
    glXWaitGL();
    blitDrawable(32,32,MSurface->getXcbDrawable(),0,0,256,256);
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

int main() {
  myWindow* window = new myWindow(256+64,256+64);
  window->setTitle("test_window");
  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}
