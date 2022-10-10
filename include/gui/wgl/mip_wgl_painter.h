#ifndef mip_wgl_painter_included
#define mip_wgl_painter_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_drawable.h"
#include "gui/base/mip_base_painter.h"
#include "gui/wgl/mip_wgl.h"
#include "gui/wgl/mip_wgl_utils.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_WglPainter
: public MIP_BasePainter {

//------------------------------
private:
//------------------------------

  HDC     MDc   = nullptr;
  HGLRC   MGlRc = nullptr;

  MIP_Drawable* MSurface = nullptr;
  MIP_Drawable* MTarget = nullptr;
//------------------------------
public:
//------------------------------

  // ASurface = pixmap
  // ATarget  = window

  MIP_WglPainter(MIP_Drawable* ASurface, MIP_Drawable* ATarget)
  : MIP_BasePainter(ASurface,ATarget) {
    MSurface = ASurface;
    MTarget = ATarget;
    HDC tempdc = GetDC(0);
    MDc = CreateCompatibleDC(tempdc); // (nullptr);
    ReleaseDC(0,tempdc);
    //MDc = ATarget->drawable_getDC();
    MGlRc = wglCreateContext(MDc);
    makeCurrent();
    loadOpenGL();
    wglDeleteContext(MGlRc);
    ReleaseDC(0,MDc);
    MIP_Print("\n");
    //resetCurrent();
  }

  //----------

  virtual ~MIP_WglPainter() {
    //ReleaseDC(0,MDc);
    //wglDeleteContext(MGlRc);
    unloadOpenGL();
    MIP_Print("\n");
  }

//------------------------------
public:
//------------------------------

//  void select() override {
//    makeCurrent();
//  }
//
//  void deselect() override {
//    resetCurrent();
//  }

  void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    MIP_Print("%i,%i - %i,%i\n",AXpos,AYpos,AWidth,AHeight);
    MDc = MTarget->drawable_getDC();
    MIP_Assert(MDc);
    MGlRc = wglCreateContext(MDc);
    makeCurrent();
    //loadOpenGL();
    glViewport(0,0,AWidth,AHeight);
    //MIP_Print("\n");
  }

  //----------

  void endPaint() override {
    //MIP_PRINT;
    swapBuffers();
    resetCurrent();
    //unloadOpenGL();
    wglDeleteContext(MGlRc);
//    ReleaseDC(0,MDc);
    //MDc = nullptr;
    //MGlRc = nullptr;
    MIP_Print("\n");
  }

//------------------------------
public:
//------------------------------

  bool makeCurrent() {
    MIP_Print("\n");
    wglMakeCurrent(MDc, MGlRc);
    MIP_Print("\n");
    return true;
  }

  bool resetCurrent() {
    MIP_Print("\n");
    wglMakeCurrent(nullptr,nullptr);
    MIP_Print("\n");
    return true;
  }

  void swapBuffers() {
    MIP_Print("\n");
    SwapBuffers(MDc);
    MIP_Print("\n");
  }

//  virtual void pushClip(MIP_DRect ARect) {
//  }
//
//  virtual void pushOverlapClip(MIP_DRect ARect) {
//  }
//
//  virtual void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
//  }
//
//  virtual void endPaint() {
//  }

  bool loadOpenGL() {
    MIP_Print("calling sogl_loadOpenGL\n");
    if (!sogl_loadOpenGL()) {
      MIP_Print("Error: sogl_loadOpenGL:\n");
      const char** failures = sogl_getFailures();
      while (*failures) {
        MIP_DPrint("  %s\n",*failures);
        failures++;
      }
      return false;
    }
    return true;
  }

  //----------

  void unloadOpenGL() {
    sogl_cleanup();
  }


};

//----------------------------------------------------------------------
#endif
