#ifndef mip_editor_included
#define mip_editor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"

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

class MIP_Editor {

//------------------------------
private:
//------------------------------

  MIP_EditorListener* MListener = nullptr;
  uint32_t            MWidth    = 640;
  uint32_t            MHeight   = 480;
  double              MScale    = 1.0;

//------------------------------
public:
//------------------------------

  MIP_Editor(MIP_EditorListener* AListener) {
    MListener = AListener;
  }

  //----------

  virtual ~MIP_Editor() {
  }

//------------------------------
public:
//------------------------------

  void updateParameter(uint32_t AIndex, double AValue, bool ARedraw=true) {
    // if index -> connected.. set value, redraw
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
    return false;
  }

  virtual bool get_resize_hints(clap_gui_resize_hints_t *hints) {
    hints->can_resize_horizontally  = false;
    hints->can_resize_vertically    = false;
    hints->aspect_ratio_width       = 1.0;
    hints->aspect_ratio_height      = 1.0;
    hints->preserve_aspect_ratio    = true;
    return false;
  }

  virtual bool adjust_size(uint32_t *width, uint32_t *height) {
    return false;
  }

  virtual bool set_size(uint32_t width, uint32_t height) {
    return false;
  }

  virtual bool set_parent(const clap_window_t *window) {
    return false;
  }

  virtual bool set_transient(const clap_window_t *window) {
    return false;
  }

  virtual void suggest_title(const char *title) {
  }

  virtual bool show() {
    return false;
  }

  virtual bool hide() {
    return false;
  }

};

//----------------------------------------------------------------------
#endif
