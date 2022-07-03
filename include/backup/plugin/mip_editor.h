#ifndef mip_editor_included
#define mip_editor_included
//----------------------------------------------------------------------

// aka MIP_GuiHandler

#include "mip.h"
#include "base/types/mip_queue.h"
#include "base/system/mip_timer.h"
#include "plugin/mip_parameter.h"
#include "plugin/clap/mip_clap_plugin.h"
#include "gui/mip_widget.h"
#include "gui/mip_window.h"

#define MIP_EDITOR_TIMER_RATE             25
#define MIP_EDITOR_MAX_EVENTS_PER_BLOCK   4096
//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_EditorListener {
public:
  virtual void on_editor_listener_beginUpdateParameter(uint32_t AIndex) {}
  virtual void on_editor_listener_parameter(uint32_t AIndex, double AValue) {}
  virtual void on_editor_listener_endUpdateParameter(uint32_t AIndex) {}
  virtual void on_editor_listener_resize(uint32_t AWidth, uint32_t AHeight) {}
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Editor
: public MIP_WindowListener
, public MIP_TimerListener {

private:

  MIP_EditorListener* MListener         = nullptr;
  MIP_ClapPlugin*     MPlugin           = nullptr;
  uint32_t            MNumParams        = 0;
  bool                MEmbedded         = false;
  MIP_Window*         MWindow           = nullptr;
  MIP_Timer*          MTimer            = nullptr;
  MIP_Widget**        MParamToWidget    = nullptr;
  double*             MGuiParamVal      = nullptr;
  double*             MGuiParamMod      = nullptr;
  MIP_WidgetArray     MDirtyWidgets     = {};
  bool                MCanResize        = false;
  uint32_t            MWidth            = 0;
  uint32_t            MHeight           = 0;
  double              MScale            = 1.0;
  uint32_t            MParentWindow     = 0;
  uint32_t            MTransientWindow  = 0;
  bool                MIsOpen           = false;
  const char*         MWindowTitle      = "";

  MIP_Queue<uint32_t,MIP_EDITOR_MAX_EVENTS_PER_BLOCK>  MGuiParamQueue  = {};
  MIP_Queue<uint32_t,MIP_EDITOR_MAX_EVENTS_PER_BLOCK>  MGuiModQueue    = {};

//------------------------------
public:
//------------------------------

  MIP_Editor(MIP_EditorListener* AListener, MIP_ClapPlugin* APlugin, uint32_t AWidth, uint32_t AHeight, bool AEmbedded=true) {
    MListener = AListener;
    MPlugin = APlugin;
    MWidth = AWidth;
    MHeight = AHeight;
    MEmbedded = AEmbedded;
    MNumParams = APlugin->params_count();
    uint32_t size = MNumParams * sizeof(MIP_Widget*);
    MParamToWidget = (MIP_Widget**)malloc(size);
    memset(MParamToWidget,0,size);
    size = MNumParams * sizeof(double);
    MGuiParamVal = (double*)malloc(size);
    MGuiParamMod = (double*)malloc(size);
    memset(MGuiParamVal,0,size);
    memset(MGuiParamMod,0,size);
    MTimer = new MIP_Timer(this);
    MWindow = new MIP_Window(AWidth,AHeight,this,AEmbedded);
  }

  //----------

  virtual ~MIP_Editor() {
    delete MTimer;
    free(MParamToWidget);
    free(MGuiParamVal);
    free(MGuiParamMod);
    if (MWindow) delete MWindow;
  }

//------------------------------
public:
//------------------------------

  virtual bool getSize(uint32_t* AWidth, uint32_t* AHeight) {
    *AWidth = MWidth;
    *AHeight = MHeight;
    return true;
  }

  //----------

  virtual bool setSize(uint32_t AWidth, uint32_t AHeight) {
    MWidth = AWidth;
    MHeight = AHeight;
    MWindow->setWindowSize(AWidth,AHeight);
    return true;
  }

  //----------

  virtual bool setScale(double AScale) {
    MScale = AScale;
    return true;
  }

  //----------

  virtual bool canResize() {
    return MCanResize;
  }

  //----------

  virtual bool getResizeHints(clap_gui_resize_hints_t *hints) {
    //hints->preseve_aspect_ratio = false;
    //hints->aspect_ratio_width = MWidth;
    //hints->aspect_ratio_height = MHeight;
    return false;
  }

  //----------

  virtual bool adjustSize(uint32_t* AWidth, uint32_t* AHeight) {
    //*AWidth = MWidth;
    //*AHeight = MHeight;
    //return true;
    return false;
  }

  //----------

  virtual bool setParent(const clap_window_t *window) {
    MParentWindow = window->x11;
    MWindow->reparent(MParentWindow);
    return true;
  }

  //----------

  virtual bool setTransient(const clap_window_t *window) {
    MTransientWindow = window->x11;
    return true;
  }

  //----------

  virtual void suggestTitle(const char *title) {
    MWindowTitle = title;
  }

  //----------

  virtual bool show() {
    //MWindow->setOwnerWindow(MWindow);
    MWindow->open();
    MTimer->start(MIP_EDITOR_TIMER_RATE);
    MIsOpen = true;
    return true;
  }

  //----------

  virtual bool hide() {
    MIsOpen = false;
    MTimer->stop();
    MWindow->close();
    return true;
  }

//------------------------------
public:
//------------------------------

  void setCanResize(bool AResize=true) {
    MCanResize = AResize;
  }

  //----------

  void connect(MIP_Widget* AWidget, MIP_Parameter* AParameter) {
    uint32_t index = AParameter->getId();
    //AWidget->setParamIndex(index);
    MParamToWidget[index] = AWidget;
    AWidget->on_widget_connect(AParameter);
  }

  //----------

  void connect(MIP_Widget* AWidget, int32_t AParamIndex) {
    //AWidget->setParamIndex(AParamIndex);
    MParamToWidget[AParamIndex] = AWidget;
    clap_param_info_t info;
    if (MPlugin->params_get_info(AParamIndex,&info)) {
      MIP_Parameter* parameter = (MIP_Parameter*)info.cookie;
      AWidget->on_widget_connect(parameter);
    }
  }

  //----------

  void setParameterValue(uint32_t AIndex, double AValue, bool ARedraw=false) {
    float value = AValue;
    MIP_Widget* widget = MParamToWidget[AIndex];
    if (widget) {
//      MIP_Parameter* param = widget->getParameter();
//      if (param) value = param->normalizeValue(value);
      widget->setValue(value);
      if (ARedraw) widget->redraw();
    }
  }

  //----------

  //void setParameterValues(MIP_ParameterManager* AParameters, bool ARedraw=false) {
  //  for (uint32_t i=0; i<AParameters->parameterCount(); i++) {
  //    MIP_Parameter* param = AParameters->getParameter(i);
  //    double value = param->getValue();
  //    //setParameterValue(i,value,ARedraw);
  //    MIP_Widget* widget = MParamToWidget[i];
  //    if (widget) {
  //      value = param->normalizeValue(value);
  //      widget->setValue(value);
  //      if (ARedraw) widget->redraw();
  //    }
  //  }
  //}

  //----------

  MIP_Window* getWindow() {
    return MWindow;
  }

//------------------------------
private: // window listener
//------------------------------

  // gui -> host

  void on_window_listener_updateWidget(MIP_Widget* AWidget) override {
    int32_t index = -1;
    MIP_Parameter* param = AWidget->getParameter();
    if (param) index = param->getId();
    if (index >= 0) {
      double value = AWidget->getValue();
      if (param->isStepped()) value = (int)value;
      //MIP_Print("%i %.3f\n",index,value);
      //value = param->denormalizeValue(value);
      if (MListener) {
        //MIP_Print("letting the editor listener know...\n");
        MListener->on_editor_listener_parameter(index,value);
      }
    }
  }

  //----------

  void on_window_listener_resize(uint32_t AWidth, uint32_t AHeight) override {
    if (MListener) MListener->on_editor_listener_resize(AWidth,AHeight);
  }

//------------------------------
public: // timer listener
//------------------------------

  // [timer]

  void on_timerCallback(void) override {
    clearDirtyWidgets();
    flushGuiParams();
    flushGuiMods();
    if (MIsOpen) {
      redrawDirtyWidgets();
    }
  }

//------------------------------
public:
//------------------------------

  // host -> gui
  // called from MIP_Plugin.on_event_listener_parameter

  void queueGuiParam(uint32_t AIndex, double AValue) {
    double value = AValue;
    MIP_Widget* widget = MParamToWidget[AIndex];
    MIP_Assert(widget);
//    MIP_Parameter* param = widget->getParameter();
//    if (param) value = param->normalizeValue(value);
    MGuiParamVal[AIndex] = value;
    MGuiParamQueue.write(AIndex);
  }

  //----------

  // [gui]

  void flushGuiParams() {
    if (MIsOpen && MWindow) {
      uint32_t index = 0;
      while (MGuiParamQueue.read(&index)) {
        double value = MGuiParamVal[index];
        MIP_Widget* widget = MParamToWidget[index];
        if (widget) {
          if (widget->getValue() != value)
          widget->setValue(value);
          if (MDirtyWidgets.findItem(widget) < 0) {
            MDirtyWidgets.append(widget);
          }
        }
      }
    }
  }

  //----------

  void queueGuiMod(uint32_t AIndex, double AValue) {
    MGuiParamMod[AIndex] = AValue;
    MGuiModQueue.write(AIndex);
  }

  //----------

  void flushGuiMods() {
    if (MIsOpen) { // && MWindow?
      uint32_t index = 0;
      while (MGuiModQueue.read(&index)) {
        double valuemod = MGuiParamMod[index];
        MIP_Widget* widget = MParamToWidget[index];
        if (widget) {
          if (widget->getModValue() != valuemod)
          widget->setModValue(valuemod);
          if (MDirtyWidgets.findItem(widget) < 0) {
            MDirtyWidgets.append(widget);
          }

        }
      }
    }
  }

  //----------

  void clearDirtyWidgets() {
    MDirtyWidgets.clear();
  }

  //----------

  void redrawDirtyWidgets() {
    if (MIsOpen && MWindow) {
      for (uint32_t i=0; i<MDirtyWidgets.size(); i++) {
        MIP_Widget* widget = MDirtyWidgets[i];
        MIP_FRect rect = widget->getRect();
        MWindow->invalidate(rect.x,rect.y,rect.w + 0,rect.h + 0); // +1
      }
    }
    //MDirtyWidgets.clear();
  }

};

//----------------------------------------------------------------------
#endif
