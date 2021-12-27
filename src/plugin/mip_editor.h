#ifndef mip_editor_incuded
#define mip_editor_incuded
//----------------------------------------------------------------------

//#include "gui/xcb/mip_xcb_window.h"
#include "gui/mip_window.h"


class MIP_Editor {

//------------------------------
private:
//------------------------------

  //MIP_XcbWindow*  MWindow = nullptr;
  MIP_Window* MWindow = nullptr;
  double      MScale  = 1.0;
  uint32_t    MWidth  = 400;
  uint32_t    MHeight = 400;

//------------------------------
public:
//------------------------------

  MIP_Editor() {
  }

  virtual ~MIP_Editor() {
    if (MWindow) delete MWindow;
  }

//------------------------------
public:
//------------------------------

  virtual MIP_Window* getWindow() {
    return MWindow;
  }

  virtual bool attach(const char* display_name, unsigned long window) {
    //MWindow = new MIP_XcbWindow(MWidth,MHeight,"Title",(void*)window);
    MWindow = new MIP_Window(MWidth,MHeight,"Title",(void*)window);
    if (MWindow) return true;
    return false;
  }

  virtual void show() {
    MWindow->open();
  }

  virtual void hide() {
    MWindow->close();
  }

  virtual void setWidth(uint32_t w) {
    MWidth = w;
    MWindow->setSize(MWidth,MHeight);
  }

  virtual void setHeight(uint32_t h) {
    MHeight = h;
    MWindow->setSize(MWidth,MHeight);
  }

  virtual void setSize(uint32_t w, uint32_t h) {
    MWidth = w;
    MHeight = h;
    MWindow->setSize(MWidth,MHeight);
  }

  virtual void setScale(double scale) {
  }

  virtual uint32_t getWidth() {
    return MWidth;
  }

  virtual uint32_t getHeight() {
    return MHeight;
  }

};

//----------------------------------------------------------------------
#endif
