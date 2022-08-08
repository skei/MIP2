#ifndef mip_editor_included
#define mip_editor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_window.h"
#include "base/system/mip_timer.h"

#define MIP_EDITOR_MAX_PARAMS 4096

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
: public MIP_Window
/*, public MIP_TimerListener*/ {

//------------------------------
protected:
//------------------------------

  MIP_EditorListener* MEditorListener = nullptr;
  uint32_t            MEditorWidth    = 100;
  uint32_t            MEditorHeight   = 100;
  double              MEditorScale    = 1.0;
  bool                MIsEditorOpen   = false;
  //MIP_Widget**        MParameterToWidget  = nullptr;
  MIP_Widget*         MParameterToWidget[MIP_EDITOR_MAX_PARAMS] = {0};

//------------------------------
public:
//------------------------------

  MIP_Editor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight)
  : MIP_Window(AWidth,AHeight,true) {
    MEditorListener = AListener;
    MEditorWidth = AWidth;
    MEditorHeight = AHeight;
    setWidgetSize(AWidth,AHeight);
    //uint32_t size = MIP_EDITOR_MAX_PARAMS * sizeof(MIP_Widget*);
    //MParameterToWidget = (MIP_Widget**)malloc(size);
    //memset(MParameterToWidget,0,size);
    //MIP_PRINT;
  }

  //----------

  virtual ~MIP_Editor() {
    //MIP_PRINT;

    if (MIsEditorOpen) {
      hide();
    }

    //MIP_PRINT;
    //free(MParameterToWidget);
  }

//------------------------------
public: // clap gui
//------------------------------

  virtual bool setScale(double scale) {
    MIP_PRINT;
    MEditorScale = scale;
    return true;
  }

  //----------

  virtual bool getSize(uint32_t *width, uint32_t *height) {
    MIP_PRINT;
    *width = MEditorWidth;
    *height = MEditorHeight;
    return true;
  }

  //----------

  virtual bool canResize() {
    MIP_PRINT;
    return true;
  }

  //----------

  virtual bool getResizeHints(clap_gui_resize_hints_t *hints) {
    MIP_PRINT;
    hints->can_resize_horizontally  = true;
    hints->can_resize_vertically    = true;
    hints->aspect_ratio_width       = 16;
    hints->aspect_ratio_height      = 9;
    hints->preserve_aspect_ratio    = false;
    return true;
  }

  //----------

  virtual bool adjustSize(uint32_t *width, uint32_t *height) {
    MIP_PRINT;
    //*width = MWidth;
    //*height = MHeight;
    //return true;
    return false;
  }

  //----------

  virtual bool setSize(uint32_t width, uint32_t height) {
    MIP_PRINT;
    MEditorWidth = width;
    MEditorHeight = height;

    // hack..
    // this should have been in the wndoow lass..
    // but there were/are some issues getting resize events..

    if (MModalWidget) {
      //MIP_Print("MModalWidget\n");
      MModalWidget->on_widget_unmodal();
    }

    //

    //MIP_Window::on_window_resize(width,height);
    setWidgetSize(width,height);
    alignChildWidgets();

    setWindowSize(width,height);
    return true;
  }

  //----------

  virtual bool setParent(const clap_window_t *window) {
    MIP_PRINT;
    reparentWindow(window->x11);
    return true;
  }

  //----------

  virtual bool setTransient(const clap_window_t *window) {
    MIP_PRINT;
    return true;
  }

  //----------

  virtual void suggestTitle(const char *title) {
    MIP_PRINT;
    setWindowTitle(title);
  }

  //----------

  virtual bool show() {
    MIP_PRINT;
    if (!MIsEditorOpen) {
      openWindow();
      MIsEditorOpen = true;
      startEventThread();
    }
    return true;
  }

  //----------

  virtual bool hide() {
    MIP_PRINT;
    if (MIsEditorOpen) {
      MIsEditorOpen = false;
      stopEventThread();
      closeWindow();
    }
    return true;
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
  }

//------------------------------
public: // timer
//------------------------------

//  virtual void startTimer(uint32_t ms) {
//    MTimer.start(ms);
//  }

  //----------

//  virtual void stopTimer() {
//    MTimer.stop();
//  }

  //----------

  // AIndex 0: system

//  void on_timerCallback(int AIndex) override {
//    MIP_Print("AIndex %i\n",AIndex);
//    MIP_Window::on_timerCallback(AIndex);
//  }

//------------------------------
public:
//------------------------------

  void connectWidget(MIP_Parameter* AParameter, MIP_Widget* AWidget) {
    uint32_t param_index = AParameter->getIndex();
    MParameterToWidget[param_index] = AWidget;
    AWidget->setParameter(AParameter);
    AWidget->setMinValue(AParameter->getMinValue());
    AWidget->setMaxValue(AParameter->getMaxValue());
    AWidget->setDefaultValue(AParameter->getDefaultValue());
    AWidget->setValue(AParameter->getDefaultValue());
    //AWidget->on_widget_connect(AParameter);
  };

  //----------

  void updateParameter(uint32_t AIndex, double AValue, bool ARedraw=true) {
    //MIP_Print("%i = %.3f\n",AIndex,AValue);
    MIP_Widget* widget = MParameterToWidget[AIndex];
    if (widget) {
      widget->setValue(AValue);
      if (ARedraw) widget->redraw();
    }
  }

  //----------

  void updateModulation(uint32_t AIndex, double AValue, bool ARedraw=true) {
    //MIP_Widget* widget = MParameterToWidget[AIndex];
    //if (widget) {
    //  //widget->setModulation(AValue);
    //  widget->redraw();
    //}
  }

};

//----------------------------------------------------------------------
#endif
