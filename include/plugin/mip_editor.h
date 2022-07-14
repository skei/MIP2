#ifndef mip_editor_included
#define mip_editor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_window.h"

//----------------------------------------------------------------------
//
// listeners
//
//----------------------------------------------------------------------

// editor -> plugin

class MIP_EditorListener {
public:
  virtual void on_editor_listener_update_parameter(uint32_t AIndex, double AValue) { MIP_PRINT; };
  virtual void on_editor_listener_resize_window(uint32_t AWidth, uint32_t AHeight) { MIP_PRINT; };
};


//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class MIP_Editor
: public MIP_Window {

//------------------------------
protected:
//------------------------------

  MIP_EditorListener* MEditorListener = nullptr;
  //MIP_Window*         MEditorWindow   = nullptr;
  uint32_t            MEditorWidth    = 256;
  uint32_t            MEditorHeight   = 256;
  double              MEditorScale    = 1.0;
  bool                MIsEditorOpen   = false;

//------------------------------
public:
//------------------------------

  MIP_Editor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight)
  : MIP_Window(AWidth,AHeight,true) {
    MEditorListener = AListener;
    MEditorWidth = AWidth;
    MEditorHeight = AHeight;
    //MEditorWindow = new MIP_Window(AWidth,AHeight,true);
    //MWindow->setWindowListener(this);
  }

  //----------

  virtual ~MIP_Editor() {
    if (MIsEditorOpen) gui_hide();
    //delete MEditorWindow;
  }

//------------------------------
public:
//------------------------------

  virtual bool gui_set_scale(double scale) {
    MEditorScale = scale;
    return true;
  }

  virtual bool gui_get_size(uint32_t *width, uint32_t *height) {
    *width = MEditorWidth;
    *height = MEditorHeight;
    return true;
  }

  virtual bool gui_can_resize() {
    return true;
  }

  virtual bool gui_get_resize_hints(clap_gui_resize_hints_t *hints) {
    hints->can_resize_horizontally  = true;
    hints->can_resize_vertically    = true;
    hints->aspect_ratio_width       = 16;
    hints->aspect_ratio_height      = 9;
    hints->preserve_aspect_ratio    = false;
    return true;
  }

  virtual bool gui_adjust_size(uint32_t *width, uint32_t *height) {
    //*width = MWidth;
    //*height = MHeight;
    //return true;
    return false;
  }

  virtual bool gui_set_size(uint32_t width, uint32_t height) {
    MEditorWidth = width;
    MEditorHeight = height;
    /*MEditorWindow->*/setSize(width,height);
    return true;
  }

  virtual bool gui_set_parent(const clap_window_t *window) {
    /*MEditorWindow->*/reparent(window->x11);
    return true;
  }

  virtual bool gui_set_transient(const clap_window_t *window) {
    return true;
  }

  virtual void gui_suggest_title(const char *title) {
    /*MEditorWindow->*/setTitle(title);
  }

  virtual bool gui_show() {
    /*MEditorWindow->*/open();
    /*MEditorWindow->*/startEventThread();
    MIsEditorOpen = true;
    return true;
  }

  virtual bool gui_hide() {
    MIsEditorOpen = false;
    /*MEditorWindow->*/stopEventThread();
    /*MEditorWindow->*/close();
    return true;
  }

};

//----------------------------------------------------------------------
#endif
