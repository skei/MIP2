#ifndef mip_editor_incuded
#define mip_editor_incuded
//----------------------------------------------------------------------

#include "gui/xcb/mip_xcb_window.h"


class MIP_Editor {

//------------------------------
private:
//------------------------------

  MIP_XcbWindow*  MWindow = nullptr;
  double          MScale  = 1.0;
  uint32_t        MWidth  = 400;
  uint32_t        MHeight = 400;

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

  bool attach(const char* display_name, unsigned long window) {
    MWindow = new MIP_XcbWindow(MWidth,MHeight,"Title",(void*)window);
    return true;
  }

  void show() {
    MWindow->open();
  }

  void hide() {
    MWindow->close();
  }

  void setWidth(uint32_t w) {
    MWidth = w;
    MWindow->setSize(MWidth,MHeight);
  }

  void setHeight(uint32_t h) {
    MHeight = h;
    MWindow->setSize(MWidth,MHeight);
  }

  void setSize(uint32_t w, uint32_t h) {
    MWidth = w;
    MHeight = h;
    MWindow->setSize(MWidth,MHeight);
  }

  void setScale(double scale) {
  }

  uint32_t getWidth() {
    return MWidth;
  }

  uint32_t getHeight() {
    return MHeight;
  }

};

//----------------------------------------------------------------------
#endif
