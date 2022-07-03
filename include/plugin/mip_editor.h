#ifndef mip_editor_included
#define mip_editor_included
//----------------------------------------------------------------------

#include "mip.h"

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

};

//----------------------------------------------------------------------
#endif
