#ifndef mip_editor_window_included
#define mip_editor_window_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_window.h"
#include "base/system/mip_timer.h"

//#define MIP_EDITOR_MAX_PARAMS 4096

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
  //beginGEsture
  //endGEsture
};


//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class MIP_EditorWindow
: public MIP_Window {

//------------------------------
protected:
//------------------------------

  MIP_EditorListener* MEditorListener = nullptr;

//------------------------------
public:
//------------------------------

  MIP_EditorWindow(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, intptr_t AParent)
  : MIP_Window(AWidth,AHeight,AParent) {
    MWindowType = "MIP_EditorWindow";
    MEditorListener = AListener;
    MOwnerWindow = this;
  }

  //----------

  virtual ~MIP_EditorWindow() {
  }

//------------------------------
public: // widget
//------------------------------

  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) override {
    //MIP_Print("%s\n",ASender->getWidgetName());
    MIP_Parameter* parameter = ASender->getParameter();
    if (parameter) {
      uint32_t index = parameter->getIndex();
      double value = ASender->getValue();
      //MIP_Print("%i = %f\n",index,value);
      if (MEditorListener) MEditorListener->on_editor_listener_update_parameter(index,value);
    }
    //MIP_Window::do_widget_update(ASender,AMode);
  }

  //----------

  void do_widget_resized(MIP_Widget* ASender, float ADeltaX=0.0f, float ADeltaY=0.0f, uint32_t AMode=0) override {
    uint32_t width = getWindowWidth() + ADeltaX;
    uint32_t height = getWindowHeight() + ADeltaY;
    if (MEditorListener) MEditorListener->on_editor_listener_resize_window(width,height);
  }

};

//----------------------------------------------------------------------
#endif
