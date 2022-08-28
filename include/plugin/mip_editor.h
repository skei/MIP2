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
  //beginGEsture
  //endGEsture
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
  uint32_t            MEditorWidth    = 200;
  uint32_t            MEditorHeight   = 100;
  double              MEditorScale    = 1.0;
  bool                MIsEditorOpen   = false;
  //MIP_Widget**        MParameterToWidget  = nullptr;
  MIP_Widget*         MParameterToWidget[MIP_EDITOR_MAX_PARAMS] = {0};

  double MInitialWidth = 0;
  double MInitialHeight = 0;

//------------------------------
public:
//------------------------------

  MIP_Editor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight)
  : MIP_Window(AWidth,AHeight,true) {
    //MIP_Print("width %i height %i\n",AWidth,AHeight);
    MEditorListener = AListener;
    MEditorWidth = AWidth;
    MEditorHeight = AHeight;
    setWidgetSize(AWidth,AHeight);
    //uint32_t size = MIP_EDITOR_MAX_PARAMS * sizeof(MIP_Widget*);
    //MParameterToWidget = (MIP_Widget**)malloc(size);
    //memset(MParameterToWidget,0,size);
    //MIP_PRINT;

    MInitialWidth = AWidth;
    MInitialHeight = AHeight;

  }

  //----------

  virtual ~MIP_Editor() {
    //MIP_PRINT;
    if (MIsEditorOpen) {
      hide();
    }
    //free(MParameterToWidget);
  }

//------------------------------
public: // clap gui
//------------------------------

  /*
    Set the absolute GUI scaling factor, and override any OS info.
    Should not be used if the windowing api relies upon logical pixels.

    If the plugin prefers to work out the scaling factor itself by querying the OS directly,
    then ignore the call.

    Returns true if the scaling could be applied
    Returns false if the call was ignored, or the scaling could not be applied.
    [main-thread]
  */

  virtual bool setScale(double scale) {
    //MIP_Print("scale %f -> true\n",scale);
    MEditorScale = scale;
    return true;
  }

  //----------

  /*
    Get the current size of the plugin UI.
    clap_plugin_gui->create() must have been called prior to asking the size.
    [main-thread]
  */

  virtual bool getSize(uint32_t *width, uint32_t *height) {
    //MIP_Print("-> true (%i, %i)\n",MEditorWidth,MEditorHeight);
    *width = MEditorWidth;
    *height = MEditorHeight;
    return true;
  }

  //----------

  /*
    Returns true if the window is resizeable (mouse drag).
    Only for embedded windows.
    [main-thread]
  */

  virtual bool canResize() {
    //MIP_Print("-> true\n");
    return true;
  }

  //----------

  /*
    Returns true if the plugin can provide hints on how to resize the window.
    [main-thread]
  */

  // ratios could be calculated from initial size..

  virtual bool getResizeHints(clap_gui_resize_hints_t *hints) {
    //MIP_Print("-> true (aspect:%i:%i)\n",420,620);
    hints->can_resize_horizontally  = true;
    hints->can_resize_vertically    = true;
    hints->aspect_ratio_width       = 420;
    hints->aspect_ratio_height      = 620;
    hints->preserve_aspect_ratio    = false;//true;
    //return true;
    return false;
  }

  //----------

  /*
    If the plugin gui is resizable, then the plugin will calculate the closest
    usable size which fits in the given size.
    This method does not change the size.
    Only for embedded windows.
    [main-thread]
  */

  // 'the given size' indicates the cointent of width/height is valid?

  virtual bool adjustSize(uint32_t *width, uint32_t *height) {
//    double aspect = 420.0 / 620.0; //(double)MEditorWidth / (double)MEditorHeight;
    double w = *width;
    double h = *height;
//    double a = w / h;
//    MIP_Print("aspect %f w %f h %f a %f\n",aspect,w,h,a);
//    //if (h > 0) {
//      if (a >= aspect) w = h * aspect;
//      else h = w / aspect;
//    //}
    //MIP_Print("%i,%i -> true (%f, %f)\n",*width,*height,w,h);
    *width = w;
    *height = h;
    return true;
  }

  //----------

  /*
    Sets the window size. Only for embedded windows.
    [main-thread]
  */

  virtual bool setSize(uint32_t width, uint32_t height) {
    //MIP_Print("%i,%i -> true\n",width,height);

    setWindowSize(width,height);

    MEditorWidth = width;
    MEditorHeight = height;

    //double xscale = (double)width / MInitialWidth;
    //double yscale = (double)height / MInitialHeight;
    //MIP_Print("xscale %f yscale %f\n",xscale,yscale);

    // hack.. the modual stuff should have been in the wndoow class..
    // but there were/are some issues getting resize events..
    if (MModalWidget) MModalWidget->on_widget_unmodal();

    //MIP_Window::on_window_resize(width,height);
    setWidgetSize(width,height);

    /*
      if there are visible widgets with autoSize, calling painter methods..
    */

    alignChildWidgets();

    //scaleChildWidgets(xscale,true);

//    setWindowSize(width,height);
    return true;
  }

  //----------

  /*
    Embbeds the plugin window into the given window.
    [main-thread & !floating]
  */

  virtual bool setParent(const clap_window_t *window) {
    //MIP_Print("%p -> true\n",window);
    reparentWindow(window->x11);
    return true;
  }

  //----------

  /*
    Set the plugin floating window to stay above the given window.
    [main-thread & floating]
  */

  virtual bool setTransient(const clap_window_t *window) {
    //MIP_Print("%p -> true\n",window);
    return true;
  }

  //----------

  /*
    Suggests a window title. Only for floating windows.
    [main-thread & floating]
  */

  virtual void suggestTitle(const char *title) {
    //MIP_Print("%s\n",title);
    setWindowTitle(title);
  }

  //----------

  /*
    Show the window.
    [main-thread]
  */

  virtual bool show() {
    //MIP_Print("-> true\n");
    if (!MIsEditorOpen) {
      openWindow();
      MIsEditorOpen = true;
      startEventThread();
    }
    return true;
  }

  //----------

  /*
    Hide the window, this method do not free the resources, it just hides
    the window content. Yet it maybe a good idea to stop painting timers.
    [main-thread]
  */

  virtual bool hide() {
    //MIP_Print("-> true\n");
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
