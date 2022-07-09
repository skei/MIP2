
#define MIP_GUI_XCB
#define MIP_USE_OPENGL
#define MIP_PAINTER_OPENGL

#include "mip.h"
#include "gui/mip_window.h"
#include "gui/mip_surface.h"

//----------------------------------------------------------------------

class myWindow
: public MIP_Window {

//------------------------------
public:
//------------------------------

  MIP_Surface*        MSurface  = nullptr;
  MIP_OpenGLPainter*  MPainter  = nullptr;

//------------------------------
public:
//------------------------------

  myWindow(uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false)
  : MIP_Window(AWidth,AHeight,AEmbedded) {
    setFillBackground();
    MSurface = new MIP_Surface(this,256,256);
    MPainter = new MIP_Painter(MSurface,this);
    //MPainter->makeCurrent();
    //glClearColor(1,1,1,1);
    //glClear(GL_COLOR_BUFFER_BIT);
    //MPainter->swapBuffers();
    //glXWaitGL();
  }

  //----------

  virtual ~myWindow() {
    delete MPainter;
    delete MSurface;
  }

  //----------

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) final {
    MIP_PRINT;

    MPainter->makeCurrent();
    glClearColor(1,0,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    //MPainter->swapBuffers();
    glXWaitGL();
    blitDrawable(100,100,MSurface->getXcbDrawable(),0,0,256,256);
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

int main() {
  myWindow* window = new myWindow(320,240);
  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}
