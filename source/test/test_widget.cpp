
#define MIP_GUI_XCB
#define MIP_PAINTER_NANOVG
//#define MIP_PAINTER_OPENGL

#include "mip.h"
#include "gui/mip_window.h"

//----------------------------------------------------------------------

class myWindow
: public MIP_Window {

//------------------------------
public:
//------------------------------

  myWindow(uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false)
  : MIP_Window(AWidth,AHeight,AEmbedded) {
  }

//------------------------------
public:
//------------------------------

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    MIP_Window::on_window_paint(AXpos,AYpos,AWidth,AHeight);
    MIP_Painter* painter = getPainter();
    double w = MRect.w;
    double h = MRect.h;
    painter->beginPaint(w,h);
    //paintChildren(getPaintContext());
    paint_nanovg(painter,w,h);
    painter->endPaint();
  }

//------------------------------
public:
//------------------------------

  void paint_nanovg(MIP_Painter* painter, double w, double h) {
    NVGcontext* nvg = painter->getNvgContext();
    for (uint32_t i=0; i<100; i++) {
      nvgBeginPath(nvg);
      float x1 = MIP_RandomRange(0,w);
      float y1 = MIP_RandomRange(0,h);
      nvgMoveTo(nvg,x1,y1);
      float x2 = MIP_RandomRange(0,w);
      float y2 = MIP_RandomRange(0,h);
      nvgLineTo(nvg,x2,y2);
      uint8_t r = MIP_RandomRangeInt(0,255);
      uint8_t g = MIP_RandomRangeInt(0,255);
      uint8_t b = MIP_RandomRangeInt(0,255);
      nvgStrokeColor(nvg, nvgRGBA(r,g,b,128));
      nvgStrokeWidth(nvg,3);
      nvgStroke(nvg);
    }
    nvgFontSize(nvg,60);
    float x = 100;
    float y = 100;;
    nvgFontBlur(nvg,5);
    nvgFillColor(nvg,nvgRGBA(0,0,0,255));
    nvgText(nvg,x+3,y+3, "multum",0);
    nvgText(nvg,x+3,y+63,"in parvo",0);
    nvgFontBlur(nvg,0);
    nvgFillColor(nvg,nvgRGBA(255,255,255,255));
    nvgText(nvg,x,y,   "multum",0);
    nvgText(nvg,x,y+60,"in parvo",0);
  }
};

//----------------------------------------------------------------------

int main() {
  myWindow* window = new myWindow(640,480);
  //window->setWindowFillBackground(true);
  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}
