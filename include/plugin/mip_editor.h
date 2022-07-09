#ifndef mip_editor_included
#define mip_editor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"

//#include "gui/nanovg/mip_nanovg_window.h"
#include "gui/mip_window.h"

//----------------------------------------------------------------------

class MIP_EditorListener {
public:
  virtual void on_editor_listener_update_parameter(uint32_t AIndex, double AValue) = 0;
  virtual void on_editor_listener_resize_window(uint32_t AWidth, uint32_t AHeight) = 0;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Editor
: public MIP_WindowListener {

//------------------------------
private:
//------------------------------

  MIP_EditorListener* MListener     = nullptr;
  MIP_Window*         MWindow       = nullptr;
  uint32_t            MWidth        = 256;
  uint32_t            MHeight       = 256;
  double              MScale        = 1.0;
  bool                MIsWindowOpen = false;

//------------------------------
public:
//------------------------------

  MIP_Editor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight) {
    MListener = AListener;
    MWidth = AWidth;
    MHeight = AHeight;
    MWindow = new MIP_Window(/*this,*/MWidth,MHeight,true);
    MWindow->setListener(this);
    //MWindow->setFillBackground();
  }

  //----------

  virtual ~MIP_Editor() {
    if (MIsWindowOpen) hide();
    delete MWindow;
  }

//------------------------------
public:
//------------------------------

  void updateParameter(uint32_t AIndex, double AValue, bool ARedraw=true) {
    // if index -> connected.. set value, redraw
  }

//------------------------------
public: // window listener
//------------------------------

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    MIP_Print("Mwidth %i MHeight %i\n",MWidth,MHeight);

    glViewport(0,0,MWindow->getWidth(),MWindow->getHeight());
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);

    NVGcontext* nvg = MWindow->getNvgContext();
    nvgBeginPath(nvg);
    nvgCircle(nvg,200,200,150);
    nvgFillColor(nvg,nvgRGB(255,255,255));
    nvgFill(nvg);

  }

//------------------------------
public:
//------------------------------

  virtual bool set_scale(double scale) {
    MScale = scale;
    return true;
  }

  virtual bool get_size(uint32_t *width, uint32_t *height) {
    *width = MWidth;
    *height = MHeight;
    return true;
  }

  virtual bool can_resize() {
    return true;
  }

  virtual bool get_resize_hints(clap_gui_resize_hints_t *hints) {
    hints->can_resize_horizontally  = true;
    hints->can_resize_vertically    = true;
    hints->aspect_ratio_width       = 16;
    hints->aspect_ratio_height      = 9;
    hints->preserve_aspect_ratio    = false;
    return true;
  }

  virtual bool adjust_size(uint32_t *width, uint32_t *height) {
    //*width = MWidth;
    //*height = MHeight;
    //return true;
    return false;
  }

  virtual bool set_size(uint32_t width, uint32_t height) {
    MWidth = width;
    MHeight = height;
    MWindow->setSize(MWidth,MHeight);
    return true;
  }

  virtual bool set_parent(const clap_window_t *window) {
    MWindow->reparent(window->x11);
    return true;
  }

  virtual bool set_transient(const clap_window_t *window) {
    return true;
  }

  virtual void suggest_title(const char *title) {
    MWindow->setTitle(title);
  }

  virtual bool show() {
    MWindow->open();
    MWindow->startEventThread();
    MIsWindowOpen = true;
    return true;
  }

  virtual bool hide() {
    MIsWindowOpen = false;
    MWindow->stopEventThread();
    MWindow->close();
    return true;
  }

};

//----------------------------------------------------------------------
#endif
